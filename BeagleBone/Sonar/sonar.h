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

struct distance_frame{
	uint16_t front;
	uint16_t back;
	uint16_t top;
};

enum distance_sensors
{
    front = 1, //zamiast średników stosuje się przecinki
    back = 2,
    top = 3,
};


class Sonar{

private:
	struct distance_frame *sended;
	char readBuf[MAX_BUFFER_SIZE];
	int result;

public:

	Sonar();
	~Sonar();
	void measureDistance();
	int getDistance(distance_sensors); //arg sonar numer from 1 to 3
};


#endif
