#include <Arduino.h>
#include "Badger.h"
#include "BadgerControl.h"
#include "common/Communication.h"

VehicleState currentState = VehicleState::BOOTING;

void initializeBadger()
{
    currentState = VehicleState::DISARMED;

    Serial.println("Badger initialized");
}

void armBadger()
{
    if (currentState == VehicleState::DISARMED)
    {
        currentState = VehicleState::ARMED;

        sendVehicleState(VehicleState::ARMED);
        Serial.println("Badger armed");
    }
    else
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        Serial.println("Cannot arm Badger");
    }
}

void disarmBadger()
{
    currentState = VehicleState::DISARMED;

    sendVehicleState(VehicleState::DISARMED);
    Serial.println("Badger disarmed");
}

void comFault()
{
    sendVehicleState(VehicleState::COM_FAULT);
    Serial.println("Com Fault");
}

void printBadgerStatus()
{
    Serial.println("---- Badger Status ----");

    switch (currentState)
    {
    case VehicleState::BOOTING:
        sendVehicleState(VehicleState::BOOTING);
        Serial.println("State: BOOTING");
        break;

    case VehicleState::DISARMED:
        sendVehicleState(VehicleState::DISARMED);
        Serial.println("State: DISARMED");
        break;

    case VehicleState::ARMED:
        sendVehicleState(VehicleState::ARMED);
        Serial.println("State: ARMED");
        break;

    case VehicleState::FAULT:
        sendVehicleState(VehicleState::FAULT);
        Serial.println("State: FAULT");
        break;
    }
}

VehicleState getBadgerState()
{
    return currentState;
}

void testBadgerForward(uint8_t duty)
{
    if (currentState != VehicleState::ARMED)
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        return;
    }

    startForwardTest(duty);
}

void testBadgerReverse(uint8_t duty)
{
    if (currentState != VehicleState::ARMED)
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        return;
    }

    startReverseTest(duty);
}