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

   std::cout << "Engine ECU started." << std::endl;
   std::cout << "Engine RPM: " << engine.getRPM() << std::endl;

    return 0;
 }