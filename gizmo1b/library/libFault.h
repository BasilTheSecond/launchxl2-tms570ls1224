#ifndef _LIB_FAULT_H_
#define _LIB_FAULT_H_

class LibFault
{
public:
    enum LibFaultState {
        LIB_FAULT_STATE_TEC_OCD_POS,
        LIB_FAULT_STATE_TEC_OCD_NEG,
        LIB_FAULT_STATE_OVERTEMP1,
        LIB_FAULT_STATE_OVERTEMP2,
    };
    enum LibFaultError {
        LIB_FAULT_OKAY,
        LIB_FAULT_ERROR_INVALID_STATE,
    };
    LibFault();
    virtual ~LibFault();
    void reset();
    int read(int state, bool& value);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_FAULT_H_ */
