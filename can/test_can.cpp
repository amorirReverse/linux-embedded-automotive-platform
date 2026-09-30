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
    CanSocket sender("vcan0");
    CanSocket receiver("vcan0");

    if (!sender.isValid() || !receiver.isValid())
    {
        std::cerr   << "Failed to initialize CAN sockets"
                    << std::endl;
        return 1;
    }

    uint32_t invalidCanId = 0;
    uint8_t invalidDataLength = 0;

    if (receiver.receive(
        invalidCanId,
        nullptr,
        invalidDataLength,
        0))
    {
        std::cerr   << "receive() accepted a null data buffer"
                    << std::endl;
        return 1;
    }

    uint8_t smallData[4] {};
    uint32_t smallBufferCanId = 0;
    uint8_t smallBufferDataLength = 0;

    if (receiver.receive(
        smallBufferCanId,
        smallData,
        smallBufferDataLength,
        sizeof(smallData)))
    {
        std::cerr   << "receive() accepted an undersized data buffer"
                    << std::endl;
        return 1;
    }

    CanSocket invalidSocket("invalid_can_interface");

    if (invalidSocket.isValid())
    {
        std::cerr   << "Invalid CAN interface was accepted"
                    << std::endl;
        return 1;
    }

    uint32_t invalidSocketCanId = 0;
    uint8_t invalidSocketData[8] {};
    uint8_t invalidSocketDataLength = 0;

    if (invalidSocket.receive(
        invalidSocketCanId,
        invalidSocketData,
        invalidSocketDataLength,
        sizeof(invalidSocketData)))
    {
        std::cerr   << "receive() accepted an invalid socket"
                    << std::endl;
        return 1;
    }

    if (sender.send(
        EngineCanMessage::CAN_ID,
        nullptr,
        1))
    {
        std::cerr   << "send() accepted a null data buffer"
                    << std::endl;
        return 1;
    }

    uint8_t data[8] {};

    EngineCanMessage::encode(
        800.0,
        90.0,
        data);
    
   

    if (!sender.send(
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

    if (!receiver.receive(
        receivedCanId,
        receivedData,
        receivedDataLength,
        sizeof(receivedData)))
    {
        std::cerr   << "Failed to receive CAN frame"
                    << std::endl;
        return 1;
    }

    if (receivedCanId != EngineCanMessage::CAN_ID)
    {
        std::cerr   << "Received unexpected CAN ID: 0x"
                    << std::hex
                    << receivedCanId
                    << std::dec
                    << std::endl;
        return 1;
    }

    if (receivedDataLength != sizeof(receivedData))
    {
        std::cerr   << "Received unexpected data length: "
                    << static_cast<int>(receivedDataLength)
                    << std::endl;
        return 1;
    }

     double decodeRpm = 0.0;
    double decodeTemperature = 0.0;

    if (!EngineCanMessage::decode(
        receivedData,
        receivedDataLength,
        decodeRpm,
        decodeTemperature))
    {
        std::cerr   << "Failed to decode CAN message"
                    << std::endl;
        return 1;
    }

    if (decodeRpm != 800.0)
    {
        std::cerr   << "Decoded RPM does not match expected value: "
                    << decodeRpm
                    << std::endl;
        return 1;
    }

    if (decodeTemperature != 90.0)
    {
        std::cerr   << "Decoded temperature does not match expected value: "
                    << decodeTemperature
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