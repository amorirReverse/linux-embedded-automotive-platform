// can/test_can.cpp

#include "CanSocket.hpp"
#include "EngineCanMessage.hpp"

#include <cstdint>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <linux/can.h>

/**
 * @brief Entry point for the CAN socket test.
 *
 * @return Zero on success, non-zero on failure.
 */

// TODO: Refactor main (fonction)
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
    CanSocket::ReceiveError error = CanSocket::ReceiveError::NONE;

    // Validate receive input buffer
    uint32_t invalidCanId = 0;
    uint8_t invalidDataLength = 0xFF;

    if (receiver.receive(
            invalidCanId,
            nullptr,
            invalidDataLength,
            0,
            error))
    {
        std::cerr << "receive() accepted a null data buffer"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::INVALID_ARGUMENT)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
                  << std::endl;
        return 1;
    }

    // Validate receive buffer capacity
    uint8_t smallBufferTestData[8]{};

    if (!sender.send(
            EngineCanMessage::CAN_ID,
            smallBufferTestData,
            sizeof(smallBufferTestData)))
    {
        std::cerr << "Failed to send CAN frame for small buffer test"
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
            sizeof(smallData),
            error))
    {
        std::cerr << "receive() accepted an undersized data buffer"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::BUFFER_TOO_SMALL)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
                  << std::endl;
        return 1;
    }

    // Validate invalid CAN socket
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
            sizeof(invalidSocketData),
            error))
    {
        std::cerr << "receive() accepted an invalid socket"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::INVALID_ARGUMENT)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
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

    // Validate send input buffer
    if (sender.send(
            EngineCanMessage::CAN_ID,
            nullptr,
            1))
    {
        std::cerr << "send() accepted a null data buffer with non-zero length"
                  << std::endl;
        return 1;
    }

    // Validate null data buffer with zero length
    if (sender.send(
            EngineCanMessage::CAN_ID,
            nullptr,
            0))
    {
        std::cerr << "send() accepted a null data buffer"
                  << std::endl;
        return 1;
    }

    // Validate zero-length CAN frame
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
            sizeof(zeroLengthData),
            error))
    {
        std::cerr << "Failed to receive zero-length CAN frame"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::NONE)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
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

    // Validate payload length
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

    // Validate engine CAN message transmission
    if (!EngineCanMessage::encode(
            800.0,
            90.0,
            data))
    {
        std::cerr << "Failed to encode engine CAN message"
                  << std::endl;
        return 1;
    }

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
            sizeof(receivedData),
            error))
    {
        std::cerr << "Failed to receive CAN frame"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::NONE)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
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

    // Validate reception into a larger buffer
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
            sizeof(largeBuffer),
            error))
    {
        std::cerr << "Failed to receive CAN frame into a larger buffer"
                  << std::endl;
        return 1;
    }

    if (error != CanSocket::ReceiveError::NONE)
    {
        std::cerr << "receive() returned unexpected error code: "
                  << static_cast<int>(error)
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

    // TESTS MOCKS

    // Test: EINTR is retried and the second write succeeds.
    {
        int attempts = 0;

        CanSocket mockWriteEintrSocket(
            42,
            [&attempts](int, const void *, size_t size) -> ssize_t
            {
                ++attempts;

                if (attempts == 1)
                {
                    errno = EINTR;
                    return -1;
                }

                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int) -> int
            {
                return 0;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        const uint8_t payload[] = {0x11, 0x22};

        if (!mockWriteEintrSocket.send(0x123, payload, sizeof(payload)) || attempts != 2)
        {
            std::cerr << "FAIL [MOCK]: EINTR retry test" << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: EINTR retry test" << std::endl;
    }

    // Test: a permanent write error returns false.
    {
        CanSocket mockWriteEioSocket(
            42,
            [](int, const void *, size_t) -> ssize_t
            {
                errno = EIO;
                return -1;
            },
            [](struct pollfd *fds, nfds_t nfds, int) -> int
            {
                return 0;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        const uint8_t payload[] = {0x11, 0x22};

        if (mockWriteEioSocket.send(0x123, payload, sizeof(payload)))
        {
            std::cerr << "FAIL [MOCK]: EIO error test" << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: EIO error test" << std::endl;
    }

    // Test: a partial frame write returns false.
    {
        CanSocket mockPartialWriteSocket(
            42,
            [](int, const void *, size_t) -> ssize_t
            {
                return 1;
            },
            [](struct pollfd *fds, nfds_t nfds, int) -> int
            {
                return 0;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        const uint8_t payload[] = {0x11, 0x22};

        if (mockPartialWriteSocket.send(0x123, payload, sizeof(payload)))
        {
            std::cerr << "FAIL [MOCK]: partial write test" << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: partial write test" << std::endl;
    }

    // Test: poll() interrupted by EINTR is retried

    {
        int pollAttempts = 0;

        CanSocket mockPollEintrSocket(
            42,
            [](int, const void *, size_t) -> ssize_t
            {
                return 1;
            },
            [&pollAttempts](
                struct pollfd *fds,
                nfds_t nfds,
                int) -> int
            {
                ++pollAttempts;

                if (pollAttempts == 1)
                {
                    errno = EINTR;
                    return -1;
                }
                fds[0].revents = POLLIN;

                return 1;
            },
            [](int, void *buffer, size_t size) -> ssize_t
            {
                struct can_frame frame{};
                frame.can_id = 0x123;
                frame.len = 2;
                frame.data[0] = 0x11;
                frame.data[1] = 0x22;

                if (size < sizeof(frame))
                {
                    errno = EIO;
                    return -1;
                }

                std::memcpy(buffer, &frame, sizeof(frame));
                return sizeof(frame);
            });

        uint32_t canId = 0;
        uint8_t payload[8]{};
        uint8_t dataLength = 0;

        const bool receiveSucceeded = mockPollEintrSocket.receive(
            canId,
            payload,
            dataLength,
            sizeof(payload),
            100,
            error);

        if (!receiveSucceeded)
        {
            std::cerr << "FAIL [MOCK]: poll EINTR retry test" << std::endl;
            return 1;
        }

        if (pollAttempts != 2)
        {
            std::cerr << "FAIL [MOCK]: poll EINTR retry test received unexpected data"
                      << std::endl;
            return 1;
        }

        if (canId != 0x123)
        {
            std::cerr << "FAIL [MOCK]: poll EINTR retry test received unexpected CAN ID: "
                      << std::hex
                      << canId
                      << std::dec
                      << std::endl;
            return 1;
        }

        if (dataLength != 2)
        {
            std::cerr << "FAIL [MOCK]: poll EINTR retry test received unexpected data length: "
                      << static_cast<int>(dataLength)
                      << std::endl;
            return 1;
        }

        if (payload[0] != 0x11 || payload[1] != 0x22)
        {
            std::cerr << "FAIL [MOCK]: poll EINTR retry test received unexpected data: "
                      << std::hex
                      << static_cast<int>(payload[0])
                      << " "
                      << static_cast<int>(payload[1])
                      << std::dec
                      << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::NONE)
        {
            std::cerr << "FAIL [MOCK]: unexpected error code after successful receive: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: poll EINTR retry test" << std::endl;
    }

    // test timeout
    {

        CanSocket mockTimeoutSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                return 0;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        uint32_t canId = 0;
        uint8_t payload[8]{};
        uint8_t payloadLength = 2;

        if (mockTimeoutSocket.receive(
                canId,
                payload,
                payloadLength,
                sizeof(payload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: timeout test" << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::RECEIVE_TIMEOUT)
        {
            std::cerr << "FAIL [MOCK]: timeout test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }

        if (payloadLength != 0)
        {
            std::cerr << "FAIL [MOCK]: timeout test returned unexpected payload length: "
                      << static_cast<int>(payloadLength)
                      << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: timeout test" << std::endl;
    }

    // test poll EIO
    {
        CanSocket mockPollSystemErrorSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                errno = EIO;
                return -1;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        uint32_t receivedCanId = 0;
        uint8_t receivedPayload[8]{};
        uint8_t receivedPayloadLength = 0;

        if (mockPollSystemErrorSocket.receive(
                receivedCanId,
                receivedPayload,
                receivedPayloadLength,
                sizeof(receivedPayload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: poll EIO test" << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::SYSTEM_ERROR)
        {
            std::cerr << "FAIL [MOCK]: poll EIO test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }
        std::cout << "PASS [MOCK]: poll EIO test" << std::endl;
    }

    // test Buffer too small
    {
        CanSocket mockBufferTooSmallSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                fds[0].revents = POLLIN;
                return 1;
            },
            [](int, void *buffer, size_t size) -> ssize_t
            {
                struct can_frame frame{};
                frame.can_id = 0x123;
                frame.len = 4;
                frame.data[0] = 0x11;
                frame.data[1] = 0x22;
                frame.data[2] = 0x33;
                frame.data[3] = 0x44;

                if (size < sizeof(frame))
                {
                    errno = EIO;
                    return -1;
                }

                std::memcpy(buffer, &frame, sizeof(frame));
                return sizeof(frame);
            });

        uint32_t receivedCanId = 0;
        uint8_t payload[2]{};
        uint8_t receivedLength = 0;

        if (mockBufferTooSmallSocket.receive(
                receivedCanId,
                payload,
                receivedLength,
                sizeof(payload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: buffer too small test" << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::BUFFER_TOO_SMALL)
        {
            std::cerr << "FAIL [MOCK]: buffer too small test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }

        if (receivedLength != 4)
        {
            std::cerr << "FAIL [MOCK]: buffer too small test returned unexpected received length: "
                      << static_cast<int>(receivedLength)
                      << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: buffer too small test" << std::endl;
    }

    // test read error
    {
        CanSocket mockReadSystemErrorSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                fds[0].revents = POLLIN;
                return 1;
            },
            [](int, void *, size_t) -> ssize_t
            {
                errno = EIO;
                return -1;
            });

        uint32_t receivedCanId = 0;
        uint8_t payload[8]{};
        uint8_t receivedLength = 2;

        if (mockReadSystemErrorSocket.receive(
                receivedCanId,
                payload,
                receivedLength,
                sizeof(payload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: read error test" << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::SYSTEM_ERROR)
        {
            std::cerr << "FAIL [MOCK]: read error test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }

        if (receivedLength != 0)
        {
            std::cerr << "FAIL [MOCK]: read error test returned unexpected received length: "
                      << static_cast<int>(receivedLength)
                      << std::endl;
            return 1;
        }
        std::cout << "PASS [MOCK]: read error test" << std::endl;
    }

    // test read invalid frame
    {
        CanSocket mockInvalidFrameSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                fds[0].revents = POLLIN;
                return 1;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        uint32_t receivedCanId = 0;
        uint8_t payload[8]{};
        uint8_t receivedLength = 0;

        if (mockInvalidFrameSocket.receive(
                receivedCanId,
                payload,
                receivedLength,
                sizeof(payload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: invalid frame test" << std::endl;
            return 1;
        }

        if (error != CanSocket::ReceiveError::INVALID_FRAME)
        {
            std::cerr << "FAIL [MOCK]: invalid frame test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }

        std::cout << "PASS [MOCK]: invalid frame test" << std::endl;
    }

    // test POLLERR
    {
        CanSocket mockPollErrorEventSocket(
            42,
            [](int, const void *, size_t size) -> ssize_t
            {
                return static_cast<ssize_t>(size);
            },
            [](struct pollfd *fds, nfds_t nfds, int)
                -> int
            {
                fds[0].revents = POLLERR;
                return 1;
            },
            [](int, void *, size_t) -> ssize_t
            {
                return 1;
            });

        uint32_t receivedCanId = 0;
        uint8_t payload[8]{};
        uint8_t receivedLength = 0;

        if (mockPollErrorEventSocket.receive(
                receivedCanId,
                payload,
                receivedLength,
                sizeof(payload),
                100,
                error))
        {
            std::cerr << "FAIL [MOCK]: POLLERR test" << std::endl;
            return 1;
        }
        if (error != CanSocket::ReceiveError::SYSTEM_ERROR)
        {
            std::cerr << "FAIL [MOCK]: POLLERR test returned unexpected error code: "
                      << static_cast<int>(error)
                      << std::endl;
            return 1;
        }
        std::cout << "PASS [MOCK]: POLLERR test" << std::endl;
    }
    return 0;
}
