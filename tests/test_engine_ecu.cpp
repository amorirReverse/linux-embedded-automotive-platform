// tests/test_engine_ecu.cpp

#include "EngineEcu.hpp"
#include "EngineCanMessage.hpp"
#include "CanSocket.hpp"

#include <cstdint>
#include <iostream>

/**
 * @brief Entry point for EngineEcu integration tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    EngineEcu engineEcu("vcan0");

    CanSocket receiver("vcan0");

    if (!receiver.isValid())
    {
        std::cerr << "Failed to create CAN socket for receiving messages"
                  << std::endl;
        return 1;
    }

    engineEcu.run(1);

    uint32_t canId = 0;
    uint8_t data[8]{};
    uint8_t dataLength = 0;

    if (!receiver.receive(canId, data, dataLength, sizeof(data)))
    {
        std::cerr << "Failed to receive CAN message" << std::endl;
        return 1;
    }

    if (canId != EngineCanMessage::CAN_ID)
    {
        std::cerr << "Received unexpected CAN ID: 0x"
                  << std::hex
                  << canId
                  << std::dec
                  << std::endl;
        return 1;
    }

    if (dataLength != EngineCanMessage::PAYLOAD_LENGTH)
    {
        std::cerr << "Received unexpected CAN payload length: "
                  << static_cast<int>(dataLength)
                  << ", expected: "
                  << EngineCanMessage::PAYLOAD_LENGTH
                  << std::endl;
        return 1;
    }

    double rpm = 0.0;
    double temperature = 0.0;

    if (!EngineCanMessage::decode(
            data,
            dataLength,
            rpm,
            temperature))
    {
        std::cerr << "Failed to decode Engine ECU CAN frame."
                  << std::endl;

        return 1;
    }

    if (rpm <= 0.0)
    {
        std::cerr << "Unexpected engine RPM: "
                  << rpm
                  << std::endl;

        return 1;
    }

    if (temperature <= 20.0)
    {
        std::cerr << "Unexpected engine temperature: "
                  << temperature
                  << std::endl;

        return 1;
    }

    EngineEcu invalidEngineEcu("invalid_interface");
    invalidEngineEcu.run(1);

    std::cout << "Engine ECU CAN frame:"
              << " RPM=" << rpm
              << " tr/min"
              << " Temperature=" << temperature
              << " °C"
              << std::endl;

    return 0;
}