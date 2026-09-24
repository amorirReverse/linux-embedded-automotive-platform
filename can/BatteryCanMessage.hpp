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
};