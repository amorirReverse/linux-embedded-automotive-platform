#include <iostream>

#include "Engine.hpp"

/**
 * @brief Entry point of the engine ECU simulation.
 * 
 * @return Zero on sucessful execution.
 */

 int main()
 {
   Engine engine;

   engine.start();

   constexpr double deltaTime = 0.1;

   for (int step = 0; step < 10; ++step)
   {
      engine.update(deltaTime);

      std::cout   << "Engine RPM: "
                  << engine.getRPM()
                  << " | Temperature: "
                  << engine.getTemperature()
                  << " °C"
                  << std::endl;
   }

   return 0;
 }