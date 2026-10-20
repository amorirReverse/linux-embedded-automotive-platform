// can/CanSocket.hpp

#pragma once

#include <cstdint>
#include <string>
#include <poll.h>
#include <sys/types.h>
#include <functional>

/**
 * @brief Provides access to a Linux SocketCAN interface.
 *
 * The CanSocket class opens a CAN_RAW socket and binds it
 * to a specific Linux CAN network interface.
 */
class CanSocket
{
public:
    enum class ReceiveError
    {
        NONE,             // no error
        RECEIVE_TIMEOUT,  // no data received within the time limit
        SYSTEM_ERROR,     // system failure
        BUFFER_TOO_SMALL, // destination buffer too small
        INVALID_ARGUMENT, // invalid parameters or initial state;
        INVALID_FRAME,    // incomplete frame or invalid payload length
    };

    /**
     * @brief Creates a CAN socket.
     *
     * @param interfaceName Linux CAN interface name, for example "vcan0".
     */
    explicit CanSocket(const std::string &interfaceName);

    using WriteFunction =
        std::function<ssize_t(int, const void *, size_t)>;

    using PollFunction =
        std::function<int(struct pollfd *, nfds_t, int)>;

    using ReadFunction =
        std::function<ssize_t(int, void *, size_t)>;

    /**
     * @brief Creates a CAN socket with a custom write function.
     *
     * @param socketFd The file descriptor of the CAN socket.
     * @param writeFunction The function to use for writing to the socket.
     * @param pollFunction The function to use for polling the socket.
     * @param readFunction The function to use for reading from the socket.
     */
    CanSocket(
        int socketFd,
        WriteFunction writeFunction,
        PollFunction pollFunction,
        ReadFunction readFunction);

    CanSocket(const CanSocket &) = delete;
    CanSocket &operator=(const CanSocket &) = delete;

    /**
     * @brief Destroys the CAN socket.
     */
    ~CanSocket();

    /**
     * @brief Sends a CAN data frame.
     *
     * @param canId CAN frame identifier.
     * @param data Pointer to the payload data.
     * @param dataLength Number of payload bytes.
     *
     * @return True if the frame was sent successfully, false otherwise.
     */
    bool send(uint32_t canId, const uint8_t *data, uint8_t dataLength);

    /**
     * @brief Receive a CAN frame with a timeout.
     *
     * @param canId Output CAN frame identifier.
     * @param data Output payload buffer.
     * @param dataLength Output payload length.
     * @param dataCapacity Maximun writable payload size.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @param error Output error code.
     *
     * @return True if a frame was received successfully, false on timeout or error.
     */
    bool receive(
        uint32_t &canId,
        uint8_t *data,
        uint8_t &dataLength,
        uint8_t dataCapacity,
        int timeoutMs,
        ReceiveError &error);

    /**
     * @brief Receives a CAN data frame.
     *
     * The function blocks until a CAN frame is received.
     * If the provided data buffer is too small for the received
     * payload, the frame is consumed but no payload data is copied
     * and the function returns false.
     *
     * @param canId Output CAN frame identifier.
     * @param data Output buffer receiving the payload data.
     * @param dataLength Output number of payload bytes.
     * @param dataCapacity Maximum number of bytes that can be written to the data buffer.
     * @param error Output error code.
     *
     * @return True if the frame was received and copied successfully,
     *         false otherwise.
     */
    bool receive(
        uint32_t &canId,
        uint8_t *data,
        uint8_t &dataLength,
        uint8_t dataCapacity,
        ReceiveError &error);

    /**
     * @brief Checks whether the CAN socket is valid.
     *
     * @return True if the socket is ready for communication, false otherwise.
     */
    [[nodiscard]] bool isValid() const;

private:
    int socketFd_;
    bool ownsSocket_;
    WriteFunction writeFunction_;
    PollFunction pollFunction_;
    ReadFunction readFunction_;
};