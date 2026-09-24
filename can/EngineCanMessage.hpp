// can/EngineCanMessage.hpp

#pragma once

#include <cstdint>

/**
 * @brief CAN message containing engine status information.
 * 
 * The message contains the engine speed and temperature
 * encoded into an 8-byte CAN payload.
 */
struct EngineCanMessage
{
    /**
     * @brief CAN identifier used for engine status messages.
     */
    static constexpr uint32_t CAN_ID = 0x100;

    /**
     * @brief Encodes engine status into a CAN payload.
     * 
     * @param rpm Engine speed in revolutions per minute.
     * @param temperature Engine temperature in degrees Celsius.
     * @param data Output buffer containing the encoded 8-byte payload.
     */
    static void encode(
        double rpm,
        double temperature,
        uint8_t* data);

    /**
     * @brief Decodes an engine status CAN payload.
     *
     * @param data Input buffer containing the CAN payload.
     * @param dataLength Number of payload bytes.
     * @param rpm Output engine speed in revolutions per minute.
     * @param temperature Output engine temperature in degrees Celsius.
     *
     * @return True if the payload was decoded successfully, false otherwise.
     */
    static bool decode(
        const uint8_t* data,
        uint8_t dataLength,
        double& rpm,
        double& temperature);
};