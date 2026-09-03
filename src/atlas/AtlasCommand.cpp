#include "AtlasCommand.h"
#include "common/Communication.h"
#include "protocol/Packet.h"

void inputCommands();
void sendHelp();
void sendUnknown(String input);

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
  Serial.println("5. TEST FORWARD");
  Serial.println("6. TEST REVERSE");
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

  if (input == "TEST FORWARD" || input == "5")
    {
    // Serial.println("[CMD] 4");
    return CommandID::TEST_FORWARD;
    }

  if (input == "TEST REVERSE" || input == "6")
    {
    // Serial.println("[CMD] 4");
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
  // Test motor actuation
  case CommandID::TEST_FORWARD:
  case CommandID::TEST_REVERSE:
    sendCommand(userCommand);
    break;

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