/**
 * \file gain_type.hpp
 * \mainpage
 * Index of the controller gain types for the new protocol
 * \author
 * yukunlin (github.com/yukunlin1124)
*/

#ifndef MYACTUATOR_RMD__ACTUATOR_STATE__GAIN_TYPE
#define MYACTUATOR_RMD__ACTUATOR_STATE__GAIN_TYPE
#pragma once

#include <cstdint>


namespace myactuator_rmd {

  /**\enum GainType
   * \brief
   * Strongly typed enum for the different controller gain function indices
   * based on the updated communication protocol.
   */
  enum class GainType: std::uint8_t {
    CURRENT_LOOP_KP  = 0x01,
    CURRENT_LOOP_KI  = 0x02,
    SPEED_LOOP_KP    = 0x04,
    SPEED_LOOP_KI    = 0x05,
    POSITION_LOOP_KP = 0x07,
    POSITION_LOOP_KI = 0x08,
    POSITION_LOOP_KD = 0x09
  };

}

#endif // MYACTUATOR_RMD__ACTUATOR_STATE__GAIN_TYPE