#include "boardTestApp.h"
#include "libSci2.h"
#include "boardTestUserLed.h"
#include "boardTestUserSwitch.h"

BoardTestApp::BoardTestApp(const char* name) :
    LibTask(name),
    m_libSci(*new LibSci2(32, 32))
{
    // launchxl2-tms570ls1224 tests
    m_boardTestMap[USER_LED]    = new BoardTestUserLed;
    m_boardTestMap[USER_SWITCH] = new BoardTestUserSwitch;
    // gizmo 1b tests
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
            if (m_libSci.write(response) == BoardTest::OKAY) {
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
        status = BoardTest::ERROR_CMD;
        break;
    case BoardTest::REG_READ:
        status = regRead(address, value);
        break;
    case BoardTest::REG_WRITE:
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

bool BoardTestApp::isAddressValid(uint32 address)
{
    return address < REG_MEMORY_MAP_MAX && m_boardTestMap[address];
}

int BoardTestApp::regRead(uint32 address, uint32& value)
{
    if (!isAddressValid(address)) {
        return BoardTest::ERROR_ADDR;
    }
    return m_boardTestMap[address]->get(address, value);
}

int BoardTestApp::regWrite(uint32 address, uint32 value)
{
    if (!isAddressValid(address)) {
        return BoardTest::ERROR_ADDR;
    }
    return m_boardTestMap[address]->set(address, value);
}
