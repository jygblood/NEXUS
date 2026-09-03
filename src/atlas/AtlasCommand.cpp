#include "AtlasCommand.h"
#include "common/Communication.h"
#include "protocol/Packet.h"

void inputCommands();
void sendHelp();
void sendUnknown(String input);

namespace
{
    bool parseDuty(String input, uint8_t& duty)
    {
        input.trim();

        int spaceIndex = input.lastIndexOf(' ');

        if (spaceIndex < 0)
        {
            return false;
        }

        String dutyText = input.substring(spaceIndex + 1);
        dutyText.trim();

        if (dutyText.length() == 0)
        {
            return false;
        }

        for (size_t index = 0; index < dutyText.length(); index++)
        {
            if (dutyText[index] < '0' || dutyText[index] > '9')
            {
                return false;
            }
        }

        long parsedDuty = dutyText.toInt();

        if (parsedDuty < 0 || parsedDuty > 255)
        {
            return false;
        }

        duty = static_cast<uint8_t>(parsedDuty);
        return true;
    }
}

void startMessage()
{
  Serial.println("==============");
  Serial.println("Atlas Station");
  Serial.println("==============");
  Serial.println("");
  inputCommands();
  Serial.println("");
  Serial.println("Enter command:");
}

void inputCommands()
{
  Serial.println("1. ARM");
  Serial.println("2. DISARM");
  Serial.println("3. STATUS");
  Serial.println("4. HELP");
  Serial.println("5 <0-255>. TEST FORWARD");
  Serial.println("6 <0-255>. TEST REVERSE");
  Serial.println("");
}


void sendHelp()
{
  Serial.println("Acceptable commands:");
  inputCommands();
}

void sendUnknown(String input)
{
  Serial.println("Unknown command: ");
  Serial.println(input);
  Serial.println("");
  sendHelp();
}

CommandID parseCommand(String input)
{
  input.toUpperCase();

  if (input == "ARM" || input == "1")
    {
    // Serial.println("[CMD] 1");
    return CommandID::ARM;
    }
  
  if (input == "DISARM" || input == "2")
    {
    // Serial.println("[CMD] 2");
    return CommandID::DISARM;
    }

  if (input == "STATUS" || input == "3")
    {
    // Serial.println("[CMD] 3");
    return CommandID::STATUS;
    }

  if (input == "HELP" || input == "4")
    {
    // Serial.println("[CMD] 4");
    return CommandID::HELP;
    }

  if (input == "TEST FORWARD" || input == "5" || input.startsWith("TEST FORWARD ") || input.startsWith("5 "))
    {
    return CommandID::TEST_FORWARD;
    }

  if (input == "TEST REVERSE" || input == "6" || input.startsWith("TEST REVERSE ") || input.startsWith("6 "))
    {
    return CommandID::TEST_REVERSE;
    }

  return CommandID::UNKNOWN;
}

void executeCommand(CommandID userCommand, String input)
{
  switch (userCommand)
  {
  case CommandID::ARM:  
  case CommandID::DISARM:
  case CommandID::STATUS:
    sendCommand(userCommand);
    break;

  // Test motor actuation
  case CommandID::TEST_FORWARD:
  case CommandID::TEST_REVERSE:
  {
    uint8_t duty;

    if (!parseDuty(input, duty))
    {
        Serial.println("Enter speed as: 5 <0-255> or 6 <0-255>");
        break;
    }

    sendCommand(userCommand, duty);
    break;
  }

  case CommandID::HELP:
    sendHelp();
    break;

  case CommandID::UNKNOWN:
    sendUnknown(input);
    break;
  }
}

void printVehicleState(VehicleState state)
{
    Serial.println("Vehicle responds: ");

    switch (state)
    {
    case VehicleState::BOOTING:
        Serial.println("BOOTING");
        break;

    case VehicleState::DISARMED:
        Serial.println("DISARMED");
        break;

    case VehicleState::ARMED:
        Serial.println("ARMED");
        break;

    case VehicleState::FAULT:
        Serial.println("FAULT");
        break;

    case VehicleState::COM_FAULT:
        Serial.println("COM_FAULT");
        break;

    case VehicleState::NOT_AVAIL:
        Serial.println("NOT AVAILABLE");
        break;
    
    default:
        Serial.println("UNKNOWN STATE");
        break;
    }
}