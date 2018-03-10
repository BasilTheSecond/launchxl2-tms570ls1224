#include "btaApp.h"
#include "libSci2.h"

BtaApp::BtaApp(const char* name) :
    LibTask(name),
    m_libGpio(*new LibGpio()),
    m_libSci(*new LibSci2(32, 32))
{
}

BtaApp::~BtaApp()
{
}

void BtaApp::run()
{
    bool resetSci = true;
    std::vector<uint8> data;
    data.reserve(32);
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
        data.clear();
        m_libSci.read(data);
        while (data.size() < sizeof(m_masterToSlave)) {
            if (!m_libSci.waitForReadyRead(10)) {
                resetSci = true;
                break;
            }
            m_libSci.read(data);
        }
        if (!resetSci) {
            uint8* buffer = m_masterToSlave.buffer;
            for (int i = 0; i < sizeof(m_masterToSlave); i++) {
                buffer[i] = data[i];
            }
            uint8 command = m_masterToSlave.msg.command;
            uint32 address = m_masterToSlave.msg.address[0] << 24
                           | m_masterToSlave.msg.address[1] << 16
                           | m_masterToSlave.msg.address[2] << 8
                           | m_masterToSlave.msg.address[3];
            uint32 value = m_masterToSlave.msg.data[0] << 24
                         | m_masterToSlave.msg.data[1] << 16
                         | m_masterToSlave.msg.data[2] << 8
                         | m_masterToSlave.msg.data[3];
            m_slaveToMaster.msg.status = OKAY;
            m_slaveToMaster.msg.data[0] = 0;
            m_slaveToMaster.msg.data[1] = 0;
            m_slaveToMaster.msg.data[2] = 0;
            m_slaveToMaster.msg.data[3] = 0;
            switch (command) {
            case REG_READ:
                switch (address) {
                case USER_LED:
                    {
                        bool value;
                        m_libGpio.getPin(LibGpio::USER_LED_AIN, value);
                        m_slaveToMaster.msg.data[3]
                                              |= (value ? LED_A_ON : LED_A_OFF);
                        m_libGpio.getPin(LibGpio::USER_LED_BIN, value);
                        m_slaveToMaster.msg.data[3]
                                              |= (value ? LED_B_ON : LED_B_OFF);
                    }
                    break;
                case USER_SWITCH:
                    {
                        bool value;
                        m_libGpio.getPin(LibGpio::USER_SWITCH_A, value);
                        m_slaveToMaster.msg.data[3] |= (value ? SWITCH_A : 0);
                        m_libGpio.getPin(LibGpio::USER_SWITCH_B, value);
                        m_slaveToMaster.msg.data[3] |= (value ? SWITCH_B : 0);
                    }
                    break;
                default:
                    m_slaveToMaster.msg.status = ERROR_ADDR;
                    break;
                }
                break;
            case REG_WRITE:
                m_slaveToMaster.msg.data[0] = m_masterToSlave.msg.data[0];
                m_slaveToMaster.msg.data[1] = m_masterToSlave.msg.data[1];
                m_slaveToMaster.msg.data[2] = m_masterToSlave.msg.data[2];
                m_slaveToMaster.msg.data[3] = m_masterToSlave.msg.data[3];
                switch (address) {
                case USER_LED:
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
                    break;
                case USER_SWITCH:
                    m_slaveToMaster.msg.status = ERROR_RO;
                    break;
                default:
                    m_slaveToMaster.msg.status = ERROR_ADDR;
                    break;
                }
                break;
            default:
                m_slaveToMaster.msg.status = ERROR_CMD;
                break;
            }
            buffer = m_slaveToMaster.buffer;
            data.clear();
            for (int i = 0; i < sizeof(m_slaveToMaster); i++) {
                data.push_back(buffer[i]);
            }
            if (m_libSci.write(data) == OKAY) {
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

