// ecu/battery/Battery.hpp

#pragma once

/**
 * @brief Represents the simulated battery state.
 * 
 * The Battery class stores and updates the main parameters
 * of the simulated automotive battery.
 */
class Battery
{
public:
    /**
     * @brief Creates a battery with default parameters.
     */
    Battery();

    /**
     * @brief Updates the battery state.
     * 
     * @param deltaTime Elapsed time in secondssince the last update.
     */
    void update(double deltaTime);

    /**
     * @brief Gets the battery voltage.
     * 
     * @return Battery voltage in volts.
     */
    double getVoltage() const;

    /**
     * @brief Gets the battery current.
     * 
     * @return Battery current in amperes.
     */
    double getCurrent() const;

    /**
     * @brief Gets the battery state of charge.
     *
     * @return State of charge as a percentage.
     */
    double getStateOfCharge() const;

private:
    double voltage_;
    double current_;
    double stateOfCharge_;
};