#include "IMU.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

IMU::IMU() {
	this->mpu = new MPU6050();
	this->dmpReady = false;
	this->devStatus = 0;
	this->packetSize = 0;
	// this->fifoCount = 0;
	for (int i = 0; i < 64; ++i) {
		this->fifoBuffer[i] = 0;
	}
	this->quaternion = new Quaternion();
	this->gravity = new VectorFloat();
	this->yawPitchRoll[0] = this->yawPitchRoll[1] = this->yawPitchRoll[2] = 0;
	this->preHeatingExitFlag = true;

	this->yawOffset = 0;
	this->pitchOffset = 0;
	this->rollOffset = 0;
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
	// 1kHz / (1 + 3) = 250Hz
	// this->mpu->setRate(3);

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
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
	uint16_t fifoCount = this->mpu->getFIFOCount();

	// Check for FIFO overflow
	if (fifoCount == 1024) {
		// reset so we can continue cleanly
		this->mpu->resetFIFO();
		fifoCount = 0;
	}

	// Check for DMP data ready interrupt (this should happen frequently)
	while (fifoCount < this->packetSize) {
		fifoCount = this->mpu->getFIFOCount();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// Read a packet from FIFO to buffer
	this->mpu->getFIFOBytes(this->fifoBuffer, this->packetSize);
	// Parse data from FIFO to gravity and pitch/yaw/roll readings
	this->mpu->dmpGetQuaternion(this->quaternion, this->fifoBuffer);
	this->mpu->dmpGetGravity(this->gravity, this->quaternion);
	this->mpu->dmpGetYawPitchRoll(this->yawPitchRoll, this->quaternion, this->gravity);
}

void IMU::getYawPitchRoll(float * yaw, float * pitch, float * roll) {
	this->readData();
	*yaw = this->yawPitchRoll[0] - this->yawOffset;
	*pitch = this->yawPitchRoll[1] - this->pitchOffset;
	*roll = this->yawPitchRoll[2] - this->rollOffset;
}

float IMU::getYaw() {
	this->readData();
	return yawPitchRoll[0] - this->yawOffset;
}

float IMU::getPitch() {
	this->readData();
	return yawPitchRoll[1] - this->pitchOffset;
}

float IMU::getRoll() {
	this->readData();
	return yawPitchRoll[2] - this->rollOffset;
}

void IMU::resetFIFO() {
	this->mpu->resetFIFO();
}

void preHeatingThreadFn(IMU * imu) {
	while (!imu->getPreHeatingExitFlag()) {
		imu->readData();
	}
}

bool IMU::getPreHeatingExitFlag() {
	return this->preHeatingExitFlag;
}

void IMU::calibrate() {
	std::cout << "Pre-heating the IMU (30s)...\n";

	// Create the IMU pre-heating thread
	this->preHeatingExitFlag = false;
	std::thread preHeatingThread(preHeatingThreadFn, this);

	// Wait 30s
	auto now = std::chrono::steady_clock::now();
	std::this_thread::sleep_until(now + std::chrono::seconds(30));

	// Notify user
	std::cout << "Fix the position and press enter\n";
	std::cin.get();

	// Create accumulators for values
	double yawSum = 0;
	double pitchSum = 0;
	double rollSum = 0;
	// Also read counter
	unsigned int iterations = 0;

	// Create timer for 5s
	now = std::chrono::steady_clock::now();
	auto end = now + std::chrono::milliseconds(5000);

	this->preHeatingExitFlag = true;

	// Until that timer...
	while (end > std::chrono::steady_clock::now()) {
		// Get a reading
		this->readData();
		yawSum += this->yawPitchRoll[0];
		pitchSum += this->yawPitchRoll[1];
		rollSum += this->yawPitchRoll[2];

		// Increase iteration counter
		++iterations;
	}

	// Calculate average reading and save it
	this->yawOffset = yawSum / iterations;
	this->pitchOffset = pitchSum / iterations;
	this->rollOffset = rollSum / iterations;

	// Notify user
	std::cout << "calibration done, offsets:\n";
	std::cout << "\tYaw: " << this->yawOffset << std::endl;
	std::cout << "\tPitch: " << this->pitchOffset << std::endl;
	std::cout << "\tRoll: " << this->rollOffset << std::endl;
	std::cout << "press enter to continue:\n";
	std::cin.get();

	// End pre-heating thread
	this->preHeatingExitFlag = true;
	preHeatingThread.join();
}
