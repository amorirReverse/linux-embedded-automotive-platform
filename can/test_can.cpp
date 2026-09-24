// can/test_can.cpp

#include "CanSocket.hpp"

#include <cstdint>
#include <iostream>

/**
 * @brief Entry point for the CAN socket test.
 * 
 * @return Zero on success, non-zero on failure.
 */
int main()
{
    CanSocket canSocket("vcan0");

    const uint8_t data[] = {
        0x11,
        0x22,
        0x33,
        0x44,
        0x55,
        0x66,
        0x77,
        0x88
    };

    if (!canSocket.send(0x123, data, sizeof(data)))
    {
        std::cerr << "Failed to send CAN frame" << std::endl;
        return 1;
    }

    std::cout << "CAN frame sent successfully" << std::endl;

    return 0;
}