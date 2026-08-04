#include "transport/Transport.h"
#include "uart/UART.h"


void transportSetup()
{
    uartSetup();
}

void transportSend(const uint8_t* data, size_t length)
{
    uartSend(data, length);
}

bool transportAvailable(size_t bytes)
{
    return uartAvailable(bytes);
}

size_t transportReceive(uint8_t* buffer, size_t maxLength)
{
    return uartReceive(buffer, maxLength);
}