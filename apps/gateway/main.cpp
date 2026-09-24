// apps/gateway/main.cpp

#include "Gateway.hpp"

/**
 * @brief Entry point of the gateway ECU simulation.
 *
 * @return Zero on successful execution.
 */
int main()
{
    Gateway gateway("vcan0");

    gateway.run(10);

    return 0;
}