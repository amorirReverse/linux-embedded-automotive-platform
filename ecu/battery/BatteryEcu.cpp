// ecu/battery/BatteryEcu.cpp

#include "BatteryEcu.hpp"
#include "BatteryCanMessage.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"

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
    Logger::info("Battery ECU started");
    Configuration configuration("config/platform.conf");
    const auto simulationPeriod =
        std::chrono::milliseconds(
            configuration.getSimulationPeriodMs());
    const double simulationDeltaTime =
        static_cast<double>(configuration.getSimulationPeriodMs()) / 1000.0;

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
            Logger::error("Failed to send battery CAN message");
        }

        std::cout << "Battery Voltage: "
                  << battery_.getVoltage()
                  << " V"
                  << " | Current: "
                  << battery_.getCurrent()
                  << " A" 
                  << " | State of Charge: "
                  << battery_.getStateOfCharge()
                  << " %"
                  << std::endl;

        std::this_thread::sleep_for(simulationPeriod);
    }
}