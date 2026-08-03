#include <Arduino.h>
#include "FireflyCommand.h"
#include "Firefly.h"

void handleCommand(CommandID command)
{
    switch (command)
    {
    case CommandID::ARM:
        Serial.println("[RX] 1");
        armFirefly();
        break;

    case CommandID::DISARM:
        disarmFirefly();
        break;
    
    case CommandID::STATUS:
        Serial.println("[RX] 3");
        printFireflyStatus();
        break;

    default:
        comFault();
        Serial.println("Unknown command");
        break;
    }
}