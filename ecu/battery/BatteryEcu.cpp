// ecu/battery/BatteryEcu.cpp

#include "BatteryEcu.hpp"
#include "BatteryCanMessage.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <algorithm>

volatile std::sig_atomic_t BatteryEcu::stopRequested_ = 0;

void BatteryEcu::handleSignal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        stopRequested_ = 1;
    }
}

BatteryEcu::BatteryEcu(const char *canInterface)
    : canSocket_(canInterface),
      battery_(),
      lastCycleCompleted_(std::chrono::steady_clock::now())
{
    std::signal(SIGTERM, BatteryEcu::handleSignal);
    std::signal(SIGINT, BatteryEcu::handleSignal);
}

void BatteryEcu::processCycle()
{
    const auto cycleStart = std::chrono::steady_clock::now();
    Configuration configuration(
        "/etc/linux-embedded-automotive/platform.conf");

    const auto simulationPeriod =
        std::chrono::milliseconds(
            configuration.getSimulationPeriodMs());

    const double simulationDeltaTime =
        static_cast<double>(
            configuration.getSimulationPeriodMs()) /
        1000.0;

    battery_.update(simulationDeltaTime);

    uint8_t data[8]{};

    if (!BatteryCanMessage::encode(
            battery_.getVoltage(),
            battery_.getCurrent(),
            battery_.getStateOfCharge(),
            data))
    {
        Logger::error("Failed to encode battery CAN message");
        return;
    }

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

    constexpr auto SLEEP_STEP = std::chrono::milliseconds(10);

    auto remainingTime = simulationPeriod;
    while (remainingTime > std::chrono::milliseconds::zero() &&
           !stopRequested_)
    {
        const auto sleepTime =
            std::min(SLEEP_STEP, remainingTime);
        std::this_thread::sleep_for(sleepTime);
        remainingTime -= sleepTime;
    }

    const auto cycleDuration =
        std::chrono::steady_clock::now() - cycleStart;

    lastCycleCompleted_ = std::chrono::steady_clock::now();

    if (cycleDuration > WATCHDOG_TIMEOUT)
    {
        Logger::error("Battery ECU watchdog timeout detected");
    }
}

void BatteryEcu::run()
{
    Logger::info("Battery ECU started");

    if (!canSocket_.isValid())
    {
        Logger::error("Battery ECU CAN socket is not valid");
        return;
    }

    while (!stopRequested_)
    {
        processCycle();
    }

    Logger::info("Battery ECU stopped");
}

void BatteryEcu::run(int cycleCount)
{
    Logger::info("Battery ECU started");
    if (!canSocket_.isValid())
    {
        Logger::error("Battery ECU CAN socket is not valid");
        return;
    }

    for (int step = 0; step < cycleCount; ++step)
    {
        processCycle();
    }
}