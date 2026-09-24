#include "Engine.hpp"

Engine::Engine()
    :   running_(false),
        rpm_(0.0),
        temperature_(20.0)
{
}

void Engine::start()
{
    running_ = true;
    rpm_ = 0.0;
}

void Engine::stop()
{
    running_ = false;
    rpm_ = 0.0;
}

void Engine::update(double deltaTime)
{
    if (!running_)
    {
        return;
    }

    constexpr double targetRpm = 800.0;
    constexpr double acceleration = 2000.0;
    constexpr double ambientTemperature = 20.0;
    constexpr double operatingTemperature = 90.0;
    constexpr double heatingRate = 5.0;
    constexpr double coolingRate = 1.0;
    if (running_)
    {
       if (rpm_ < targetRpm)
        {
            rpm_ += acceleration * deltaTime;

            if (rpm_ > targetRpm)
            {
                rpm_ = targetRpm;
            }
        }
    
        if (temperature_ < operatingTemperature)
        {
            temperature_ += heatingRate * deltaTime;

            if (temperature_ > operatingTemperature)
            {
                temperature_ = operatingTemperature;
            }
        }
    }
    else
    {

        if (temperature_ > ambientTemperature)
        {
            temperature_ -= coolingRate * deltaTime;

            if (temperature_ < ambientTemperature)
            {
                temperature_ = ambientTemperature;
            }
        }
    }  
}

double Engine::getRPM() const
{
    return rpm_;
}

double Engine::getTemperature() const
{
    return temperature_;
}
