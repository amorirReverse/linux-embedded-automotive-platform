// can/EngineCanMessage.cpp

#include "EngineCanMessage.hpp"

#include <cstring>

void EngineCanMessage::encode(
    double rpm,
    double temperature,
    uint8_t* data)
{
    const auto rpmValue = static_cast<uint16_t>(rpm);
    const auto temperatureValue = 
        static_cast<uint16_t>(temperature * 10.0);

    data[0] = static_cast<uint8_t>(rpmValue & 0xFF);
    data[1] = static_cast<uint8_t>((rpmValue >> 8) & 0xFF);

    data[2] = static_cast<uint8_t>(temperatureValue & 0xFF);
    data[3] = static_cast<uint8_t>(
        (temperatureValue >> 8) & 0xFF);

    std::memset(data + 4, 0, 4);
}