#include <Arduino.h>
#include "uart/UART.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);
    uartSetup();

    Serial.println("=======================");
    Serial.println("Vehicle Controller v0.1");
    Serial.println("=======================");
  
}

void loop()
{
    if (uartAvail())
    {
        String message = uartReceive();
        Serial.print("Received: ");
        Serial.println(message);
        
        if (message == "PING")
        {
            uartSend("PONG");
        }
    }
}