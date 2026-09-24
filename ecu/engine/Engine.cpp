#include "Engine.hpp"

Engine::Engine()
    :   running_(false),
        rpm_(0.0)
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

    if (rpm_ < targetRpm)
    {
        rpm_ += acceleration * deltaTime;

        if (rpm_ > targetRpm)
        {
            rpm_ = targetRpm;
        }
    }
}

double Engine::getRPM() const
{
    return rpm_;
}
