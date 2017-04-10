#include "Motors.h"

Motors::Motors() {
	this->distance = 0;
	this->speedLeft = 0;
	this->speedRight = 0;
	this->microstep = 0;
	this->maxMotorSpeed = MAX_MOTOR_SPEED;
	this->minMotorSpeed = MIN_MOTOR_SPEED;
}

Motors::~Motors() {
	this->disable();
	this->pruFile.close();
}

void Motors::initialize() {
	std::ofstream file;
	file.open("/sys/class/gpio/export", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio/export"));
	}
	file << "78";
	file << "79";
	file.close();

	file.open("/sys/class/gpio/gpio78/direction", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio78/direction"));
	}
	file << "out";
	file.close();

	file.open("/sys/class/gpio/gpio79/direction", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio79/direction"));
	}
	file << "out";
	file.close();

	this->disable();

	this->pruFile.open(DEVICE_NAME, std::ofstream::out | std::ofstream::binary);
	if (!this->pruFile.is_open() || !this->pruFile.good()) {
		this->pruFile.close();
		throw(std::string("Failed opening file: ") + std::string(DEVICE_NAME));
	}
}

void Motors::updateOdometry(float dt) {
	this->distance += (this->speedLeft + this->speedRight) / 2 * dt * 0.7;
}

void Motors::setSpeed(float speedLeft, float speedRight, int microstep) {
	if (speedLeft == this->speedLeft && speedRight == this->speedRight && microstep == this->microstep) {
		return;
	}

	DataFrame dataFrame;

	// Calculate max speeds for given microstep value
	if (microstep == 1) {
		this->maxMotorSpeed = MAX_MOTOR_SPEED;
		this->minMotorSpeed = MIN_MOTOR_SPEED;
	} else if (microstep == 2) {
		this->maxMotorSpeed = MAX_MOTOR_SPEED/2;
		this->minMotorSpeed = MIN_MOTOR_SPEED/2;
	} else if (microstep == 3) {
		this->maxMotorSpeed = MAX_MOTOR_SPEED/4;
		this->minMotorSpeed = MIN_MOTOR_SPEED/4;
	} else if (microstep == 4) {
		this->maxMotorSpeed = MAX_MOTOR_SPEED/8;
		this->minMotorSpeed = MIN_MOTOR_SPEED/8;
	} else {
		throw(std::string("Bad microstep value!"));
	}
	this->microstep = microstep;
	dataFrame.microstep = microstep;

	// Left motor speed
	// Clip value
	if (speedLeft != 0 && speedLeft < this->maxMotorSpeed) {
		speedLeft = (unsigned int)this->maxMotorSpeed;
	}
	// Clip acceleration (change of value), set target speed
	if (speedLeft > (this->speedLeft + MAX_ACCELERATION)) {
		this->speedLeft = this->speedLeft + MAX_ACCELERATION;
	} else if (speedLeft < (this->speedLeft - MAX_ACCELERATION)) {
		this->speedLeft = this->speedLeft - MAX_ACCELERATION;
	} else {
		this->speedLeft = speedLeft;
	}
	// Calculate and write speed to data frame
	if (this->speedLeft == 0) {
		dataFrame.speedLeft = 0;
	} else {
		dataFrame.speedLeft = (int)this->minMotorSpeed * (1.0/this->speedLeft);
	}
	// Write direction to data frame, reverse speed if needed
	if (this->speedLeft >= 0) {
		dataFrame.directionLeft = 0;
	} else {
		dataFrame.directionLeft = 1;
		dataFrame.speedLeft = -dataFrame.speedLeft;
	}

	// Right motor speed
	// Clip value
	if (speedRight != 0 && speedRight < this->maxMotorSpeed) {
		speedRight = (unsigned int)this->maxMotorSpeed;
	}
	// Clip acceleration (change of value), set target speed
	if (speedRight > (this->speedRight + MAX_ACCELERATION)) {
		this->speedRight = this->speedRight + MAX_ACCELERATION;
	} else if (speedRight < (this->speedRight - MAX_ACCELERATION)) {
		this->speedRight = this->speedRight - MAX_ACCELERATION;
	} else {
		this->speedRight = speedRight;
	}
	// Calculate and write speed to data frame
	if (this->speedRight == 0) {
		dataFrame.speedRight = 0;
	} else {
		dataFrame.speedRight = (int)this->minMotorSpeed * (1.0/this->speedRight);
	}
	// Write direction to data frame, reverse speed if needed
	if (this->speedRight >= 0) {
		dataFrame.directionRight = 0;
	} else {
		dataFrame.directionRight = 1;
		dataFrame.speedRight = -dataFrame.speedRight;
	}

	// Write data frame to file (PRU communication)
	if (!this->pruFile.is_open() || !this->pruFile.good()) {
		throw(std::string("Failed writing to file: ") + std::string(DEVICE_NAME));
	}
	this->pruFile.write((char*)(&dataFrame), 13);
}

void Motors::enable() {
	std::ofstream file;

	file.open("/sys/class/gpio/gpio78/value", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio78/value"));
	}
	file << "0";
	file.close();

	file.open("/sys/class/gpio/gpio79/value", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio79/value"));
	}
	file << "0";
	file.close();
}

void Motors::disable() {
	std::ofstream file;

	file.open("/sys/class/gpio/gpio78/value", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio78/value"));
	}
	file << "1";
	file.close();

	file.open("/sys/class/gpio/gpio79/value", std::ofstream::out);
	if (!file.is_open() || !file.good()) {
		file.close();
		throw(std::string("Failed opening file: /sys/class/gpio79/value"));
	}
	file << "1";
	file.close();
}

float Motors::getDistance() {
	return distance;
}

void Motors::resetDistance() {
	distance = 0.0;
}

float Motors::getSpeedLeft() {
	return this->speedLeft;
}

float Motors::getSpeedRight() {
	return this->speedRight;
}
