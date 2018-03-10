#include "libGpio.h"
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
        gioInit();
        mibspiInit();
        s_isInitialized = true;
    }
}

LibGpio::~LibGpio()
{
}

int LibGpio::setPin(int pin, bool set)
{
    uint32 result = OKAY;
    switch (pin) {
    case USER_LED_AOUT:
        gioSetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN, set);
        break;
    case USER_LED_BOUT:
        gioSetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN, set);
        break;
    case DOUT_0:
    case DOUT_1:
    case DOUT_2:
    case DOUT_3:
    case DOUT_4:
    case DOUT_5:
    case DOUT_6:
    case DOUT_7:
        break;
    default:
        result = INVALID_OUTPUT_PIN;
        break;
    }
    return result;
}

int LibGpio::getPin(int pin, bool& isSet)
{
    int result = OKAY;
    switch (pin) {
    case USER_LED_AIN:
        isSet = gioGetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN) != 0;
        break;
    case USER_LED_BIN:
        isSet = gioGetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN) != 0;
        break;
    case USER_SWITCH_A:
        isSet = LIB_GPIO_USER_SWITCH_A_VALUE != 0;
        break;
    case USER_SWITCH_B:
        isSet = LIB_GPIO_USER_SWITCH_B_VALUE != 0;
        break;
    case DIN_0:
    case DIN_1:
    case DIN_2:
    case DIN_3:
    case DIN_4:
    case DIN_5:
        isSet = false;
        break;
    default:
        result = INVALID_INPUT_PIN;
        break;
    }
    return result;
}
