#ifndef _SONAR_H_
#define _SONAR_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <fstream>

#define MAX_BUFFER_SIZE		512
#define DEVICE1_NAME		"/dev/rpmsg_pru30"


class Sonar{

private:
	char readBuf[MAX_BUFFER_SIZE];
	int result;

public:

	Sonar();
	int getDistance(int); //arg sonar numer from 1 to 3
};


#endif
