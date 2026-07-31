#pragma once
#include "atlas/AtlasCommand.h"
#include "firefly/FireflyCommand.h"
#include "firefly/Firefly.h"

void send2Firefly(CommandType command);
void send2Atlas(VehicleState respond);