#include <iostream>
#include <chrono>
#include <thread>

#include "Engine.hpp"

/**
 * @brief Entry point of the engine ECU simulation.
 * 
 * the simulation loop runs periodically and provides
 * the elapsed time to the engine model.
 * @return Zero on sucessful execution.
 */

 int main()
 {
   Engine engine;

   engine.start();

   constexpr auto simulationPeriod = std::chrono::milliseconds(100);
   constexpr double simulationDeltaTime = 0.1;

   auto previousTime = std::chrono::steady_clock::now();

   for (int step = 0; step < 10; ++step)
   {
      // const auto currentTime = std::chrono::steady_clock::now();

      // const std::chrono::duration<double> elapsedTime = 
      //    currentTime - previousTime;
      
      // const double deltaTime = elapsedTime.count();

      // previousTime = currentTime;


      engine.update(simulationDeltaTime);

      std::cout   << "Engine RPM: "
                  << engine.getRPM()
                  << " | Temperature: "
                  << engine.getTemperature()
                  << " °C"
                  << std::endl;

      std::this_thread::sleep_for(simulationPeriod);
   }

   return 0;
 }