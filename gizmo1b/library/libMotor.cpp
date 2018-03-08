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
    case POSITIVE:
    case NEGATIVE:
        break;
    default:
        return ERROR_INVALID_DIRECTION;
    }
    if (stepsCount < 0 || stepsCount > 1000000) {
        return ERROR_STEPS_COUNT;
    }
    return OKAY;
}

int LibMotor::moveAbs(int position)
{
    return OKAY;
}

int LibMotor::setMicroSteps(int value)
{
    switch (value) {
    case MICROSTEPS_0:
    case MICROSTEPS_1:
    case MICROSTEPS_2:
    case MICROSTEPS_3:
    case MICROSTEPS_4:
    case MICROSTEPS_5:
    case MICROSTEPS_6:
    case MICROSTEPS_7:
        break;
    default:
        return ERROR_INVALID_MICROSTEPS;
    }
    return OKAY;
}

int LibMotor::setSpeed(float speed)
{
    return OKAY;
}

int LibMotor::setAcceleration(float acceleration)
{
    return OKAY;
}

int LibMotor::setDeceleration(float deceleration)
{
    return OKAY;
}

int LibMotor::setHoldCurrent(float holdCurrent)
{
    return OKAY;
}

