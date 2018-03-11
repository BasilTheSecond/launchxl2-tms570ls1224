#include "boardTestApp.h"
#include "libSci2.h"

struct BoardTestApp::RegAccessMethods BoardTestApp::s_regAccessMap[] = {
    // launchxl2-tms570ls1224 tests
    [USER_LED]    = {
        .m_readMethod  = &BoardTestApp::userLedGet,
        .m_writeMethod = &BoardTestApp::userLedSet,
    },
    [USER_SWITCH] = {
        .m_readMethod  = &BoardTestApp::userSwitchGet,
        .m_writeMethod = 0,
    },
    // gizmo 1b tests
};

BoardTestApp::BoardTestApp(const char* name) :
    LibTask(name),
    m_libGpio(*new LibGpio()),
    m_libSci(*new LibSci2(32, 32))
{
}

BoardTestApp::~BoardTestApp()
{
}

void BoardTestApp::run()
{
    bool resetSci = true;
    std::vector<uint8> message;
    message.reserve(32);
    std::vector<uint8> response;
    response.reserve(32);
    while (true) {
        if (resetSci) {
            m_libSci.close();
            m_libSci.setBaudRate(LibSci::BAUD_115200);
            m_libSci.setParity(LibSci::NO_PARITY);
            m_libSci.setStopBits(LibSci::TWO_STOP);
            m_libSci.open();
            resetSci = false;
        }
        while (!m_libSci.waitForReadyRead(1000)) {
            continue;
        }
        message.clear();
        m_libSci.read(message);
        while (message.size() < sizeof(m_masterToSlave)) {
            if (!m_libSci.waitForReadyRead(10)) {
                resetSci = true;
                break;
            }
            m_libSci.read(message);
        }
        if (!resetSci) {
            decodeMessage(message, response);
            if (m_libSci.write(response) == OKAY) {
                while (!m_libSci.waitForBytesWritten(1000)) {
                    continue;
                }
            }
            else {
                resetSci = true;
            }
        }
    }
}

void BoardTestApp::decodeMessage(std::vector<uint8>& message,
                                                   std::vector<uint8>& response)
{
    uint8* buffer = m_masterToSlave.buffer;
    for (int i = 0; i < sizeof(m_masterToSlave); i++) {
        buffer[i] = message[i];
    }
    struct MasterToSlaveMsg::msgMasterToSlave& msgMtoS = m_masterToSlave.msg;
    uint8 command = msgMtoS.command;
    uint32 address = msgMtoS.address[0] << 24
                   | msgMtoS.address[1] << 16
                   | msgMtoS.address[2] << 8
                   | msgMtoS.address[3];
    uint32 value = msgMtoS.data[0] << 24
                 | msgMtoS.data[1] << 16
                 | msgMtoS.data[2] << 8
                 | msgMtoS.data[3];
    uint32 status;
    switch (command) {
    default:
        status = ERROR_CMD;
        break;
    case REG_READ:
        status = regRead(address, value);
        break;
    case REG_WRITE:
        status = regWrite(address, value);
        break;
    }
    struct SlaveToMasterMsg::msgSlaveToMaster& msgStoM = m_slaveToMaster.msg;
    msgStoM.status  = status;
    msgStoM.data[0] = value >> 24;
    msgStoM.data[1] = value >> 16;
    msgStoM.data[2] = value >>  8;
    msgStoM.data[3] = value;
    buffer = m_slaveToMaster.buffer;
    response.clear();
    for (int i = 0; i < sizeof(m_slaveToMaster); i++) {
        response.push_back(buffer[i]);
    }
}

int BoardTestApp::regRead(uint32 address, uint32& value)
{
    if (address >= REG_MEMORY_MAP_MAX) {
        return ERROR_ADDR;
    }
    if (!s_regAccessMap[address].m_readMethod) {
        return ERROR_WO;
    }
    value = (this->*s_regAccessMap[address].m_readMethod)();
    return OKAY;
}

int BoardTestApp::regWrite(uint32 address, uint32 value)
{
    if (address >= REG_MEMORY_MAP_MAX) {
        return ERROR_ADDR;
    }
    if (!s_regAccessMap[address].m_writeMethod) {
        return ERROR_RO;
    }
    (this->*s_regAccessMap[address].m_writeMethod)(value);
    return OKAY;
}

// launchxl2-tms570ls1224 tests
uint32 BoardTestApp::userLedGet()
{
    bool isSet;
    uint32 value = 0;
    m_libGpio.getPin(LibGpio::USER_LED_AIN, isSet);
    value |= (isSet ? LED_A_ON : LED_A_OFF);
    m_libGpio.getPin(LibGpio::USER_LED_BIN, isSet);
    value |= (isSet ? LED_B_ON : LED_B_OFF);
    return value;
}

uint32 BoardTestApp::userSwitchGet()
{
    bool isSet;
    uint32 value = 0;
    m_libGpio.getPin(LibGpio::USER_SWITCH_A, isSet);
    value |= (isSet ? SWITCH_A : 0);
    m_libGpio.getPin(LibGpio::USER_SWITCH_B, isSet);
    value |= (isSet ? SWITCH_B : 0);
    return value;
}

void BoardTestApp::userLedSet(uint32 value)
{
    if (value & LED_A_ON) {
        m_libGpio.setPin(LibGpio::USER_LED_AOUT, true);
    }
    if (value & LED_A_OFF) {
        m_libGpio.setPin(LibGpio::USER_LED_AOUT, false);
    }
    if (value & LED_B_ON) {
        m_libGpio.setPin(LibGpio::USER_LED_BOUT, true);
    }
    if (value & LED_B_OFF) {
        m_libGpio.setPin(LibGpio::USER_LED_BOUT, false);
    }
}

// gizmo 1b tests
