// common/Configuration.hpp

#pragma once

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
     * @brief Creates a configuration with default values.
     */
    Configuration();

    /**
     * @brief Gets the ECU simulation period.
     *
     * @return Simulation period in milliseconds.
     */
    int getSimulationPeriodMs() const;

private:
    int simulationPeriodMs_;
};