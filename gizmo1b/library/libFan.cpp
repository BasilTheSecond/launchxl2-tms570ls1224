#include "libFan.h"

bool LibFan::s_isInitialized = false;

LibFan::LibFan()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibFan::~LibFan()
{
}

int LibFan::set(int value)
{
    if (value < 0 || value > 100) {
        return LIB_FAN_ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    return LIB_FAN_OKAY;
}

int LibFan::read()
{
    return 0;
}
