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

int LibAdc::read(int channel, double& value)
{
    switch (channel) {
    case LIB_ADC_CHANNEL_0:
    case LIB_ADC_CHANNEL_1:
    case LIB_ADC_CHANNEL_2:
    case LIB_ADC_CHANNEL_3:
    case LIB_ADC_CHANNEL_4:
    case LIB_ADC_CHANNEL_5:
        value = 0.0;
        break;
    default:
        return LIB_ADC_ERROR_INVALID_CHANNEL;
    }
    return LIB_ADC_OKAY;
}
