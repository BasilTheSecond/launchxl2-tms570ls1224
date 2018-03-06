#include "libMotor.h"

bool LibMotor::s_isInitialized = false;

LibMotor::LibMotor()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibMotor::~LibMotor()
{
}

void LibMotor::initialize()
{

}

int LibMotor::moveRelative(int direction, int stepsCount)
{
    switch (direction) {
    case LIB_MOTOR_DIRECTION_POSITIVE:
    case LIB_MOTOR_DIRECTION_NEGATIVE:
        break;
    default:
        return LIB_MOTOR_ERROR_INVALID_DIRECTION;
    }
    if (stepsCount < 0 || stepsCount > 1000000) {
        return LIB_MOTOR_ERROR_STEPS_COUNT;
    }
    return LIB_MOTOR_OKAY;
}

int LibMotor::moveAbs(int position)
{
    return LIB_MOTOR_OKAY;
}

int LibMotor::setMicroSteps(int value)
{
    switch (value) {
    case LIB_MOTOR_MICROSTEPS_0:
    case LIB_MOTOR_MICROSTEPS_1:
    case LIB_MOTOR_MICROSTEPS_2:
    case LIB_MOTOR_MICROSTEPS_3:
    case LIB_MOTOR_MICROSTEPS_4:
    case LIB_MOTOR_MICROSTEPS_5:
    case LIB_MOTOR_MICROSTEPS_6:
    case LIB_MOTOR_MICROSTEPS_7:
        break;
    default:
        return LIB_MOTOR_ERROR_INVALID_MICROSTEPS;
    }
    return LIB_MOTOR_OKAY;
}

int LibMotor::setSpeed(double speed)
{
    return LIB_MOTOR_OKAY;
}

int LibMotor::setAcceleration(double acceleration)
{
    return LIB_MOTOR_OKAY;
}

int LibMotor::setDeceleration(double deceleration)
{
    return LIB_MOTOR_OKAY;
}

int LibMotor::setHoldCurrent(double holdCurrent)
{
    return LIB_MOTOR_OKAY;
}

