// ecu/battery/BatteryEcu.hpp

#pragma once

#include "Battery.hpp"

/**
 * @brief Represents the battery electronic control unit.
 * 
 * The BatteryEcu class manages the battery model
 * and executes its simulation cycle.
 */
class BatteryEcu
{
public:
    /**
     * @brief Creates a battery ECU.
     */
    BatteryEcu();

    /**
     * @brief Runs the battery ECU simulation.
     * 
     * @param cycleCount The number of simulation cycles to run.
     */
    void run(int cycleCount);

private:
    Battery battery_;
};