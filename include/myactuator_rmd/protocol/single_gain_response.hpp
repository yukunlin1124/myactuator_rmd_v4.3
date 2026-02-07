#ifndef MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_RESPONSE
#define MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_RESPONSE
#pragma once

#include <cstdint>
#include <cstring>

#include "myactuator_rmd/protocol/command_type.hpp"
#include "myactuator_rmd/protocol/single_motor_message.hpp" 
#include "myactuator_rmd/actuator_state/gain_type.hpp"

namespace myactuator_rmd {

  /**\class SingleGainResponse
   * \brief
   *    Response to request for getting a single gain value
  */
  template <CommandType C>
  class SingleGainResponse: public SingleMotorResponse<C> {
    public:
      using SingleMotorResponse<C>::SingleMotorResponse;

      [[nodiscard]]
      GainType getGainType() const noexcept;

      [[nodiscard]]
      float getValue() const noexcept;
  };

  template <CommandType C>
  GainType SingleGainResponse<C>::getGainType() const noexcept {
    return static_cast<GainType>(this->data_[1]);
  }

  template <CommandType C>
  float SingleGainResponse<C>::getValue() const noexcept {
    // Reconstruct the 32-bit integer from 4 bytes (Big Endian logic: data[7] is MSB)
    std::uint32_t i = 0;
    i |= (static_cast<std::uint32_t>(this->data_[7]) << 24);
    i |= (static_cast<std::uint32_t>(this->data_[6]) << 16);
    i |= (static_cast<std::uint32_t>(this->data_[5]) << 8);
    i |= (static_cast<std::uint32_t>(this->data_[4]));

    // Convert integer bits to float
    union { std::uint32_t i; float f; } converter;
    converter.i = i;
    return converter.f;
  }

  using GetSingleControllerGainResponse = SingleGainResponse<CommandType::READ_PID_PARAMETERS>;
  using SetSingleControllerGainResponse = SingleGainResponse<CommandType::WRITE_PID_PARAMETERS_TO_RAM>;
  using SetSingleControllerGainPersistentlyResponse = SingleGainResponse<CommandType::WRITE_PID_PARAMETERS_TO_ROM>;
}

#endif // MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_RESPONSE