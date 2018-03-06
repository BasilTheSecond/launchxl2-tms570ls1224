#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

class LibAdc
{
public:
    enum LibAdcChannel {
        LIB_ADC_CHANNEL_0,
        LIB_ADC_CHANNEL_1,
        LIB_ADC_CHANNEL_2,
        LIB_ADC_CHANNEL_3,
        LIB_ADC_CHANNEL_4,
        LIB_ADC_CHANNEL_5,
    };
    enum LibAdcError {
        LIB_ADC_OKAY,
        LIB_ADC_ERROR_INVALID_CHANNEL,
    };
    LibAdc();
    virtual ~LibAdc();
    int read(int channel, double& value); // 0-5V
private:
    static bool s_isInitialized;
};

#endif /* _LIB_ADC_H_ */
