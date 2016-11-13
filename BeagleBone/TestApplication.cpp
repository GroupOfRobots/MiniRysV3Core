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
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <fstream>
#include <math.h>
#include "MPU6050/MPU6050_6Axis_MotionApps20.h"

#define MAX_BUFFER_SIZE		512
char readBuf[MAX_BUFFER_SIZE];

MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container


struct data_frame{
	unsigned int speedl;
	unsigned int speedr;
	uint8_t dirl;
	uint8_t dirr;
	uint8_t mstep;
};

#define DEVICE_NAME		"/dev/rpmsg_pru31"

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


int setSpeed(int leftspeed, int rightspeed,int microstep){
	struct pollfd pollfds[1];

		int result = 0;
		struct data_frame *sended;
		sended = (struct data_frame*)malloc(sizeof(struct data_frame));
		sended->speedl = abs(leftspeed);
		sended->speedr = abs(rightspeed);
		if(leftspeed>0)sended->dirl = 0;
		else sended->dirl = 1;
		if(rightspeed>0)sended->dirr = 0;
		else sended->dirr = 1;
		sended->mstep = microstep;

		/* Open the rpmsg_pru character device file */
		pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

		/*
		 * If the RPMsg channel doesn't exist yet the character device
		 * won't either.
		 * Make sure the PRU firmware is loaded and that the rpmsg_pru
		 * module is inserted.
		 */
		if (pollfds[0].fd < 0) {
			printf("Failed to open %s\n", DEVICE_NAME);
			return -1;
		}

		/* The RPMsg channel exists and the character device is opened */
		printf("Opened %s, sending %d messages\n\n", DEVICE_NAME, 1);

		/* Send data structure to the PRU through the RPMsg channel */
		result = write(pollfds[0].fd, sended, 13);
		if (result > 0)
			printf("Message %d: Sent to PRU\n", 1);

		//result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
		//if (result > 0)
		//	printf("Message %d received from PRU:%s\n\n", 1, readBuf);

		/* Received all the messages the example is complete */
		//printf("Received %d messages, closing %s\n", 1, DEVICE_NAME);

		/* Close the rpmsg_pru character device file */
		close(pollfds[0].fd);
		return 0;
}

void setMotors(int enable){ //function for enable/disable motors.
   std::ofstream fs;

   fs.open("/sys/class/gpio/export",std::ofstream::out);
   if(fs.fail())throw 5;
   fs << "78";
   fs.close();
   fs.open("/sys/class/gpio/gpio78/direction",std::ofstream::out);
   fs << "out";
   fs.close();
   fs.open("/sys/class/gpio/export",std::ofstream::out);
   fs << "79";
   fs.close();
   fs.open("/sys/class/gpio/gpio79/direction",std::ofstream::out);
   fs << "out";
   fs.close();
   fs.open("/sys/class/gpio/gpio78/value",std::ofstream::out);
   if(enable)fs << "0";
   else fs << "1";
   fs.close();
   fs.open("/sys/class/gpio/gpio79/value",std::ofstream::out);
   if(enable)fs << "0";
   else fs << "1";
   fs.close();
}

int main(void)
{

		setup(); // initialize IMU
	    usleep(100000);
	    setMotors(1); //enable motor
	    for (int i=0;i<100;i++){
	    	readData(); // read data from IMU
	    	if(ypr[2]>0)setSpeed(5000,5000,1); //Set speed for motors
	    	else setSpeed(-5000,-5000,1);
	    	usleep(100000);
	    }
	    setMotors(0); //disable motor

	return 0;
}

