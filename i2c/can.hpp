#pragma once

#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDebug>

class Can {
 public:
  explicit Can();

  bool connect();
  bool send(QCanBusFrame const& frame);

 private slots:
  void onCanErrorOcurred();

 private:
  QCanBusDevice* dev;
};
