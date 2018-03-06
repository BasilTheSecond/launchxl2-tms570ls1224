#include "libThermistor.h"

bool LibThermistor::s_isInitialized = false;

LibThermistor::LibThermistor()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibThermistor::~LibThermistor()
{
}

int LibThermistor::readTemp(int channel, double& value)
{
    switch (channel) {
    case LIB_THERMISTOR_TEMP_CHANNEL_0:
    case LIB_THERMISTOR_TEMP_CHANNEL_1:
    case LIB_THERMISTOR_TEMP_CHANNEL_2:
    case LIB_THERMISTOR_TEMP_CHANNEL_3:
    case LIB_THERMISTOR_TEMP_CHANNEL_4:
        value = 0.0;
        break;
    default:
        return LIB_THERMISTOR_ERROR_INVALID_NTC;
    }
    return LIB_THERMISTOR_OKAY;
}

int LibThermistor::readNtcPresent(int ntc, bool& isNtcPresent)
{
    switch (ntc) {
    case LIB_THERMISTOR_NTC1:
    case LIB_THERMISTOR_NTC2:
        isNtcPresent = false;
        break;
    default:
        return LIB_THERMISTOR_ERROR_INVALID_NTC;
    }
    return LIB_THERMISTOR_OKAY;
}

