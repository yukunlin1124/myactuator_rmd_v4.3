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

  /**\class SetMotionModeRequest
   * \brief
   *    Request for the mixed motion control mode (Command 0x400).
   *    Inherits directly from Message, utilizing the base 8-byte data array.
  */
  class SetMotionModeRequest : public Message {
    public:
      /**\fn SetMotionModeRequest
       * \brief Class constructor
       * \param[in] p_des  Desired Position [-12.5, 12.5] (rad)
       * \param[in] v_des  Desired Velocity [-45.0, 45.0] (rad/s)
       * \param[in] kp     Position Gain    [0.0, 500.0]
       * \param[in] kd     Velocity Gain    [0.0, 5.0]
       * \param[in] t_ff   Feedforward Torque [-24.0, 24.0] (Nm)
      */
      SetMotionModeRequest(float const p_des, float const v_des, float const kp, float const kd, float const t_ff);

      // Standard boilerplate to prevent empty or invalid requests
      SetMotionModeRequest() = delete;
      SetMotionModeRequest(SetMotionModeRequest const&) = default;
      SetMotionModeRequest& operator = (SetMotionModeRequest const&) = default;
      SetMotionModeRequest(SetMotionModeRequest&&) = default;
      SetMotionModeRequest& operator = (SetMotionModeRequest&&) = default;

      // Note: No getter methods are strictly necessary here unless you want 
      // to read the values back out of the packed bytes later.
  };

}

#endif // MYACTUATOR_RMD__PROTOCOL__SET_MOTION_MODE_REQUEST