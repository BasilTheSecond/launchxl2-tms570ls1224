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

int LibThermistor::readTemp(int channel, float& value)
{
    switch (channel) {
    case AIN_A:
    case AIN_B:
    case AIN_C:
    case AIN_D:
        value = 0.0;
        break;
    default:
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    return OKAY;
}
