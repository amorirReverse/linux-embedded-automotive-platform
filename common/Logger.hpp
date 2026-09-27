// common/Logger.hpp

#pragma once

#include <string>

namespace Logger
{
/**
 * @brief Logs an informational message.
 *
 * @param message Message to log.
 */
void info(const std::string& message);

/**
 * @brief Logs a warning message.
 *
 * @param message Message to log.
 */
void warning(const std::string& message);

/**
 * @brief Logs an error message.
 *
 * @param message Message to log.
 */
void error(const std::string& message);
}