//ecu/engineEngineEcu.cpp

#include "EngineEcu.hpp"

#include <chrono>
#include <iostream>
#include <thread>

EngineEcu::EngineEcu()
{
    engine_.start();
}

void EngineEcu::run(int cycleCount)
{
    constexpr auto simulationPeriod = std::chrono::milliseconds(100);
    constexpr double simulationDeltaTime = 0.1;

    for (int step = 0; step < cycleCount; ++step)
    {
        engine_.update(simulationDeltaTime);

        std::cout << "Engine RPM: "
                  << engine_.getRpm()
                  << " | Temperature: "
                  << engine_.getTemperature()
                  << " °C"
                  << std::endl;

        std::this_thread::sleep_for(simulationPeriod);
    }
}