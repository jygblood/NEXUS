#include "UART.h"

HardwareSerial telemetrySerial(2);

void uartSetup()
{
    telemetrySerial.begin(115200, SERIAL_8N1, 16, 17);
}

void uartSend(String message)
{
    telemetrySerial.println(message);
}

bool uartAvail()
{
    return telemetrySerial.available();
}

String uartReceive()
{
    String message = telemetrySerial.readStringUntil('\n');
    message.trim();
    
    return message;
}