#pragma once

#include <stdint.h>

struct ImuSample
{
    float accelX;
    float accelY;
    float accelZ;

    float gyroX;
    float gyroY;
    float gyroZ;

    float magX;
    float magY;
    float magZ;

    float temperatureC;
    uint32_t timestampMs;
};

bool imuSetup();
bool imuRead(ImuSample& sample);
bool imuHealthy();