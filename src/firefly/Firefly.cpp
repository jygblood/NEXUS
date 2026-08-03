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

        sendVehicleState(VehicleState::ARMED);
        Serial.println("Firefly armed");
    }
    else
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        Serial.println("Cannot arm Firefly");
    }
}

void disarmFirefly()
{
    currentState = VehicleState::DISARMED;

    sendVehicleState(VehicleState::DISARMED);
    Serial.println("Firefly disarmed");
}

void comFault()
{
    sendVehicleState(VehicleState::COM_FAULT);
    Serial.println("Com Fault");
}

void printFireflyStatus()
{
    Serial.println("---- Firefly Status ----");

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

VehicleState getFireflyState()
{
    return currentState;
}