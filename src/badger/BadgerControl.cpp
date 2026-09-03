#include <Arduino.h>
#include "BadgerControl.h"
#include "Badger.h"

// GPIO pins
namespace
{
    constexpr uint8_t F_MOTOR_IN1_PIN = 32;
    constexpr uint8_t F_MOTOR_IN2_PIN = 33;
    constexpr uint8_t F_MOTOR_ENABLE_PIN = 14;

    constexpr uint8_t R_MOTOR_IN1_PIN = 25;
    constexpr uint8_t R_MOTOR_IN2_PIN = 26;
    constexpr uint8_t R_MOTOR_ENABLE_PIN = 13;

    constexpr uint32_t TEST_DURATION_MS = 2000;

    constexpr uint8_t F_MOTOR_PWM_CHANNEL = 0;
    constexpr uint8_t R_MOTOR_PWM_CHANNEL = 1;
    constexpr uint32_t MOTOR_PWM_FREQUENCY_HZ = 1000;
    constexpr uint8_t MOTOR_PWM_RESOLUTION_BITS = 8;
    constexpr uint8_t TEST_MOTOR_SPEED = 128;

    bool testActive = false;
    uint32_t testStartMs = 0;
} 


void badgerControlSetup()
{
    pinMode(F_MOTOR_IN1_PIN, OUTPUT);
    pinMode(F_MOTOR_IN2_PIN, OUTPUT);

    ledcSetup(F_MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION_BITS);
    ledcAttachPin(F_MOTOR_ENABLE_PIN, F_MOTOR_PWM_CHANNEL);

    pinMode(R_MOTOR_IN1_PIN, OUTPUT);
    pinMode(R_MOTOR_IN2_PIN, OUTPUT);

    ledcSetup(R_MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION_BITS);
    ledcAttachPin(R_MOTOR_ENABLE_PIN, R_MOTOR_PWM_CHANNEL);

    stopMotors();
}

void moveForward()
{
    digitalWrite(F_MOTOR_IN1_PIN, LOW);
    digitalWrite(F_MOTOR_IN2_PIN, HIGH);
    digitalWrite(R_MOTOR_IN1_PIN, HIGH);
    digitalWrite(R_MOTOR_IN2_PIN, LOW);

    setMotorSpeed(TEST_MOTOR_SPEED);
}

void moveBackward()
{
    digitalWrite(F_MOTOR_IN1_PIN, LOW);
    digitalWrite(F_MOTOR_IN2_PIN, HIGH);
    digitalWrite(R_MOTOR_IN1_PIN, HIGH);
    digitalWrite(R_MOTOR_IN2_PIN, LOW);

    setMotorSpeed(TEST_MOTOR_SPEED);
}

void stopMotors()
{
    setMotorSpeed(0);

    digitalWrite(F_MOTOR_IN1_PIN, LOW);
    digitalWrite(F_MOTOR_IN2_PIN, LOW);
    digitalWrite(R_MOTOR_IN1_PIN, LOW);
    digitalWrite(R_MOTOR_IN2_PIN, LOW);

    testActive = false;
}

void startForwardTest()
{
    moveForward();
    testStartMs = millis();
    testActive = true;
}

void startReverseTest()
{
    moveBackward();
    testStartMs = millis();
    testActive = true;
}

void updateBadgerControl()
{
    if (testActive && millis() - testStartMs >= TEST_DURATION_MS)
    {
        stopMotors();
    }
}

void setMotorSpeed(uint8_t duty)
{
    ledcWrite(F_MOTOR_PWM_CHANNEL, duty);
    ledcWrite(R_MOTOR_PWM_CHANNEL, duty);
}