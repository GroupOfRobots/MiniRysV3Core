#include <chrono>
#include <csignal>
#include <iostream>
#include <string>

#include "../IMU/IMU.h"

IMU imu;
volatile int exitFlag = 0;

void sigintHandler(int) {
	exitFlag = 1;
}

int main() {
	signal(SIGINT, sigintHandler);

	try {
		imu.initialize();
	} catch (std::string & error) {
		std::cout << "Error initializing: " << error << std::endl;
		return 1;
	}

	usleep(100 * 1000);

	try {
		imu.calibrate();
		imu.resetFIFO();
	} catch (std::string & error) {
		std::cout << "Error starting up: " << error << std::endl;
		return 2;
	}

	usleep(100 * 1000);
	while(!exitFlag) {
		float angle = 0.0;
		try {
			angle = imu.getRoll();
		} catch (std::string & error) {
			std::cout << "Error getting IMU reading: " << error << std::endl;
			exitFlag = 1;
			break;
		}
		angle = angle * 180/3.1415;

		std::cout << "Angle: " << angle << std::endl;

		usleep(500 * 1000);
	}

	return 0;
}

