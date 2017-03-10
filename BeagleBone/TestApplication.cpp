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
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>


#include "Motors/motors.h" //motors class
#include "Battery/battery.h" //Battery measurement class
#include "IMU/imu.h" // angle measurement class
#include "Controller/controller.h" //angle & speed control
#include "Sonar/sonar.h" //distance sensors class
#include "Socket/socket.h" //websocket communication


Motors silniki;
Battery lipol;
Imu imu;
Sonar hcr;
Controller pid;
float angle;
int nowyspeed=0;
float finalleftspeed;
float finalrightspeed;
int steering;
int throttle;

void balancing(){


	while(1){
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle = angle/4*180/3.1415;
		pid.calculate_speed(angle,silniki.getLeftSpeed(),silniki.getRightSpeed(),steering,throttle,finalleftspeed,finalrightspeed);
		silniki.setSpeed(finalleftspeed,finalrightspeed,0.0,4);
		angle = 0.0;
	}
}

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

int main(void)
{

	silniki.disable();
	imu.setup(); // initialize IMU
	usleep(100000);
	silniki.enable();


	//if(!lipol.isGood())printf("niski poziom napiecia baterii");
	imu.resetFIFO();




	char c;
	int exit=1;
	pid.timerStart();


	std::thread balance(balancing);

	while(exit){

		//printf("ADC5: %d\n",lipol.getRaw());


		switch (c){
		case 'w':
			if(throttle<0)throttle = 0;
			throttle+=5;
		break;
		case 'a':
			steering +=1;
		break;
		case 's':
			if(throttle>0)throttle =0;
			throttle-=1;
		break;
		case 'd':
			steering-=1;
		break;
		case 'q':
			silniki.disable();
			exit =0;
			usleep(10000);
		break;
		case 'r':
			steering=0;
			throttle=0;
		break;
		default:

			usleep(1000);
			//silniki.setSpeed(100,100,3);
			//usleep(100000);

			//printf("Message received from PRU:%d\n", hcr.getDistance(distance_sensors::front));
			//printf("ADC5: %d\n",lipol.getRaw());
		break;
		}

		silniki.setSpeed(throttle,throttle,0.0,1);

		c = getch();
	}

	balance.detach();
	return 0;
}

