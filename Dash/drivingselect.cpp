#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(QWidget *parent) :
    QDialog(parent),
    currentSetting(Setting::TC), ui(new Ui::DrivingSelect)
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
    QFile file(presets);

    if(not(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        logger.add(presets + " couldn't be read");
    }

    if (side)
        file.readLine();

    QString line = file.readLine();

    auto list = line.split(',');

    uint8_t newSetting = list.at(1 + static_cast<uint8_t>(scroll)).toUInt();

    settingsFrame = currentSettings;

    if (side == Side::Left)
        settingsFrame.max_power = newSetting;
    else
        settingsFrame.regen_power = newSetting;

    sendSettings();


    file.close();
}

void DrivingSelect::sendSettings()
{
    settingsFrame.CRC = calculateCRC();

    QCanBusFrame frame;
    frame.setFrameId(DASH_TCS_FRAME_CAN_ID);

    frame.setPayload(QByteArray(reinterpret_cast<char *>(&settingsFrame), DASH_TCS_FRAME_CAN_DLC));


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

    QFile file(valuesFile);

    if (not(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        logger.add(valuesFile + "couldn't be opened");
        return 1;
    }

    if (not(file.seek(lineNo))) {
        logger.add("File of unexpected length");
        return 1;
    }
    QString line(file.readLine());

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

    file.close();
    return current;
    return 1;
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

const QString DrivingSelect::getSettingName(Setting setting, uint8_t value) const
{
    if (setting == Setting::TC) {
        return (value) ? "Active" : "Inactive";
    }
    if (value > 10) {
        logger.add("Incorrect setting name read. Processed.", LogType::AppError);
        return QString("App error");
    }

    QFile file(descriptionsFile);
    if (not(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        logger.add(descriptionsFile + " couldn't be opened");
        return QString();
    }

    if (not(file.seek(1 + 10 * (setting == Setting::Algorithm))))
        logger.add("File of unexpected length");

    for (auto iter = 0; iter < value; iter++)
        file.readLine();

    file.close();

    return QString(file.readLine());
}
