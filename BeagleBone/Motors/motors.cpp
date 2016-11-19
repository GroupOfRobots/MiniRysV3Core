#include "motors.h"

Motors::Motors(){
	sended = new data_frame;
	result = 0;
}

Motors::~Motors(){
	delete sended;
	this->disable();
	fs.open("/sys/class/gpio/export",std::ofstream::out);
	if(fs.fail())throw 5;
	fs << "78";
	fs.close();
	fs.open("/sys/class/gpio/gpio78/direction",std::ofstream::out);
	fs << "out";
	fs.close();
	fs.open("/sys/class/gpio/export",std::ofstream::out);
	fs << "79";
	fs.close();
	fs.open("/sys/class/gpio/gpio79/direction",std::ofstream::out);
	fs << "out";
	fs.close();
}

int Motors::setSpeed(int leftspeed, int rightspeed,int microstep){

	struct pollfd pollfds[1];

	if(leftspeed>=0){
		sended->dirl = 0;
		sended->speedl = leftspeed;
	}
	else{
		sended->dirl = 1;
		sended->speedl = -1*leftspeed;
	}
	if(rightspeed>=0){
		sended->dirr = 0;
		sended->speedr = rightspeed;
	}
	else{
		sended->dirr = 1;
		sended->speedr = -1*rightspeed;
	}
	sended->mstep = microstep;

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

	/*
	 * If the RPMsg channel doesn't exist yet the character device
	 * won't either.
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted.
	 */
	if (pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	//printf("Opened %s, sending %d messages\n\n", DEVICE_NAME, 1);

	result = 0;

	/* Send data structure to the PRU through the RPMsg channel */
	result = write(pollfds[0].fd, sended, 13);
	if (result > 0)
		//printf("Message %d: Sent to PRU\n", 1);

	//result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
	//if (result > 0)
	//	printf("Message %d received from PRU:%s\n\n", 1, readBuf);

	/* Received all the messages the example is complete */
	//printf("Received %d messages, closing %s\n", 1, DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}

void Motors::enable(){

   fs.open("/sys/class/gpio/gpio78/value",std::ofstream::out);
   fs << "0";
   fs.close();
   fs.open("/sys/class/gpio/gpio79/value",std::ofstream::out);
   fs << "0";
   fs.close();
}

void Motors::disable(){

   fs.open("/sys/class/gpio/gpio78/value",std::ofstream::out);
   fs << "1";
   fs.close();
   fs.open("/sys/class/gpio/gpio79/value",std::ofstream::out);
   fs << "1";
   fs.close();
}
