#pragma once

#include "protocol/Packet.h"


void initializeFirefly();

void armFirefly();

void disarmFirefly();

void printFireflyStatus();

void comFault();

VehicleState getFireflyState();