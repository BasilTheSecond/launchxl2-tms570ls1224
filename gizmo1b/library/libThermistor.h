#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

class LibThermistor
{
public:
    enum LibThermistorTempChannel {
        AIN_A,
        AIN_B,
        AIN_C,
        AIN_D,
    };
    enum LibThermistorStatus {
        OKAY,
        ERROR_INVALID_TEMP_CHANNEL,
    };
    LibThermistor();
    virtual ~LibThermistor();
    // Calculated Temp Based on Thermistor resistance table
    int readTemp(int channel, float& value);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_THERMISTOR_H_ */
