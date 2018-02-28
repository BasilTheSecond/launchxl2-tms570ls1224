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
#include "gio.h"
#include "pinmux.h"
#include "mibspi.h"
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

    USER_LED_MAX
};

enum userSwitch_t {
    USER_SWITCH_A,
    USER_SWITCH_B,

    USER_SWITCH_MAX
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
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    muxInit();
    mibspiInit();
    while (1) {
        userSetLed(USER_LED_A, !userGetSwitch(USER_SWITCH_A));
        userSetLed(USER_LED_B, !userGetSwitch(USER_SWITCH_B));
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
