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

bool BatteryCanMessage::decode(
    const uint8_t* data,
    uint8_t dataLength,
    double& voltage,
    double& current,
    double& stateOfCharge)
{
    if (data == nullptr || dataLength < 6)
    {
        return false;
    }

    const uint16_t voltageValue =
        static_cast<uint16_t>(data[0])
        | (static_cast<uint16_t>(data[1]) << 8);

    const uint16_t currentValue =
        static_cast<uint16_t>(data[2])
        | (static_cast<uint16_t>(data[3]) << 8);

    const uint16_t stateOfChargeValue =
        static_cast<uint16_t>(data[4])
        | (static_cast<uint16_t>(data[5]) << 8);

    voltage =
        static_cast<double>(voltageValue) / 100.0;

    current =
        static_cast<double>(currentValue) / 100.0;

    stateOfCharge =
        static_cast<double>(stateOfChargeValue) / 100.0;

    return true;
}