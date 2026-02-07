#ifndef MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_REQUEST
#define MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_REQUEST
#pragma once

#include <cstdint>
#include <cstring>

#include "myactuator_rmd/protocol/command_type.hpp"
#include "myactuator_rmd/protocol/single_motor_message.hpp"
#include "myactuator_rmd/actuator_state/gain_type.hpp"

namespace myactuator_rmd {

  template <CommandType C>
    /**\class GetSingleGainRequest
   * \brief
   *    Request for getting a single gain value
  */
  class GetSingleGainRequest: public SingleMotorRequest<C> {
    public:
      constexpr GetSingleGainRequest(GainType const gain_type) noexcept;

      using SingleMotorRequest<C>::SingleMotorRequest;

      [[nodiscard]]
      constexpr GainType getGainType() const noexcept;
  };

  template <CommandType C>
  constexpr GetSingleGainRequest<C>::GetSingleGainRequest(GainType const gain_type) noexcept
  : SingleMotorRequest<C>{} {
    // Set Function Index
    this->data_[1] = static_cast<std::uint8_t>(gain_type);
    // All other bytes remain 0x00
  }

  template <CommandType C>
  constexpr GainType GetSingleGainRequest<C>::getGainType() const noexcept {
    return static_cast<GainType>(this->data_[1]);
  }

  /**\class SetSingleGainRequest
   * \brief
   * Request to set a single gain value (PI/PID)
   * * \tparam C
   * Type of the command (e.g. WRITE_SINGLE_PARAMETER_TO_RAM)
  */
  template <CommandType C>
  class SetSingleGainRequest: public SingleMotorRequest<C> {
    public:
      /**\fn SetSingleGainRequest
       * \brief
       * Class constructor
       * * \param[in] gain_type
       * The specific gain to set (e.g. CURRENT_LOOP_KP)
       * \param[in] value
       * The float value for the gain
      */
      SetSingleGainRequest(GainType const gain_type, float const value) noexcept;
      
      // Standard boilerplate to disable default constructors
      SetSingleGainRequest() = delete;
      SetSingleGainRequest(SetSingleGainRequest const&) = default;
      SetSingleGainRequest& operator = (SetSingleGainRequest const&) = default;
      SetSingleGainRequest(SetSingleGainRequest&&) = default;
      SetSingleGainRequest& operator = (SetSingleGainRequest&&) = default;
      using SingleMotorRequest<C>::SingleMotorRequest;

      /**\fn getValue
       * \brief
       * Get the float value from the request
       * * \return
       * The float value
      */
      [[nodiscard]]
      float getValue() const noexcept;

      /**\fn getGainType
       * \brief
       * Get the gain type
       * * \return
       * The gain type enum
      */
      [[nodiscard]]
      GainType getGainType() const noexcept;
  };

  // --- IMPLEMENTATION ---

  template <CommandType C>
  SetSingleGainRequest<C>::SetSingleGainRequest(GainType const gain_type, float const value) noexcept
  : SingleMotorRequest<C>{} {
    // 1. Set the Index (Byte 1)
    // This tells the motor WHICH gain we are updating
    this->data_[1] = static_cast<std::uint8_t>(gain_type);

    // 2. Set the Value (Bytes 4-7)
    // We use std::memcpy to convert the float to 4 raw bytes (IEEE 754 representation)
    // We assume data starts at index 4 (standard for RMD 32-bit value commands)
    static_assert(sizeof(float) == 4, "Float must be 32-bit");
    std::memcpy(&(this->data_[4]), &value, sizeof(float));
    
    return;
  }

  template <CommandType C>
  float SetSingleGainRequest<C>::getValue() const noexcept {
    float value;
    // Extract bytes 4-7 back into a float
    std::memcpy(&value, &(this->data_[4]), sizeof(float));
    return value;
  }

  template <CommandType C>
  GainType SetSingleGainRequest<C>::getGainType() const noexcept {
    return static_cast<GainType>(this->data_[1]);
  }

  using GetSingleControllerGainRequest = GetSingleGainRequest<CommandType::READ_PID_PARAMETERS>;
  using SetSingleControllerGainRequest = SetSingleGainRequest<CommandType::WRITE_PID_PARAMETERS_TO_RAM>;
  using SetSingleControllerGainPersistentlyRequest = SetSingleGainRequest<CommandType::WRITE_PID_PARAMETERS_TO_ROM>;
}

#endif // MYACTUATOR_RMD__PROTOCOL__SINGLE_GAIN_REQUEST