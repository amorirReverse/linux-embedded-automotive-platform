// common/Configuration.cpp

#include "Configuration.hpp"

#include <fstream>
#include <sstream>
#include <string>

Configuration::Configuration(const std::string& configPath)
    : simulationPeriodMs_(100)
{
    std::ifstream configFile(configPath);

    if (!configFile.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(configFile, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream lineStream(line);
        std::string key;
        std::string value;

        if (!std::getline(lineStream, key, '='))
        {
            continue;
        }
        
        if (!std::getline(lineStream, value))
        {
            continue;
        }

        if (key == "simulation_period_ms")
        {
            simulationPeriodMs_ = std::stoi(value);
        }
    }
}

int Configuration::getSimulationPeriodMs() const
{
    return simulationPeriodMs_;
}