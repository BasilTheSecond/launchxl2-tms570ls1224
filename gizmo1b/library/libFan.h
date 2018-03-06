#ifndef _LIB_FAN_H_
#define _LIB_FAN_H_

class LibFan
{
public:
    enum LibFanError {
        LIB_FAN_OKAY,
        LIB_FAN_ERROR_SET_VALUE_OUT_OF_RANGE,
    };
    LibFan();
    virtual ~LibFan();
    int set(int value); // 0-100
    int read(); // 0-100
private:
    static bool s_isInitialized;
};

#endif /* _LIB_FAN_H_ */
