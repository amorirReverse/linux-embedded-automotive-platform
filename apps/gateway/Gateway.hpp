// apps/gateway/Gateway.hpp

#pragma once

#include "CanSocket.hpp"

#include <csignal>

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
    explicit Gateway(const char *canInterface);

    /**
     * @brief Runs the gateway.
     *
     * @param frameCount Number of CAN frames to receive.
     */
    /**
     * @brief Runs the gateway continuously.
     */
    void run();

    /**
     * @brief Runs the gateway for a fixed number of CAN frames.
     *
     * @param frameCount Number of CAN frames to process.
     */
    void run(int frameCount);

private:
    CanSocket canSocket_;

    /**
     * @brief Processes a received CAN frame.
     */
    void processFrame();

    /**
     * @brief Handles process termination signals.
     *
     * @param signal Signal number.
     */
    static void handleSignal(int signal);

    /**
     * @brief Indicates whether the gateway should stop.
     */
    static volatile std::sig_atomic_t stopRequested_;
};