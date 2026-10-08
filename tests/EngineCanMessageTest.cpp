// tests/EngineCanMessageTest.cpp

#include "EngineCanMessage.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>

namespace
{
    bool almostEqual(double lhs, double rhs)
    {
        return std::fabs(lhs - rhs) < 0.001;
    }
}

bool testEncodeDecode(
    double expectedRpm,
    double expectedTemperature)
{
    uint8_t data[EngineCanMessage::PAYLOAD_LENGTH]{};

    if (!EngineCanMessage::encode(
            expectedRpm,
            expectedTemperature,
            data))
    {
        std::cerr << "Failed to encode engine CAN message"
                  << std::endl;
        return false;
    }

    double rpm = 0.0;
    double temperature = 0.0;

    if (!EngineCanMessage::decode(
            data,
            sizeof(data),
            rpm,
            temperature))
    {
        std::cerr << "Failed to decode engine CAN message"
                  << std::endl;
        return false;
    }

    if (!almostEqual(rpm, expectedRpm))
    {
        std::cerr << "Unexpected RPM value: "
                  << rpm
                  << ", expected: "
                  << expectedRpm
                  << std::endl;
        return false;
    }

    if (!almostEqual(temperature, expectedTemperature))
    {
        std::cerr << "Unexpected temperature value: "
                  << temperature
                  << ", expected: "
                  << expectedTemperature
                  << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Entry point for the EngineCanMessage test.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    uint8_t data[EngineCanMessage::PAYLOAD_LENGTH]{};
    double rpm = 0.0;
    double temperature = 0.0;

    if (!testEncodeDecode(800.0, 90.0))
    {
        std::cerr << "Failed nominal engine CAN message test"
                  << std::endl;

        return 1;
    }

    if (!testEncodeDecode(0.0, 0.0))
    {
        std::cerr << "Failed zero-value engine CAN message test"
                  << std::endl;

        return 1;
    }

    if (!testEncodeDecode(800.0, -20.0))
    {
        std::cerr << "Failed negative-temperature engine CAN message test"
                  << std::endl;

        return 1;
    }

    if (!testEncodeDecode(1234.0, 37.5))
    {
        std::cerr << "Failed fractional-temperature engine CAN message test"
                  << std::endl;

        return 1;
    }

    if (!testEncodeDecode(0.0, -40.0))
    {
        std::cerr << "Failed minimum-temperature engine CAN message test"
                  << std::endl;

        return 1;
    }

    if (!testEncodeDecode(65535.0, 100.0))
    {
        std::cerr << "Failed maximum-RPM engine CAN message test"
                  << std::endl;

        return 1;
    }
    uint8_t largePayload[8]{};

    if (!EngineCanMessage::encode(
            1500.0,
            42.5,
            largePayload))
    {
        std::cerr << "Failed to encode engine CAN message"
                  << "for large payload test"
                  << std::endl;

        return 1;
    }

    largePayload[4] = 0xAA;
    largePayload[5] = 0xBB;
    largePayload[6] = 0xCC;
    largePayload[7] = 0xDD;

    if (!EngineCanMessage::decode(
            largePayload,
            sizeof(largePayload),
            rpm,
            temperature))
    {
        std::cerr << "Failed to decode engine CAN message from large payload"
                  << std::endl;

        return 1;
    }

    if (!almostEqual(rpm, 1500.0))
    {
        std::cerr << "Unexpected RPM value from large payload: "
                  << rpm
                  << std::endl;

        return 1;
    }

    if (!almostEqual(temperature, 42.5))
    {
        std::cerr << "Unexpected temperature value from large payload: "
                  << temperature
                  << std::endl;

        return 1;
    }

    uint8_t exactPayload[4]{};

    if (!EngineCanMessage::encode(
            2000.0,
            55.5,
            exactPayload))
    {
        std::cerr << "Failed to encode engine CAN message"
                  << "for exact payload test"
                  << std::endl;

        return 1;
    }

    if (!EngineCanMessage::decode(
            exactPayload,
            sizeof(exactPayload),
            rpm,
            temperature))
    {
        std::cerr << "Failed to decode engine CAN message with exact payload length"
                  << std::endl;

        return 1;
    }

    if (!almostEqual(rpm, 2000.0))
    {
        std::cerr << "Unexpected RPM value with exact payload length: "
                  << rpm
                  << std::endl;

        return 1;
    }

    if (!almostEqual(temperature, 55.5))
    {
        std::cerr << "Unexpected temperature value with exact payload length: "
                  << temperature
                  << std::endl;

        return 1;
    }

    uint8_t invalidData[3]{};

    if (EngineCanMessage::encode(
            0.0,
            0.0,
            nullptr))
    {
        std::cerr << "Encode unexpectedly accepted"
                  << " an invalid payload"
                  << std::endl;

        return 1;
    }

    if (EngineCanMessage::decode(
            invalidData,
            sizeof(invalidData),
            rpm,
            temperature))
    {
        std::cerr << "Decode unexpectedly accepted"
                  << " an invalid payload"
                  << std::endl;

        return 1;
    }

    if (EngineCanMessage::decode(
            nullptr,
            0,
            rpm,
            temperature))
    {
        std::cerr << "Decode unexpectedly accepted"
                  << " a null payload"
                  << std::endl;

        return 1;
    }

    std::cout << "EngineCanMessage tests passed"
              << std::endl;

    return 0;
}
