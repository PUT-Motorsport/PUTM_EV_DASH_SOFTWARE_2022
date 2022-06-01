#include "dvselect.h"
#include "ui_dvselect.h"

DvSelect::DvSelect(QWidget *parent)
    : QDialog(parent), ui(new Ui::DvSelect), missionCount(0),
      currentMission(0) {
  ui->setupUi(this);
  // load mission names
  if (missionNames.length() != 0)
    ui->mission->setText(missionNames.at(0)); // display the first mission
}

DvSelect::~DvSelect() { delete ui; }

void DvSelect::navigate(buttonStates navigation) {
  switch (navigation) {
  case buttonStates::button1:
    this->done(QDialog::Accepted);
    break;
  case buttonStates::button2:
    sendCANframe();
    break;
  case buttonStates::button3:
    toggleMission(-1);
    break;
  case buttonStates::button4:
    toggleMission(1);
    break;
  default:
    return; // warning suppression
  }
}

void DvSelect::raiseError(const QString &errorMessage) {
  ui->error->setText("Error: " + errorMessage);
  QTimer::singleShot(3000, [this]() { ui->error->setText(""); });
}

void DvSelect::toggleMission(int direction) {
  if (missionNames.length() == 0) {
    raiseError("File missing");
    return;
  }
  currentMission += direction;
  if (currentMission >= missionCount)
    currentMission = 0;
  if (currentMission < 0)
    currentMission = missionCount - 1;
  ui->mission->setText(missionNames.at(currentMission));
}

void DvSelect::sendCANframe() {
  QDomNode iterator = missionsFile.firstChild();
  for (int i = 0; i < currentMission; i++) {
    iterator = iterator.nextSibling(); // move the document tree branch to the
                                       // correct mission
  }
  if (iterator.isNull()) {
    raiseError("File Error");
    logger.add("File error while sending dv can frame", LogType::Error);
    return;
  }
  QDomElement element = iterator.toElement();
  int frameID;
  if (element.attribute("frameID") != "")
    frameID = element.attribute("frameID").toInt();
  else {
    raiseError("Can Sender Error"); // inform the user live
    logger.add("Can dv frame couldn't be sent", LogType::Error);
    return;
  }
  QString payloadStr = element.attribute("framePayload");
  QCanBusFrame canFrame;
  canFrame.setFrameId(frameID);
  canFrame.setPayload(QByteArray::fromHex(payloadStr.toUtf8()));
  canFrame.setFrameType(QCanBusFrame::FrameType::DataFrame);

  if (canHandler.send(canFrame)) {
    logger.add("Sent dv can frame " + element.attribute("description"));
    canHandler.setHeartbeat(Dash_states::Driverless);
    ui->sent->setText("Sent!");
  } else {
    ui->sent->setText("NOT Sent!");
  }
  QTimer::singleShot(1000, [this]() { ui->sent->setText(""); });
}
