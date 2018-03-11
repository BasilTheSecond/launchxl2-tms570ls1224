// launchxl2-tms570ls1224
#ifndef _BOARD_TEST_USER_LED_H_
#define _BOARD_TES_USER_LED_H_

#include "boardTest.h"
#include "libGpio.h"

class BoardTestUserLed : public BoardTest
{
public:
    BoardTestUserLed();
    virtual ~BoardTestUserLed();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    enum Bits {
        LED_A_ON  = (uint32)((uint32)1U << 0U),
        LED_A_OFF = (uint32)((uint32)1U << 1U),
        LED_B_ON  = (uint32)((uint32)1U << 2U),
        LED_B_OFF = (uint32)((uint32)1U << 3U),
    };
    LibGpio m_libGpio;
};

#endif // _BOARD_TES_USER_LED_H_
