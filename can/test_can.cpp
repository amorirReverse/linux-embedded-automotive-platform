// can/test_can.cpp

#include "CanSocket.hpp"
#include "EngineCanMessage.hpp"

#include <cstdint>
#include <iostream>
#include <cstring>

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
        std::cerr << "Failed to initialize CAN sockets"
                  << std::endl;
        return 1;
    }

    uint32_t invalidCanId = 0;
    uint8_t invalidDataLength = 0xFF;

    if (receiver.receive(
            invalidCanId,
            nullptr,
            invalidDataLength,
            0))
    {
        std::cerr << "receive() accepted a null data buffer"
                  << std::endl;
        return 1;
    }

    uint8_t smallData[4]{};
    uint32_t smallBufferCanId = 0;
    uint8_t smallBufferDataLength = 0xFF;

    if (receiver.receive(
            smallBufferCanId,
            smallData,
            smallBufferDataLength,
            sizeof(smallData)))
    {
        std::cerr << "receive() accepted an undersized data buffer"
                  << std::endl;
        return 1;
    }

    CanSocket invalidSocket("invalid_can_interface");

    if (invalidSocket.isValid())
    {
        std::cerr << "Invalid CAN interface was accepted"
                  << std::endl;
        return 1;
    }

    uint32_t invalidSocketCanId = 0;
    uint8_t invalidSocketData[8]{};
    uint8_t invalidSocketDataLength = 0xFF;

    if (invalidSocket.receive(
            invalidSocketCanId,
            invalidSocketData,
            invalidSocketDataLength,
            sizeof(invalidSocketData)))
    {
        std::cerr << "receive() accepted an invalid socket"
                  << std::endl;
        return 1;
    }

    if (invalidSocket.send(
            EngineCanMessage::CAN_ID,
            invalidSocketData,
            sizeof(invalidSocketData)))
    {
        std::cerr << "send() accepted an invalid socket"
                  << std::endl;
        return 1;
    }

    if (sender.send(
            EngineCanMessage::CAN_ID,
            nullptr,
            1))
    {
        std::cerr << "send() accepted a null data buffer with non-zero length"
                  << std::endl;
        return 1;
    }

    uint8_t data[8]{};

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            data,
            0))
    {
        std::cerr << "Failed to send zero-length CAN frame"
                  << std::endl;
        return 1;
    }

    uint32_t zeroLengthCanId = 0;
    uint8_t zeroLengthData[8]{};
    uint8_t zeroLengthDataLength = 0;

    if (!receiver.receive(
            zeroLengthCanId,
            zeroLengthData,
            zeroLengthDataLength,
            sizeof(zeroLengthData)))
    {
        std::cerr << "Failed to receive zero-length CAN frame"
                  << std::endl;
        return 1;
    }

    if (zeroLengthDataLength != 0)
    {
        std::cerr << "Zero-length CAN frame reported unexpected data length"
                  << std::endl;
        return 1;
    }

    if (zeroLengthCanId != EngineCanMessage::CAN_ID)
    {
        std::cerr << "Zero-length CAN frame received unexpected CAN ID"
                  << std::endl;
        return 1;
    }

    if (std::memcmp(zeroLengthData, data, 0) != 0)
    {
        std::cerr << "Zero-length CAN frame received unexpected data"
                  << std::endl;
        return 1;
    }
    uint8_t oversizedData[9]{};

    if (sender.send(
            EngineCanMessage::CAN_ID,
            oversizedData,
            sizeof(oversizedData)))
    {
        std::cerr << "send() accepted a payload larger than CAN_MAX_DLEN"
                  << std::endl;
        return 1;
    }

    EngineCanMessage::encode(
        800.0,
        90.0,
        data);

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            data,
            sizeof(data)))
    {
        std::cerr << "Failed to send CAN frame"
                  << std::endl;
        return 1;
    }

    std::cout << "CAN frame sent successfully"
              << std::endl;

    uint32_t receivedCanId = 0;
    uint8_t receivedData[8]{};
    uint8_t receivedDataLength = 0;

    if (!receiver.receive(
            receivedCanId,
            receivedData,
            receivedDataLength,
            sizeof(receivedData)))
    {
        std::cerr << "Failed to receive CAN frame"
                  << std::endl;
        return 1;
    }

    if (receivedCanId != EngineCanMessage::CAN_ID)
    {
        std::cerr << "Received unexpected CAN ID: 0x"
                  << std::hex
                  << receivedCanId
                  << std::dec
                  << std::endl;
        return 1;
    }

    if (receivedDataLength != sizeof(receivedData))
    {
        std::cerr << "Received unexpected data length: "
                  << static_cast<int>(receivedDataLength)
                  << std::endl;
        return 1;
    }

    if (std::memcmp(receivedData, data, sizeof(data)) != 0)
    {
        std::cerr << "Received CAN frame contains corrupted data"
                  << std::endl;
        return 1;
    }

    uint8_t largeBuffer[16]{};
    uint32_t largeBufferCanId = 0;
    uint8_t largeBufferDataLength = 0;

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            data,
            sizeof(data)))
    {
        std::cerr << "Failed to send CAN frame for large buffer test"
                  << std::endl;
        return 1;
    }

    if (!receiver.receive(
            largeBufferCanId,
            largeBuffer,
            largeBufferDataLength,
            sizeof(largeBuffer)))
    {
        std::cerr << "Failed to receive CAN frame into a larger buffer"
                  << std::endl;
        return 1;
    }

    if (largeBufferCanId != EngineCanMessage::CAN_ID)
    {
        std::cerr << "Large buffer test received unexpected CAN ID"
                  << std::endl;
        return 1;
    }

    if (largeBufferDataLength != sizeof(data))
    {
        std::cerr << "Large buffer test received unexpected data length"
                  << std::endl;
        return 1;
    }

    if (largeBufferDataLength > sizeof(largeBuffer))
    {
        std::cerr << "Large buffer test reported an invalid data length"
                  << std::endl;
        return 1;
    }

    if (std::memcmp(largeBuffer, data, sizeof(data)) != 0)
    {
        std::cerr << "Large buffer test received corrupted CAN data"
                  << std::endl;
        return 1;
    }

    for (std::size_t index = sizeof(data);
         index < sizeof(largeBuffer);
         ++index)
    {
        if (largeBuffer[index] != 0)
        {
            std::cerr << "Large buffer test modified data beyond CAN payload"
                      << std::endl;
            return 1;
        }
    }

    double decodeRpm = 0.0;
    double decodeTemperature = 0.0;

    if (!EngineCanMessage::decode(
            receivedData,
            receivedDataLength,
            decodeRpm,
            decodeTemperature))
    {
        std::cerr << "Failed to decode CAN message"
                  << std::endl;
        return 1;
    }

    if (decodeRpm != 800.0)
    {
        std::cerr << "Decoded RPM does not match expected value: "
                  << decodeRpm
                  << std::endl;
        return 1;
    }

    if (decodeTemperature != 90.0)
    {
        std::cerr << "Decoded temperature does not match expected value: "
                  << decodeTemperature
                  << std::endl;
        return 1;
    }

    std::cout << "CAN message received:"
              << " ID=0x"
              << std::hex
              << receivedCanId
              << std::dec
              << " DLC="
              << static_cast<int>(receivedDataLength)
              << std::endl;

    std::cout << "Decoded CAN message:"
              << " RPM="
              << decodeRpm
              << " tr/min"
              << " Temperature="
              << decodeTemperature
              << " °C"
              << std::endl;

    return 0;
}