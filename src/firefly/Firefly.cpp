#include <Arduino.h>
#include "Firefly.h"
#include "common/Communication.h"

VehicleState currentState = VehicleState::BOOTING;

void initializeFirefly()
{
    currentState = VehicleState::DISARMED;

    Serial.println("Firefly initialized");
}

void armFirefly()
{
    if (currentState == VehicleState::DISARMED)
    {
        currentState = VehicleState::ARMED;

        send2Atlas(VehicleState::ARMED);
        Serial.println("Firefly armed");
    }
    else
    {
        send2Atlas(VehicleState::FAULT);
        Serial.println("Cannot arm Firefly");
    }
}

void disarmFirefly()
{
    currentState = VehicleState::DISARMED;

    send2Atlas(VehicleState::DISARMED);
    Serial.println("Firefly disarmed");
}

void comFault()
{
    send2Atlas(VehicleState::COM_FAULT);
    Serial.println("Com Fault");
}

void printFireflyStatus()
{
    Serial.println("---- Firefly Status ----");

    switch (currentState)
    {
    case VehicleState::BOOTING:
        send2Atlas(VehicleState::BOOTING);
        Serial.println("State: BOOTING");
        break;

    case VehicleState::DISARMED:
        send2Atlas(VehicleState::DISARMED);
        Serial.println("State: DISARMED");
        break;

    case VehicleState::ARMED:
        send2Atlas(VehicleState::ARMED);
        Serial.println("State: ARMED");
        break;

    case VehicleState::FAULT:
        send2Atlas(VehicleState::FAULT);
        Serial.println("State: FAULT");
        break;
    }
}

VehicleState getFireflyStatus()
{
    return currentState;
}