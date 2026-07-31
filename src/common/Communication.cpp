#include <Arduino.h>
#include "Communication.h"
#include "uart/UART.h"

void send2Firefly(CommandType command)
{
  switch (command)
  {
    case CommandType::ARM:
      uartSend("ARM");
      Serial.println("[TX] ARM");
      break;

    case CommandType::DISARM:
      uartSend("DISARM");
      Serial.println("[TX] DISARM");
      break;

    case CommandType::STATUS:
      uartSend("STATUS");
      Serial.println("[TX] STATUS");
      break;

    default:
      break;
    }
}

void send2Atlas(VehicleState respond)
{
  switch (respond)
  {
    case VehicleState::ARMED:
      uartSend("Firefly ARMED");
      Serial.println("[TX] ARMED");
      break;

    case VehicleState::DISARMED:
      uartSend("Firefly DISARMED");
      Serial.println("[TX] DISARMED");
      break;

    case VehicleState::BOOTING:
      uartSend("Firefly BOOTING");
      Serial.println("[TX] BOOTING");
      break;

    case VehicleState::FAULT:
      uartSend("Firefly FAULT");
      Serial.println("[TX] FAULT");
      break;

    case VehicleState::COM_FAULT:
      uartSend("COM_FAULT");
      Serial.println("[TX] COM_FAULT");

    default:
      break;
    }
}