#include "Engine.hpp"

Engine::Engine()
    :   running_(false),
        rpm_(0.0)
{
}

void Engine::start()
{
    running_ = true;
    rpm_ = 800.0;
}

void Engine::stop()
{
    running_ = false;
    rpm_ = 0.0;
}

bool Engine::isRunning() const
{
    return running_;
}

double Engine::getRPM() const
{
    return rpm_;
}
