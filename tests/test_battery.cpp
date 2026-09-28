// tests/test_battery.cpp

#include "Battery.hpp"

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
 * @brief Entry point for the Battery tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
        Battery battery;

    if (!almostEqual(battery.getVoltage(), 12.6))
    {
        std::cerr << "Unexpected initial voltage: "
                  << battery.getVoltage()
                  << std::endl;

        return 1;
    }

    if (!almostEqual(battery.getCurrent(), 0.0))
    {
        std::cerr << "Unexpected initial current: "
                  << battery.getCurrent()
                  << std::endl;

        return 1;
    }

    if (!almostEqual(battery.getStateOfCharge(), 100.0))
    {
        std::cerr << "Unexpected initial state of charge: "
                  << battery.getStateOfCharge()
                  << std::endl;

        return 1;
    }

        battery.update(0.1);

    if (!almostEqual(battery.getVoltage(), 12.6))
    {
        std::cerr << "Unexpected voltage after update: "
                  << battery.getVoltage()
                  << std::endl;

        return 1;
    }

    if (!almostEqual(battery.getCurrent(), 10.0))
    {
        std::cerr << "Unexpected current after update: "
                  << battery.getCurrent()
                  << std::endl;

        return 1;
    }

    if (!almostEqual(battery.getStateOfCharge(), 99.99))
    {
        std::cerr << "Unexpected state of charge after update: "
                  << battery.getStateOfCharge()
                  << std::endl;

        return 1;
    }

    battery.update(1000.0);

    if (!almostEqual(battery.getStateOfCharge(), 0.0))
    {
        std::cerr << "State of charge should not go below zero: "
                  << battery.getStateOfCharge()
                  << std::endl;

        return 1;
    }

    std::cout << "Battery discharge test passed"
              << std::endl;

    return 0;
}