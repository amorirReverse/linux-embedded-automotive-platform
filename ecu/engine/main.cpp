// ecu/engine/main.cpp

#include <iostream>
#include <chrono>
#include <thread>

#include "EngineEcu.hpp"

/**
 * @brief Entry point of the engine ECU simulation.
 * 
 * @return Zero on sucessful execution.
 */

 int main()
 {
   EngineEcu ecu("vcan0");

   ecu.run(20);

   return 0;
 }