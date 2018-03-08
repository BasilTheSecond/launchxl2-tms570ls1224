#ifndef _LIB_FAULT_H_
#define _LIB_FAULT_H_

class LibFault
{
public:
    enum LibFaultState {
        TEC_OCD_POS,
        TEC_OCD_NEG,
        OVERTEMP1, // only valid if NTC1 present
        OVERTEMP2, // only valid if NTC2 present
    };
    enum LibFaultNtc {
        NTC1,
        NTC2,
    };
    enum LibFaultStatus {
        OKAY,
        INVALID_STATE,
        INVALID_NTC,
    };
    LibFault();
    virtual ~LibFault();
    void reset();
    int readState(int state, bool& isFault); // monitor every 1s
    int readNtcPresent(int ntc, bool& isNtcPresent);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_FAULT_H_ */
