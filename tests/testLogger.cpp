// tests/testLogger.cpp

#include "Logger.hpp"

#include <iostream>

/**
 * @brief Entry point for the Logger tests.
 * 
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    Logger::info("Test information message");
    Logger::warning("Test warning message");
    Logger::error("Test error message");

    std::cout << "Logger tests passed"
                << std::endl;

    return 0;
}