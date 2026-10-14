// tests/BatteryCanMessageTest.cpp

#include "BatteryCanMessage.hpp"

#include <cmath>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <limits>

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
    uint8_t data[8]{};

    if (!BatteryCanMessage::encode(
            12.6,
            10.0,
            99.8,
            data))
    {
        std::cerr << "Failed to encode battery CAN message"
                  << std::endl;
        return 1;
    }

    const uint8_t expectedData[8] = {
        0xEC, 0x04, 0xE8, 0x03, 0xFC, 0x26, 0x00, 0x00};

    for (size_t i = 0; i < sizeof(expectedData); ++i)
    {
        if (data[i] != expectedData[i])
        {
            std::cerr << "Unexpected byte at index " << i
                      << std::endl;
            return 1;
        }
    }

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

    uint8_t invalidData[5]{};

    if (BatteryCanMessage::decode(
            invalidData,
            sizeof(invalidData),
            voltage,
            current,
            stateOfCharge))
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

    if (BatteryCanMessage::encode(
            0.0,
            0.0,
            0.0,
            nullptr))
    {
        std::cerr << "Encode unexpectedly accepted"
                  << " a null payload"
                  << std::endl;

        return 1;
    }

    // Reject invalid battery values
    if (BatteryCanMessage::encode(-1.0, 10.0, 50.0, data) ||
        BatteryCanMessage::encode(12.6, -1.0, 50.0, data) ||
        BatteryCanMessage::encode(12.6, 10.0, -1.0, data) ||
        BatteryCanMessage::encode(12.6, 10.0, 101.0, data) ||
        BatteryCanMessage::encode(
            std::numeric_limits<double>::quiet_NaN(),
            10.0, 50.0, data))
    {
        std::cerr << "Encode accepted invalid battery values"
                  << std::endl;
        return 1;
    }

    // Boundary values must be accepted
    if (!BatteryCanMessage::encode(0.0, 0.0, 0.0, data) ||
        !BatteryCanMessage::encode(655.35, 655.35, 100.0, data))
    {
        std::cerr << "Encode rejected valid boundary values"
                  << std::endl;
        return 1;
    }

    std::cout << "BatteryCanMessage tests passed"
              << std::endl;

    return 0;
}