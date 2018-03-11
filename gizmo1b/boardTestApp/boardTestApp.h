#ifndef _BOARD_TEST_APP_H_
#define _BOARD_TEST_APP_H_

#include <vector>
#include "sys_common.h"
#include "libSci.h"
#include "libTask.h"
#include "boardTest.h"

class BoardTestApp: public LibTask {
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

    // Register memory map
    enum RegMemoryMap {
        // launchxl2-tms570ls1224
        USER_LED,     // USER_LED register
        USER_SWITCH,  // USER_SWITCH register
        // gizmo 1b
        REG_MEMORY_MAP_MAX
    };
public:
    BoardTestApp(const char* name);
    virtual ~BoardTestApp();
private:
    void decodeMessage(std::vector<uint8>& message,
                                                  std::vector<uint8>& response);
    int regRead(uint32 address, uint32& value);
    int regWrite(uint32 address, uint32 value);
    bool isAddressValid(uint32 address);
private:
    virtual void run();
    LibSci& m_libSci;
    union MasterToSlaveMsg m_masterToSlave;
    union SlaveToMasterMsg m_slaveToMaster;
    BoardTest* m_boardTestMap[REG_MEMORY_MAP_MAX];
};

#endif // _BOARD_TEST_APP_H_
