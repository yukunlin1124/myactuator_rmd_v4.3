/**
 * \file driver.hpp
 * \mainpage
 *    Contains the driver base class defining the interface
 * \author
 *    Tobit Flatscher (github.com/2b-t)
*/

#ifndef MYACTUATOR_RMD__DRIVER__DRIVER
#define MYACTUATOR_RMD__DRIVER__DRIVER
#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "myactuator_rmd/protocol/message.hpp"


namespace myactuator_rmd {

  class ActuatorInterface;

  /**\class Driver
   * \brief
   *    Pure abstract base class for drivers
  */
  class Driver {
    public:
      /**\fn addId
       * \brief
       *    Updates the id as well as the send and receive ids in a consistent manner
       * 
       * \param[in] actuator_id
       *    The id of the actuator
      */
      virtual void addId(std::uint32_t const actuator_id) = 0;

      /**\fn send
       * \brief
       *    Writes the given data to the participant with the actuator id actuator_id
       * 
       * \param[in] msg
       *    The message that should be sent to the corresponding actuator
       * \param[in] actuator_id
       *    The ID of the actuator that the message should be sent to
      */
      virtual void send(Message const& msg, std::uint32_t const actuator_id) = 0;

      // --- edit ---
      /**\fn send
       * \brief
       * Overload: Writes data using a CUSTOM base offset
       * * \param[in] base_offset
       * The custom CAN ID offset (e.g. 0x400)
      */
      virtual void send(Message const& msg, std::uint32_t const actuator_id, std::uint32_t const base_offset) = 0;
      // ---------------------------

      /**\fn sendRecv
       * \brief
       *    Writes the given data to the actuator with the corresponding id and waits for a corresponding reply
       * 
       * \param[in] request
       *    Request that should be sent to the corresponding actuator
       * \param[in] actuator_id
       *    The ID of the actuator that the message should be sent to
       * \return
       *    The response bytes
      */
      [[nodiscard]]
      virtual std::array<std::uint8_t,8> sendRecv(Message const& request, std::uint32_t const actuator_id) = 0;

      // --- edit ---
      /**\fn sendRecv
       * \brief
       * Overload: Sends with CUSTOM request offset and waits for CUSTOM response offset
       * * \param[in] request_offset
       * The send ID base (e.g. 0x400)
       * \param[in] response_offset
       * The expected reply ID base (e.g. 0x500)
      */
      [[nodiscard]]
      virtual std::array<std::uint8_t,8> sendRecv(Message const& request, std::uint32_t const actuator_id, std::uint32_t const request_offset, std::uint32_t const response_offset) = 0;
      // -----------------------------------------------------------------------

    protected:
      Driver() = default;
      Driver(Driver const&) = default;
      Driver& operator = (Driver const&) = default;
      Driver(Driver&&) = default;
      Driver& operator = (Driver&&) = default;

      friend ActuatorInterface;
  };

}

#endif // MYACTUATOR_RMD__DRIVER__DRIVER
