#include "FreeRTOS.h"
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

#define portRESET_PRIVILEGE(xRunningPrivileged) \
                         if( xRunningPrivileged == 0 ) portSWITCH_TO_USER_MODE()
#ifdef __cplusplus
#pragma SWI_ALIAS(1)
#else
#pragma SWI_ALIAS(prvRaisePrivilege, 1)
#endif
extern "C" BaseType_t prvRaisePrivilege(void);

LibGpio::LibGpio()
{
    if (!s_isInitialized) {
        BaseType_t xRunningPrivileged = prvRaisePrivilege();
        gioInit();
        mibspiInit();
        portRESET_PRIVILEGE( xRunningPrivileged );
        s_isInitialized = true;
    }
}

LibGpio::~LibGpio()
{
}

int LibGpio::setPin(int pin, uint32 value)
{
    BaseType_t xRunningPrivileged = prvRaisePrivilege();
    uint32 result = LIB_GPIO_OKAY;
    switch (pin) {
    case LIB_GPIO_USER_LED_A:
        gioSetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN, value);
        break;
    case LIB_GPIO_USER_LED_B:
        gioSetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN, value);
        break;
    default:
        result = LIB_GPIO_ERROR_INVALID_PIN;
        break;
    }
    portRESET_PRIVILEGE( xRunningPrivileged );
    return result;
}

int LibGpio::getPin(int pin)
{
    BaseType_t xRunningPrivileged = prvRaisePrivilege();
    uint32 result = LIB_GPIO_ERROR_INVALID_PIN;
    switch (pin) {
    case LIB_GPIO_USER_LED_A:
        result = gioGetBit(LIB_GPIO_USER_LED_A_PORT, LIB_GPIO_USER_LED_A_PIN);
        break;
    case LIB_GPIO_USER_LED_B:
        result = gioGetBit(LIB_GPIO_USER_LED_B_PORT, LIB_GPIO_USER_LED_B_PIN);
        break;
    case LIB_GPIO_USER_SWITCH_A:
        result = LIB_GPIO_USER_SWITCH_A_VALUE ? 1 : 0;
        break;
    case LIB_GPIO_USER_SWITCH_B:
        result = LIB_GPIO_USER_SWITCH_B_VALUE ? 1 : 0;
        break;
    default:
        break;
    }
    portRESET_PRIVILEGE( xRunningPrivileged );
    return result;
}
