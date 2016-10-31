#ifndef MPU_H_
#define MPU_H_

extern "C"{

#include "MPU6050.h"
}

class imu{
	private:
		short accel[3], gyro[3], sensors[1];
	    long quat[4];
	    unsigned long timestamp;
	    unsigned char more[0];
	    //struct pollfd fdset[1];
	    char buf[1];


	public:
	    void initialize();
	    void estimete();
	    void setSens();
	    float getAngle();
};



#endif
