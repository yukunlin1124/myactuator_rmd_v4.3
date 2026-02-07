/**
 * \file motion_control_response.hpp
 * \brief Response for the 0x400 Motion Control Command
 * Parses the echo of parameters sent back by the actuator.
 */

#ifndef MYACTUATOR_RMD__PROTOCOL__MOTION_CONTROL_RESPONSE
#define MYACTUATOR_RMD__PROTOCOL__MOTION_CONTROL_RESPONSE
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cmath>

#include "myactuator_rmd/protocol/message.hpp"

namespace myactuator_rmd {

  class MotionControlResponse: public Message {
    public:
      /**
       * \brief Constructor
       * \param[in] data The raw 8-byte response from the CAN bus
       */
      MotionControlResponse(std::array<std::uint8_t, 8> const& data)
      : Message{data} {
        return;
      }

      MotionControlResponse() = delete;
      MotionControlResponse(MotionControlResponse const&) = default;
      MotionControlResponse& operator = (MotionControlResponse const&) = default;
      MotionControlResponse(MotionControlResponse&&) = default;
      MotionControlResponse& operator = (MotionControlResponse&&) = default;

      /**
       * \brief Get the actuator CAN ID echoed in the response (Byte 0)
       * \return The CAN ID of the device
       */
      [[nodiscard]]
      std::uint8_t getEchoCanId() const noexcept {
        return data_[0];
      }

      /**
       * \brief Get the Desired Position (p_des) echoed by the motor
       * Parsed from Bytes 1 and 2
       * \return Position in radians [-12.5, 12.5]
       */
      [[nodiscard]]
      float getPosition() const noexcept {
        // Combine Byte 1 (High) and Byte 2 (Low)
        std::uint16_t const raw_value = (static_cast<std::uint16_t>(data_[1]) << 8) | data_[2];
        return uintToFloat(raw_value, -12.5f, 12.5f, 16);
      }

      /**
       * \brief Get the Desired Velocity (v_des) echoed by the motor
       * Parsed from Byte 3 and Byte 4 (High Nibble)
       * \return Velocity in rad/s [-45.0, 45.0]
       */
      [[nodiscard]]
      float getVelocity() const noexcept {
        // Byte 3 is the upper 8 bits
        // Byte 4 (upper 4 bits) is the lower 4 bits of value
        std::uint16_t const raw_value = (static_cast<std::uint16_t>(data_[3]) << 4) | (data_[4] >> 4);
        return uintToFloat(raw_value, -45.0f, 45.0f, 12);
      }

      /**
       * \brief Get the Feedforward Torque (t_ff) echoed by the motor
       * Parsed from Byte 4 (Low Nibble) and Byte 5
       * \return Torque in Nm [-24.0, 24.0]
       */
      [[nodiscard]]
      float getTorque() const noexcept {
        // Byte 4 (lower 4 bits) is the upper 4 bits of value
        // Byte 5 is the lower 8 bits of value
        std::uint16_t const raw_value = (static_cast<std::uint16_t>(data_[4] & 0x0F) << 8) | data_[5];
        return uintToFloat(raw_value, -24.0f, 24.0f, 12);
      }

    private:
      /**
       * \brief Helper to map unsigned int range back to float range
       */
      float uintToFloat(std::uint16_t const val, float const min, float const max, int const bits) const noexcept {
        float const span = max - min;
        float const max_int = static_cast<float>((1 << bits) - 1);
        return (static_cast<float>(val) * span / max_int) + min;
      }
  };

}

#endif // MYACTUATOR_RMD__PROTOCOL__MOTION_CONTROL_RESPONSE