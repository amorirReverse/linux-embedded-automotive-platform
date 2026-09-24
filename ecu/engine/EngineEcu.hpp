// ecu/engine/EngineEcu.hpp

#pragma once

#include "Engine.hpp"

/**
 * @brief Represents the engine electronic control units.
 * 
 * The EngineEcu class manages the engine model and
 * executes its simulation cycle.
 */

 class EngineEcu
 {
public:
    /**
     * @brief Creates an engine ECU.
     */
    EngineEcu();

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
    Engine engine_;
};