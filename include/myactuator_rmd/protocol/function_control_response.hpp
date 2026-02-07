#ifndef MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_RESPONSE
#define MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_RESPONSE
#pragma once

#include <array>
#include <cstdint>

#include "myactuator_rmd/protocol/command_type.hpp"
#include "myactuator_rmd/protocol/single_motor_message.hpp"
#include "myactuator_rmd/actuator_state/function_control_type.hpp"

namespace myactuator_rmd {

  /**\class FunctionControlResponse
   * \brief
   * Response to a function control request (Integer based)
   * Decodes the return bytes back into a uint32_t integer.
  */
  template <CommandType C>
  class FunctionControlResponse: public SingleMotorResponse<C> {
    public:
      // Inherit constructor
      using SingleMotorResponse<C>::SingleMotorResponse;

      /**\fn getFunctionType
       * \brief
       * Get the function type confirmed by the motor
      */
      [[nodiscard]]
      constexpr FunctionControlType getFunctionType() const noexcept;

      /**\fn getValue
       * \brief
       * Get the integer value returned by the motor
      */
      [[nodiscard]]
      constexpr std::uint32_t getValue() const noexcept;
  };

  // --- IMPLEMENTATION ---

  template <CommandType C>
  constexpr FunctionControlType FunctionControlResponse<C>::getFunctionType() const noexcept {
    return static_cast<FunctionControlType>(this->data_[1]);
  }

  template <CommandType C>
  constexpr std::uint32_t FunctionControlResponse<C>::getValue() const noexcept {
    // Reconstruct 32-bit integer from 4 bytes (Little Endian)
    // We use bitwise OR (|) and Left Shift (<<) to combine the bytes
    std::uint32_t value = 0;
    value |= static_cast<std::uint32_t>(this->data_[4]);       // Low byte
    value |= static_cast<std::uint32_t>(this->data_[5]) << 8;
    value |= static_cast<std::uint32_t>(this->data_[6]) << 16;
    value |= static_cast<std::uint32_t>(this->data_[7]) << 24; // High byte
    return value;
  }

  using SetFunctionControlResponse = FunctionControlResponse<CommandType::FUNCTION_CONTROL>;
}

#endif // MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_RESPONSE