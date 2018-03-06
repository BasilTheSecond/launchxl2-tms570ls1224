#include "libGpio.h"
#include "libRaisePrivilege.h"
#include "gio.h"
#include "mibspi.h"

#define LIB_GPIO_USER_LED_A_PORT     gioPORTB
#define LIB_GPIO_USER_LED_A_PIN      1
#define LIB_GPIO_USER_LED_B_PORT     gioPORTB
#define LIB_GPIO_USER_LED_B_PIN      2
#define LIB_GPIO_USER_SWITCH_A_VALUE (mibspiREG3->PC2 & ((uint32)((uint32)1U << 0U)))
#define LIB_GPIO_USER_SWITCH_B_VALUE (mibspiREG1->PC2 & ((uint32)((uint32)1U << 4U)))

bool LibGpio::s_isInitialized = false;

LibGpio::LibGpio()
{
    if (!s_isInitialized) {
        LibRaisePrivilege libRaisePrivilege;
        gioInit();
        mibspiInit();
        s_isInitialized = true;
    }
}

LibGpio::~LibGpio()
{
}

int LibGpio::setPin(int pin, bool value)
{
    LibRaisePrivilege libRaisePrivilege;
    uint32 result = LIB_GPIO_OKAY;
    switch (pin) {
    case LIB_GPIO_USER_LED_A:
        gioSetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN, value);
        break;
    case LIB_GPIO_USER_LED_B:
        gioSetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN, value);
        break;
    case LIB_GPIO_DOUT_0:
    case LIB_GPIO_DOUT_1:
    case LIB_GPIO_DOUT_2:
    case LIB_GPIO_DOUT_3:
    case LIB_GPIO_DOUT_4:
    case LIB_GPIO_DOUT_5:
    case LIB_GPIO_DOUT_6:
    case LIB_GPIO_DOUT_7:
        break;
    default:
        result = LIB_GPIO_ERROR_INVALID_PIN;
        break;
    }
    return result;
}

int LibGpio::getPin(int pin, bool& value)
{
    LibRaisePrivilege libRaisePrivilege;
    int result = LIB_GPIO_OKAY;
    switch (pin) {
    case LIB_GPIO_USER_LED_A:
        value = gioGetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN) != 0;
        break;
    case LIB_GPIO_USER_LED_B:
        value = gioGetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN) != 0;
        break;
    case LIB_GPIO_USER_SWITCH_A:
        value = LIB_GPIO_USER_SWITCH_A_VALUE != 0;
        break;
    case LIB_GPIO_USER_SWITCH_B:
        value = LIB_GPIO_USER_SWITCH_B_VALUE != 0;
        break;
    case LIB_GPIO_DIN_0:
    case LIB_GPIO_DIN_1:
    case LIB_GPIO_DIN_2:
    case LIB_GPIO_DIN_3:
    case LIB_GPIO_DIN_4:
    case LIB_GPIO_DIN_5:
        value = false;
        break;
    default:
        result = LIB_GPIO_ERROR_INVALID_PIN;
        break;
    }
    return result;
}
