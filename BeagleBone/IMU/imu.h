#ifndef _IMU_H_
#define _IMU_H_

#include "MPU6050.h"

class Imu {
private:
	MPU6050 mpu;
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container

public:
	void setup();
	void readData();
	float getPitch();
	float getRoll();
	float getYaw();
};

#endif
