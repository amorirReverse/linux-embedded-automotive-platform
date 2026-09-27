// common/Logger.cpp

#include "Logger.hpp"

#include <iostream>

namespace Logger
{
void info(const std::string& message)
{
    std::cout << "[INFO] "
              << message
              << std::endl;
}

void warning(const std::string& message)
{
    std::cout << "[WARNING] "
              << message
              << std::endl;
}

void error(const std::string& message)
{
    std::cerr << "[ERROR] "
              << message
              << std::endl;
}
}