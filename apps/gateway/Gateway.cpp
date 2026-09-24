// apps/gateway/Gateway.cpp

#include "Gateway.hpp"

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
    }
}