#include "boardTestUserSwitch.h"

BoardTestUserSwitch::BoardTestUserSwitch()
{
}

BoardTestUserSwitch::~BoardTestUserSwitch()
{
}

int BoardTestUserSwitch::get(uint32 address, uint32& value)
{
    bool isSet;
    value = 0;
    m_libGpio.getPin(LibGpio::USER_SWITCH_A, isSet);
    value |= (isSet ? SWITCH_A : 0);
    m_libGpio.getPin(LibGpio::USER_SWITCH_B, isSet);
    value |= (isSet ? SWITCH_B : 0);
    return OKAY;
}
