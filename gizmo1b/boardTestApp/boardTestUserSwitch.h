// launchxl2-tms570ls1224
#ifndef _BOARD_TEST_USER_SWITCH_H_
#define _BOARD_TEST_USER_SWITCH_H_

#include "boardTest.h"
#include "libGpio.h"

class BoardTestUserSwitch : public BoardTest
{
public:
    BoardTestUserSwitch();
    virtual ~BoardTestUserSwitch();
    virtual int get(uint32 address, uint32& value);
private:
    enum Bits {
        SWITCH_A = (uint32)((uint32)1U << 0U),
        SWITCH_B = (uint32)((uint32)1U << 1U),
    };
    LibGpio m_libGpio;
};

#endif // _BOARD_TEST_USER_SWITCH_H_
