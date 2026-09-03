#pragma once

#include <Arduino.h>
#include "protocol/Packet.h"

void badgerControlSetup();

void setMotorSpeed(uint8_t duty);

void moveForward();

void moveBackward();

// void moveRight();

// void moveLeft();

void startForwardTest();

void startReverseTest();

void stopMotors();

void updateBadgerControl();