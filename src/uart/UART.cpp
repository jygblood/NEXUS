#include "UART.h"

HardwareSerial telemetrySerial(2);

void uartSetup()
{
    telemetrySerial.begin(115200, SERIAL_8N1, 16, 17);
}

bool uartAvailable(size_t bytes)
{
    return telemetrySerial.available() >= bytes;
}

void uartSend(const uint8_t* data, size_t length)
{
    telemetrySerial.write(data, length);
}

size_t uartReceive(uint8_t* buffer, size_t maxLength)
{   
    return telemetrySerial.readBytes(buffer, maxLength);
}