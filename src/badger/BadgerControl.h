#pragma once

#include <Arduino.h>
#include "protocol/Packet.h"

void badgerControlSetup();

void setMotorSpeed(uint8_t duty);

void moveForward(uint8_t duty);

void moveBackward(uint8_t duty);

// void moveRight();

// void moveLeft();

void startForwardTest(uint8_t duty);

void startReverseTest(uint8_t duty);

void stopMotors();

void updateBadgerControl();