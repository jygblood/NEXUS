#include "AtlasControl.h"

namespace
{
    constexpr int JOYSTICK_Y_CENTER = 1972;
    constexpr int JOYSTICK_Y_DEADZONE = 200;

    // Change to true is joystick is opposite of vehicle direction
    constexpr bool JOYSTICK_Y_INVERTED = false;
}

int8_t getDriveThrottle(const AtlasInputState& input)
{
    int delta = input.joystickY - JOYSTICK_Y_CENTER;
    
    if (abs(delta) <= JOYSTICK_Y_DEADZONE)
    {
        return 0;
    }

    int maximumDistance = (delta > 0) ? (4095 - JOYSTICK_Y_CENTER) : JOYSTICK_Y_CENTER;

    int magnitude = map(abs(delta), JOYSTICK_Y_DEADZONE + 1, maximumDistance, 1, 127);

    magnitude = constrain(magnitude, 1, 127);

    int throttle = (delta > 0) ? magnitude : -magnitude;

    if (JOYSTICK_Y_INVERTED)
    {
        throttle = -throttle;
    }

    return static_cast<int8_t>(throttle);
}