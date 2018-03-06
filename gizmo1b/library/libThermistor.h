#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

class LibThermistor
{
public:
    enum LibThermistorNtc {
        LIB_THERMISTOR_NTC1,
        LIB_THERMISTOR_NTC2,
    };
    enum LibThermistorTempChannel {
        LIB_THERMISTOR_TEMP_CHANNEL_0,
        LIB_THERMISTOR_TEMP_CHANNEL_1,
        LIB_THERMISTOR_TEMP_CHANNEL_2,
        LIB_THERMISTOR_TEMP_CHANNEL_3,
        LIB_THERMISTOR_TEMP_CHANNEL_4,
    };
    enum LibThermistorError {
        LIB_THERMISTOR_OKAY,
        LIB_THERMISTOR_ERROR_INVALID_NTC,
        LIB_THERMISTOR_ERROR_INVALID_TEMP_CHANNEL,
    };
    LibThermistor();
    virtual ~LibThermistor();
    // Calculated Temp Based on Thermistor resistance table
    int readTemp(int channel, double& value);
    int readNtcPresent(int ntc, bool& isNtcPresent);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_THERMISTOR_H_ */
