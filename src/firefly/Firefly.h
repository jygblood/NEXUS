#pragma once

enum class VehicleState
{
    BOOTING,
    DISARMED,
    ARMED,
    FAULT,
    COM_FAULT
};

void initializeFirefly();

void armFirefly();

void disarmFirefly();

void printFireflyStatus();

void comFault();

VehicleState getFireflyState();