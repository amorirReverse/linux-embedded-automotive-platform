// ecu/battery/Battery.cpp

#include "Battery.hpp"

Battery::Battery()
    : voltage_(12.6),
      current_(0.0),
      stateOfCharge_(100.0)
{
}

void Battery::update(double deltaTime)
{
    constexpr double nominalVoltage = 12.6;
    constexpr double dischargeCurrent = 10.0;
    constexpr double dischargeRate = 0.1;

    voltage_ = nominalVoltage;

    current_ = dischargeCurrent;

    if (stateOfCharge_ > 0.0)
    {
        stateOfCharge_ -= dischargeRate * deltaTime;

        if (stateOfCharge_ < 0.0)
        {
            stateOfCharge_ = 0.0;
        }
    }
}

double Battery::getVoltage() const
{
    return voltage_;
}

double Battery::getCurrent() const
{
    return current_;
}

double Battery::getStateOfCharge() const
{
    return stateOfCharge_;
}