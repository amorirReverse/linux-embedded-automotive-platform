// ecu/battery/BatteryEcu.hpp

#pragma once

#include "Battery.hpp"
#include "CanSocket.hpp"

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
    explicit BatteryEcu(const char* canInterface);

    /**
     * @brief Runs the battery ECU simulation.
     * 
     * @param cycleCount The number of simulation cycles to run.
     */
    void run(int cycleCount);

private:
    CanSocket canSocket_;
    Battery battery_;
};