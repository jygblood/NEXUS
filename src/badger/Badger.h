#pragma once

#include "protocol/Packet.h"

void initializeBadger();

void armBadger();

void disarmBadger();

void printBadgerStatus();

void comFault();

VehicleState getBadgerState();

void testBadgerForward(uint8_t duty);
void testBadgerReverse(uint8_t duty);

void driveBadger(int8_t throttle);

void updateBadgerDriveWatchdog();