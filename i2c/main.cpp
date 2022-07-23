#include <QCoreApplication>
#include <QTimer>

#include "as5601.hpp"
#include "can.hpp"

constexpr uint8_t frameID{0x8c};

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  AS5601 sensor("/dev/i2c-1");
  QTimer tim;
  Can can{};

  can.connect();

  QObject::connect(&tim, &QTimer::timeout, [&sensor, &can]() {
    auto angle = sensor.getAngle();
    QCanBusFrame frame{};
    frame.setFrameId(frameID);
    const QByteArray payload{reinterpret_cast<char *>(&angle), sizeof(angle)};
    frame.setPayload(payload);

    can.send(frame);
    qDebug() << sensor.getAngle() << sensor.getRawAngle();
  });

  tim.start(500);

  return a.exec();
}
