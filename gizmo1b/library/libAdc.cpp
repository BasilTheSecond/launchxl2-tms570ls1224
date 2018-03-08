#include "libAdc.h"

bool LibAdc::s_isInitialized = false;

LibAdc::LibAdc()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibAdc::~LibAdc()
{
}

int LibAdc::read(int channel, float& value)
{
    switch (channel) {
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
        value = 0.0;
        break;
    default:
        return ERROR_INVALID_CHANNEL;
    }
    return OKAY;
}
