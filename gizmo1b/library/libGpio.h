#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

class LibGpio {
public:
    enum LibGpioInputPin {
        /* launchxl2-tms570ls1224 */
        USER_LED_AIN,
        USER_LED_BIN,
        USER_SWITCH_A,
        USER_SWITCH_B,
        /* gizmo 1b */
        DIN_0,
        DIN_1,
        DIN_2,
        DIN_3,
        DIN_4,
        DIN_5,
    };
    enum LibGpioOutputPin {
        /* launchxl2-tms570ls1224 */
        USER_LED_AOUT,
        USER_LED_BOUT,
        /* gizmo 1b */
        DOUT_0,
        DOUT_1,
        DOUT_2,
        DOUT_3,
        DOUT_4,
        DOUT_5,
        DOUT_6,
        DOUT_7,
    };
    enum LibGpioStatus {
        OKAY,
        INVALID_INPUT_PIN,
        INVALID_OUTPUT_PIN,
    };
public:
    LibGpio();
    virtual ~LibGpio();
    int setPin(int pin, bool set);
    int getPin(int pin, bool& isSet);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_GPIO_H_ */
