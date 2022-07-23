#pragma once

#include <QDebug>
#include <cstdint>
#include <string>

#include "lib/i2c.h"

typedef uint16_t address_t;

namespace {
constexpr address_t deviceAddress{0x36};
struct ByteRegister {
  static constexpr address_t ZMCO = 0x00;
  static constexpr address_t ABN = 0x09;
  static constexpr address_t PUSHTHR = 0x0A;
  static constexpr address_t STATUS = 0x0B;
  static constexpr address_t AGC = 0x1A;
  static constexpr address_t BURN = 0xFF;
};
struct WordRegister {
  static constexpr address_t ZPOS = 0x01;
  static constexpr address_t CONF = 0x03;
  static constexpr address_t RAWANGLE = 0x0C;
  static constexpr address_t ANGLE = 0x0E;
  static constexpr address_t MAGNITUDE = 0x1B;
};
}  // namespace

class AS5601 {
 public:
  explicit AS5601(const char* busName) {
    if ((bus = i2c_open(busName)) == -1) {
      qDebug() << "Couldn't open bus file";
    }
    dev.bus = bus;
    dev.addr = deviceAddress;
    dev.iaddr_bytes = 1;
    dev.page_bytes = 16;
  }

  bool magnetDetected() const {
    const auto reg = read8bit(ByteRegister::STATUS);

    const uint8_t mask{1 << 5};

    return (reg bitand mask);
  }

  uint16_t getMagnitude() const { return read16bit(WordRegister::MAGNITUDE); }

  uint8_t getGain() const { return read8bit(ByteRegister::AGC); }

  uint16_t getRawAngle() const {
    const auto reg1{read8bit(WordRegister::RAWANGLE)};
    const auto reg2{read8bit(WordRegister::RAWANGLE + 1)};

    const uint16_t reg{static_cast<uint16_t>(reg1 << 8 bitor reg2)};

    const uint16_t mask{(1 << 12) - 1};

    return (reg bitand mask);
  }

  void setZeroPosition(uint16_t rawAngle) {
    write16bit(WordRegister::ZPOS, rawAngle);
  }

  void setResolution(uint16_t angleSteps) {
    char power = -1;

    // coerce angle steps to supported values (8, 16, 32, …, 2048)
    angleSteps = std::min<uint16_t>(std::max<uint16_t>(angleSteps, 8), 2048);

    // find dual logarithm (2^power >= angleSteps)
    // (by comparing increasing powers of two with angleSteps)
    while ((1 << ++power) < angleSteps)
      ;

    // send ABN setting command (-3 (2^3 = 8) shifts the powers 3…11 (for
    // 8…2048) to 0…8)
    write8bit(ByteRegister::ABN, power - 3);
  }

  uint16_t getAngle() const {
    const auto reg{read16bit(WordRegister::ANGLE)};

    const uint16_t mask{(1 << 12) - 1};

    return (reg bitand mask);
  }

 private:
  uint8_t read8bit(address_t reg) const {
    uint8_t buff;
    std::size_t size = 1;

    if (i2c_read(&dev, reg, &buff, size) not_eq size) {
      qDebug() << "Incorrect 8-bit read";
    }
    return buff;
  }
  uint16_t read16bit(address_t reg) const {
    uint16_t buff;
    std::size_t size = 2;

    if (i2c_read(&dev, reg, &buff, size) not_eq size) {
      qDebug() << "Incorrect 16-bit read";
    }
    return buff;
  }
  void write8bit(address_t reg, uint8_t data) {
    if (i2c_write(&dev, reg, &data, 1) not_eq 1) {
      qDebug() << "Incorrect 8-bit write";
    }
  }
  void write16bit(address_t reg, uint16_t data) {
    if (i2c_write(&dev, reg, &data, 2) not_eq 2) {
      qDebug() << "Incorrect 16-bit write";
    }
  }
  int bus;
  I2CDevice dev;
};
