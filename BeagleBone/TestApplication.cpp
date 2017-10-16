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
int balancingactive;
int nowyspeed=0;
float finalleftspeed;
float finalrightspeed;
int steering;
int throttle;

 enum Position{
	 layfront = 0,
	 layback = 1,
	 standing = 2
 };

volatile Position actual;

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
		angle =angle/4*180/3.1415;
		pid.calculate_speed(angle,silniki.getLeftSpeed(),silniki.getRightSpeed(),steering,throttle,finalleftspeed,finalrightspeed);
		if(balancingactive)silniki.setSpeed(finalleftspeed,finalrightspeed,0.0,4);

		//printf("%f\n",angle);
		if(angle>40.0){
			actual = Position::layfront;
			balancingactive = 0;

		}
		else if (angle<-40.0){
			actual = Position::layback;
			balancingactive = 0;
		}
		else actual = Position::standing;


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

	std::thread balance(balancing); //create balancing thread
	usleep(500000);
	usleep(500000);
	while(1){

		switch (actual){
		case standing:
			balancingactive  =1;
			usleep(200000);
		break;
		case layfront:
			std::cout<<"I'm trying to stand front"<<std::endl;
			silniki.setSpeed(0.0,0.0,0.0,4);
			silniki.disable();
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			silniki.enable();
			silniki.setSpeed(400.0,400.0,0.0,1);
			usleep(200000);
			silniki.setSpeed(500.0,500.0,0.0,1);
			usleep(200000);
			silniki.setSpeed(550.0,550.0,0.0,1);
			usleep(500000);
			silniki.setSpeed(-400.0,-400.0,0.0,1);
			usleep(150000);
			silniki.setSpeed(-500.0,-500.0,0.0,1);
			usleep(50000);
			actual = Position::standing;
			std::cout<<"I'm standing"<<std::endl;
		break;
		case layback:
			std::cout<<"I'm trying to stand back"<<std::endl;
			silniki.setSpeed(0.0,0.0,0.0,1);
			silniki.disable();
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			usleep(500000);
			silniki.enable();
			silniki.setSpeed(-400.0,-400.0,0.0,1);
			usleep(200000);
			silniki.setSpeed(-500.0,-500.0,0.0,1);
			usleep(200000);
			silniki.setSpeed(-550.0,-550.0,0.0,1);
			usleep(500000);
			silniki.setSpeed(350.0,350.0,0.0,1);
			usleep(100000);
			silniki.setSpeed(550.0,550.0,0.0,1);
			usleep(50000);
			actual = Position::standing;
			std::cout<<"I'm standing"<<std::endl;
		break;
		}
	}

	balance.detach();
	return 0;
}

