/**
 * \file function_control_type.hpp
 * \mainpage
 * Index of the controller function control types for the new protocol
 * \author
 * yukunlin (github.com/yukunlin1124)
*/

#ifndef MYACTUATOR_RMD__ACTUATOR_STATE__FUNCTION_CONTROL_TYPE
#define MYACTUATOR_RMD__ACTUATOR_STATE__FUNCTION_CONTROL_TYPE
#pragma once

#include <cstdint>


namespace myactuator_rmd {

  /**\enum FunctionControlType
   * \brief
   * Strongly typed enum for the different controller function control indices
   * based on the updated communication protocol.
   */
  enum class FunctionControlType: std::uint8_t {
    CLEAR_MULTI_TURN_VALUE = 0x01,
    CANID_FILTER_ENABLE = 0x02,
    ERROR_STATUS_TRANSMISSION_ENABLE = 0x03,
    SAVE_MULTI_TURN_VALUE = 0x04,
    SET_CANID = 0x05,
    SET_MAX_POSITIVE_POSITION_FOR_POSITION_MODE = 0x06,
    SET_MAX_NEGATIVE_POSITION_FOR_POSITION_MODE = 0x07
  };

}

#endif // MYACTUATOR_RMD__ACTUATOR_STATE__FUNCTION_CONTROL_TYPE