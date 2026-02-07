/**
 * \file motion_control_status.hpp
 * \mainpage
 * Contains struct with motion control status (actual physical parameters)
 */

#ifndef MYACTUATOR_RMD__ACTUATOR_STATE__MOTION_CONTROL_STATUS
#define MYACTUATOR_RMD__ACTUATOR_STATE__MOTION_CONTROL_STATUS
#pragma once

#include <cstdint>
#include <iostream>

namespace myactuator_rmd {

  /**\class MotionControlStatus
   * \brief
   * Status object containing the real-time feedback returned by the Motion Control command (0x400)
  */
  class MotionControlStatus {
    public:
      /**\fn MotionControlStatus
       * \brief
       * Class constructor
       * * \param[in] can_id_
       * The CAN ID of the device that responded (Byte 0)
       * \param[in] shaft_angle_
       * The current physical position of the motor in radians [-12.5, 12.5]
       * \param[in] shaft_speed_
       * The current physical velocity of the motor in rad/s [-45.0, 45.0]
       * \param[in] torque_
       * The current physical torque of the motor in Nm [-24.0, 24.0]
      */
      constexpr MotionControlStatus(int const can_id_ = 0, float const shaft_angle_ = 0.0f, float const shaft_speed_ = 0.0f, float const torque_ = 0.0f) noexcept;
      
      MotionControlStatus(MotionControlStatus const&) = default;
      MotionControlStatus& operator = (MotionControlStatus const&) = default;
      MotionControlStatus(MotionControlStatus&&) = default;
      MotionControlStatus& operator = (MotionControlStatus&&) = default;

      int can_id;
      float shaft_angle;
      float shaft_speed;
      float torque;
  };

  constexpr MotionControlStatus::MotionControlStatus(int const can_id_, float const shaft_angle_, float const shaft_speed_, float const torque_) noexcept
  : can_id{can_id_}, shaft_angle{shaft_angle_}, shaft_speed{shaft_speed_}, torque{torque_} {
    return;
  }

  inline std::ostream& operator << (std::ostream& os, MotionControlStatus const& status) {
    os << "MotionControlStatus("
       << "can_id=" << status.can_id << ", "
       << "shaft_angle=" << status.shaft_angle << ", "
       << "shaft_speed=" << status.shaft_speed << ", "
       << "torque=" << status.torque << ")";
    return os;
  }
}

#endif // MYACTUATOR_RMD__ACTUATOR_STATE__MOTION_CONTROL_STATUS