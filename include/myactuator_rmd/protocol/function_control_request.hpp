#ifndef MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_REQUEST
#define MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_REQUEST
#pragma once

#include <cstdint>

#include "myactuator_rmd/protocol/command_type.hpp"
#include "myactuator_rmd/protocol/single_motor_message.hpp"
#include "myactuator_rmd/actuator_state/function_control_type.hpp" 

namespace myactuator_rmd {

  /**\class FunctionControlRequest
   * \brief
   * Request to set a function control value (Integer based)
   * \tparam C
   * Type of the command (e.g. WRITE_FUNCTION_CONTROL_TO_RAM)
  */
  template <CommandType C>
  class FunctionControlRequest: public SingleMotorRequest<C> {
    public:
      /**\fn FunctionControlRequest
       * \brief
       * Class constructor
       * \param[in] function_type
       * The specific function to set (e.g. ACCELERATION_SETTING)
       * \param[in] value
       * The integer value for the function
      */
      constexpr FunctionControlRequest(FunctionControlType const function_type, std::uint32_t const value) noexcept;
      
      // Standard boilerplate
      FunctionControlRequest() = delete;
      FunctionControlRequest(FunctionControlRequest const&) = default;
      FunctionControlRequest& operator = (FunctionControlRequest const&) = default;
      FunctionControlRequest(FunctionControlRequest&&) = default;
      FunctionControlRequest& operator = (FunctionControlRequest&&) = default;
      using SingleMotorRequest<C>::SingleMotorRequest;

      /**\fn getValue
       * \brief
       * Get the integer value from the request
      */
      [[nodiscard]]
      constexpr std::uint32_t getValue() const noexcept;

      /**\fn getFunctionType
       * \brief
       * Get the function control type
      */
      [[nodiscard]]
      constexpr FunctionControlType getFunctionType() const noexcept;
  };

  // --- IMPLEMENTATION ---

  template <CommandType C>
  constexpr FunctionControlRequest<C>::FunctionControlRequest(FunctionControlType const function_type, std::uint32_t const value) noexcept
  : SingleMotorRequest<C>{} {
    // 1. Set the Index (Byte 1)
    this->data_[1] = static_cast<std::uint8_t>(function_type);

    // 2. Set the Value (Bytes 4-7) - Little Endian Integer
    // No memcpy needed for integers, we use bit shifting
    this->data_[4] = static_cast<std::uint8_t>(value & 0xFF);
    this->data_[5] = static_cast<std::uint8_t>((value >> 8) & 0xFF);
    this->data_[6] = static_cast<std::uint8_t>((value >> 16) & 0xFF);
    this->data_[7] = static_cast<std::uint8_t>((value >> 24) & 0xFF);
  }

  template <CommandType C>
  constexpr std::uint32_t FunctionControlRequest<C>::getValue() const noexcept {
    // Reconstruct the 32-bit integer from 4 bytes (Little Endian)
    std::uint32_t value = 0;
    value |= static_cast<std::uint32_t>(this->data_[4]);
    value |= static_cast<std::uint32_t>(this->data_[5]) << 8;
    value |= static_cast<std::uint32_t>(this->data_[6]) << 16;
    value |= static_cast<std::uint32_t>(this->data_[7]) << 24;
    return value;
  }

  template <CommandType C>
  constexpr FunctionControlType FunctionControlRequest<C>::getFunctionType() const noexcept {
    return static_cast<FunctionControlType>(this->data_[1]);
  }

  using SetFunctionControlRequest = FunctionControlRequest<CommandType::FUNCTION_CONTROL>; 
}

#endif // MYACTUATOR_RMD__PROTOCOL__FUNCTION_CONTROL_REQUEST