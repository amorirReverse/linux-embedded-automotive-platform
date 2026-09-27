// tests/BatteryCanMessageTest.cpp

#include "BatteryCanMessage.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>

namespace
{
bool almostEqual(double lhs, double rhs)
{
    return std::abs(lhs - rhs) < 0.001;
}
}

/**
 * @brief Entry point for the BatteryCanMessage tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    uint8_t data[8] {};

    BatteryCanMessage::encode(
        12.6,
        10.0,
        99.8,
        data);

    double voltage = 0.0;
    double current = 0.0;
    double stateOfCharge = 0.0;

    if (!BatteryCanMessage::decode(
            data,
            sizeof(data),
            voltage,
            current,
            stateOfCharge))
    {
        std::cerr << "Failed to decode battery CAN message"
                  << std::endl;

        return 1;
    }

    if (!almostEqual(voltage, 12.6))
    {
        std::cerr << "Unexpected voltage value: "
                  << voltage
                  << std::endl;

        return 1;
    }

    if (!almostEqual(current, 10.0))
    {
        std::cerr << "Unexpected current value: "
                  << current
                  << std::endl;

        return 1;
    }

    if (!almostEqual(stateOfCharge, 99.8))
    {
        std::cerr << "Unexpected state of charge value: "
                  << stateOfCharge
                  << std::endl;

        return 1;
    }

    uint8_t invalidData[5] {};

    if(BatteryCanMessage::decode(
        invalidData,
        sizeof(invalidData),
        voltage,
        current,
        stateOfCharge
    ))
    {
        std::cerr << "Decode unexpectedly accepted"
                  << " an invalid payload"
                  << std::endl;

        return 1;
    }

    if (BatteryCanMessage::decode(
        nullptr,
        0,
        voltage,
        current,
        stateOfCharge))
    {
        std::cerr << "Decode unexpectedly accepted"
                << " a null payload"
                << std::endl;

        return 1;
    }

    std::cout << "BatteryCanMessage tests passed"
              << std::endl;

    return 0;
}