#ifndef _LIB_FAN_H_
#define _LIB_FAN_H_

class LibFan
{
public:
    enum LibFanStatus {
        OKAY,
        ERROR_SET_VALUE_OUT_OF_RANGE,
    };
    LibFan();
    virtual ~LibFan();
    int set(int value); // 0-100
    int read(); // 0-100
private:
    static bool s_isInitialized;
};

#endif /* _LIB_FAN_H_ */
