#include <Arduino.h>
#include "atlas/AtlasCommand.h"
#include "uart/UART.h"
#include "common/Communication.h"


bool fireflyReady = false;

uint32_t lastHeartbeatSent = 0;
uint32_t lastValidResponse = 0;

constexpr uint32_t HEARTBEAT_INTERVAL_MS = 500;
constexpr uint32_t LINK_TIMEOUT_MS = 2000;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    uartSetup();
    startMessage();
    Serial.println("Waiting for Firefly link....");
}

void loop()
{
    // // Automatic [TX] for Firefly debugging
    // CommandID userCommand = CommandID::STATUS;
    // sendCommand(userCommand);
    // delay(5000);

    VehicleState state;

    if (receiveVehicleState(state))
    {
        lastValidResponse = millis();

        if (!fireflyReady)
        {
            fireflyReady = true;
            Serial.println("Firefly link synchronized.");
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

    if (fireflyReady && millis() - lastValidResponse >= LINK_TIMEOUT_MS)
    {
        fireflyReady = false;
        Serial.println("Firefly link lost.");
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
                else if (fireflyReady)
                {
                    executeCommand(userCommand, input);
                }
                else
                {
                    Serial.println("Waiting for Firefly link. Command not sent.");
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

