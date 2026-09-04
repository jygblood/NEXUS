#include <Arduino.h>
#include "Badger.h"
#include "BadgerControl.h"
#include "common/Communication.h"

VehicleState currentState = VehicleState::BOOTING;

namespace
{
    constexpr uint32_t DRIVE_COMMAND_TIMEOUT_MS = 300;

    uint32_t lastDriveCommandMs = 0;
    bool driveCommandActive = false;
}

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
    stopMotors();
    driveCommandActive = false;
    currentState = VehicleState::DISARMED;

    sendVehicleState(VehicleState::DISARMED);
    Serial.println("Badger disarmed");
}

void comFault()
{
    stopMotors();
    driveCommandActive = false;
    currentState = VehicleState::DISARMED;

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

    driveCommandActive = false;
    startForwardTest(duty);
}

void testBadgerReverse(uint8_t duty)
{
    if (currentState != VehicleState::ARMED)
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        return;
    }

    driveCommandActive = false;
    startReverseTest(duty);
}

void driveBadger(int8_t throttle)
{
    lastDriveCommandMs = millis();
    driveCommandActive = (throttle != 0);
    
    if (throttle == 0)
    {
        stopMotors();
        return;
    }
    
    if (currentState != VehicleState::ARMED)
    {
        sendVehicleState(VehicleState::NOT_AVAIL);
        return;
    }

    //Also cancel any motor actuation
    stopMotors();

    uint8_t duty = static_cast<uint8_t>((throttle > 0) ? throttle : -static_cast<int>(throttle));

    if (throttle > 0)
    {
        moveForward(duty);
    }
    else
    {
        moveBackward(duty);
    }
}

void updateBadgerDriveWatchdog()
{
    if (driveCommandActive && millis() - lastDriveCommandMs >= DRIVE_COMMAND_TIMEOUT_MS)
    {
        stopMotors();
        driveCommandActive = false;

        Serial.println("Drive command timeout: motors stopped.");
    }
}