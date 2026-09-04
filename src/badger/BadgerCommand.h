#pragma once

#include <Arduino.h>
#include "protocol/Packet.h"

void handleCommand(CommandID command, uint8_t commandData);