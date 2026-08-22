#include <Arduino.h>
#include <WiFi.h>

#include "badger/Badger.h"
#include "badger/BadgerCommand.h"
#include "protocol/Packet.h"
#include "common/Communication.h"
#include "transport/Transport.h"

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
    Serial.println("Badger Controller");
    Serial.println("=======================");
    Serial.println("");

    initializeBadger();

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
}