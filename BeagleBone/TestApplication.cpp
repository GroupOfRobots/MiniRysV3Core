/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <chrono>

#include "Motors/motors.h"
#include "Battery/battery.h"
#include "IMU/imu.h"


Motors silniki;
Battery lipol;
Imu imu;

int main(void)
{
		int nowyspeed=0;
		float angle;
		imu.setup(); // initialize IMU
	    usleep(100000);
	    silniki.enable(); //enable motor
	    if(!lipol.isGood())printf("niski poziom napiecia baterii");

	    silniki.setSpeed(5500,5500,1);
	    usleep(100000);
	    usleep(100000);
	    usleep(100000);
	    usleep(100000);

	    silniki.setSpeed(-5500,-5500,1);
	    usleep(100000);
	    usleep(100000);
	    usleep(100000);
	    usleep(100000);
	    usleep(35000);


	    for(int i=0;i<800;i++){
	    	angle += imu.getRoll();
	    	usleep(1250);
	    	angle += imu.getRoll();
	    	usleep(1250);
	    	angle += imu.getRoll();
	    	usleep(1250);
	    	angle += imu.getRoll();
	    	usleep(1250);
	    	if(angle/4!=0.0)nowyspeed = (angle-0.04)*70;

	    	silniki.setSpeed(nowyspeed,nowyspeed,2); //Set speed for motors;
	    	angle =0.0;
	    }
	    silniki.disable(); //disable motor

	return 0;
}

