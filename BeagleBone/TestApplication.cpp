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
#include <thread>

#include "Motors/motors.h" //motors class
#include "Battery/battery.h" //Battery measurement class
#include "IMU/imu.h" // angle measurement class
#include "Controller/controller.h" //angle & speed control
#include "Sonar/sonar.h" //distance sensors class


Motors silniki;
Battery lipol;
Imu imu;
Sonar hcr;
// Controller pid; add timer to use it
float angle;
int nowyspeed=0;

void balancing(){

	angle += imu.getRoll();
	usleep(1250);
	angle += imu.getRoll();
	usleep(1250);
	angle += imu.getRoll();
	usleep(1250);
	angle += imu.getRoll();
	usleep(1250);
	if(angle/4!=0.0)nowyspeed = -1*(angle-0.04)*700;

	silniki.setSpeed(nowyspeed,nowyspeed,4); //Set speed for motors;
	angle = 0.0;
}

int main(void)
{
	//std::thread t(&balancing);

	int state=2; // 0 laying front, 1 laying back, 2 balancing
	//imu.setup(); // initialize IMU
	usleep(100000);
	if(!lipol.isGood())printf("niski poziom napiecia baterii");



	//if(imu.getRoll()>0.8)state = 1;
	//else if (imu.getRoll()< -0.8)state = 0;
	//else state = 2;
	//imu.resetFIFO();


	while(1){
		//imu.resetFIFO();

		switch (state){
		case 0:
			silniki.setSpeed(950,950,1);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(100000);

			silniki.setSpeed(-950,-950,1);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(35000);
			state = 2;
		break;
		case 1:
			silniki.setSpeed(-950,-950,1);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(100000);

			silniki.setSpeed(950,950,1);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(100000);
			usleep(35000);
			state = 2;

		break;
		case 2:
			//t.detach();
			//balancing();
			silniki.disable();
			//printf("Message received from PRU:%d\n", hcr.getDistance(distance_sensors::front));
			printf("ADC5: %d\n",lipol.getRaw());
			usleep(100000);
		break;
		}
	}



	return 0;
}

