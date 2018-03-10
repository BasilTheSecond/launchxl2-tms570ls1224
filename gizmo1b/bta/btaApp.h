#ifndef _BTA_APP_H_
#define _BTA_APP_H_

#include "sys_common.h"
#include "libSci.h"
#include "libTask.h"
#include "libGpio.h"

class BtaApp: public LibTask {
public:
    // Communication protocol between master (M) and slave (S):
    // Register read:
    // M->S: RD A0 A1 A2 A3 X0 X1 X2 X3
    // S->M: S D0 D1 D2 D3
    // RD: Register read command (1 byte)
    // A0..3: Register address (4 bytes)
    // X0..3: "Don't care" data (4 bytes)
    // S: Command status (1 byte)
    // D0..3: Register value (4 bytes)
    // A0: Register address MSB
    // A3: Register address LSB
    // D0: Register value MSB
    // D3: Register value LSB
    // Register write:
    // M->S: WR A0 A1 A2 A3 D0 D1 D2 D3
    // S->M: S D0 D1 D2 D3
    // WR: Register write command (1 byte)
    // A0..3: Register address (4 bytes)
    // D0..3: Register value (4 bytes)
    // S: Command status (1 byte)
    // A0: Register address MSB
    // A3: Register address LSB
    // D0: Register value MSB
    // D3: Register value LSB

    // Messages
    union MasterToSlaveMsg {
        struct msgMasterToSlave {
            uint8 command;
            uint8 address[4];
            uint8 data[4];
        } msg;
        uint8 buffer[sizeof(struct msgMasterToSlave)];
    };

    union SlaveToMasterMsg {
        struct msgSlaveToMaster {
            uint8 status;
            uint8 data[4];
        } msg;
        uint8 buffer[sizeof(struct msgSlaveToMaster)];
    };

    // Command
    enum Command {
        REG_READ  = 0x01, // register read
        REG_WRITE = 0x02, // register write
        REG_MAX
    };

    // Command status
    enum Status {
        OKAY,       // Okay
        ERROR_CMD,  // Error unknown command
        ERROR_ADDR, // Error unknown register address
        ERROR_RO,   // Error RO register
    };

    // Register memory map
    enum RegMemoryMap {
        USER_LED    = 0x00000000,  // USER_LED register, R/W
        USER_SWITCH = 0x00000004,  // USER_SWITCH register, R/O
    };

    // Register bits
    enum RegUserLedBits {
        LED_A_ON  = (uint32)((uint32)1U << 0U),
        LED_A_OFF = (uint32)((uint32)1U << 1U),
        LED_B_ON  = (uint32)((uint32)1U << 2U),
        LED_B_OFF = (uint32)((uint32)1U << 3U),
    };

    enum UserSwitchBits {
        SWITCH_A = (uint32)((uint32)1U << 0U),
        SWITCH_B = (uint32)((uint32)1U << 1U),
    };
    BtaApp(const char* name);
    virtual ~BtaApp();
private:
    virtual void run();
    LibGpio& m_libGpio;
    LibSci& m_libSci;
    union MasterToSlaveMsg m_masterToSlave;
    union SlaveToMasterMsg m_slaveToMaster;
};

#endif /* _BTA_APP_H_ */
