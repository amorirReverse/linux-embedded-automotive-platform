// can/test_can.cpp

#include "CanSocket.hpp"
#include "EngineCanMessage.hpp"

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

    uint8_t data[8] {};

    EngineCanMessage::encode(
        800.0,
        90.0,
        data);
    
   

    if (!canSocket.send(
        EngineCanMessage::CAN_ID,
        data,
        sizeof(data)))
    {
        std::cerr   << "Failed to send CAN frame"
                    << std::endl;
        return 1;
    }

    std::cout   << "CAN frame sent successfully"
                << std::endl;
    
    uint32_t receivedCanId = 0;
    uint8_t receivedData[8] {};
    uint8_t receivedDataLength = 0;

    if (!canSocket.receive(
        receivedCanId,
        receivedData,
        receivedDataLength))
    {
        std::cerr   << "Failed to receive CAN frame"
                    << std::endl;
        return 1;
    }

     double decodeRpm = 0.0;
    double decodeTemperature = 0.0;

    if (!EngineCanMessage::decode(
        data,
        sizeof(data),
        decodeRpm,
        decodeTemperature))
    {
        std::cerr   << "Failed to decode CAN message"
                    << std::endl;
        return 1;
    }

    std::cout << "CAN message received:"
              << " ID=0x"
              << std::hex
              << receivedCanId
              << std::dec
              <<" DLC="
              << static_cast<int>(receivedDataLength)
              << std::endl;

    std::cout   << "Decoded CAN message:"
                << " RPM=" 
                << decodeRpm
                << " tr/min"
                << " Temperature=" 
                << decodeTemperature
                << " °C"
                << std::endl;

    

    return 0;
}