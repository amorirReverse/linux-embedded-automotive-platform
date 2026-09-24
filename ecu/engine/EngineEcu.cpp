// ecu/engine/EngineEcu.cpp

#include "EngineEcu.hpp"
#include "EngineCanMessage.hpp"

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
    constexpr auto simulationPeriod = std::chrono::milliseconds(100);
    constexpr double simulationDeltaTime = 0.1;

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
            std::cerr << "Failed to send CAN message" 
                      << std::endl;
        }

        std::cout << "Engine RPM: "
                  << engine_.getRpm()
                  << " | Temperature: "
                  << engine_.getTemperature()
                  << " °C"
                  << std::endl;

        std::this_thread::sleep_for(simulationPeriod);
    }
}