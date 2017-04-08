#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <fstream>

#define MAX_BUFFER_SIZE		512
#define MAX_ACCELERATION	2000
#define MAX_MOTOR_SPEED		3000
#define MIN_MOTOR_SPEED		4000000
#define MAX_USER_SPEED		800
#define MIN_USER_SPEED		0
#define DEVICE_NAME		"/dev/rpmsg_pru31"

struct data_frame{
	unsigned int speedl;
	unsigned int speedr;
	uint8_t dirl;
	uint8_t dirr;
	uint8_t mstep;
};

class Motors{

private:
	struct data_frame *sended;
	char readBuf[MAX_BUFFER_SIZE];
	int result;
	std::ofstream fs;
	float userspeedleft;
	float userspeedright;
	float maxmotorspeed;
	float minmotorspeed;
	long distance;

public:

	Motors();
	~Motors();
	int setSpeed(float,float,float,int); //speed from -1000 to 1000
	void enable();
	void disable();
	float getDistance();
	void resetDistance();
	float getLeftSpeed();
	float getRightSpeed();
};


#endif
