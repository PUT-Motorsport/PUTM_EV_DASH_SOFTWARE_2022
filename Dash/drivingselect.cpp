#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(QWidget *parent) :
    QDialog(parent),
    currentSetting(Setting::TC), ui(new Ui::DrivingSelect), presetsFile(presets), descriptionsFile(descriptions), valuesFile(values)
{
    ui->setupUi(this);

    labels = {{ui->tc, ui->tcValue}, {ui->slipRatio, ui->slipRatioValue}, {ui->algorithm, ui->algorithmValue},
              {ui->power, ui->powerValue}, {ui->regenPower, ui->regenValue}, {ui->sensitivity, ui->sensitivityValue}, {ui->apps, ui->appsValue}};

    changeHighlight();
}

DrivingSelect::~DrivingSelect()
{
    delete ui;
}

void DrivingSelect::navigate(buttonStates navigation)
{
    switch (navigation) {
    case buttonStates::button2_3:
        sendSettings();
        break;
    case buttonStates::button2:
        changeSettingValue();
        break;
    case buttonStates::button3:
        resetToCurrent();
        changeHighlight();
        break;
    case buttonStates::button4:
        resetToCurrent();
        this->done(QDialog::Accepted);
    default:
        return;
    }
}

void DrivingSelect::raiseError(const QString &errorMessage)
{
    ui->error->setText("Error: " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
    });
}

void DrivingSelect::setPreset(Side side, scrollStates scroll)
{
    if (side)
        presetsFile.readLine();

    QString line = presetsFile.readLine();

    auto list = line.split(',');

    uint8_t newSetting = list.at(1 + static_cast<uint8_t>(scroll)).toUInt();

    settingsFrame = currentSettings;

    if (side == Side::Left)
        settingsFrame.max_power = newSetting;
    else
        settingsFrame.regen_power = newSetting;

    sendSettings();

    presetsFile.close();
    qDebug() << presetsFile.open(QIODevice::ReadOnly | QIODevice::Text);
}

void DrivingSelect::sendSettings()
{
    settingsFrame.CRC_byte = calculateCRC();

    QCanBusFrame frame;
    frame.setFrameId(DASH_TCS_CAN_ID);

    frame.setPayload(QByteArray(reinterpret_cast<char *>(&settingsFrame), DASH_TCS_CAN_DLC));


    if (not(canHandler.send(frame))) {
        raiseError("Frame sending failed");
    }
    else {
        currentSettings = settingsFrame;
        logger.add("TC settings change has been requested");
        this->raiseError("Sent!");
    }
    resetToCurrent();
}

void DrivingSelect::changeHighlight()
{
    //getNextSetting
    if (currentSetting == Setting::Sensitivity)
        currentSetting = Setting::TC;

    else
        currentSetting = static_cast<Setting>(static_cast<int>(currentSetting) + 1);

    QString white{"color: white;"};
    QString red{"color: red;"};

    //clear previous highlight
    for (auto pair: labels) {
        pair.first->setStyleSheet(white);
        pair.second->setStyleSheet(white);
    }
    //highlight setting

    labels.at(static_cast<int>(currentSetting)).first->setStyleSheet(red);
    labels.at(static_cast<int>(currentSetting)).second->setStyleSheet(red);

}

void DrivingSelect::resetToCurrent()
{
    ui->tcValue->setText((currentSettings.on) ? "Active" : "Inactive");
    ui->slipRatioValue->setText(QString::number(currentSettings.max_slip_ratio));
    ui->algorithmValue->setText(
                getSettingName(Setting::Algorithm, static_cast<uint8_t>(currentSettings.algorithm)));
    ui->powerValue->setText(QString::number(currentSettings.max_power));
    ui->regenValue->setText(QString::number(currentSettings.regen_power));
    ui->sensitivityValue->setText(QString::number(currentSettings.sensivity));

}

uint8_t DrivingSelect::calculateCRC()
{
    return 0; //todo
}

uint8_t DrivingSelect::getNextSettingValue(Setting setting)
{
    const auto lineNo = 2 + static_cast<uint8_t>(setting);

    if (not(valuesFile.seek(lineNo))) {
        logger.add("File of unexpected length");
        return 1;
    }
    QString line(valuesFile.readLine());

    QStringList list = line.split(',', Qt::SkipEmptyParts);

    auto min = list.at(1).toInt();
    auto max = list.at(2).toInt();
    auto increment = list.at(3).toInt();

    //get current value through pointer arithmetic

    uint8_t * data = reinterpret_cast<uint8_t *>(&settingsFrame);

    auto &current = *(data + static_cast<uint8_t>(setting));

    if (current + increment < max) {
        current += increment;
    }
    else {
        current = min;
    }

    valuesFile.close();
    valuesFile.open(QIODevice::ReadOnly | QIODevice::Text);
    return current;
}

void DrivingSelect::changeSettingValue()
{
    auto next = getNextSettingValue(currentSetting);

    if (currentSetting == Setting::TC or currentSetting == Setting::Algorithm or currentSetting == Setting::AppsCurve) {
        labels.at(static_cast<uint8_t>(currentSetting)).second->setText(getSettingName(currentSetting, next));
    }
    else
        labels.at(static_cast<uint8_t>(currentSetting)).second->setText(QString::number(next));

    uint8_t * data = reinterpret_cast<uint8_t * >(&settingsFrame);

    *(data + static_cast<uint8_t>(currentSetting)) = next;
}

const QString DrivingSelect::getSettingName(Setting setting, uint8_t value)
{
    if (setting == Setting::TC) {
        return (value) ? "Active" : "Inactive";
    }
    if (value > 10) {
        logger.add("Incorrect setting name read. Processed.", LogType::AppError);
        return QString("App error");
    }

    if (not(descriptionsFile.seek(1 + 10 * (setting == Setting::Algorithm))))
        logger.add("File of unexpected length");

    for (auto iter = 0; iter < value; iter++)
        descriptionsFile.readLine();

    descriptionsFile.close();

    return QString(descriptionsFile.readLine());
}
