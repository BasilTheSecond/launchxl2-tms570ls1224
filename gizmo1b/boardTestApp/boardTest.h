#ifndef _BOARD_TEST_H_
#define _BOARD_TEST_H_

#include "sys_common.h"

class BoardTest
{
public:
    // Command
    enum Command {
        REG_READ  = 1, // register read
        REG_WRITE = 2, // register write
    };

    // Command status
    enum Status {
        OKAY,       // Okay
        ERROR_CMD,  // Error unknown command
        ERROR_ADDR, // Error unknown register address
        ERROR_RO,   // Error read-only register
        ERROR_WO,   // Error write-only register
    };
    BoardTest();
    virtual ~BoardTest();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
};

#endif // _BOARD_TEST_H_
