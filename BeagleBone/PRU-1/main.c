/***************************************************************************************
 * MAIN.C
 *
 * Description: main source file for PRU development
 *
 * Rafael de Souza
 * (C) 2015 Texas Instruments, Inc
 * 
 * Built with Code Composer Studio v6
 **************************************************************************************/

//create shared memory pointer and read speed and dir of mototr,
//distance sensor on PRU0

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_ctrl.h>
#include <pru_iep.h>

struct motor_data{

    int leftSpeed;
    int rightSpeed;
    int leftDir;
    int rightDir;
    int leftSteps;
    int rightSteps;
    int resetSteps;
};

volatile register unsigned int __R31, __R30;
volatile unsigned int stepsL;
volatile unsigned int stepsR;

int main(void) {
    unsigned int loops, delay;

    /* allow OCP master port access by the PRU so the PRU can read external memories */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    void *p = (void*)0x00002000;

    struct motor_data* share_buff = (struct motor_data*)p;

    //struct motor_data share_buff;

    while(1){
		for (loops = 0; loops < 10000; loops++) {

			if(loops%share_buff->leftSpeed){
				__R30 = __R30 | (1 << 0); // Toogle step
				stepsL++;
			}
			if(loops%share_buff->rightSpeed){
				__R30 = __R30 | (1 << 0); // Toogle step
				stepsL++;
			}

			for (delay = 0; delay < 200; delay++);
			__R30 = __R30 & ~(1 << 0); // Toggle off
			__R30 = __R30 & ~(1 << 1); // Toggle off

			//Setting direction
			if(share_buff->leftDir) __R30 = __R30 | (1 << 0);
			else __R30 = __R30 & ~(1 << 0);
			if(share_buff->rightDir) __R30 = __R30 | (1 << 0);
			else __R30 = __R30 & ~(1 << 0);

			if(share_buff->resetSteps)stepsL = stepsR = 0;
			share_buff->rightSteps = stepsR;
			share_buff->leftSteps = stepsL;
		}
    }

    // Send interrupt to host and halt
    __R31 = 32 | 3;
    __halt();
    return 0;
}
