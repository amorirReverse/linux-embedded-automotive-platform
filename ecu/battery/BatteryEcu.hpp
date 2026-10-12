// ecu/battery/BatteryEcu.hpp

#pragma once

#include "Battery.hpp"
#include "CanSocket.hpp"

#include <csignal>
#include <chrono>

/**
 * @brief Represents the battery electronic control unit.
 *
 * The BatteryEcu class manages the battery model,
 * executes its simulation cycle, and publishes
 * battery status messages over CAN.
 */
class BatteryEcu
{
public:
    /**
     * @brief Creates a battery ECU.
     *
     * The ECU uses the specified CAN interface
     * to transmit battery status messages.
     *
     * @param canInterface CAN interface name.
     */
    explicit BatteryEcu(const char *canInterface);

    /**
     * @brief Runs the battery ECU simulation.
     *
     * @param cycleCount The number of simulation cycles to run.
     */
    void run(int cycleCount);

    /**
     * @brief Runs the battery ECU continuously.
     */
    void run();

private:
    CanSocket canSocket_;
    Battery battery_;

    /**
     * @brief Maximum allowed duration for one simulation cycle.
     */
    static constexpr std::chrono::seconds WATCHDOG_TIMEOUT{2};

    /**
     * @brief Time when the last simulation cycle completed.
     */
    std::chrono::steady_clock::time_point lastCycleCompleted_;

    /**
     * @brief Runs the battery ECU continuously.
     */
    void processCycle();

    /**
     * @brief Handles process termination signals.
     *
     * @param signal Signal number.
     */
    static void handleSignal(int signal);

    /**
     * @brief Indicates whether the ECU should stop.
     */
    static volatile std::sig_atomic_t stopRequested_;
};