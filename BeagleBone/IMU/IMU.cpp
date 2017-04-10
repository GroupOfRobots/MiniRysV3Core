#include "IMU.h"
#include <string>

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
	if (this->mpu->testConnection()) {
		throw(new std::string("MPU6050 connection failed"));
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
		throw(std::string("DMP Initialization failed (code %d)\n", devStatus));
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
