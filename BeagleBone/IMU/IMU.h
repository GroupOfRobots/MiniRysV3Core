#ifndef _IMU_H_
#define _IMU_H_

#include "MPU6050.h"

class IMU {
	private:
		// The MPU object (from I2Cdev)
		MPU6050 *mpu;
		// Whether DMP init was successful
		bool dmpReady;
		// Status of last operation (0 - success, not 0 - error)
		uint8_t devStatus;
		// expected DMP packet size (default is 42 bytes)
		uint16_t packetSize;
		// Number of all bytes currently in FIFO
		uint16_t fifoCount;
		// FIFO storage buffer
		uint8_t fifoBuffer[64];
		// [w, x, y, z] - quaternion container
		Quaternion *quaternion;
		// [x, y, z] - gravity vector
		VectorFloat *gravity;
		// [yaw, pitch, roll] - yaw/pitch/roll container
		float yawPitchRoll[3];
	public:
		IMU();
		~IMU();
		void initialize();
		void readData();
		float getPitch();
		float getRoll();
		float getYaw();
		void resetFIFO();
};

#endif