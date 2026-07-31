#include <Arduino.h>
#include "FireflyCommand.h"
#include "Firefly.h"

void handleCommand(RecCommandType command)
{
    switch (command)
    {
    case RecCommandType::ARM:
        armFirefly();
        break;

    case RecCommandType::DISARM:
        disarmFirefly();
        break;
    
    case RecCommandType::STATUS:
        printFireflyStatus();
        break;

    case RecCommandType::FAULT:
        comFault();
        Serial.println("Fault command received");
        break;
    }
}