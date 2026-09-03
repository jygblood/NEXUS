#include <Arduino.h>
#include "BadgerCommand.h"
#include "Badger.h"


void handleCommand(CommandID command)
{
    switch (command)
    {
    case CommandID::ARM:
        Serial.println("[RX] 1");
        armBadger();
        break;

    case CommandID::DISARM:
        disarmBadger();
        break;
    
    case CommandID::STATUS:
        Serial.println("[RX] 3");
        printBadgerStatus();
        break;

    case CommandID::TEST_FORWARD:
        testBadgerForward();
        break;

    case CommandID::TEST_REVERSE:
        testBadgerReverse();
        break;

    default:
        comFault();
        Serial.println("Unknown command");
        break;
    }
}