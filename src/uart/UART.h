#pragma once
#include <Arduino.h>

void uartSetup();

bool uartAvailable(size_t bytes);

void uartSend(const uint8_t* data, size_t length);

size_t uartReceive(uint8_t* buffer, size_t maxLength);
