#include <Arduino.h>
#include "BadgerCommand.h"
#include "Badger.h"


void handleCommand(CommandID command, uint8_t commandData)
{
    switch (command)
    {
    case CommandID::ARM:
        armBadger();
        break;

    case CommandID::DISARM:
        disarmBadger();
        break;
    
    case CommandID::STATUS:
        printBadgerStatus();
        break;

    case CommandID::TEST_FORWARD:
        testBadgerForward(commandData);
        break;

    case CommandID::TEST_REVERSE:
        testBadgerReverse(commandData);
        break;

    case CommandID::DRIVE:
        driveBadger(decodeDriveThrottle(commandData));
        break;

    default:
        comFault();
        Serial.println("Unknown command");
        break;
    }
}