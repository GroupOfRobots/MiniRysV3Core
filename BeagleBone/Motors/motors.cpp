#include "motors.h"

Motors::Motors(){
	sended = new data_frame;
	result = 0;
	distance = 0;
	userspeedleft=0;
	userspeedright=0;
	maxmotorspeed = MAX_MOTOR_SPEED;
	minmotorspeed = MIN_MOTOR_SPEED;
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

Motors::~Motors(){
	delete sended;
	this->disable();
}

int Motors::setSpeed(float leftspeed, float rightspeed,float dt, int microstep){

	struct pollfd pollfds[1];

	if(leftspeed!=userspeedleft||rightspeed!=userspeedright||microstep!=sended->mstep){

		if ((userspeedleft - leftspeed) > MAX_ACCELERATION)
			userspeedleft -= MAX_ACCELERATION;
		else if ((userspeedleft - leftspeed) < -MAX_ACCELERATION)
			userspeedleft += MAX_ACCELERATION;
		else
			userspeedleft = leftspeed;

		if ((userspeedright - rightspeed) > MAX_ACCELERATION)
			userspeedright -= MAX_ACCELERATION;
		else if ((userspeedright - rightspeed) < -MAX_ACCELERATION)
			userspeedright += MAX_ACCELERATION;
		else
			userspeedright = rightspeed;

		if(microstep==1){
			maxmotorspeed = MAX_MOTOR_SPEED;
			minmotorspeed = MIN_MOTOR_SPEED;
		}
		else if (microstep==2){
			maxmotorspeed = MAX_MOTOR_SPEED/2;
			minmotorspeed = MIN_MOTOR_SPEED/2;
		}
		else if (microstep==3){
			maxmotorspeed = MAX_MOTOR_SPEED/4;
			minmotorspeed = MIN_MOTOR_SPEED/4;
		}
		else if (microstep==4){
			maxmotorspeed = MAX_MOTOR_SPEED/8;
			minmotorspeed = MIN_MOTOR_SPEED/8;
		}
		else throw 20;

		distance+= (userspeedleft+userspeedright)/2*dt*0.7;

		if(userspeedleft>=0){
			sended->dirl = 0;
			if(userspeedleft==0)sended->speedl = 0;
			else{
				//userspeedleft = 1/userspeedleft;
				sended->speedl = (int)minmotorspeed*(1/userspeedleft);
			}
		}
		else{
			sended->dirl = 1;
			//userspeedleft = -1/userspeedleft;
			sended->speedl = (int)minmotorspeed*(-1/userspeedleft);
		}
		if(rightspeed>=0){
			sended->dirr = 0;
			if(userspeedright==0)sended->speedr = 0;
			else {
				//userspeedright = 1/userspeedright;
				sended->speedr = (int)minmotorspeed*(1/userspeedright);
			}
		}
		else{
			sended->dirr = 1;
			//userspeedright = -1/userspeedright;
			sended->speedr = (int)minmotorspeed*(-1/userspeedright);
		}

		if(sended->speedl!=0&&sended->speedl<maxmotorspeed)sended->speedl = (unsigned int)maxmotorspeed;
		if(sended->speedr!=0&&sended->speedr<maxmotorspeed)sended->speedr = (unsigned int)maxmotorspeed;

		sended->mstep = microstep;

		/* Open the rpmsg_pru character device file */
		pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

		if (pollfds[0].fd < 0) {
			printf("Failed to open %s\n", DEVICE_NAME);
			return -1;
		}

		result = 0;

		result = write(pollfds[0].fd, sended, 13);

		close(pollfds[0].fd);
	}

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

float Motors::getDistance(){

	return distance;
}

void Motors::resetDistance(){
	distance = 0.0;
}

float Motors::getLeftSpeed(){
	return userspeedleft;
}

float Motors::getRightSpeed(){
	return userspeedright;
}
