// ecu/engine/EngineEcu.cpp

#include "EngineEcu.hpp"
#include "EngineCanMessage.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <signal.h>
#include <algorithm>

volatile std::sig_atomic_t EngineEcu::stopRequested_ = 0;

void EngineEcu::handleSignal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        stopRequested_ = 1;
    }
}

EngineEcu::EngineEcu(const char *canInterface)
    : canSocket_(canInterface),
      engine_(),
      lastCycleCompleted_(std::chrono::steady_clock::now())
{
    engine_.start();
    std::signal(SIGINT, EngineEcu::handleSignal);
    std::signal(SIGTERM, EngineEcu::handleSignal);
}

void EngineEcu::processCycle()
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

    engine_.update(simulationDeltaTime);

    uint8_t data[8]{};

    if (!EngineCanMessage::encode(
            engine_.getRpm(),
            engine_.getTemperature(),
            data))
    {
        Logger::error("Failed to encode engine CAN message");
        return;
    }

    if (!canSocket_.send(
            EngineCanMessage::CAN_ID,
            data,
            sizeof(data)))
    {
        Logger::error("Failed to send engine CAN message");
    }

    std::cout << "Engine RPM: "
              << engine_.getRpm()
              << " tr/min"
              << " | Temperature: "
              << engine_.getTemperature()
              << " °C"
              << std::endl;

    constexpr auto SLEEP_STEP = std::chrono::milliseconds(10);

    auto remainingTime = simulationPeriod;

    while (remainingTime > std::chrono::milliseconds::zero() &&
           !stopRequested_)
    {
        const auto sleepTime =
            std::min(SLEEP_STEP, remainingTime);

        std::this_thread::sleep_for(sleepTime);

        remainingTime -= SLEEP_STEP;
    }
    const auto cycleDuration =
        std::chrono::steady_clock::now() - cycleStart;

    lastCycleCompleted_ = std::chrono::steady_clock::now();

    if (cycleDuration > WATCHDOG_TIMEOUT)
    {
        Logger::error("Engine ECU simulation cycle exceeded watchdog timeout");
    }
}

void EngineEcu::run()
{
    Logger::info("Engine ECU started");
    if (!canSocket_.isValid())
    {
        Logger::error("Engine ECU CAN socket is not valid");
        return;
    }

    while (!stopRequested_)
    {
        processCycle();
    }
    Logger::info("Engine ECU stopped");
}

void EngineEcu::run(int cycleCount)
{
    Logger::info("Engine ECU started");
    if (!canSocket_.isValid())
    {
        Logger::error("Engine ECU CAN socket is not valid");
        return;
    }
    Configuration configuration("config/platform.conf");
    const auto simulationPeriod =
        std::chrono::milliseconds(
            configuration.getSimulationPeriodMs());

    const double simulationDeltaTime =
        static_cast<double>(configuration.getSimulationPeriodMs()) / 1000.0;

    for (int step = 0; step < cycleCount; ++step)
    {
        processCycle();
    }
}