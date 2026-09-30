// common/Configuration.cpp

#include "Configuration.hpp"
#include "Logger.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

Configuration::Configuration(const std::string& configPath)
    : simulationPeriodMs_(100)
{
    std::ifstream configFile(configPath);

    if (!configFile.is_open())
    {
        Logger::warning(
            "Configuration file could not be opened: " + configPath);
        
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
            try
            {
                const int simulationPeriodMs = std::stoi(value);

                if (simulationPeriodMs > 0)
                {
                    simulationPeriodMs_ = simulationPeriodMs;
                }
                else
                {
                    Logger::warning(
                        "simulation_period_ms must be greater than zero: " + value);
                        
                    simulationPeriodMs_ = 100;
                }
            }
            catch (const std::invalid_argument&)
            {
                Logger::warning(
                    "Invalid value for simulation_period_ms in configuration file: " + value);
                simulationPeriodMs_ = 100;
            }
            catch (const std::out_of_range&)
            {
                Logger::warning(
                    "Value for simulation_period_ms in configuration file is out of range: " + value);
                simulationPeriodMs_ = 100;
            }
        }
    }
}


int Configuration::getSimulationPeriodMs() const
{
    return simulationPeriodMs_;
}