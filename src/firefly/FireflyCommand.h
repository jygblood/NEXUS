#pragma once
#include <Arduino.h>

enum class RecCommandType : uint8_t
{
    ARM,
    DISARM,
    STATUS,
    FAULT
};


void handleCommand(RecCommandType command);