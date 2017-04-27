#include "IMU.h"
#include <string>
#include <iostream>

IMU::IMU() {
	this->mpu = new MPU6050();
	this->dmpReady = false;
	this->devStatus = 0;
	this->packetSize = 0;
	this->fifoCount = 0;
	for (int i = 0; i < 64; ++i) {
		this->fifoBuffer[i] = 0;
	}
	this->quaternion = new Quaternion();
	this->gravity = new VectorFloat();
	this->yawPitchRoll[0] = this->yawPitchRoll[1] = this->yawPitchRoll[2] = 0;
}

IMU::~IMU() {
	delete this->mpu;
	delete this->quaternion;
	delete this->gravity;
}

void IMU::initialize() {
	// initialize device
	this->mpu->initialize();

	// verify connection
	if (!this->mpu->testConnection()) {
		throw(std::string("MPU6050 connection failed"));
	}

	// load and configure the DMP
	devStatus = this->mpu->dmpInitialize();

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
		//printf("Enabling DMP...\n");
		this->mpu->setDMPEnabled(true);

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = this->mpu->dmpGetFIFOPacketSize();
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		throw(std::string("DMP Initialization failed (code %d)", devStatus));
	}
}

void IMU::readData() {
	// if programming failed, don't try to do anything
	if (!dmpReady) {
		return;
	}

	// Get current FIFO length
	fifoCount = this->mpu->getFIFOCount();

	// Check for FIFO overflow
	if (fifoCount == 1024) {
		// reset so we can continue cleanly
		this->mpu->resetFIFO();
		// printf("FIFO overflow!\n");
		return;
	}

	// Check for DMP data ready interrupt (this should happen frequently)
	if (fifoCount >= 42) {
		// Read a packet from FIFO to buffer
		this->mpu->getFIFOBytes(fifoBuffer, packetSize);
		// Parse data from FIFO to gravity and pitch/yaw/roll readings
		this->mpu->dmpGetQuaternion(quaternion, fifoBuffer);
		this->mpu->dmpGetGravity(gravity, quaternion);
		this->mpu->dmpGetYawPitchRoll(yawPitchRoll, quaternion, gravity);
	}
}

float IMU::getPitch() {
	readData();
	return yawPitchRoll[1];
}

float IMU::getRoll() {
	readData();
	return yawPitchRoll[2];
}

float IMU::getYaw() {
	readData();
	return yawPitchRoll[0];
}

void IMU::resetFIFO() {
	this->mpu->resetFIFO();
}

void IMU::calibrate() {
	std::cout << "Fix the position and hit enter\n";
	std::cin.get();
	this->offsetXAcceleration = this->mpu->setXAccelOffset();
	this->offsetYAcceleration = this->mpu->setYAccelOffset();
	this->offsetZAcceleration = this->mpu->setZAccelOffset();
	this->offsetXRotation = this->mpu->setXGyroOffset();
	this->offsetYRotation = this->mpu->setYGyroOffset();
	this->offsetZRotation = this->mpu->setZGyroOffset();

	std::cout << "calibration initial values:\n";
	std::cout << "\tXAcceleration: " << this->offsetXAcceleration << std::endl;
	std::cout << "\tYAcceleration: " << this->offsetYAcceleration << std::endl;
	std::cout << "\tZAcceleration: " << this->offsetZAcceleration << std::endl;
	std::cout << "\tXRotation: " << this->offsetXRotation << std::endl;
	std::cout << "\tYRotation: " << this->offsetYRotation << std::endl;
	std::cout << "\tZRotation: " << this->offsetZRotation << std::endl;

	for (int i = 0; i < IMU_CALIBRATION_READINGS; ++i) {
		int16_t ax, ay, az, gx, gy, gz;
		this->mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		this->offsetXAcceleration += ax;
		this->offsetYAcceleration += ay;
		this->offsetZAcceleration += az;
		this->offsetXRotation += gx;
		this->offsetYRotation += gy;
		this->offsetZRotation += gz;
		usleep(1000);
	}

	this->offsetXAcceleration /= IMU_CALIBRATION_READINGS;
	this->offsetYAcceleration /= IMU_CALIBRATION_READINGS;
	this->offsetZAcceleration /= IMU_CALIBRATION_READINGS;
	this->offsetXRotation /= IMU_CALIBRATION_READINGS;
	this->offsetYRotation /= IMU_CALIBRATION_READINGS;
	this->offsetZRotation /= IMU_CALIBRATION_READINGS;

	// this->mpu->setXAccelOffset(-this->offsetXAcceleration);
	// this->mpu->setYAccelOffset(-this->offsetYAcceleration);
	// this->mpu->setZAccelOffset(-this->offsetZAcceleration);
	this->mpu->setXGyroOffset(-this->offsetXRotation);
	this->mpu->setYGyroOffset(-this->offsetYRotation);
	this->mpu->setZGyroOffset(-this->offsetZRotation);

	std::cout << "calibration done, offsets:\n";
	std::cout << "\tXAcceleration: " << this->offsetXAcceleration << std::endl;
	std::cout << "\tYAcceleration: " << this->offsetYAcceleration << std::endl;
	std::cout << "\tZAcceleration: " << this->offsetZAcceleration << std::endl;
	std::cout << "\tXRotation: " << this->offsetXRotation << std::endl;
	std::cout << "\tYRotation: " << this->offsetYRotation << std::endl;
	std::cout << "\tZRotation: " << this->offsetZRotation << std::endl;
	std::cout << "press enter to continue:\n";
	std::cin.get();
}
