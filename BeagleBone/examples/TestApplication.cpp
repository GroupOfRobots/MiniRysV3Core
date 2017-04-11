/*#include <assert.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <memory>
#include <termios.h>
#include <unistd.h>*/

#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "../Motors/Motors.h"
// #include "../Battery/battery.h"
#include "../IMU/IMU.h"
#include "../Controller/controller.h"

enum Position {
	layfront = 0,
	layback = 1,
	standing = 2
};

// Battery lipol;
Motors silniki;
IMU imu;
Controller pid;
volatile Position actual;
volatile int exitFlag = 0;

void sigintHandler(int) {
	exitFlag = 1;
}

// balancing thread function
void balancing() {
	int steering = 0;
	int throttle = 0;
	float finalLeftSpeed = 0;
	float finalRightSpeed = 0;

	while (!exitFlag) {
		// Update odometry with given loop time (dt)
		silniki.updateOdometry(0.0);

		// Retrieve 4 IMU readings and calculate average (noise reduction)
		float angle = 0.0;
		try {
			angle += imu.getRoll();
			usleep(50);
			angle += imu.getRoll();
			usleep(50);
			angle += imu.getRoll();
			usleep(50);
			angle += imu.getRoll();
			usleep(50);
		} catch (std::string & error) {
			std::cout << "Error getting IMU reading: " << error << std::endl;
			exitFlag = 1;
			break;
		}
		angle = angle/4*180/3.1415;

		// Set current position
		if (angle > 40.0) {
			actual = Position::layfront;
		} else if (angle < -40.0) {
			actual = Position::layback;
		} else {
			actual = Position::standing;
			std::cout << "I'm balancing, angle: " << angle << std::endl;

			// Calculate target speeds for motors
			pid.calculate_speed(angle, silniki.getSpeedLeft(), silniki.getSpeedRight(), steering, throttle, finalLeftSpeed, finalRightSpeed);
			// Set target speeds
			try {
				silniki.setSpeed(finalLeftSpeed, finalRightSpeed, 4);
			} catch (std::string & error) {
				std::cout << "Error setting motors speed: " << error << std::endl;
				exitFlag = 1;
				break;
			}
		}
	}
}

int main() {
	signal(SIGINT, sigintHandler);

	try {
		silniki.initialize();
		imu.initialize();
	} catch (std::string & error) {
		std::cout << "Error initializing: " << error << std::endl;
		return 1;
	}

	usleep(100 * 1000);

	/*if (!lipol.isGood()) {
		printf("niski poziom napiecia baterii");
	}*/
	try {
		silniki.enable();
		imu.resetFIFO();
		pid.timerStart();
	} catch (std::string & error) {
		std::cout << "Error starting up: " << error << std::endl;
		return 2;
	}

	std::thread balance(balancing);
	usleep(1000 * 1000);
	std::cout<< "4\n";
	while(!exitFlag) {
		switch (actual) {
			case standing:
				usleep(500 * 1000);
				break;
			case layfront:
			case layback:
				std::cout << "I'm trying to stand" << std::endl;
				int mult = (actual == layfront ? 1 : -1);
				try {
					silniki.setSpeed(0.0, 0.0, 1);
					silniki.disable();
					// 5s
					usleep(5000 * 1000);
					silniki.enable();
					silniki.setSpeed(mult * 700.0, mult * 700.0, 1);
					// 1s
					usleep(1000 * 1000);
					silniki.setSpeed(-mult * 700.0, -mult * 700.0, 1);
					usleep(500 * 1000);
				} catch (std::string & error) {
					std::cout << "Error standing up from laying: " << error << std::endl;
					exitFlag = 1;
					break;
				}
				std::cout << "I'm standing(?)" << std::endl;
				break;
		}
	}

	std::cout << "Closing..." << std::endl;

	balance.join();
	try {
		silniki.setSpeed(0.0, 0.0, 1);
		silniki.disable();
	} catch (std::string & error) {
		std::cout << "Error disabling motors: " << error << std::endl;
		return 3;
	}
	// 1s
	usleep(1000 * 1000);

	return 0;
}

