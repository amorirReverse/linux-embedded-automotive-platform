// apps/gateway/Gateway.cpp

#include "Gateway.hpp"
#include "EngineCanMessage.hpp"
#include "BatteryCanMessage.hpp"

#include <cstdint>
#include <iostream>

Gateway::Gateway(const char* canInterface)
    : canSocket_(canInterface)
{
}

void Gateway::run(int frameCount)
{
    for (int frame = 0; frame < frameCount; ++frame)
    {
        uint32_t canId = 0;
        uint8_t data[8] {};
        uint8_t dataLength = 0;

        if (!canSocket_.receive(
                canId,
                data,
                dataLength))
        {
            std::cerr << "Failed to receive CAN message"
                      << std::endl;

            continue;
        }

        std::cout << "Gateway received CAN frame:"
                  << " ID=0x"
                  << std::hex
                  << canId
                  << std::dec
                  << " DLC="
                  << static_cast<int>(dataLength)
                  << std::endl;
        
        if (canId == EngineCanMessage::CAN_ID)
        {
            double rpm = 0.0;
            double temperature = 0.0;

            if (EngineCanMessage::decode(
                    data,
                    dataLength,
                    rpm,
                    temperature))
            {
                std::cout << "Engine status"
                          << " RPM="
                          << rpm
                          << " tr/min"
                          << " Temperature="
                          << temperature
                          << " °C"
                          << std::endl;
            }

        
        }

        if (canId == BatteryCanMessage::CAN_ID)
        {
             double voltage = 0.0;
            double current = 0.0;
            double stateOfCharge = 0.0;

            if (BatteryCanMessage::decode(
                    data,
                    dataLength,
                    voltage,
                    current,
                    stateOfCharge))
            {
                std::cout << "Battery status:"
                          << " Voltage="
                          << voltage
                          << " V"
                          << " Current="
                          << current
                          << " A"
                          << " State of Charge="
                          << stateOfCharge
                          << " %"
                          << std::endl;
            }
        }
    }
}