// can/CanSocket.hpp

#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Provides access to a Linux SocketCAN interface.
 * 
 * The CanSocket class opens a CAN_RAW socket and binds it
 * to a specific Linux CAN network interface.
 */
class CanSocket
{
public:
    /**
     * @brief Creates a CAN socket.
     * 
     * @param interfaceName Linux CAN interface name, for example "vcan0".
     */
    explicit  CanSocket(const std::string& interfaceName);

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
    bool send(uint32_t canId, const uint8_t* data, uint8_t dataLength);

    /**
     * @brief Receives a CAN data frame.
     * 
     * @param canId Output CAN frame identifier.
     * @param data Output buffer containing the payload data.
     * @param dataLength Output number of payload bytes.
     *
     * @return True if a frame was received successfully, false otherwise.
     */
    bool receive(
        uint32_t& canId,
        uint8_t* data,
        uint8_t& dataLength);

private:
    int socketFd_;
};