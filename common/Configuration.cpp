// common/Configuration.cpp

#include "Configuration.hpp"

Configuration::Configuration()
    : simulationPeriodMs_(100)
{
}

int Configuration::getSimulationPeriodMs() const
{
    return simulationPeriodMs_;
}