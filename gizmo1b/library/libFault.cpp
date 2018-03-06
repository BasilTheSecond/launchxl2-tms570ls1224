#include "libFault.h"

bool LibFault::s_isInitialized = false;

LibFault::LibFault()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibFault::~LibFault()
{
}

void LibFault::reset()
{
}

int LibFault::read(int state, bool& value)
{
    switch (state) {
    case LIB_FAULT_STATE_TEC_OCD_POS:
    case LIB_FAULT_STATE_TEC_OCD_NEG:
    case LIB_FAULT_STATE_OVERTEMP1:
    case LIB_FAULT_STATE_OVERTEMP2:
        value = false;
        break;
    default:
        return LIB_FAULT_ERROR_INVALID_STATE;
    }
    return LIB_FAULT_OKAY;
}
