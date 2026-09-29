// common/Configuration.hpp

#pragma once

#include <string>

/**
 * @brief Stores platform configuration values.
 *
 * The Configuration class provides common runtime parameters
 * used by the simulated embedded platform.
 */
class Configuration
{
public:
    /**
     * @brief Creates a configuration from a configuration file.
     * 
     * @param configPath Path to the configuration file.
     */
    explicit Configuration(const std::string& configPath);

    /**
     * @brief Gets the ECU simulation period.
     *
     * @return Simulation period in milliseconds.
     */
    int getSimulationPeriodMs() const;

private:
    int simulationPeriodMs_;
};