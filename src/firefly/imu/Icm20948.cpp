#include "Icm20948.h"

#include <Wire.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>


namespace
{
    constexpr uint8_t IMU_SDA_PIN = 21;
    constexpr uint8_t IMU_SCL_PIN = 22;
    constexpr uint8_t IMU_I2C_ADDRESS = 0x69;

    Adafruit_ICM20948 icm;
    bool initialized = false;
}

bool imuSetup()
{
    Wire.begin(IMU_SDA_PIN, IMU_SCL_PIN);

    initialized = icm.begin_I2C(IMU_I2C_ADDRESS, &Wire);

    return initialized;
}

bool imuRead(ImuSample& sample)
{
    if (!initialized)
    {
        return false;
    }

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temperature;
    sensors_event_t mag;

    icm.getEvent(&accel, &gyro, &temperature, &mag);

    sample.accelX = accel.acceleration.x;
    sample.accelY = accel.acceleration.y;
    sample.accelZ = accel.acceleration.z;

    sample.gyroX = gyro.gyro.x;
    sample.gyroY = gyro.gyro.y;
    sample.gyroZ = gyro.gyro.z;

    sample.magX = mag.magnetic.x;
    sample.magY = mag.magnetic.y;
    sample.magZ = mag.magnetic.z;

    sample.temperatureC = temperature.temperature;
    sample.timestampMs = millis();

    return true;
}

bool imuHealthy()
{
    return initialized;
}