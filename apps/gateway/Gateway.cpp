// apps/gateway/Gateway.cpp

#include "Gateway.hpp"
#include "EngineCanMessage.hpp"
#include "BatteryCanMessage.hpp"
#include "Logger.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

volatile std::sig_atomic_t Gateway::stopRequested_ = 0;

void Gateway::handleSignal(int signal)
{
    if (signal == SIGTERM || signal == SIGINT)
    {
        stopRequested_ = 1;
    }
}

Gateway::Gateway(const char *canInterface)
    : canSocket_(canInterface)
{
    std::signal(SIGTERM, Gateway::handleSignal);
    std::signal(SIGINT, Gateway::handleSignal);
}

void Gateway::processFrame()
{
    uint32_t canId = 0;
    uint8_t data[8]{};
    uint8_t dataLength = 0;

    const bool frameReceived = canSocket_.receive(
        canId,
        data,
        dataLength,
        sizeof(data),
        100);

    if (!frameReceived)
    {
        return;
    }

    std::cout << "Gateway received CAN frame:"
              << " ID=0x"
              << std::hex
              << canId
              << std::dec
              << " DLC="
              << static_cast<int>(dataLength)
              << std::endl;

    if (canId == EngineCanMessage::CAN_ID)
    {
        double rpm = 0.0;
        double temperature = 0.0;

        if (EngineCanMessage::decode(
                data,
                dataLength,
                rpm,
                temperature))
        {
            std::cout << "Engine status"
                      << " RPM="
                      << rpm
                      << " tr/min"
                      << " Temperature="
                      << temperature
                      << " °C"
                      << std::endl;
        }
        else
        {
            Logger::warning("Invalid Engine CAN frame received");
        }
    }

    else if (canId == BatteryCanMessage::CAN_ID)
    {
        double voltage = 0.0;
        double current = 0.0;
        double stateOfCharge = 0.0;

        if (BatteryCanMessage::decode(
                data,
                dataLength,
                voltage,
                current,
                stateOfCharge))
        {
            std::cout << "Battery status:"
                      << " Voltage="
                      << voltage
                      << " V"
                      << " Current="
                      << current
                      << " A"
                      << " State of Charge="
                      << stateOfCharge
                      << " %"
                      << std::endl;
        }
        else
        {
            Logger::warning("Invalid Battery CAN frame received");
        }
    }
    else
    {
        std::ostringstream messages;

        messages << "Unknown CAN frame received with ID = 0x"
                 << std::hex
                 << canId;

        Logger::warning(messages.str());
    }
}

void Gateway::run()
{
    Logger::info("Gateway started");

    if (!canSocket_.isValid())
    {
        Logger::error("CAN socket is not valid.");
        return;
    }

    while (!stopRequested_)
    {
        processFrame();
    }

    Logger::info("Gateway stopped");
}

void Gateway::run(int frameCount)
{
    Logger::info("Gateway started");

    if (!canSocket_.isValid())
    {
        Logger::error("CAN socket is not valid.");
        return;
    }
    for (int frame = 0; frame < frameCount; ++frame)
    {
        processFrame();
    }
}