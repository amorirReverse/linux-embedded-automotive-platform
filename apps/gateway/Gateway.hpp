// apps/gateway/Gateway.hpp

#pragma once

#include "CanSocket.hpp"

/**
 * @brief Represents the gateway ECU.
 *
 * The Gateway class receives CAN frames from the vehicle
 * network and processes them.
 */
class Gateway
{
public:
    /**
     * @brief Creates a gateway.
     *
     * @param canInterface CAN interface name.
     */
    explicit Gateway(const char* canInterface);

    /**
     * @brief Runs the gateway.
     *
     * @param frameCount Number of CAN frames to receive.
     */
    void run(int frameCount);

private:
    CanSocket canSocket_;
};