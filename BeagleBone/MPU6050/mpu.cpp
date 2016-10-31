#include "mpu.h"


void imu::initialize(){
	init();
}

float imu::getAngle(){

	int fifo_read = dmp_read_fifo(gyro, accel, quat, &timestamp, sensors, more);
	            if (fifo_read != 0) {
	                //printf("Error reading fifo.\n");
	            }
	float angles[NOSENTVALS];
	rescale_l(quat, angles+9, QUAT_SCALE, 4);
    rescale_s(gyro, angles+3, GYRO_SCALE, 3);
    rescale_s(accel, angles+6, ACCEL_SCALE, 3);
    euler(angles+9, angles);

    return 0.0;

    //printf("Yaw: %+5.1f\tPitch: %+5.1f\tRoll: %+5.1f\n", angles[0]*180.0/PI, angles[1]*180.0/PI, angles[2]*180.0/PI);
}
