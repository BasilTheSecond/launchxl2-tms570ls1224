#include "btaApp.h"
#include "libRaisePrivilege.h"

const TickType_t BtaApp::k_taskDelay = pdMS_TO_TICKS(1000);
volatile uint32 BtaApp::s_flags;
SemaphoreHandle_t BtaApp::s_sem;
bool BtaApp::s_isInitialized;

/* Communication protocol between master (M) and slave (S):
 * Register read:
 * M->S: RD A0 A1 A2 A3 X0 X1 X2 X3
 * S->M: S D0 D1 D2 D3
 * RD: Register read command (1 byte)
 * A0..3: Register address (4 bytes)
 * X0..3: "Don't care" data (4 bytes)
 * S: Command status (1 byte)
 * D0..3: Register value (4 bytes)
 * A0: Register address MSB
 * A3: Register address LSB
 * D0: Register value MSB
 * D3: Register value LSB
 * Register write:
 * M->S: WR A0 A1 A2 A3 D0 D1 D2 D3
 * S->M: S D0 D1 D2 D3
 * WR: Register write command (1 byte)
 * A0..3: Register address (4 bytes)
 * D0..3: Register value (4 bytes)
 * S: Command status (1 byte)
 * A0: Register address MSB
 * A3: Register address LSB
 * D0: Register value MSB
 * D3: Register value LSB
 */
union commMasterToSlave_t {
    struct msgMasterToSlave_t {
        uint8 command;
        uint8 address[4];
        uint8 data[4];
    } msg;
    uint8 buffer[sizeof(struct msgMasterToSlave_t)];
};

union commSlaveToMaster_t {
    struct msgSlaveToMaster_t {
        uint8 status;
        uint8 data[4];
    } msg;
    uint8 buffer[sizeof(struct msgSlaveToMaster_t)];
};

/* Commands */
enum commCommand {
    COMM_COMMAND_REG_READ  = 0x01,  /* register read */
    COMM_COMMAND_REG_WRITE = 0x02,  /* register write */
};

/* Command status */

enum commStatus {
    COMM_COMMAND_STATUS_OKAY,       /* Okay */
    COMM_COMMAND_STATUS_ERROR_CMD,  /* Error unknown command */
    COMM_COMMAND_STATUS_ERROR_ADDR, /* Error unknown register address */
    COMM_COMMAND_STATUS_ERROR_RO,   /* Error RO register */
};

/* Register memory map */
enum regMemoryMap {
    REG_USER_LED    = 0x00000000,  /* USER_LED register, R/W */
    REG_USER_SWITCH = 0x00000004,  /* USER_SWITCH register, R/O */
};

/* Register bits */
enum regUserLedBits {
    REG_USER_LED_A_ON  = (uint32)((uint32)1U << 0U),
    REG_USER_LED_A_OFF = (uint32)((uint32)1U << 1U),
    REG_USER_LED_B_ON  = (uint32)((uint32)1U << 2U),
    REG_USER_LED_B_OFF = (uint32)((uint32)1U << 3U),
};

enum regUserSwitchBits {
    REG_USER_SWITCH_A = (uint32)((uint32)1U << 0U),
    REG_USER_SWITCH_B = (uint32)((uint32)1U << 1U),
};

static union commMasterToSlave_t g_commMasterToSlave;
static union commSlaveToMaster_t g_commSlaveToMaster;

BtaApp::BtaApp(const char* name) :
    LibTask(name)
{
    if (!s_isInitialized) {
        s_sem = xSemaphoreCreateBinary();
        s_isInitialized = true;
    }
}

BtaApp::~BtaApp()
{
}

void BtaApp::clearFlags(uint32 flags)
{
    s_flags &= ~flags;
}

void BtaApp::setFlagsFromIsr(uint32 flags)
{
    s_flags |= flags;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(s_sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void BtaApp::run()
{
    s_flags |= SCI_TX_INT;
    while (1) {
        if (s_flags & SCI_RX_INT) {
            clearFlags(SCI_RX_INT);
            uint8 command = g_commMasterToSlave.msg.command;
            uint32 address = g_commMasterToSlave.msg.address[0] << 24
                           | g_commMasterToSlave.msg.address[1] << 16
                           | g_commMasterToSlave.msg.address[2] << 8
                           | g_commMasterToSlave.msg.address[3];
            uint32 value = g_commMasterToSlave.msg.data[0] << 24
                         | g_commMasterToSlave.msg.data[1] << 16
                         | g_commMasterToSlave.msg.data[2] << 8
                         | g_commMasterToSlave.msg.data[3];

            g_commSlaveToMaster.msg.status = COMM_COMMAND_STATUS_OKAY;
            g_commSlaveToMaster.msg.data[0] = 0;
            g_commSlaveToMaster.msg.data[1] = 0;
            g_commSlaveToMaster.msg.data[2] = 0;
            g_commSlaveToMaster.msg.data[3] = 0;
            switch (command) {
            case COMM_COMMAND_REG_READ:
                switch (address) {
                case REG_USER_LED:
                    {
                        bool value;
                        m_libGpio.getPin(LibGpio::USER_LED_AIN, value);
                        g_commSlaveToMaster.msg.data[3]
                            |= (value ? REG_USER_LED_A_ON : REG_USER_LED_A_OFF);
                        m_libGpio.getPin(LibGpio::USER_LED_BIN, value);
                        g_commSlaveToMaster.msg.data[3]
                            |= (value ? REG_USER_LED_B_ON : REG_USER_LED_B_OFF);
                    }
                    break;
                case REG_USER_SWITCH:
                    {
                        bool value;
                        m_libGpio.getPin(LibGpio::USER_SWITCH_A, value);
                        g_commSlaveToMaster.msg.data[3]
                            |= (value ? REG_USER_SWITCH_A : 0);
                        m_libGpio.getPin(LibGpio::USER_SWITCH_B, value);
                        g_commSlaveToMaster.msg.data[3]
                            |= (value ? REG_USER_SWITCH_B : 0);
                    }
                    break;
                default:
                    g_commSlaveToMaster.msg.status = COMM_COMMAND_STATUS_ERROR_ADDR;
                    break;
                }
                break;
            case COMM_COMMAND_REG_WRITE:
                g_commSlaveToMaster.msg.data[0] = g_commMasterToSlave.msg.data[0];
                g_commSlaveToMaster.msg.data[1] = g_commMasterToSlave.msg.data[1];
                g_commSlaveToMaster.msg.data[2] = g_commMasterToSlave.msg.data[2];
                g_commSlaveToMaster.msg.data[3] = g_commMasterToSlave.msg.data[3];
                switch (address) {
                case REG_USER_LED:
                    if (value & REG_USER_LED_A_ON) {
                        m_libGpio.setPin(LibGpio::USER_LED_AOUT, true);
                    }
                    if (value & REG_USER_LED_A_OFF) {
                        m_libGpio.setPin(LibGpio::USER_LED_AOUT, false);
                    }
                    if (value & REG_USER_LED_B_ON) {
                        m_libGpio.setPin(LibGpio::USER_LED_BOUT, true);
                    }
                    if (value & REG_USER_LED_B_OFF) {
                        m_libGpio.setPin(LibGpio::USER_LED_BOUT, false);
                    }
                    break;
                case REG_USER_SWITCH:
                    g_commSlaveToMaster.msg.status = COMM_COMMAND_STATUS_ERROR_RO;
                    break;
                default:
                    g_commSlaveToMaster.msg.status = COMM_COMMAND_STATUS_ERROR_ADDR;
                    break;
                }
                break;
            default:
                g_commSlaveToMaster.msg.status = COMM_COMMAND_STATUS_ERROR_CMD;
                break;
            }
            LibRaisePrivilege libRaisePrivilege;
            sciSend(scilinREG, sizeof(g_commSlaveToMaster),
                                                    g_commSlaveToMaster.buffer);
        }
        if (s_flags & SCI_TX_INT) {
            clearFlags(SCI_TX_INT);
            LibRaisePrivilege libRaisePrivilege;
            sciReceive(scilinREG, sizeof(g_commMasterToSlave),
                                                    g_commMasterToSlave.buffer);
        }
        if (xSemaphoreTake(s_sem, k_taskDelay) == pdFALSE) {
            continue;
        }
    }
}

void sciNotification(sciBASE_t *sci, uint32 flags)
{
    BtaApp::setFlagsFromIsr(flags);
}
