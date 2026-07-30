#include <Arduino.h>
#include "Communication.h"
#include "uart/UART.h"

void sendCommand(CommandType command)
{
  switch (command)
  {
    case CommandType::ARM:
      uartSend("ARM");
      break;

    case CommandType::DISARM:
      uartSend("DISARM");
      break;

    case CommandType::STATUS:
      uartSend("STATUS");
      break;

    default:
      break;
    }
}