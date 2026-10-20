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
#include <utility>

CanSocket::CanSocket(const std::string &interfaceName)
    : socketFd_(-1),
      ownsSocket_(true),
      writeFunction_(
          [](int fd, const void *buf, size_t size)
          {
              return write(fd, buf, size);
          }),
      pollFunction_(
          [](struct pollfd *fds, nfds_t nfds, int timeoutMs)
          {
              return poll(fds, nfds, timeoutMs);
          }),
      readFunction_(
          [](int fd, void *buf, size_t size)
          {
              return read(fd, buf, size);
          })
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
    if (ownsSocket_ && socketFd_ >= 0)
    {
        close(socketFd_);
    }
}

CanSocket::CanSocket(int socketFd,
                     WriteFunction writeFunction,
                     PollFunction pollFunction,
                     ReadFunction readFunction)
    : socketFd_(socketFd),
      ownsSocket_(false),

      writeFunction_(std::move(writeFunction)),
      pollFunction_(std::move(pollFunction)),
      readFunction_(std::move(readFunction))
{
}

bool CanSocket::send(
    uint32_t canId,
    const uint8_t *data,
    uint8_t dataLength)
{
    if (socketFd_ < 0 || !writeFunction_ || data == nullptr || dataLength > CAN_MAX_DLEN)
    {
        return false;
    }

    struct can_frame frame{};

    frame.can_id = canId;
    frame.len = dataLength;

    std::memcpy(frame.data, data, dataLength);

    ssize_t bytesSent;
    do
    {
        bytesSent = writeFunction_(
            socketFd_,
            &frame,
            sizeof(frame));
    } while (bytesSent < 0 && errno == EINTR);

    if (bytesSent < 0)
    {
        Logger::error(
            "Failed to send CAN frame: " +
            std::string(std::strerror(errno)));

        return false;
    }

    if (bytesSent != sizeof(frame))
    {
        Logger::error("Incomplete CAN frame write");

        return false;
    }

    return true;
}

bool CanSocket::receive(
    uint32_t &canId,
    uint8_t *data,
    uint8_t &dataLength,
    uint8_t dataCapacity,
    ReceiveError &error)
{
    return receive(
        canId,
        data,
        dataLength,
        dataCapacity,
        -1,
        error);
}

bool CanSocket::receive(
    uint32_t &canId,
    uint8_t *data,
    uint8_t &dataLength,
    uint8_t dataCapacity,
    int timeoutMs,
    ReceiveError &error)
{
    error = ReceiveError::NONE;
    dataLength = 0;

    if (socketFd_ < 0 || !readFunction_ || !pollFunction_ || data == nullptr)
    {
        error = ReceiveError::INVALID_ARGUMENT;
        return false;
    }

    if (timeoutMs < -1)
    {
        error = ReceiveError::INVALID_ARGUMENT;
        return false;
    }

    struct can_frame frame{};
    pollfd descriptor{};
    descriptor.fd = socketFd_;
    descriptor.events = POLLIN;

    int pollResult;

    do
    {
        descriptor.revents = 0;
        pollResult = pollFunction_(&descriptor, 1, timeoutMs);
    } while (pollResult < 0 && errno == EINTR);

    if (pollResult == 0)
    {
        error = ReceiveError::RECEIVE_TIMEOUT;
        return false;
    }

    if (pollResult < 0)
    {
        Logger::error("CAN socket poll failed" +
                      std::string(std::strerror(errno)));
        error = ReceiveError::SYSTEM_ERROR;
        return false;
    }

    if ((descriptor.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0)
    {
        Logger::error("CAN socket poll reported an error event: " +
                      std::to_string(descriptor.revents));
        error = ReceiveError::SYSTEM_ERROR;
        return false;
    }

    if ((descriptor.revents & POLLIN) == 0)
    {
        Logger::warning("CAN socket has no readable data");
        error = ReceiveError::SYSTEM_ERROR;
        return false;
    }

    ssize_t bytesRead;

    do
    {
        bytesRead = readFunction_(
            socketFd_,
            &frame,
            sizeof(frame));
    } while (bytesRead < 0 && errno == EINTR);

    if (bytesRead < 0)
    {
        error = ReceiveError::SYSTEM_ERROR;
        return false;
    }

    if (bytesRead != sizeof(frame))
    {
        error = ReceiveError::INVALID_FRAME;
        return false;
    }

    if (frame.len > CAN_MAX_DLEN)
    {
        Logger::error("Received CAN frame has an invalid payload length");
        error = ReceiveError::INVALID_FRAME;
        return false;
    }

    if (dataCapacity < frame.len)
    {
        Logger::warning("CAN receive buffer is too small for the frame");
        error = ReceiveError::BUFFER_TOO_SMALL;
        dataLength = frame.len;
        return false;
    }

    canId = frame.can_id;
    dataLength = frame.len;

    std::memcpy(data, frame.data, dataLength);

    return true;
}