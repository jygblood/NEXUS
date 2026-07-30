#pragma once
#include <Arduino.h>

void uartSetup();
void uartSend(String message);
bool uartAvail();

String uartReceive();