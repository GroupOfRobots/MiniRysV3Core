#include <assert.h>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <iostream>
#include <memory>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

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
int balancingActive;
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
		// get 4 reads and take out average
		float angle = 0.0;
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle = angle/4*180/3.1415;
		pid.calculate_speed(angle, silniki.getSpeedLeft(), silniki.getSpeedRight(), steering, throttle, finalLeftSpeed, finalRightSpeed);
		if (balancingActive) {
			silniki.setSpeed(finalLeftSpeed, finalRightSpeed,  4);
			std::cout << "I'm balancing, angle: " << angle << std::endl;
		}

		if (angle > 40.0) {
			actual = Position::layfront;
			balancingActive = 0;
		} else if (angle < -40.0) {
			actual = Position::layback;
			balancingActive = 0;
		} else {
			actual = Position::standing;
		}
	}
}

int main() {
	signal(SIGINT, sigintHandler);

	silniki.initialize();
	imu.initialize();
	usleep(1000);

	silniki.enable();
	/*if (!lipol.isGood()) {
		printf("niski poziom napiecia baterii");
	}*/
	imu.resetFIFO();
	pid.timerStart();

	std::thread balance(balancing);
	usleep(1000000);
	while(!exitFlag) {
		switch (actual) {
			case standing:
				balancingActive = 1;
				usleep(200000);
				break;
			case layfront:
				std::cout << "I'm trying to stand front" << std::endl;
				silniki.setSpeed(0.0, 0.0, 1);
				silniki.disable();
				// 5s
				usleep(5000000);
				silniki.enable();
				silniki.setSpeed(700.0, 700.0, 1);
				// 1s
				usleep(1000000);
				silniki.setSpeed(-700.0, -700.0, 1);
				usleep(500000);
				// actual = Position::standing;
				std::cout << "I'm standing(?)" << std::endl;
				break;
			case layback:
				std::cout << "I'm trying to stand back" << std::endl;
				silniki.setSpeed(0.0, 0.0, 1);
				silniki.disable();
				usleep(5000000);
				silniki.enable();
				silniki.setSpeed(-700.0, -700.0, 1);
				usleep(1000000);
				silniki.setSpeed(700.0, 700.0, 1);
				usleep(500000);
				// actual = Position::standing;
				std::cout << "I'm standing(?)" << std::endl;
				break;
		}
	}

	balance.join();
	silniki.setSpeed(0.0, 0.0, 1);
	silniki.disable();
	// 1s
	usleep(1000000);

	return 0;
}

