#pragma once
#include "protocol/Packet.h"


void sendCommand(CommandID command);
bool receiveCommand(CommandID& command);

void sendVehicleState(VehicleState state);
bool receiveVehicleState(VehicleState& state);

void sendHeartbeat();
void sendHeartbeatAck();