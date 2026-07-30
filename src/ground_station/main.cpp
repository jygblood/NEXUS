#include <Arduino.h>
#include "common/Command.h"
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
    uartSend("PING");
    Serial.println("PING");
    delay(1000);

    if (uartAvail())
    {
        String message = uartReceive();

        Serial.print("Vehicle responds: ");
        Serial.println(message);
    }
    // if (Serial.available()){
    //   String input = Serial.readStringUntil('\n');
    //   input.trim();
      
    //   CommandType receivedCommand = parseCommand(input);

    //   executeCommand(receivedCommand, input);
      
    // }
}

