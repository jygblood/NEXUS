#pragma once

#include <Arduino.h>

struct AtlasInputState
{
    int joystickX;
    int joystickY;
};

void atlasInputSetup();
AtlasInputState readAtlasInputs();