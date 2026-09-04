#include "AtlasInput.h"

namespace
{
    constexpr uint8_t JOYSTICK_X_PIN = 34;
    constexpr uint8_t JOYSTICK_Y_PIN = 35;
}

void atlasInputSetup()
{
    analogSetPinAttenuation(JOYSTICK_X_PIN, ADC_11db);
    analogSetPinAttenuation(JOYSTICK_Y_PIN, ADC_11db);
}

AtlasInputState readAtlasInputs()
{
    AtlasInputState input;

    input.joystickX = analogRead(JOYSTICK_X_PIN);
    input.joystickY = analogRead(JOYSTICK_Y_PIN);

    return input;
}