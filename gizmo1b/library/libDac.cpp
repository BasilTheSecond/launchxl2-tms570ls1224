#include "libDac.h"

bool LibDac::s_isInitialized = false;

LibDac::LibDac()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibDac::~LibDac()
{
}

int LibDac::set(float value)
{
    if (value < 0 || value > 5) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    return OKAY;
}
