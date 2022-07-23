#include "canhandler.h"

CanHandler::CanHandler(QObject *parent)
    : QObject(parent) {
  if (!QCanBus::instance()->plugins().contains("socketcan"))
    logger.add("Cansockets plugin missing", LogType::AppError);

}

CanHandler::~CanHandler() {
  delete canDevice;
}

bool CanHandler::connect() {
  QString errorString;
  canDevice = QCanBus::instance()->createDevice(
      QStringLiteral("socketcan"), QStringLiteral("can0"), &errorString);
  if (!canDevice) {
    logger.add("Can Device creation failed: " + errorString, LogType::Critical);
    return false;
  }
  if (!canDevice->connectDevice()) {
    logger.add("Can Device created, but failed to connect", LogType::Critical);
    return false;
  } else
    logger.add("Can Device created successfully");
  QObject::connect(canDevice, &QCanBusDevice::framesReceived, this,
                   &CanHandler::onCanFrameReceived);
  QObject::connect(canDevice, &QCanBusDevice::errorOccurred, this,
                   &CanHandler::onCanErrorOcurred);

  return true;

  canDevice->clear();
}

bool CanHandler::connected() { return canDevice->state(); }

bool CanHandler::send(const QCanBusFrame &toSend) {
  if (not(connected())) {
    logger.add("Attempted sending in unconnected state", LogType::Critical);
    return false;
  }
  if (canDevice->writeFrame(toSend))
    return true;

  logger.add("Frame sending failed");
  return false;
}

void CanHandler::startNewDataCycle() {
  for (auto &device : canData.synchronousFrames)
    device->hasBeenUpdated = false;
}

void CanHandler::onCanFrameReceived() {

    if (canDevice->framesAvailable() > 100) {
        qDebug() << "overflow";
        canDevice->clear(QCanBusDevice::Direction::Input);
    }

  QCanBusFrame frame = canDevice->readFrame();

  logger.addCAN(frame.toString());

  bool processed{false};

  for (auto dev : canData.synchronousFrames) {
    if (dev->id == frame.frameId()) {
      canData.mtx.lock();
      std::memcpy(dev->dataPtr, frame.payload().constData(), dev->dlc);
      dev->hasBeenUpdated = true;
      canData.mtx.unlock();
      processed = true;
      break;
    }
  }

  if (not processed) {

  for (auto event : canData.asynchronousFrames) {
    if (event->id == frame.frameId()) {
      canData.mtx.lock();
      std::memcpy(event->dataPtr, frame.payload().constData(), event->dlc);
      event->hasBeenUpdated = true;
      canData.mtx.unlock();
    }
  }
  }

  if (canDevice->framesAvailable() > 0) {
      onCanFrameReceived();
  }
}

void CanHandler::onCanErrorOcurred() {
  auto const error = canDevice->errorString();
  logger.add("Can Error detected: " + error);
}

void CanHandler::heartbeat() {

  QCanBusFrame heartbeatFrame;
  heartbeatFrame.setFrameId(DASH_MAIN_CAN_ID);

  Dash_Main dash_main;
  dash_main.device_state = this->heartBeatState;

  heartbeatFrame.setPayload(
      QByteArray(reinterpret_cast<char *>(&dash_main), sizeof(dash_main)));

  send(heartbeatFrame);
}
