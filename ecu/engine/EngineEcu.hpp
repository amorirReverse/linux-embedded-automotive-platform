// ecu/engine/EngineEcu.hpp

#pragma once

#include "Engine.hpp"
#include "CanSocket.hpp"

#include <csignal>
#include <chrono>

/**
 * @brief Represents the engine electronic control units.
 *
 * The EngineEcu class manages the engine model,
 * executes its simulation cycle, and publishes
 * engine status messages over the CAN bus.
 */

class EngineEcu
{
public:
    /**
     * @brief Creates an engine ECU.
     *
     * the ECU uses the specified CAN interface
     * to transit engine status messages.
     *
     * @param canInterface CAN interface name.
     */
    explicit EngineEcu(const char *canInterface);

    /**
     * @brief Runs the engine ECU simulation.
     *
     * The simulation executes periodically until the
     * configured number of cycles has been completed
     *
     * @param cycleCount Number of simulation cycles to execute.
     */
    void run(int cycleCount);

    /**
     * @brief Runs the engine ECU continuously.
     */
    void run();

private:
    CanSocket canSocket_;
    Engine engine_;

    /**
     * @brief Maximum allowed duration for one simulation cycle.
     */
    static constexpr std::chrono::seconds WATCHDOG_TIMEOUT{2};

    /**
     * @brief Time when the last simulation cycle completed.
     */
    std::chrono::steady_clock::time_point lastCycleCompleted_;

    /**
     * @brief Executes one engine ECU simulation cycle.
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