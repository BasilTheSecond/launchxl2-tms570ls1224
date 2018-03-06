#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

class LibGpio {
public:
    enum LibGpioPin {
        /* launchxl2-tms570ls1224 */
        LIB_GPIO_USER_LED_A,
        LIB_GPIO_USER_LED_B,
        LIB_GPIO_USER_SWITCH_A,
        LIB_GPIO_USER_SWITCH_B,
        /* gizmo 1b */
        LIB_GPIO_DOUT_0,
        LIB_GPIO_DOUT_1,
        LIB_GPIO_DOUT_2,
        LIB_GPIO_DOUT_3,
        LIB_GPIO_DOUT_4,
        LIB_GPIO_DOUT_5,
        LIB_GPIO_DOUT_6,
        LIB_GPIO_DOUT_7,
        LIB_GPIO_DIN_0,
        LIB_GPIO_DIN_1,
        LIB_GPIO_DIN_2,
        LIB_GPIO_DIN_3,
        LIB_GPIO_DIN_4,
        LIB_GPIO_DIN_5,
    };
    enum LibGpioError {
        LIB_GPIO_OKAY,
        LIB_GPIO_ERROR_INVALID_PIN,
    };
public:
    LibGpio();
    virtual ~LibGpio();
    int setPin(int pin, bool value);
    int getPin(int pin, bool& value);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_GPIO_H_ */
