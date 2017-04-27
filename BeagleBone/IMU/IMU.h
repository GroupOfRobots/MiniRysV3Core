#ifndef _IMU_H_
#define _IMU_H_

#include "MPU6050.h"
#define IMU_CALIBRATION_READINGS 5

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
		// uint16_t fifoCount;
		// FIFO storage buffer
		uint8_t fifoBuffer[64];
		// [w, x, y, z] - quaternion container
		Quaternion *quaternion;
		// [x, y, z] - gravity vector
		VectorFloat *gravity;
		// [yaw, pitch, roll] - yaw/pitch/roll container
		float yawPitchRoll[3];
		// calibration data - X, Y, Z; acceleration and rotation
		int16_t offsetXAcceleration;
		int16_t offsetYAcceleration;
		int16_t offsetZAcceleration;
		int16_t offsetXRotation;
		int16_t offsetYRotation;
		int16_t offsetZRotation;
	public:
		IMU();
		~IMU();
		void initialize();
		void readData();
		float getPitch();
		float getRoll();
		float getYaw();
		void getYawPitchRoll(float *, float *, float *);
		void resetFIFO();
		// Static calibration - reads the sensor n times, averages it and sets as offset
		void calibrate();
};

#endif
