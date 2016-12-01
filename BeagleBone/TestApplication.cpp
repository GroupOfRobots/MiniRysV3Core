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
	if(angle/4!=0.0)nowyspeed = -1*(angle-0.74)*700;

	silniki.setSpeed(nowyspeed,nowyspeed,1); //Set speed for motors;
	angle = 0.0;
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
	//std::thread t(&balancing);

	int state=2; // 0 laying front, 1 laying back, 2 balancing
	imu.setup(); // initialize IMU
	usleep(100000);
	//if(!lipol.isGood())printf("niski poziom napiecia baterii");



	if(imu.getRoll()>0.8)state = 1;
	else if (imu.getRoll()< -0.8)state = 0;
	else state = 2;
	imu.resetFIFO();

	char c;
	float speedl=0.0;
	float speedr=0.0;

	while(1){
		//imu.resetFIFO();

		printf("ADC5: %d\n",lipol.getRaw());

		switch (c){
		case 'w':
			if(speedl>0||speedr>0)speedl=speedr=20.0;
			speedl=speedr-=20.0;
		break;
		case 'a':
			speedr+=5.0;
			speedl-=5.0;
		break;
		case 's':
			if(speedl<0||speedr<0)speedl=speedr=-20.0;
			speedl=speedr+=20.0;

		break;
		case 'd':
			speedr-=5.0;
			speedl+=5.0;
		break;
		case 'q':
			silniki.disable();
			usleep(10000);
		break;
		default:

			usleep(1000);
			//t.detach();
			//balancing();
			//silniki.setSpeed(100,100,3);
			//usleep(100000);

			//printf("Message received from PRU:%d\n", hcr.getDistance(distance_sensors::front));
			//printf("ADC5: %d\n",lipol.getRaw());
		break;
		}
		if(speedr>1000)speedr=1000;
		if(speedl>1000)speedl=1000;
		if(speedr<-1000)speedr=-1000;
		if(speedl<-1000)speedl=-1000;

		silniki.setSpeed(speedl,speedr,3);
		c = getch();
	}



	return 0;
}

