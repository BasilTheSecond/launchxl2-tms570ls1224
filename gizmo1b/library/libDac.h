#ifndef _LIB_DAC_H_
#define _LIB_DAC_H_

class LibDac
{
public:
    enum LibDacError {
        LIB_DAC_OKAY,
        LIB_DAC_ERROR_SET_VALUE_OUT_OF_RANGE,
    };
    LibDac();
    virtual ~LibDac();
    int set(double value); // 0-5V
private:
    static bool s_isInitialized;
};

#endif /* _LIB_DAC_H_ */
