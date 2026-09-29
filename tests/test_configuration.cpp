// tests/test_configuration.cpp

#include "Configuration.hpp"

#include <iostream>

/**
 * @brief Entry point for the Configuration tests.
 * 
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    Configuration configuration("config/platform.conf");

    if (configuration.getSimulationPeriodMs() != 250)
    {
        std::cerr << "Unexpected simulation period: "
                  << configuration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::cout << "Configuration tests passed." 
              << std::endl;
              
    return 0;
}