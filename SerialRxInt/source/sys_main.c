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
#include <sci.h>
#include <string.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
static volatile uint8 g_pressedKey;
static const char g_prompt[] = "\n\r> ";
static volatile int g_pressedKeyReady;
static volatile int g_promptReady;
static char g_echo[32];
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    _enable_IRQ();
    sciInit();
    g_pressedKeyReady = 0;
    g_promptReady = 0;
    sciEnableNotification(scilinREG, SCI_TX_INT | SCI_RX_INT);
    sciSend(scilinREG, strlen(g_prompt), (uint8*) g_prompt);
    while (1) {
        if (g_promptReady) {
            g_promptReady = 0;
            sciReceive(scilinREG, 1, &g_pressedKey);
        }
        else if (g_pressedKeyReady) {
            char result[2] = { [0] = g_pressedKey, [1] = 0 };

            g_pressedKeyReady = 0;
            strcpy(g_echo, result);
            strcat(g_echo, g_prompt);
            sciSend(scilinREG, strlen(g_echo), (uint8*) g_echo);
        }
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void sciNotification(sciBASE_t *sci, uint32 flags)
{
    if (flags == SCI_RX_INT) {
        g_pressedKeyReady = 1;
    }
    if (flags == SCI_TX_INT) {
        g_promptReady = 1;
    }
}
/* USER CODE END */
