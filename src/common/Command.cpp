#include "Command.h"
#include "Communication.h"

void inputCommands();
void sendHelp();
void sendUnknown(String input);

void startMessage()
{
  Serial.println("====================");
  Serial.println("Ground Station v0.1");
  Serial.println("====================");
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

CommandType parseCommand(String input)
{
  input.toUpperCase();

  if (input == "ARM" || input == "1")
    return CommandType::ARM;
  
  if (input == "DISARM" || input == "2")
    return CommandType::DISARM;

  if (input == "STATUS" || input == "3")
    return CommandType::STATUS;

  if (input == "HELP" || input == "4")
    return CommandType::HELP;

  return CommandType::UNKNOWN;
}

void executeCommand(CommandType receivedCommand, String input)
{
  switch (receivedCommand)
  {
  case CommandType::ARM:  
  case CommandType::DISARM:
  case CommandType::STATUS:
    sendCommand(receivedCommand);
    break;

  case CommandType::HELP:
    sendHelp();
    break;

  case CommandType::UNKNOWN:
    sendUnknown(input);
    break;
  }
}