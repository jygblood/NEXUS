#pragma once
#include <Arduino.h>
#include "protocol/Packet.h"

void startMessage();

CommandID parseCommand(String input);
void executeCommand(CommandID userCommand, String input);

void printVehicleState(VehicleState state);