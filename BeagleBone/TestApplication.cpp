/*
Wake up example
 */


#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <iostream>


#include "Motors/motors.h" //motors class
#include "Battery/battery.h" //Battery measurement class
#include "IMU/imu.h" // angle measurement class
#include "Controller/controller.h" //angle & speed control


Motors silniki;
Battery lipol;
Imu imu;
Controller pid;
float angle;
float actualangle;
int nowyspeed=0;
float finalleftspeed;
float finalrightspeed;
int steering;
int throttle;

 enum Position{
	 layfront = 0,
	 layback = 1,
	 standing = 2,
	 check = 3
 };


void balancing(){ //thread balancing function

	while(1){
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle += imu.getRoll();
		usleep(50);
		angle = actualangle =angle/4*180/3.1415;
		pid.calculate_speed(angle,silniki.getLeftSpeed(),silniki.getRightSpeed(),steering,throttle,finalleftspeed,finalrightspeed);
		silniki.setSpeed(finalleftspeed,finalrightspeed,0.0,4);
		angle = 0.0;
	}
}

int main(void)
{

	silniki.disable();
	imu.setup(); // initialize IMU
	usleep(1000);
	silniki.enable();
	//if(!lipol.isGood())printf("niski poziom napiecia baterii");
	imu.resetFIFO();
	pid.timerStart(); //start pid timer

	//std::thread balance(balancing);
	//balance.detach();

	Position actual;


	while(1){

		switch (actual){
		case check:
			imu.resetFIFO();
			actualangle = imu.getRoll();
			actualangle =actualangle*180/3.1415;
			std::cout<<"my angle: "<<actualangle<<std::endl;
			if(actualangle>40.0)actual = Position::layfront;
			else if (actualangle<-40.0)actual = Position::layfront;
			else actual = Position::standing;
		break;
		case standing:
			//balance.join();
			std::cout<<"I'm standing"<<std::endl;
			usleep(2000000);
			actual = Position::check;
		break;
		case layfront:
			std::cout<<"I'm trying to stand front"<<std::endl;
			//balance.detach();
			usleep(5000000);
			silniki.setSpeed(-800.0,-800.0,0.0,1);
			usleep(500000);
			silniki.setSpeed(800.0,800.0,0.0,1);
			usleep(100000);
			//balance.join();
			actual = Position::check;
		break;
		case layback:
			std::cout<<"I'm trying to stand back"<<std::endl;
			//balance.detach();
			usleep(5000000);
			silniki.setSpeed(800.0,800.0,0.0,1);
			usleep(500000);
			silniki.setSpeed(-800.0,-800.0,0.0,1);
			usleep(100000);
			//balance.join();
			actual = Position::check;
		break;
		}
	}

	//balance.detach();
	return 0;
}

