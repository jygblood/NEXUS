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
    DRIVE = 0x12,
    HELP = 0x04,
    UNKNOWN = 0x05,
    HEARTBEAT = 0xF0
};

constexpr uint8_t DRIVE_THROTTLE_NEUTRAL = 127;

inline uint8_t encodeDriveThrottle(int8_t throttle)
{
    return static_cast<uint8_t>(static_cast<int>(throttle) + DRIVE_THROTTLE_NEUTRAL);
}

inline int8_t decodeDriveThrottle(uint8_t data)
{
    int decodedThrottle = static_cast<int>(data) - DRIVE_THROTTLE_NEUTRAL;

    // Data value 255 is outside -127 to +127 range
    if (decodedThrottle > 127)
    {
        return 0;
    }

    return static_cast<int8_t>(decodedThrottle);
}

struct CommandPacket
{
    uint8_t header;
    CommandID command;
    uint8_t data;
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
    uint8_t data;
    uint8_t footer;
};

static_assert(sizeof(CommandPacket) == 4);
static_assert(sizeof(VehicleStatePacket) == 4);
