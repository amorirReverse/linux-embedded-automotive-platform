// can/CanSocket.cpp

#include "CanSocket.hpp"

#include <linux/can.h>
#include <linux/can/raw.h>

#include <cstring>
#include <iostream>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

CanSocket::CanSocket(const std::string& interfaceName)
    : socketFd_(-1)
{
    socketFd_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (socketFd_ < 0)
    {
        std::cerr << "Failed to create CAN socket" << std::endl;
        return;
    }

    struct ifreq interfaceRequest {};
    std::strncpy(
        interfaceRequest.ifr_name,
        interfaceName.c_str(),
        IFNAMSIZ - 1);

    if (ioctl(socketFd_, SIOCGIFINDEX, &interfaceRequest) < 0)
    {
        std::cerr << "Failed to get CAN interface index" << std::endl;
        close(socketFd_);
        socketFd_ = -1;
        return;
    }

    struct sockaddr_can address {};
    address.can_family = AF_CAN;
    address.can_ifindex = interfaceRequest.ifr_ifindex;

    if (bind(
            socketFd_,
            reinterpret_cast<struct sockaddr*>(&address),
            sizeof(address)) < 0)
    {
        std::cerr << "Failed to bind CAN socket" << std::endl;
        close(socketFd_);
        socketFd_ = -1;
    }
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
    const uint8_t* data,
    uint8_t dataLength)
{
    if (socketFd_ < 0 || dataLength > CAN_MAX_DLEN)
    {
        return false;
    }

    struct can_frame frame {};

    frame.can_id = canId;
    frame.len = dataLength;

    std::memcpy(frame.data, data, dataLength);

    const ssize_t bytesSent = write(
        socketFd_,
        &frame,
        sizeof(frame));

    return bytesSent == sizeof(frame);
}