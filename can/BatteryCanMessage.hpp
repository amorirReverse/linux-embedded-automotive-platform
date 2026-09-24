// can/BatteryCanMessage.hpp

#pragma once

#include <cstdint>

/**
 * @brief CAN message containing battery status information.
 *
 * The message contains battery voltage, current,
 * and state of charge encoded into an 8-byte CAN payload.
 */
struct BatteryCanMessage
{
    /**
     * @brief CAN identifier used for battery status messages.
     */
    static constexpr uint32_t CAN_ID = 0x200;

    /**
     * @brief Encodes battery status into a CAN payload.
     *
     * @param voltage Battery voltage in volts.
     * @param current Battery current in amperes.
     * @param stateOfCharge Battery state of charge in percent.
     * @param data Output buffer containing the encoded 8-byte payload.
     */
    static void encode(
        double voltage,
        double current,
        double stateOfCharge,
        uint8_t* data);

    /**
    * @brief Decodes a battery status CAN payload.
    *
    * @param data Input buffer containing the CAN payload.
    * @param dataLength Number of payload bytes.
    * @param voltage Output battery voltage in volts.
    * @param current Output battery current in amperes.
    * @param stateOfCharge Output battery state of charge in percent.
    *
    * @return True if the payload was decoded successfully, false otherwise.
    */
    static bool decode(
        const uint8_t* data,
        uint8_t dataLength,
        double& voltage,
        double& current,
        double& stateOfCharge);
    
    
};