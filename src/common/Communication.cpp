#include <Arduino.h>
#include "Communication.h"
#include "protocol/Packet.h"
#include "transport/Transport.h"


namespace
{
  bool receiveFramePayload(uint8_t& payload, uint8_t& data)
  {
    enum class ReceiveStep
    {
      WAITING_FOR_HEADER,
      WAITING_FOR_PAYLOAD,
      WAITING_FOR_DATA,
      WAITING_FOR_FOOTER
    };

    static ReceiveStep step = ReceiveStep::WAITING_FOR_HEADER;
    static uint8_t receivedPayload = 0;
    static uint8_t receivedData = 0;

    while (transportAvailable(1))
    {
      uint8_t byte;

      if (transportReceive(&byte, 1) != 1)
      {
        return false;
      }
      
      switch (step)
      {
      case ReceiveStep::WAITING_FOR_HEADER:
        if (byte == PACKET_HEADER)
        {
          step = ReceiveStep::WAITING_FOR_PAYLOAD;
        }
        break;

      case ReceiveStep::WAITING_FOR_PAYLOAD:
        receivedPayload = byte;
        step = ReceiveStep::WAITING_FOR_DATA;
        break;

      case ReceiveStep::WAITING_FOR_DATA:
        receivedData = byte;
        step = ReceiveStep::WAITING_FOR_FOOTER;
        break;

      case ReceiveStep::WAITING_FOR_FOOTER:
        if (byte == PACKET_FOOTER)
        {
          payload = receivedPayload;
          data = receivedData;
          step = ReceiveStep::WAITING_FOR_HEADER;
          return true;
        }

        // Discard invalid partial frame and search for new header
        step = (byte == PACKET_HEADER)
          ? ReceiveStep::WAITING_FOR_PAYLOAD
          : ReceiveStep::WAITING_FOR_HEADER;
        break;
      }
    }
    return false;
  }
}

void sendCommand(CommandID command, uint8_t data)
{
  CommandPacket packet
  {
    PACKET_HEADER,
    command,
    data,
    PACKET_FOOTER
  };

  transportSend(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));

};

void sendHeartbeat()
{
  sendCommand(CommandID::HEARTBEAT);
}

void sendHeartbeatAck()
{
  sendVehicleState(VehicleState::HEARTBEAT_ACK);
}

bool receiveCommand(CommandID& command, uint8_t& data)
{
  uint8_t payload;
  uint8_t receivedData;

  if (!receiveFramePayload(payload, receivedData))
  {
      return false;
  }

  CommandID receivedCommand = static_cast<CommandID>(payload);

  switch (receivedCommand)
  {
      case CommandID::ARM:
      case CommandID::DISARM:
      case CommandID::STATUS:
      // Testing motor actuation
      case CommandID::TEST_FORWARD:
      case CommandID::TEST_REVERSE:
      case CommandID::DRIVE:
      case CommandID::HEARTBEAT:
          command = receivedCommand;
          data = receivedData;
          return true;

      default:
          return false;
  }
}

void sendVehicleState(VehicleState state, uint8_t data)
{
  VehicleStatePacket packet
  {
    PACKET_HEADER,
    state,
    data,
    PACKET_FOOTER
  };

  transportSend(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));
}

bool receiveVehicleState(VehicleState& state, uint8_t& data)
{
  uint8_t payload;
  uint8_t receivedData;

  if (!receiveFramePayload(payload, receivedData))
  {
      return false;
  }

  VehicleState receivedState = static_cast<VehicleState>(payload);

  switch (receivedState)
  {
      case VehicleState::BOOTING:
      case VehicleState::DISARMED:
      case VehicleState::ARMED:
      case VehicleState::FAULT:
      case VehicleState::COM_FAULT:
      case VehicleState::NOT_AVAIL:
      case VehicleState::HEARTBEAT_ACK:
          state = receivedState;
          data = receivedData;
          return true;

      default:
          return false;
  }
}
