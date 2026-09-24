// ecu/battery/BatteryEcu.cpp

#include "BatteryEcu.hpp"

#include <chrono>
#include <iostream>
#include <thread>

BatteryEcu::BatteryEcu()
    : battery_()
{
}

void BatteryEcu::run(int cycleCount)
{
    constexpr auto simulationPeriod = std::chrono::milliseconds(100);
    constexpr double simulationDeltaTime = 0.1;

    for (int step = 0; step < cycleCount; ++step)
    {
        battery_.update(simulationDeltaTime);

        std::cout << "Battery Voltage: "
                  << battery_.getVoltage()
                  << " V"
                  << " | Current: "
                  << battery_.getCurrent()
                  << " A" 
                  << "| State of Charge: "
                  << battery_.getStateOfCharge()
                  << " %"
                  << std::endl;

        std::this_thread::sleep_for(simulationPeriod);
    }
}