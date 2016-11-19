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
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <chrono>

#include "Motors/motors.h"
#include "Battery/battery.h"
#include "MPU6050/MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
Motors silniki;
Battery lipol;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container


void setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    mpu.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    // load and configure the DMP
    printf("Initializing DMP...\n");
    devStatus = mpu.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        printf("Enabling DMP...\n");
        mpu.setDMPEnabled(true);

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready!\n");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}

void readData() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    if (fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        printf("FIFO overflow!\n");

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (fifoCount >= 42) {
        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            printf("r %7.2f \n", ypr[2]);
    }
}

int main(void)
{


		int nowyspeed=0;
		int speed=0;
		int motorspeed=0;
		float angle;
		setup(); // initialize IMU
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
	    	readData(); // read data from IMU
	    	angle += ypr[2];
	    	usleep(1250);
	    	readData(); // read data from IMU
	    	angle += ypr[2];
	    	usleep(1250);
	    	readData(); // read data from IMU
	    	angle += ypr[2];
	    	usleep(1250);
	    	readData(); // read data from IMU
	    	angle += ypr[2];
	    	usleep(1250);
	    	if(angle/4!=0.0)nowyspeed = (angle-0.04)*70;
	    	else nowyspeed = 1;
	    	if ((speed - nowyspeed) > 20)
				speed -= 20;
			else if ((speed - nowyspeed) < -20)
				speed += 20;
			else
				speed = nowyspeed;
	    	printf("%d/n",speed);
	    	if(speed!=0)motorspeed = 200000/speed;
	    	else motorspeed = 200000;
	    	if(speed == 1)motorspeed =0;
	    	if(motorspeed<0&&motorspeed>-2000)motorspeed = -2000; //move speed limitations to motor class
	    	if(motorspeed>0&&motorspeed<2000)motorspeed = 2000;
	    	silniki.setSpeed(motorspeed,motorspeed,2); //Set speed for motors;
	    	angle =0.0;
	    }
	    silniki.disable(); //disable motor

	return 0;
}

