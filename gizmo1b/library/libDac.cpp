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

int LibDac::set(double value)
{
    if (value < 0 || value > 5) {
        return LIB_DAC_ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    return LIB_DAC_OKAY;
}
