#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(CanHandler * can, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrivingSelect), current(Setting::Regain), can(can), valuesFile("ProgramData/settings.csv")
{
    ui->setupUi(this);
    ui->regain->setStyleSheet("color: red;");
    loadSettings();
}

DrivingSelect::~DrivingSelect()
{
    delete ui;
}

void DrivingSelect::navigate(Navigation pressed)
{
    switch (pressed) {
    case Navigation::X:
        changeValue();
        break;
    case Navigation::Y:
        sendCANFrame();
        break;
    case Navigation::A:
        changeHighlight();
        break;
    case Navigation::B:
        this->done(QDialog::Accepted);
    default:
        return;
    }
}

void DrivingSelect::raiseError(int errorCode, const QString &errorMessage)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
}

void DrivingSelect::changeHighlight()
{

    if (current == Setting::Apps)
        current = Setting::Regain;      //TODO: Make it so that if a change is skipped, its value returns to the correct one
    else
        current = static_cast<Setting>(static_cast<int>(current) + 1);

    QString white = "color: white;";
    QString red = "color: red;";
    ui->power->setStyleSheet(white);
    ui->apps->setStyleSheet(white);
    ui->regain->setStyleSheet(white);
    ui->fan->setStyleSheet(white);
    ui->traction->setStyleSheet(white);

    switch (current) {
    case Setting::Apps:
        ui->apps->setStyleSheet(red);
        break;
    case Setting::Fan:
        ui->fan->setStyleSheet(red);
        break;
    case Setting::Traction:
        ui->traction->setStyleSheet(red);
        break;
    case Setting::Power:
        ui->power->setStyleSheet(red);
        break;
    case Setting::Regain:
        ui->regain->setStyleSheet(red);
        break;
    default:
        Logger::add("changeHighlight received a wrong argument");
    }
}

void DrivingSelect::sendCANFrame()
{
    QCanBusFrame frame;
    frame.setFrameType(QCanBusFrame::FrameType::DataFrame);

    QString payloadString;

    switch (current) {      //TODO: Documentation
    case Setting::Regain:
        frame.setFrameId(payloads.at(0).at(1).toInt());
        payloadString = payloads.at(0).at(2 +   //a shift by 3 items to skip descrpitions
                                          regainValues.indexOf(ui->regainValue->text()));
        frame.setPayload(QByteArray::fromHex(payloadString.toUtf8()));
        break;
    case Setting::Traction:
        frame.setFrameId(payloads.at(1).at(1).toInt());
        payloadString = payloads.at(1).at(2 +
                                          tractionValues.indexOf(ui->tractionValue->text()));
        frame.setPayload(QByteArray::fromHex(payloadString.toUtf8()));
        break;
    case Setting::Power:
        frame.setFrameId(payloads.at(2).at(1).toInt());
        payloadString = payloads.at(2).at(2 +
                                          powerValues.indexOf(ui->powerValue->text()));
        frame.setPayload(QByteArray::fromHex(payloadString.toUtf8()));
        break;
    case Setting::Fan:
        frame.setFrameId(payloads.at(3).at(1).toInt());
        payloadString = payloads.at(3).at(2 +
                                          fanValues.indexOf(ui->fanValue->text()));
        frame.setPayload(QByteArray::fromHex(payloadString.toUtf8()));
        break;
    case Setting::Apps:
                    //TODO
        break;
    }
    qDebug() << "Payload " << payloadString;
    can->send(frame);
}

void DrivingSelect::changeValue()
{
    int index;
    switch (current) {
    case Setting::Regain:
        //find the index of current setting, add one, display
        index = regainValues.indexOf(ui->regainValue->text()) + 1;      //FIXME? Two dimensional list to declutter?
        if (index >= regainValues.length())
            index = 1;
        ui->regainValue->setText(regainValues.at(index));
        break;
    case Setting::Traction:
        index = tractionValues.indexOf(ui->tractionValue->text()) + 1;
        if (index >= tractionValues.length())
            index = 1;
        ui->tractionValue->setText(tractionValues.at(index));
        break;
    case Setting::Power:
        index = powerValues.indexOf(ui->powerValue->text()) + 1;
        if (index >= powerValues.length())
            index = 1;
        ui->powerValue->setText(powerValues.at(index));
        break;
    case Setting::Fan:
        index = fanValues.indexOf(ui->fanValue->text()) + 1;
        if (index >= fanValues.length())
            index = 1;
        ui->fanValue->setText(fanValues.at(index));
        break;
    case Setting::Apps:
        changeAppsCurve();
        break; //TODO after curve graphics are created
    default:
        Logger::add("Driving Select changeValue() error", LogType::AppError);
    }
}

void DrivingSelect::loadSettings()
{
    if (not(valuesFile.open(QIODevice::ReadOnly))) {
        Logger::add(valuesFile.errorString(), LogType::AppError);
        return;
    }
    QRegularExpression spliter(",|\n");
    QByteArray line = valuesFile.readLine();
    regainValues = QString(line).split(spliter, Qt::SkipEmptyParts);
    line = valuesFile.readLine();
    tractionValues = QString(line).split(spliter, Qt::SkipEmptyParts);
    line = valuesFile.readLine();
    powerValues = QString(line).split(spliter, Qt::SkipEmptyParts);
    line = valuesFile.readLine();
    fanValues = QString(line).split(spliter, Qt::SkipEmptyParts);
    while (not(valuesFile.atEnd())) {
        QByteArray line = valuesFile.readLine();    //first dimension will be settings, second - setting values
        payloads.append(QString(line).split(spliter, Qt::SkipEmptyParts));
    }
    assert(payloads.length() == 5);
    //display the settings:
    ui->regainValue->setText(regainValues.at(1));       //the first item is the description
    ui->tractionValue->setText(tractionValues.at(1));
    ui->powerValue->setText(powerValues.at(1));
    ui->fanValue->setText(fanValues.at(1));
}

void DrivingSelect::changeAppsCurve()
{
    return; //TODO after adding curve graphics
}
