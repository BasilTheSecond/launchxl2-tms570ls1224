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

int LibFault::readState(int state, bool& isFault)
{
    switch (state) {
    case TEC_OCD_POS:
    case TEC_OCD_NEG:
    case OVERTEMP1:
    case OVERTEMP2:
        isFault = false;
        break;
    default:
        return INVALID_STATE;
    }
    return OKAY;
}

int LibFault::readNtcPresent(int ntc, bool& isNtcPresent)
{
    switch (ntc) {
    case NTC1:
    case NTC2:
        isNtcPresent = false;
        break;
    default:
        return INVALID_NTC;
    }
    return OKAY;
}
