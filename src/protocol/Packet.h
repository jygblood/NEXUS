#pragma once

#include <stdint.h>

constexpr uint8_t PACKET_HEADER = 0xAA;
constexpr uint8_t PACKET_FOOTER = 0x55;

enum class CommandID : uint8_t
{
    ARM = 0x01,
    DISARM = 0x02,
    STATUS = 0x03,
    TEST_FORWARD = 0x10,
    TEST_REVERSE = 0x11,
    HELP = 0x04,
    UNKNOWN = 0x05,
    HEARTBEAT = 0xF0
};

struct CommandPacket
{
    uint8_t header;
    CommandID command;
    uint8_t footer;
};

enum class VehicleState : uint8_t
{
    BOOTING = 0x01,
    DISARMED = 0x02,
    ARMED = 0x03,
    FAULT = 0x04,
    COM_FAULT = 0x05,
    NOT_AVAIL = 0x06,
    HEARTBEAT_ACK = 0xF1
};

struct VehicleStatePacket
{
    uint8_t header;
    VehicleState state;
    uint8_t footer;
};

static_assert(sizeof(CommandPacket) == 3);
static_assert(sizeof(VehicleStatePacket) == 3);
