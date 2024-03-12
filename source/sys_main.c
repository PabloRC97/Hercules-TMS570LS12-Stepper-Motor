/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
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
#include "rti.h"
#include "gio.h"
#include "sys_core.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define _100US_A_MS 10
unsigned int timer_100us = 0;
unsigned int var_rpm = 10;
int i = 0, j = 0;
int check = 0;
float step_degree = 5.625 / 64;
int posicion_d = 90;
int posicion_act = 0;
int n_step = 0;
int error = 0;
int home = 0;
int pasos_medio[8][4] = {
        { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 },
        { 0, 1, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 1, 1 },
        { 0, 0, 0, 1 }, { 1, 0, 0, 1 }
};
int trans[4] = { 0, 0, 0, 0 };

void rtiNotification(uint32 notification);
void delay_timer_100us(unsigned int *timer_local_100us, const unsigned int time_us) {
    *timer_local_100us = 0;

    rtiStartCounter(rtiCOUNTER_BLOCK0);
    while (*timer_local_100us < time_us);
    rtiStopCounter(rtiCOUNTER_BLOCK0);
}
unsigned int rpm2T(const unsigned int rpm) {
    return 146.484375/(float)rpm;
}


/* USER CODE END */

int main(void
{
/* USER CODE BEGIN (3) */
    rtiInit();
    gioInit();

    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    rtiStartCounter(rtiCOUNTER_BLOCK0);
//  rtiStopCounter(rtiCOUNTER_BLOCK0);
    _enable_interrupt_();


    while (1) {
        n_step = posicion_d / step_degree;

        error = n_step - check;

        if (error > 0) {
            trans[0] = 0;
            trans[1] = 1;
            trans[2] = 2;
            trans[3] = 3;
        } else {
            trans[0] = 3;
            trans[1] = 2;
            trans[2] = 1;
            trans[3] = 0;
        }

        for (i = 0; i < 8; i++) {
            if (error != 0) {

                gioSetBit(gioPORTA, 0, pasos_medio[i][trans[0]]);
                gioSetBit(gioPORTA, 1, pasos_medio[i][trans[1]]);
                gioSetBit(gioPORTA, 2, pasos_medio[i][trans[2]]);
                gioSetBit(gioPORTA, 5, pasos_medio[i][trans[3]]);
                delay_timer_100us(&timer_100us, rpm2T(var_rpm));
//              __delay_cycles(90000);

                if (check < n_step) {
                    check++;
                } else {
                    check--;
                }
                error = n_step - check;

            }
    }
        home = check;
    }
    /* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification) {
    ++timer_100us;
}
/* USER CODE END */
