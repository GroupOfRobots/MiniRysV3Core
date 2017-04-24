#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "../Motors/Motors.h"
// #include "../Battery/battery.h"
#include "../IMU/IMU.h"
#include "../Controller/Controller.h"

enum Position {
	layfront = 0,
	layback = 1,
	standing = 2
};

// Battery lipol;
Motors motors;
IMU imu;
Controller controller;
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
		motors.updateOdometry(0.0);

		// Retrieve 4 IMU readings and calculate average (noise reduction)
		float angle = 0.0;
		try {
			angle += imu.getRoll();
			usleep(50);
			angle += imu.getRoll();
			// usleep(50);
			// angle += imu.getRoll();
			// usleep(50);
			// angle += imu.getRoll();
			// usleep(50);
		} catch (std::string & error) {
			std::cout << "Error getting IMU reading: " << error << std::endl;
			exitFlag = 1;
			break;
		}
		angle = angle/2*180/3.1415;

		// Set current position
		if (angle > 40.0) {
			actual = Position::layfront;
		} else if (angle < -40.0) {
			actual = Position::layback;
		} else {
			actual = Position::standing;

			// Calculate target speeds for motors
			controller.calculateSpeed(angle, motors.getSpeedLeft(), motors.getSpeedRight(), steering, throttle, finalLeftSpeed, finalRightSpeed);
			// std::cout << "I'm balancing, angle: " << angle << " speed: " << finalLeftSpeed << std::endl;
			// Set target speeds
			try {
				motors.setSpeed(finalLeftSpeed, finalRightSpeed, 4);
			} catch (std::string & error) {
				std::cout << "Error setting motors speed: " << error << std::endl;
				// exitFlag = 1;
				break;
			}

		}
	}
}

int main() {
	signal(SIGINT, sigintHandler);

	try {
		motors.initialize();
		imu.initialize();
	} catch (std::string & error) {
		std::cout << "Error initializing: " << error << std::endl;
		return 1;
	}

	usleep(100 * 1000);

	// Original values
	// controller.setSpeedPID(0.009, 0.1, 0);
	// controller.setStabilityPID(50, 0, 20);
	controller.setSpeedPID(0.03, 0.0001, 0.008);
	controller.setStabilityPID(50, 0.05, 20);

	/*if (!lipol.isGood()) {
		printf("niski poziom napiecia baterii");
	}*/
	try {
		motors.enable();
		imu.resetFIFO();
		controller.restartTimer();
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
					motors.setSpeed(0.0, 0.0, 1);
					motors.disable();
					// 3s
					usleep(3000 * 1000);
					motors.enable();
					motors.setSpeed(mult * 400.0, mult * 400.0, 1);
					// 0.5s
					usleep(500 * 1000);
					motors.setSpeed(-mult * 600.0, -mult * 600.0, 1);
					// 0.5s
					usleep(500 * 1000);
					controller.zeroPIDs();
				} catch (std::string & error) {
					std::cout << "Error standing up from laying: " << error << std::endl;
					// exitFlag = 1;
					break;
				}
				std::cout << "I'm standing(?)" << std::endl;
				break;
		}
	}

	std::cout << "Closing..." << std::endl;

	balance.join();
	try {
		motors.setSpeed(0.0, 0.0, 1);
		motors.disable();
	} catch (std::string & error) {
		std::cout << "Error disabling motors: " << error << std::endl;
		return 3;
	}
	// 1s
	usleep(1000 * 1000);

	return 0;
}

