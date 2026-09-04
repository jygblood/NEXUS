#include <Arduino.h>
#include <WiFi.h>

#include "atlas/AtlasCommand.h"
#include "atlas/AtlasInput.h"
#include "atlas/AtlasControl.h"
#include "transport/Transport.h"
#include "common/Communication.h"


bool vehicleReady = false;

bool vehicleArmed = false;
bool joystickNeutralSeen = false;

bool driveCommandSent = false;
int8_t lastSentThrottle = 0;
uint32_t lastDriveCommandSent = 0;

uint32_t lastHeartbeatSent = 0;
uint32_t lastValidResponse = 0;

constexpr uint32_t HEARTBEAT_INTERVAL_MS = 500;
constexpr uint32_t LINK_TIMEOUT_MS = 2000;
constexpr uint32_t DRIVE_COMMAND_INTERVAL_MS = 100;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);

    // display setup
    transportSetup();
    atlasInputSetup();
    startMessage();
    Serial.println("Waiting for vehicle link....");
}

void loop()
{
    AtlasInputState atlasInput = readAtlasInputs();
    int8_t throttle = getDriveThrottle(atlasInput);
    
    static uint32_t lastInputPrintMs = 0;
    constexpr uint32_t INPUT_PRINT_INTERVAL_MS = 250;

    if (millis() - lastInputPrintMs >= INPUT_PRINT_INTERVAL_MS)
    {
        lastInputPrintMs = millis();

        // Serial.print("X: ");
        // Serial.print(atlasInput.joystickX);
        // Serial.print(" Y: ");
        // Serial.print(atlasInput.joystickY);
        // Serial.print(" Throttle: ");
        // Serial.println(static_cast<int>(throttle));
    }

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
        
        
        if (state != VehicleState::HEARTBEAT_ACK)
        {
            if (state == VehicleState::ARMED && !vehicleArmed)
            {
                vehicleArmed = true;

                // After each arm event, teh joystick must return to neutral before dirivng enabled
                joystickNeutralSeen = false;
            }
            else if (state == VehicleState::DISARMED ||
                    state == VehicleState::COM_FAULT ||
                    state == VehicleState::FAULT ||
                    state == VehicleState::BOOTING)
            {
                vehicleArmed = false;
                joystickNeutralSeen = false;
                driveCommandSent = false;
            }
            
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
        vehicleArmed = false;
        joystickNeutralSeen = false;
        driveCommandSent = false;
        Serial.println("Vehicle link lost.");
    }
    
    if (vehicleReady && vehicleArmed)
    {
        if (throttle == 0)
        {
            joystickNeutralSeen = true;
        }

        if (joystickNeutralSeen)
        {
            bool throttleChanged = !driveCommandSent || throttle != lastSentThrottle;

            bool refreshRequired = (throttle != 0) && (millis() - lastDriveCommandSent >= DRIVE_COMMAND_INTERVAL_MS);

            if (throttleChanged || refreshRequired)
            {
                sendCommand(CommandID::DRIVE, encodeDriveThrottle(throttle));

                lastSentThrottle = throttle;
                lastDriveCommandSent = millis();
                driveCommandSent = true;
            }
        }
    }
    else
    {
        driveCommandSent = false;
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

