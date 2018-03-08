#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

class LibAdc
{
public:
    enum LibAdcChannel {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
    };
    enum LibAdcStatus {
        OKAY,
        ERROR_INVALID_CHANNEL,
    };
    LibAdc();
    virtual ~LibAdc();
    int read(int channel, float& value); // 0-5V
private:
    static bool s_isInitialized;
};

#endif /* _LIB_ADC_H_ */
