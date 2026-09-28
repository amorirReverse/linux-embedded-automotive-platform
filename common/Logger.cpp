// common/Logger.cpp

#include "Logger.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace Logger
{
    namespace
    {
        std::string getCurrentTimestamp()
        {
            const auto now = std::chrono::system_clock::now();
            const std::time_t currentTime = 
                std::chrono::system_clock::to_time_t(now);
            
            std::tm localTime{};
            localtime_r(&currentTime, &localTime);

            std::ostringstream timestamp;

            timestamp << std::put_time(
                &localTime,
                "%Y-%m-%d %H:%M:%S");
            
            return timestamp.str();
        }
    }
void info(const std::string& message)
{
    std::cout << "["
              << getCurrentTimestamp()
              << "] [INFO] "
              << message
              << std::endl;
}

void warning(const std::string& message)
{
    std::cout << "["
              << getCurrentTimestamp()
              << "] [WARNING] "
              << message
              << std::endl;
}

void error(const std::string& message)
{
    std::cerr << "["
              << getCurrentTimestamp()
              << "] [ERROR] "
              << message
              << std::endl;
}
}