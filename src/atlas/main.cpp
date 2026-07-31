#include <Arduino.h>
#include "atlas/AtlasCommand.h"
#include "uart/UART.h"


void setup()
{
    Serial.begin(115200);
    delay(1000);
    uartSetup();
    startMessage();
}

void loop()
{
    if (Serial.available()){
      String input = Serial.readStringUntil('\n');
      input.trim();
      
      CommandType userCommand = parseCommand(input);

      executeCommand(userCommand, input);
      
    }

    if (uartAvail())
    {
        String message = uartReceive();

        Serial.print("Vehicle responds: ");
        Serial.println(message);
    }
}

