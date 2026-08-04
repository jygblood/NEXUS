#pragma once

#include <Arduino.h>

void transportSetup();
void transportSend(const uint8_t* data, size_t length);
bool transportAvailable(size_t bytes);
size_t transportReceive(uint8_t* buffer, size_t maxLength);