// ecu/engine/Engine.hpp

#pragma once

/**
 * @brief Represents the simulated engine state.
 * 
 * The Engine class stores and updates the main parameters
 * of the simulated automotive engine.
 */

 class Engine
 {
public:
    /**
     * @brief Creates an engine in the stopped state.
     */
    Engine();

    /**
     * @brief Starts the engine.
     */
    void start();

    /**
     * @brief Stops the engine.
     */
    void stop();

    /**
     * @brief Updates the engine state.
     * 
     * The engine speed gradually approaches the target idle speed
     * when the engine is running.
     * 
     * @param deltaTime time in seconds since the last update.
     */
    void update(double deltaTime);

    /**
     * @brief Checks wether the engine is running.
     * 
     * @return True if the engine is running, false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Gets the current engine speed in RPM.
     * 
     * @return The current engine speed in revolutions per minute.
     */
    double getRpm() const;

    /**
     * @brief Gets the current engine temperature.
     * 
     * @return Engine temperature in degrees Celsius.
     */
    double getTemperature() const;

private:
    bool running_;
    double rpm_;
    double temperature_;
 };
