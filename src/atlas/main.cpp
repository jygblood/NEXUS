#include <Arduino.h>
#include <WiFi.h>

#include "atlas/AtlasCommand.h"
#include "transport/Transport.h"
#include "common/Communication.h"


bool vehicleReady = false;

uint32_t lastHeartbeatSent = 0;
uint32_t lastValidResponse = 0;

constexpr uint32_t HEARTBEAT_INTERVAL_MS = 500;
constexpr uint32_t LINK_TIMEOUT_MS = 2000;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);

    // display setup
    transportSetup();
    startMessage();
    Serial.println("Waiting for vehicle link....");
}

void loop()
{
    // // Automatic [TX] for Firefly debugging
    // CommandID userCommand = CommandID::STATUS;
    // sendCommand(userCommand);
    // delay(5000);

    VehicleState state;
    uint8_t stateData;

    if (receiveVehicleState(state, stateData))
    {
        lastValidResponse = millis();

        if (!vehicleReady)
        {
            vehicleReady = true;
            Serial.println("Vehicle link synchronized.");
        }
        
        // // debug ACK check
        // if (state == VehicleState::HEARTBEAT_ACK)
        // {
        //     Serial.println("[HB] ACK received");
        // }
        
        if (state != VehicleState::HEARTBEAT_ACK)
        {
            printVehicleState(state);
        }
    }

    if (millis() - lastHeartbeatSent >= HEARTBEAT_INTERVAL_MS)
    {
        sendHeartbeat();
        lastHeartbeatSent = millis();
    }

    if (vehicleReady && millis() - lastValidResponse >= LINK_TIMEOUT_MS)
    {
        vehicleReady = false;
        Serial.println("Vehicle link lost.");
    }
    
    static String input;

    while (Serial.available() > 0)
    {
        char character = static_cast<char>(Serial.read());

        if (character == '\n' || character == '\r')
        {
            input.trim();

            if (input.length() > 0)
            {
                CommandID userCommand = parseCommand(input);
                if (userCommand == CommandID::HELP || userCommand == CommandID::UNKNOWN)
                {
                    executeCommand(userCommand,input);
                }
                else if (vehicleReady)
                {
                    executeCommand(userCommand, input);
                }
                else
                {
                    Serial.println("Waiting for vehicle link. Command not sent.");
                }
                
            }

            input = "";
        }
        else
        {
            input += character;
        }
    }
    
}

