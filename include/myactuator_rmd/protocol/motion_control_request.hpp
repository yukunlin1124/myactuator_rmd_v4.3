/**
 * \file motion_control_request.hpp
 * \brief Request for the 0x400 Motion Control Command
 */

#ifndef MYACTUATOR_RMD__PROTOCOL__MOTION_CONTROL_REQUEST
#define MYACTUATOR_RMD__PROTOCOL__MOTION_CONTROL_REQUEST
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

#include "myactuator_rmd/protocol/message.hpp"

namespace myactuator_rmd {

  class MotionControlRequest: public Message {
    public:
       /**\ fn MotionControlRequest
       * \brief Constructor
       * \param[in] p_des  Desired Position [-12.5, 12.5] (rad)
       * \param[in] v_des  Desired Velocity [-45.0, 45.0] (rad/s)
       * \param[in] kp     Position Gain    [0.0, 500.0]
       * \param[in] kd     Velocity Gain    [0.0, 5.0]
       * \param[in] t_ff   Feedforward Torque [-24.0, 24.0] (Nm)
       */
      MotionControlRequest(float const p_des, float const v_des, float const kp, float const kd, float const t_ff) {
        
        // 1. CONVERT FLOATS TO INTEGERS
        // We clamp the values first to ensure they don't overflow the 12-bit/16-bit limits
        
        // P_des: 16-bit, Range [-12.5, 12.5]
        auto const p_int = floatToUint(p_des, -12.5f, 12.5f, 16);
        
        // V_des: 12-bit, Range [-45, 45]
        auto const v_int = floatToUint(v_des, -45.0f, 45.0f, 12);
        
        // Kp: 12-bit, Range [0, 500]
        auto const kp_int = floatToUint(kp, 0.0f, 500.0f, 12);
        
        // Kd: 12-bit, Range [0, 5]
        auto const kd_int = floatToUint(kd, 0.0f, 5.0f, 12);
        
        // T_ff: 12-bit, Range [-24, 24]
        auto const t_int = floatToUint(t_ff, -24.0f, 24.0f, 12);


        // 2. PACK BITS INTO BYTES
        // Reference: Your uploaded image table

        // Byte 0: p_des[15-8] (High Byte)
        data_[0] = static_cast<std::uint8_t>(p_int >> 8);
        
        // Byte 1: p_des[7-0] (Low Byte)
        data_[1] = static_cast<std::uint8_t>(p_int & 0xFF);
        
        // Byte 2: v_des[11-4] (High 8 bits)
        data_[2] = static_cast<std::uint8_t>(v_int >> 4);
        
        // Byte 3: v_des[3-0] (High 4 bits of byte) | kp[11-8] (Low 4 bits of byte)
        // Note: The table says v_des is 4-7bit (upper nibble) and kp is 0-3bit (lower nibble)
        data_[3] = static_cast<std::uint8_t>(((v_int & 0xF) << 4) | ((kp_int >> 8) & 0xF));
        
        // Byte 4: kp[7-0] (Low 8 bits)
        data_[4] = static_cast<std::uint8_t>(kp_int & 0xFF);
        
        // Byte 5: kd[11-4] (High 8 bits)
        data_[5] = static_cast<std::uint8_t>(kd_int >> 4);
        
        // Byte 6: kd[3-0] (High 4 bits of byte) | t_ff[11-8] (Low 4 bits of byte)
        data_[6] = static_cast<std::uint8_t>(((kd_int & 0xF) << 4) | ((t_int >> 8) & 0xF));
        
        // Byte 7: t_ff[7-0] (Low 8 bits)
        data_[7] = static_cast<std::uint8_t>(t_int & 0xFF);

        return;
      }

      // No need to override getData(), the base Message class handles it.

    private:
      /**
       * \brief Helper to map float range to unsigned int range
       */
      std::uint16_t floatToUint(float const val, float const min, float const max, int const bits) {
        // 1. Clamp value to valid range
        float const clamped = std::max(min, std::min(val, max));
        
        // 2. Calculate the max integer value (e.g. 4095 for 12-bit)
        float const span = max - min;
        float const max_int = static_cast<float>((1 << bits) - 1);
        
        // 3. Map
        return static_cast<std::uint16_t>((clamped - min) * max_int / span);
      }
  };

}

#endif