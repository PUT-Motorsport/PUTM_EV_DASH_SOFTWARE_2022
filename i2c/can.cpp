#include "can.hpp"

Can::Can() {
  if (not QCanBus::instance()->plugins().contains("socketcan")) {
    qDebug() << "socketcan plugin missing";
  }
}

bool Can::connect() {
  QString errorString;
  dev = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                          QStringLiteral("can0"), &errorString);
  if (not dev) {
    qDebug() << errorString;
    return false;
  }
  if (not dev->connectDevice()) {
    qDebug() << "Device failed to connect";
    return false;
  } else {
    QObject::connect(dev, &QCanBusDevice::errorOccurred, this,
                     &Can::onCanErrorOcurred);
  }
  return true;
}

bool Can::send(const QCanBusFrame &frame) { return dev->writeFrame(frame); }

void Can::onCanErrorOcurred() {
  auto const errorStr = dev->errorString();

  qDebug() << errorStr;
}
