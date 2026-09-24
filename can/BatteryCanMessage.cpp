// can/BatteryCanMessage.cpp

#include "BatteryCanMessage.hpp"

#include <cstring>

void BatteryCanMessage::encode(
    double voltage,
    double current,
    double stateOfCharge,
    uint8_t* data)
{
    const auto voltageValue =
        static_cast<uint16_t>(voltage * 100.0);

    const auto currentValue =
        static_cast<uint16_t>(current * 100.0);

    const auto stateOfChargeValue =
        static_cast<uint16_t>(stateOfCharge * 100.0);

    data[0] = static_cast<uint8_t>(voltageValue & 0xFF);
    data[1] = static_cast<uint8_t>((voltageValue >> 8) & 0xFF);

    data[2] = static_cast<uint8_t>(currentValue & 0xFF);
    data[3] = static_cast<uint8_t>((currentValue >> 8) & 0xFF);

    data[4] = static_cast<uint8_t>(
        stateOfChargeValue & 0xFF);

    data[5] = static_cast<uint8_t>(
        (stateOfChargeValue >> 8) & 0xFF);

    std::memset(data + 6, 0, 2);
}