#include <chrono>
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
volatile float angle;
volatile int exitFlag = 0;

void sigintHandler(int) {
	exitFlag = 1;
}

void inertialUnitThreadFn() {
	while (!exitFlag) {
		// Retrieve 2 IMU readings and calculate average (noise reduction)
		float angle = 0.0;
		try {
			angle += imu.getRoll();
			angle += imu.getRoll();
		} catch (std::string & error) {
			std::cout << "Error getting IMU reading: " << error << std::endl;
			exitFlag = 1;
			break;
		}
		angle = angle/2*180/3.1415;
	}

	std::cout << "IMU thread ended!\n";
}

void balancingThreadFn() {
	int steering = 0;
	int throttle = 0;
	float finalLeftSpeed = 0;
	float finalRightSpeed = 0;

	// auto loopTime = 100ms;
	auto loopTime = std::chrono::milliseconds(10);
	// Loop time in nanoseconds
	// uint64_t loopTime = 1000*1000*1000*(1/loopFrequency);

	// Durations in nanoseconds
	uint64_t totalDuration = 0;
	uint64_t maxDuration = 0;
	uint64_t minDuration = 1000*1000*1000;
	uint64_t iterations = 0;
	// std::chrono::steady_clock::time_point
	auto nextTimePoint = std::chrono::steady_clock::now();
	auto previousTimePoint = nextTimePoint - loopTime;

	while (!exitFlag) {
		// Update odometry with given loop time (dt)
		motors.updateOdometry(0.0);

		float localAngle = angle;
		// Set current position
		if (localAngle > 40.0) {
			actual = Position::layfront;
		} else if (localAngle < -40.0) {
			actual = Position::layback;
		} else {
			actual = Position::standing;

			// Calculate target speeds for motors
			controller.calculateSpeed(localAngle, motors.getSpeedLeft(), motors.getSpeedRight(), steering, throttle, finalLeftSpeed, finalRightSpeed);

			// Set target speeds
			try {
				motors.setSpeed(finalLeftSpeed, finalRightSpeed, 4);
			} catch (std::string & error) {
				std::cout << "Error setting motors speed: " << error << std::endl;
				// exitFlag = 1;
				break;
			}
		}

		// Calculate duration of current iteration
		auto now = std::chrono::steady_clock::now();
		uint64_t duration = (std::chrono::duration_cast<std::chrono::nanoseconds>(now - previousTimePoint)).count();
		// Save current time as reference for next iteration
		previousTimePoint = now;
		// Add total measurements duration
		totalDuration += duration;
		// Check and save max and min iteration duration
		if (duration > maxDuration) {
			maxDuration = duration;
		}
		if (duration < minDuration) {
			minDuration = duration;
		}

		++iterations;
		// Sleep until next
		nextTimePoint += loopTime;
		std::this_thread::sleep_until(nextTimePoint);
	}

	if (iterations == 0) {
		iterations = 1;
	}

	std::cout << "\nMax duration: " << maxDuration << "ns" << std::endl;
	std::cout << "Min duration: " << minDuration << "ns" << std::endl;
	std::cout << "Avg duration: " << totalDuration/iterations << "ns" << std::endl;
	std::cout << "Avg frequency: " << 1000000000/(totalDuration/iterations) << "Hz" << std::endl;
}

int main() {
	signal(SIGINT, sigintHandler);

	std::cout << "Initializing...\n";

	try {
		motors.initialize();
		imu.initialize();
		// imu.calibrate();
	} catch (std::string & error) {
		std::cout << "Error initializing: " << error << std::endl;
		return 1;
	}
	usleep(100 * 1000);

	std::cout << "Setup...\n";

	// controller.setSpeedPID(0.03, 0.0001, 0.008);
	controller.setSpeedPID(0.03, 0.0002, 0.01);
	// controller.setStabilityPID(50, 0.05, 20);
	controller.setStabilityPID(50, 0.03, 20);
	controller.setSpeedFilterFactor(0.92);

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

	std::cout << "Starting threads...\n";

	std::thread imuThread(inertialUnitThreadFn);
	std::thread balanceThread(balancingThreadFn);
	// 1s
	usleep(1000 * 1000);

	std::cout << "Running!\n";

	while(!exitFlag) {
		switch (actual) {
			case standing:
				// 0.5s
				usleep(500 * 1000);
				break;
			case layfront:
			case layback:
				std::cout << "I'm trying to stand" << std::endl;
				int mult = (actual == layfront ? 1 : -1);
				try {
					motors.setSpeed(0.0, 0.0, 1);
					motors.disable();
					// 2s
					usleep(2000 * 1000);
					motors.enable();
					usleep(500 * 1000);

					// Drive backwards full-speed
					motors.setSpeed(mult * 400.0, mult * 400.0, 1);
					// Wait 0.5s
					usleep(500 * 1000);
					// Drive forward full-speed
					motors.setSpeed(-mult * 600.0, -mult * 600.0, 1);
					// Wait until we're vertical enough
					while (true) {
						float localAngle = angle;
						// +- 5 deg should be enough
						if (localAngle > -5 && localAngle < 5) {
							std::cout << "Stood up(?), angle: " << localAngle << std::endl;
							break;
						}
					}

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

	balanceThread.join();
	imuThread.join();

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

