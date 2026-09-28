// tests/test_engine.cpp

#include "Engine.hpp"

#include <cmath>
#include <iostream>

namespace
{
    bool almostEqual(double lhs, double rhs)
    {
        return std::abs(lhs - rhs) < 0.001;
    }
}

/**
 * @brief Entry point for the Engine tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    Engine engine;

    

    

    if (engine.isRunning())
    {
        std::cerr << "Engine should be stopped initially"
                    << std::endl;

        return 1;
    }

    

    if (!almostEqual(engine.getRpm(), 0.0))
    {
        std::cerr << "Unexpected initial RPM: "
                << engine.getRpm()
                 << std::endl;

        return 1;
    }

    if (!almostEqual(engine.getTemperature(), 20.0))
    {
        std::cerr << "Unexpected initial temperature: "
                  << engine.getTemperature()
                  << std::endl;

        return 1;
    }

    engine.start();

    if (!engine.isRunning())
    {
        std::cerr << "Engine should be running after start"
                  << std::endl;
        return 1;
    }

    engine.update(0.1);

    if (!almostEqual(engine.getRpm(), 200.0))
    {
        std::cerr << "Unexpected RPM after first update: "
                  << engine.getRpm()
                  << std::endl;

        return 1;
    }

    if (!almostEqual(engine.getTemperature(), 20.5))
    {
        std::cerr << "Unexpected temperature after first update: "
                  << engine.getTemperature()
                  << std::endl;

        return 1;
    }

    engine.stop();

    if (engine.isRunning())
    {
        std::cerr << "Engine should be stopped after stop"
                  << std::endl;

        return 1;
    }

    if (!almostEqual(engine.getRpm(), 0.0))
    {
        std::cerr << "RPM should be zero after stop: "
                  << engine.getRpm()
                  << std::endl;

        return 1;
    }

    engine.update(0.1);

    if (!almostEqual(engine.getRpm(), 0.0))
    {
        std::cerr << "RPM should remain zero while engine is stopped: "
                << engine.getRpm()
                << std::endl;

        return 1;
    }

    std::cout << "Engine tests passed"
              << std::endl;

    return 0;
}