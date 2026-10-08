// apps/gateway/test_gateway.cpp

#include "Gateway.hpp"
#include "EngineCanMessage.hpp"
#include "BatteryCanMessage.hpp"
#include "CanSocket.hpp"

#include <iostream>
#include <thread>

/**
 * @brief Entry point for Gateway tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    CanSocket sender("vcan0");

    if (!sender.isValid())
    {
        std::cerr << "Failed to create CAN socket."
                  << std::endl;
        return 1;
    }

    uint8_t data[8]{};
    uint8_t batteryData[8]{};
    uint8_t invalidEngineData[2]{};
    uint8_t invalidBatteryData[2]{};

    if (!EngineCanMessage::encode(1500.0, 85.0, data))
    {
        std::cerr << "Failed to encode Engine CAN message."
                  << std::endl;
        return 1;
    }

    if (!BatteryCanMessage::encode(
            12.5,
            10.0,
            70.0,
            batteryData))
    {
        std::cerr << "Failed to encode Battery CAN message."
                  << std::endl;
        return 1;
    }

    Gateway gateway("vcan0");

    std::thread gatewayThread([&gateway]()
                              { gateway.run(5); });

    std::cout << "Gateway thread started. Sending Engine CAN message..."
              << std::endl;

    std::cout << "Sending Engine CAN frame."
              << std::endl;

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            data,
            8))
    {
        std::cerr << "Failed to send Engine CAN message."
                  << std::endl;
        return 1;
    }

    if (!sender.send(
            BatteryCanMessage::CAN_ID,
            batteryData,
            8))
    {
        std::cerr << "Failed to send Battery CAN message."
                  << std::endl;
        return 1;
    }

    if (!sender.send(
            0x300,
            data,
            8))
    {
        std::cerr << "Failed to send unknown CAN message."
                  << std::endl;
        return 1;
    }

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            invalidEngineData,
            2))
    {
        std::cerr << "Failed to send invalid Engine CAN message."
                  << std::endl;
        return 1;
    }

    if (!sender.send(
            BatteryCanMessage::CAN_ID,
            invalidBatteryData,
            2))
    {
        std::cerr << "Failed to send invalid Battery CAN message."
                  << std::endl;
        return 1;
    }

    gatewayThread.join();

    std::cout << "Gateway tests passed."
              << std::endl;

    return 0;
}