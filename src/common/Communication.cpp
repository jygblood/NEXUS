#include <Arduino.h>
#include "Communication.h"
#include "protocol/Packet.h"
#include "transport/Transport.h"


namespace
{
  bool receiveFramePayload(uint8_t& payload)
  {
    enum class ReceiveStep
    {
      WAITING_FOR_HEADER,
      WAITING_FOR_PAYLOAD,
      WAITING_FOR_FOOTER
    };

    static ReceiveStep step = ReceiveStep::WAITING_FOR_HEADER;
    static uint8_t receivedPayload = 0;

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
        step = ReceiveStep::WAITING_FOR_FOOTER;
        break;

      case ReceiveStep::WAITING_FOR_FOOTER:
        if (byte ==PACKET_FOOTER)
        {
          payload = receivedPayload;
          step = ReceiveStep::WAITING_FOR_HEADER;
          return true;
        }

        step = (byte == PACKET_HEADER)
          ? ReceiveStep::WAITING_FOR_PAYLOAD
          : ReceiveStep::WAITING_FOR_HEADER;
        break;
      }
    }
    return false;
  }
}

void sendCommand(CommandID command)
{
  CommandPacket packet
  {
    PACKET_HEADER,
    command,
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

bool receiveCommand(CommandID& command)
{
  uint8_t payload;

  if (!receiveFramePayload(payload))
  {
      return false;
  }

  CommandID receivedCommand = static_cast<CommandID>(payload);

  switch (receivedCommand)
  {
      case CommandID::ARM:
      case CommandID::DISARM:
      case CommandID::STATUS:
      case CommandID::HEARTBEAT:
          command = receivedCommand;
          return true;

      default:
          return false;
  }
}

void sendVehicleState(VehicleState state)
{
  VehicleStatePacket packet
  {
    PACKET_HEADER,
    state,
    PACKET_FOOTER
  };

  transportSend(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));
}

bool receiveVehicleState(VehicleState& state)
{
  uint8_t payload;

  if (!receiveFramePayload(payload))
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
          return true;

      default:
          return false;
  }
}
