#include "boardTestUserLed.h"

BoardTestUserLed::BoardTestUserLed()
{
}

BoardTestUserLed::~BoardTestUserLed()
{
}

int BoardTestUserLed::get(uint32 address, uint32& value)
{
    bool isSet;
    value = 0;
    m_libGpio.getPin(LibGpio::USER_LED_AIN, isSet);
    value |= (isSet ? LED_A_ON : LED_A_OFF);
    m_libGpio.getPin(LibGpio::USER_LED_BIN, isSet);
    value |= (isSet ? LED_B_ON : LED_B_OFF);
    return OKAY;
}

int BoardTestUserLed::set(uint32 address, uint32 value)
{
    if (value & LED_A_ON) {
        m_libGpio.setPin(LibGpio::USER_LED_AOUT, true);
    }
    if (value & LED_A_OFF) {
        m_libGpio.setPin(LibGpio::USER_LED_AOUT, false);
    }
    if (value & LED_B_ON) {
        m_libGpio.setPin(LibGpio::USER_LED_BOUT, true);
    }
    if (value & LED_B_OFF) {
        m_libGpio.setPin(LibGpio::USER_LED_BOUT, false);
    }
    return OKAY;
}
