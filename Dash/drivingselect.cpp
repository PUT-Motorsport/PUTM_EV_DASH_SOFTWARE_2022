#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(QWidget *parent)
    : QDialog(parent), currentSetting(Setting::TC), ui(new Ui::DrivingSelect),
      table(
          CRC::Parameters<uint8_t, sizeof(uint8_t)>({.polynomial = 0x1d,
                                                     .initialValue = 0xff,
                                                     .finalXOR = 0,
                                                     .reflectInput = false,
                                                     .reflectOutput = false})) {
  currentSettings = {.on = true,
                     .max_slip_ratio = 50,
                     .algorithm = 0,
                     .max_power = 50,
                     .apps_curve = 0,
                     .regen_power = 50,
                     .sensivity = 50,
                     .CRC_byte = 0};

  previousSettings = currentSettings;

  ui->setupUi(this);

  descriptions = loadFile(descriptionsFileName);
  values = loadFile(valuesFileName);
  presets = loadFile(presetsFileName);

  labels = {{ui->tc, ui->tcValue},
            {ui->slipRatio, ui->slipRatioValue},
            {ui->algorithm, ui->algorithmValue},
            {ui->power, ui->powerValue},
            {ui->apps, ui->appsValue},
            {ui->regenPower, ui->regenValue},
            {ui->sensitivity, ui->sensitivityValue}};

  changeHighlight();
}

DrivingSelect::~DrivingSelect() { delete ui; }

void DrivingSelect::navigate(buttonStates navigation) {
  switch (navigation) {
  case buttonStates::button2_3:
    sendSettings();
    break;
  case buttonStates::button1_4:
    resetToCurrent();
    break;
  case buttonStates::button1:
    changeSettingValue(true);
    break;
  case buttonStates::button2:
    changeSettingValue(false);
    break;
  case buttonStates::button3:
    changeHighlight();
    break;
  case buttonStates::button4:
    resetToCurrent();
    this->done(QDialog::Accepted);
  default:
    return;
  }
}

void DrivingSelect::raiseError(const QString &errorMessage) {
  ui->error->setText("Error: " + errorMessage);
  QTimer::singleShot(3000, [this]() { ui->error->setText(""); });
}

void DrivingSelect::setPreset(Side side, scrollStates scroll) {

  auto list = presets.at(side).split(',', Qt::SkipEmptyParts);

  uint8_t newSetting = list.at(1 + static_cast<uint8_t>(scroll)).toUInt();

  previousSettings = currentSettings;

  if (side == Side::Left)
    previousSettings.max_power = newSetting;
  else
    previousSettings.regen_power = newSetting;

  sendSettings();
}

void DrivingSelect::sendSettings() {
  currentSettings.CRC_byte = calculateCRC();

  QCanBusFrame frame;
  frame.setFrameId(DASH_TCS_CAN_ID);

  frame.setPayload(
      QByteArray(reinterpret_cast<char *>(&currentSettings), DASH_TCS_CAN_DLC));

  if (not(canHandler.send(frame))) {
    raiseError("Frame sending failed");
  } else {
    previousSettings = currentSettings;
    logger.add("TC settings change has been requested");
    this->raiseError("Sent!");
  }
  resetToCurrent();
}

void DrivingSelect::changeHighlight() {
  // getNextSetting
  if (currentSetting == Setting::Sensitivity)
    currentSetting = Setting::TC;

  else
    currentSetting = static_cast<Setting>(static_cast<int>(currentSetting) + 1);

  QString white{"color: white;"};
  QString red{"color: red;"};

  // clear previous highlight
  for (auto pair : labels) {
    pair.first->setStyleSheet(white);
    pair.second->setStyleSheet(white);
  }
  // highlight setting

  labels.at(static_cast<int>(currentSetting)).first->setStyleSheet(red);
  labels.at(static_cast<int>(currentSetting)).second->setStyleSheet(red);
}

void DrivingSelect::resetToCurrent() {
  ui->tcValue->setText(getSettingName(Setting::TC, previousSettings.on));
  ui->slipRatioValue->setText(QString::number(previousSettings.max_slip_ratio));
  ui->algorithmValue->setText(getSettingName(
      Setting::Algorithm, static_cast<uint8_t>(previousSettings.algorithm)));
  ui->powerValue->setText(QString::number(previousSettings.max_power));
  ui->regenValue->setText(QString::number(previousSettings.regen_power));
  ui->sensitivityValue->setText(QString::number(previousSettings.sensivity));
}

uint8_t DrivingSelect::calculateCRC() {
  return CRC::Calculate(&currentSettings, sizeof(Dash_TCS) - 1, table);
}

uint8_t DrivingSelect::getSettingValue(Setting setting, bool next) {
  auto list = values.at(1 + static_cast<uint8_t>(setting))
                  .split(',', Qt::SkipEmptyParts);

  auto min = list.at(1).toInt();
  auto max = list.at(2).toInt();
  auto increment = list.at(3).toInt();

  // get current value
  uint8_t *data = reinterpret_cast<uint8_t *>(&currentSettings);

  auto &current = data[static_cast<uint8_t>(setting)];

  if (next) {
    if (current + increment <= max) {
      current += increment;
    } else {
      current = min;
    }
  } else {
    if (current - increment >= min)
      current -= increment;

    else
      current = max;
  }

  return current;
}

void DrivingSelect::changeSettingValue(bool next) {
  auto nextSetting = getSettingValue(currentSetting, next);

  if (currentSetting == Setting::TC or currentSetting == Setting::Algorithm or
      currentSetting == Setting::AppsCurve) {
    labels.at(static_cast<uint8_t>(currentSetting))
        .second->setText(getSettingName(currentSetting, nextSetting));
  } else
    labels.at(static_cast<uint8_t>(currentSetting))
        .second->setText(QString::number(nextSetting) + '%');
}

const QString DrivingSelect::getSettingName(Setting setting, uint8_t value) {
  if (setting == Setting::TC) {
    return (value) ? "Active" : "Inactive";
  }

  return descriptions.at(1 + 10 * (setting == Setting::AppsCurve) + value)
      .trimmed();
}

QList<QString> DrivingSelect::loadFile(const QString &fileName) {
  QFile file(fileName);

  if (not file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    logger.add(fileName + " couldn't be loaded", LogType::Critical);
    return QList<QString>();
  }

  QList<QString> result;

  while (not file.atEnd()) {
    result.append(QString(file.readLine()));
  }

  return result;
}
