#include <Arduino.h>
#include <WiFi.h>

#include "firefly/Firefly.h"
#include "firefly/FireflyCommand.h"
#include "protocol/Packet.h"
#include "common/Communication.h"
#include "transport/Transport.h"
#include "firefly/imu/Icm20948.h"


uint32_t lastValidCommand = 0;
bool atlasLinkLost = false;

constexpr uint32_t ATLAS_LINK_TIMEOUT_MS = 3000;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);

    transportSetup();

    Serial.println("=======================");
    Serial.println("Firefly Controller");
    Serial.println("=======================");
    Serial.println("");

    if (!imuSetup())
    {
        Serial.println("ICM-20948 initialization failed");
    }
    else{
        Serial.println("ICM-20948 initialized");
    }
    
    initializeFirefly();

    lastValidCommand = millis();
  
}

void loop()
{
    CommandID command;

    if (receiveCommand(command))
    {
        lastValidCommand = millis();

        if (atlasLinkLost)
        {
            atlasLinkLost = false;
            Serial.println("Atlas link restored.");
        }

        if (command == CommandID::HEARTBEAT)
        {
            sendHeartbeatAck();
        }
        else
        {
            handleCommand(command);
        }
    }

    if (!atlasLinkLost && millis() - lastValidCommand >= ATLAS_LINK_TIMEOUT_MS)
    {
        atlasLinkLost = true;
        comFault();
    }
        

    // temp print for IMU data
    static uint32_t lastImuPrintMs = 0;
    constexpr uint32_t IMU_PRINT_INTERVAL_MS = 250;

    if (millis() - lastImuPrintMs >= IMU_PRINT_INTERVAL_MS)
    {
        lastImuPrintMs = millis();

        ImuSample sample;

        if (imuRead(sample))
        {
            Serial.print("Accel [m/s^2]: ");
            Serial.print(sample.accelX, 2);
            Serial.print(", ");
            Serial.print(sample.accelY, 2);
            Serial.print(", ");
            Serial.println(sample.accelZ, 2);

            Serial.print("Gyro [rad/s]: ");
            Serial.print(sample.gyroX, 3);
            Serial.print(", ");
            Serial.print(sample.gyroY, 3);
            Serial.print(", ");
            Serial.println(sample.gyroZ, 3);

            Serial.print("Mag [uT]: ");
            Serial.print(sample.magX, 2);
            Serial.print(", ");
            Serial.print(sample.magY, 2);
            Serial.print(", ");
            Serial.println(sample.magZ, 2);

            Serial.print("Temp [C]: ");
            Serial.println(sample.temperatureC, 1);
            Serial.println();
        }
        else
        {
            Serial.println("IMU read failed");
        }
    }
}