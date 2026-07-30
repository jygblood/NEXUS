#include <Arduino.h>
#include<Command.h>

enum class CommandType : uint8_t
{
  ARM,
  DISARM,
  STATUS,
  HELP,
  FAULT
};

// PacktType

void startMessage();
void inputCommands();

void sendArm();
void sendDisarm();
void sendStatus();
void sendHelp();
void sendFault(String input);
CommandType parseCommand(String input);
void executeCommand(CommandType receivedCommand, String input);


void setup()
{
    Serial.begin(115200);
    delay(1000);
    startMessage();
  
}

void loop()
{
    if (Serial.available()){
      String input = Serial.readStringUntil('\n');
      input.trim();
      
      CommandType receivedCommand = parseCommand(input);

      executeCommand(receivedCommand, input);
      
    }
}

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

void sendArm()
{
  Serial.println("ARM sent!");
  Serial.println("");
}

void sendDisarm()
{
  Serial.println("DISARM sent!");
  Serial.println("");
}

void sendStatus()
{
  Serial.println("STATUS sent!");
  Serial.println("");
}

void sendHelp()
{
  Serial.println("Acceptable commands:");
  inputCommands();
}

void sendFault(String input)
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

  return CommandType::FAULT;
}

void executeCommand(CommandType receivedCommand, String input)
{
  switch (receivedCommand)
  {
  case CommandType::ARM:
    Serial.println("ARM command received");
    sendArm();
    break;
  
  case CommandType::DISARM:
    Serial.println("DISARM command received");
    sendDisarm();
    break;

  case CommandType::STATUS:
    Serial.println("STATUS command received");
    sendStatus();
    break;

  case CommandType::HELP:
    Serial.println("HELP command received");
    sendHelp();
    break;

  case CommandType::FAULT:
    sendFault(input);
    break;
  }
}
