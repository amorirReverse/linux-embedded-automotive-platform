// can/CanSocket.cpp

#include "CanSocket.hpp"
#include "Logger.hpp"

#include <linux/can.h>
#include <linux/can/raw.h>

#include <cstring>
#include <iostream>
#include <cerrno>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

CanSocket::CanSocket(const std::string &interfaceName)
    : socketFd_(-1)
{
    socketFd_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (socketFd_ < 0)
    {
        std::cerr << "Failed to create CAN socket: "
                  << std::strerror(errno)
                  << std::endl;
        return;
    }

    struct ifreq interfaceRequest{};
    std::strncpy(
        interfaceRequest.ifr_name,
        interfaceName.c_str(),
        IFNAMSIZ - 1);

    if (ioctl(socketFd_, SIOCGIFINDEX, &interfaceRequest) < 0)
    {
        std::cerr << "Failed to get CAN interface index: "
                  << std::strerror(errno)
                  << std::endl;

        close(socketFd_);
        socketFd_ = -1;
        return;
    }

    struct sockaddr_can address{};
    address.can_family = AF_CAN;
    address.can_ifindex = interfaceRequest.ifr_ifindex;

    if (bind(
            socketFd_,
            reinterpret_cast<struct sockaddr *>(&address),
            sizeof(address)) < 0)
    {
        std::cerr << "Failed to bind CAN socket: "
                  << std::strerror(errno)
                  << std::endl;
        close(socketFd_);
        socketFd_ = -1;
    }
}

bool CanSocket::isValid() const
{
    return socketFd_ >= 0;
}

CanSocket::~CanSocket()
{
    if (socketFd_ >= 0)
    {
        close(socketFd_);
    }
}

bool CanSocket::send(
    uint32_t canId,
    const uint8_t *data,
    uint8_t dataLength)
{
    if (socketFd_ < 0 || data == nullptr || dataLength > CAN_MAX_DLEN)
    {
        return false;
    }

    struct can_frame frame{};

    frame.can_id = canId;
    frame.len = dataLength;

    std::memcpy(frame.data, data, dataLength);

    const ssize_t bytesSent = write(
        socketFd_,
        &frame,
        sizeof(frame));

    if (bytesSent != sizeof(frame))
    {
        Logger::error(
            "Failed to send CAN frame: " +
            std::string(std::strerror(errno)));

        return false;
    }

    return true;
}

bool CanSocket::receive(
    uint32_t &canId,
    uint8_t *data,
    uint8_t &dataLength,
    uint8_t dataCapacity)
{
    return receive(
        canId,
        data,
        dataLength,
        dataCapacity,
        -1);
}

bool CanSocket::receive(
    uint32_t &canId,
    uint8_t *data,
    uint8_t &dataLength,
    uint8_t dataCapacity,
    int timeoutMs)
{
    if (socketFd_ < 0 || data == nullptr)
    {
        return false;
    }

    struct can_frame frame{};
    pollfd descriptor{};
    descriptor.fd = socketFd_;
    descriptor.events = POLLIN;

    const int pollResult = poll(&descriptor, 1, timeoutMs);

    if (pollResult == 0)
    {
        return false;
    }

    if (pollResult < 0)
    {
        if (errno == EINTR)
        {
            return false;
        }

        Logger::error("CAN socket poll failed");
        return false;
    }

    if ((descriptor.revents & POLLIN) == 0)
    {
        Logger::error("CAN socket is not ready for reading");
        return false;
    }

    const ssize_t bytesRead = read(
        socketFd_,
        &frame,
        sizeof(frame));

    if (bytesRead != sizeof(frame))
    {
        Logger::error(
            "Failed to read CAN frame: " +
            std::string(std::strerror(errno)));

        return false;
    }

    if (frame.len > CAN_MAX_DLEN)
    {
        return false;
    }

    if (dataCapacity < frame.len)
    {
        return false;
    }

    canId = frame.can_id;
    dataLength = frame.len;

    std::memcpy(data, frame.data, dataLength);

    return true;
}