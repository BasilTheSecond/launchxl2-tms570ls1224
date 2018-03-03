/** @file sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "FreeRTOS.h"
#include "os_task.h"
#include "gio.h"
#include "mibspi.h"
#include "sci.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define USER_LED_A_PORT     gioPORTB
#define USER_LED_A_PIN      1
#define USER_LED_B_PORT     gioPORTB
#define USER_LED_B_PIN      2
#define USER_SWITCH_A_VALUE (mibspiREG3->PC2 & ((uint32)((uint32)1U << 0U)))
#define USER_SWITCH_B_VALUE (mibspiREG1->PC2 & ((uint32)((uint32)1U << 4U)))

enum userLed_t {
    USER_LED_A,
    USER_LED_B,
};

enum userSwitch_t {
    USER_SWITCH_A,
    USER_SWITCH_B,
};

static void userSetLed(enum userLed_t userLed, uint32 value)
{
    switch (userLed) {
    default:
    case USER_LED_A:
        gioSetBit(USER_LED_A_PORT, USER_LED_A_PIN, value);
        break;
    case USER_LED_B:
        gioSetBit(USER_LED_B_PORT, USER_LED_B_PIN, value);
        break;
    }
}

static uint32 userGetLed(enum userLed_t userLed)
{
    uint32 result;

    switch (userLed) {
    default:
    case USER_LED_A:
        result = gioGetBit(USER_LED_A_PORT, USER_LED_A_PIN);
        break;
    case USER_LED_B:
        result = gioGetBit(USER_LED_B_PORT, USER_LED_B_PIN);
        break;
    }
    return result;
}

static uint32 userGetSwitch(enum userSwitch_t userSwitch)
{
    uint32 result;

    switch (userSwitch) {
    default:
    case USER_SWITCH_A:
        result = USER_SWITCH_A_VALUE;
        break;
    case USER_SWITCH_B:
        result = USER_SWITCH_B_VALUE;
        break;
    }
    return result;
}

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

/* UART communication task */
static void mainUartCommTask(void* parameter);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    /* Initialize peripherals */
    gioInit();
    mibspiInit();
    sciInit();
    /* Start the tasks. */
    xTaskCreate(mainUartCommTask, "UARTComm", configMINIMAL_STACK_SIZE,
                                              NULL, tskIDLE_PRIORITY + 1, NULL);
    /* Start the scheduler. */
    vTaskStartScheduler();
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for(;;);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
static void mainUartCommTask(void* parameter)
{
    /* Start listening */
    sciReceive(scilinREG, sizeof(g_commMasterToSlave),
                                                    g_commMasterToSlave.buffer);
    while (1) {
    }
}

void sciNotification(sciBASE_t *sci, uint32 flags)
{
    if (flags == SCI_RX_INT) {
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
                g_commSlaveToMaster.msg.data[3] |= (userGetLed(USER_LED_A) ? REG_USER_LED_A_ON : REG_USER_LED_A_OFF)
                                                |  (userGetLed(USER_LED_B) ? REG_USER_LED_B_ON : REG_USER_LED_B_OFF);
                break;
            case REG_USER_SWITCH:
                g_commSlaveToMaster.msg.data[3] |= (userGetSwitch(USER_SWITCH_A) ? REG_USER_SWITCH_A : 0)
                                                |  (userGetSwitch(USER_SWITCH_B) ? REG_USER_SWITCH_B : 0);
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
                    userSetLed(USER_LED_A, 1);
                }
                if (value & REG_USER_LED_A_OFF) {
                    userSetLed(USER_LED_A, 0);
                }
                if (value & REG_USER_LED_B_ON) {
                    userSetLed(USER_LED_B, 1);
                }
                if (value & REG_USER_LED_B_OFF) {
                    userSetLed(USER_LED_B, 0);
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
        sciSend(scilinREG, sizeof(g_commSlaveToMaster), g_commSlaveToMaster.buffer);
    }
    if (flags == SCI_TX_INT) {
        sciReceive(scilinREG, sizeof(g_commMasterToSlave), g_commMasterToSlave.buffer);
    }
}
/* USER CODE END */
