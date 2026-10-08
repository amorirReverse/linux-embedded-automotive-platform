// common/Configuration.cpp

#include "Configuration.hpp"
#include "Logger.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

namespace
{
    constexpr int DEFAULT_SIMULATION_PERIOD_MS = 100;


    /**
     * @brief Removes leading and trailing whitespace from a string.
     *
     * @param value Input string.
     * @return Trimmed string.
     */

    std::string trim(const std::string &value)
    {
        const std::string whitespace = " \t\n\r";

        const std::size_t first = value.find_first_not_of(whitespace);

        if (first == std::string::npos)
        {
            return {};
        }

        const std::size_t last = value.find_last_not_of(whitespace);

        return value.substr(first, last - first + 1);
    }
}

Configuration::Configuration(const std::string &configPath)
    : simulationPeriodMs_(100)
{
    std::ifstream configFile(configPath);

    if (!configFile.is_open())
    {
        Logger::warning(
            "Configuration file could not be opened: '" +
            configPath +
            "'");

        return;
    }

    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(configFile, line))
    {
        ++lineNumber;

        line = trim(line);

        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream lineStream(line);
        std::string key;
        std::string value;

        std::getline(lineStream, key, '=');

        if (!std::getline(lineStream, value))
        {
            Logger::warning(
                "Malformed configuration line " +
                std::to_string(lineNumber) +
                ": missing value");
            continue;
        }

        key = trim(key);
        value = trim(value);

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
                        "simulation_period_ms must be greater than zero at line " +
                        std::to_string(lineNumber) +
                        ": " +
                        value);

                    simulationPeriodMs_ = DEFAULT_SIMULATION_PERIOD_MS;
                }
            }
            catch (const std::invalid_argument &)
            {
                Logger::warning(
                    "Invalid value for simulation_period_ms at line " +
                    std::to_string(lineNumber) +
                    ": " +
                    value);
                simulationPeriodMs_ = DEFAULT_SIMULATION_PERIOD_MS;
            }
            catch (const std::out_of_range &)
            {
                Logger::warning(
                    "Value for simulation_period_ms is out of range at line " +
                    std::to_string(lineNumber) +
                    ": " +
                    value);
                simulationPeriodMs_ = DEFAULT_SIMULATION_PERIOD_MS;
            }
        }
    }
}

int Configuration::getSimulationPeriodMs() const
{
    return simulationPeriodMs_;
}