#include <Arduino.h>
#include "uart/UART.h"
#include "firefly/Firefly.h"
#include "firefly/FireflyCommand.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);
    uartSetup();

    Serial.println("=======================");
    Serial.println("Firefly Controller");
    Serial.println("=======================");

    initializeFirefly();
  
}

void loop()
{
    if (uartAvail())
    {
        String received = uartReceive();
        Serial.print("Received: ");
        Serial.println(received);
        
        if (received == "ARM")
        {
            handleCommand(RecCommandType::ARM);
        }
        else if (received == "DISARM")
        {
            handleCommand(RecCommandType::DISARM);
        }
        else if (received == "STATUS")
        {
            handleCommand(RecCommandType::STATUS);
        }
        else
        {
            handleCommand(RecCommandType::FAULT);
        }
        
    }
}