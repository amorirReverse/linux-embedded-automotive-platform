// ecu/engine/EngineEcu.cpp

#include "EngineEcu.hpp"
#include "EngineCanMessage.hpp"
#include "Logger.hpp"
#include "Configuration.hpp"

#include <chrono>
#include <iostream>
#include <thread>

EngineEcu::EngineEcu(const char* canInterface)
    : canSocket_(canInterface),
      engine_()
{
    engine_.start();
}

void EngineEcu::run(int cycleCount)
{
    Logger::info("Engine ECU started");
    Configuration configuration("config/platform.conf");
    const auto simulationPeriod = 
        std::chrono::milliseconds(
            configuration.getSimulationPeriodMs());

    const double simulationDeltaTime = 
        static_cast<double>(configuration.getSimulationPeriodMs()) / 1000.0;


    for (int step = 0; step < cycleCount; ++step)
    {
        engine_.update(simulationDeltaTime);

        uint8_t data[8] {};

        EngineCanMessage::encode(
            engine_.getRpm(),
            engine_.getTemperature(),
            data);

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

        std::this_thread::sleep_for(simulationPeriod);
    }
}