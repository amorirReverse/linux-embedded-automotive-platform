// tests/test_battery_ecu.cpp

#include "BatteryEcu.hpp"
#include "BatteryCanMessage.hpp"
#include "CanSocket.hpp"

#include <iostream>
#include <cstdint>

/**
 * @brief Entry point for BatteryEcu integration tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    BatteryEcu batteryEcu("vcan0");

    CanSocket receiver("vcan0");

    if (!receiver.isValid())
    {
        std::cerr << "Failed to create CAN socket for receiving messages"
                  << std::endl;
        return 1;
    }

    batteryEcu.run(1);

    uint32_t canId = 0;
    uint8_t data[8]{};
    uint8_t dataLength = 0;

    if (!receiver.receive(
            canId,
            data,
            dataLength,
            sizeof(data)))
    {
        std::cerr << "Failed to receive CAN message"
                  << std::endl;
        return 1;
    }

    if (canId != BatteryCanMessage::CAN_ID)
    {
        std::cerr << "Received CAN message with unexpected ID: 0x"
                  << std::hex
                  << canId
                  << std::dec
                  << std::endl;
        return 1;
    }

    if (dataLength != BatteryCanMessage::PAYLOAD_LENGTH)
    {
        std::cerr << "Received CAN message with unexpected payload length: "
                  << static_cast<int>(dataLength)
                  << ", expected: "
                  << BatteryCanMessage::PAYLOAD_LENGTH
                  << std::endl;
        return 1;
    }

    double voltage = 0.0;
    double current = 0.0;
    double stateOfCharge = 0.0;

    if (!BatteryCanMessage::decode(
            data,
            dataLength,
            voltage,
            current,
            stateOfCharge))
    {
        std::cerr << "Failed to decode battery CAN message"
                  << std::endl;
        return 1;
    }

    if (voltage <= 0.0)
    {
        std::cerr << "Unexpected battery voltage: "
                  << voltage
                  << std::endl;
        return 1;
    }

    if (current < 0.0)
    {
        std::cerr << "Unexpected battery current: "
                  << current
                  << std::endl;
        return 1;
    }

    if (stateOfCharge < 0.0 || stateOfCharge > 100.0)
    {
        std::cerr << "Unexpected battery state of charge: "
                  << stateOfCharge
                  << std::endl;
        return 1;
    }

    BatteryEcu invalidBatteryEcu("invalid_interface");
    invalidBatteryEcu.run(1);

    std::cout << "Battery ECU CAN frame:"
              << " Voltage=" << voltage
              << " V"
              << " Current=" << current
              << " A"
              << " StateOfCharge=" << stateOfCharge
              << " %"
              << std::endl;

    std::cout << "BatteryEcu tests passed"
              << std::endl;

    return 0;
}