// ecu/battery/main.cpp

#include "BatteryEcu.hpp"

/**
 * @brief Entry point of the battery ECU simulation.
 * 
 * @return Zero on successful execution.
 */
int main()
{
    BatteryEcu ecu("vcan0");

    ecu.run(20);

    return 0;
}