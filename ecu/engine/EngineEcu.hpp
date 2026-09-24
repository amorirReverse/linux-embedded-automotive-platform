// ecu/engine/EngineEcu.hpp

#pragma once

#include "Engine.hpp"
#include "CanSocket.hpp"

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
    explicit EngineEcu(const char* canInterface);

    /**
     * @brief Runs the engine ECU simulation.
     * 
     * The simulation executes periodically until the
     * configured number of cycles has been completed
     * 
     * @param cycleCount Number of simulation cycles to execute.
     */
    void run(int cycleCount);

private:
    CanSocket canSocket_;
    Engine engine_;
};