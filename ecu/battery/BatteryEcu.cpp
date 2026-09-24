// ecu/battery/BatteryEcu.cpp

#include "BatteryEcu.hpp"
#include "BatteryCanMessage.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

BatteryEcu::BatteryEcu(const char* canInterface)
    : canSocket_(canInterface), 
      battery_()
{
}

void BatteryEcu::run(int cycleCount)
{
    constexpr auto simulationPeriod = std::chrono::milliseconds(100);
    constexpr double simulationDeltaTime = 0.1;

    for (int step = 0; step < cycleCount; ++step)
    {
        battery_.update(simulationDeltaTime);

        uint8_t data[8] {};

        BatteryCanMessage::encode(
            battery_.getVoltage(),
            battery_.getCurrent(),
            battery_.getStateOfCharge(),
            data
        );

        if (!canSocket_.send(
            BatteryCanMessage::CAN_ID,
            data,
            sizeof(data)))
        {
            std::cerr << "Failed to send CAN message" 
                      << std::endl;
        }

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