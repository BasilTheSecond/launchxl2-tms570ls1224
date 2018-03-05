#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#include "sys_common.h"

class LibGpio {
public:
    enum libGpioPin {
        LIB_GPIO_USER_LED_A,
        LIB_GPIO_USER_LED_B,
        LIB_GPIO_USER_SWITCH_A,
        LIB_GPIO_USER_SWITCH_B,
    };
    enum libGpioError {
        LIB_GPIO_OKAY,
        LIB_GPIO_ERROR_INVALID_PIN = 0x80000000,
    };
public:
    LibGpio();
    virtual ~LibGpio();
    static int setPin(int pin, uint32 value);
    static int getPin(int pin);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_GPIO_H_ */
