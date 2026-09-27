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

/**
 * @brief Entry point for the EngineCanMessage test.
 * 
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    uint8_t data[8] {};

    EngineCanMessage::encode(
        800.0,
        90.0,
        data);
    
    double  rpm = 0.0;
    double temperature = 0.0;

    if (!EngineCanMessage::decode(
            data,
            sizeof(data),
            rpm,
            temperature))
    {
        std::cerr << "Failed to decode engine CAN message"
                  << std::endl;

        return 1;
    }

    if (!almostEqual(rpm, 800.0))
    {
        std::cerr << "Unexpected RPM value: "
                  << rpm
                  << std::endl;

        return 1;
    }

    if (!almostEqual(temperature, 90.0))
    {
        std::cerr << "Unexpected temperature value: "
                  << temperature
                  << std::endl;

        return 1;
    }

    uint8_t invalidData[3] {};

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
