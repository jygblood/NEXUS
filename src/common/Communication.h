#pragma once
#include "protocol/Packet.h"


void sendCommand(CommandID command, uint8_t data = 0);
bool receiveCommand(CommandID& command, uint8_t& data);

void sendVehicleState(VehicleState state, uint8_t data = 0);
bool receiveVehicleState(VehicleState& state, uint8_t& data);

void sendHeartbeat();
void sendHeartbeatAck();