#ifndef SENSORS_H
#define SENSORS_H

#include "mbed.h"
#define I2C_FREQ 400000
#define SCL PTC8
#define SDA PTC9
#define PI 3.1415926

#define ACCEL_ADDRESS (0xA6) // 0x53 = 0xA6 / 2
#define MAGN_ADDRESS  (0x3C) // 0x1E = 0x3C / 2
#define GYRO_ADDRESS  (0xD2) // 0x68 = 0xD0 / 2

// SENSOR CALIBRATION
/*****************************************************************/
// How to calibrate? Read the tutorial at http://dev.qu.tu-berlin.de/projects/sf-razor-9dof-ahrs
// Put MIN/MAX and OFFSET readings for your board here!
// Accelerometer
// "accel x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define ACCEL_X_MIN (-260.0f)
#define ACCEL_X_MAX (257.0f)
#define ACCEL_Y_MIN (-260.0f)
#define ACCEL_Y_MAX (263.0f)
#define ACCEL_Z_MIN (-245.0f)
#define ACCEL_Z_MAX (256.0f)

// Magnetometer
// "magn x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define MAGN_X_MIN (-520.0f)
#define MAGN_X_MAX (850.0f)
#define MAGN_Y_MIN (-640.0f)
#define MAGN_Y_MAX (684.0f)
#define MAGN_Z_MIN (-213.0f)
#define MAGN_Z_MAX (850.0f)

// Gyroscope
// "gyro x,y,z (current/average) = .../OFFSET_X  .../OFFSET_Y  .../OFFSET_Z
#define GYRO_X_OFFSET (-20.5f)
#define GYRO_Y_OFFSET (0.5f)
#define GYRO_Z_OFFSET (10.5f)

//*****************************************************************************/

#define GRAVITY 255.0f  //this equivalent to 1G in the raw data coming from the accelerometer 

#define ACCEL_X_OFFSET ((ACCEL_X_MIN + ACCEL_X_MAX) / 2.0f)
#define ACCEL_Y_OFFSET ((ACCEL_Y_MIN + ACCEL_Y_MAX) / 2.0f)
#define ACCEL_Z_OFFSET ((ACCEL_Z_MIN + ACCEL_Z_MAX) / 2.0f)
#define ACCEL_X_SCALE (GRAVITY / (ACCEL_X_MAX - ACCEL_X_OFFSET))
#define ACCEL_Y_SCALE (GRAVITY / (ACCEL_Y_MAX - ACCEL_Y_OFFSET))
#define ACCEL_Z_SCALE (GRAVITY / (ACCEL_Z_MAX - ACCEL_Z_OFFSET))

#define MAGN_X_OFFSET ((MAGN_X_MIN + MAGN_X_MAX) / 2.0f)
#define MAGN_Y_OFFSET ((MAGN_Y_MIN + MAGN_Y_MAX) / 2.0f)
#define MAGN_Z_OFFSET ((MAGN_Z_MIN + MAGN_Z_MAX) / 2.0f)
#define MAGN_X_SCALE (100.0f / (MAGN_X_MAX - MAGN_X_OFFSET))
#define MAGN_Y_SCALE (100.0f / (MAGN_Y_MAX - MAGN_Y_OFFSET))
#define MAGN_Z_SCALE (100.0f / (MAGN_Z_MAX - MAGN_Z_OFFSET))

// Gyro gain (conversion from raw to degree per seconds)
#define GYRO_GAIN 0.061035156f
#define GYRO_GAIN_X 0.061035156f //X axis Gyro gain
#define GYRO_GAIN_Y 0.061035156f //Y axis Gyro gain
#define GYRO_GAIN_Z 0.061035156f //Z axis Gyro gain

#define DEG2RAD(x) (x * 0.01745329252)  // *pi/180
#define RAD2DEG(x) (x * 57.2957795131)  // *180/pi

typedef char byte;

class GY80
{
public:
    I2C Wire;
    GY80();
    ~GY80();
    void Read_Accel(float* accel_v);
    void Read_Gyro(float* );
    void Read_Magn(float* );

private:
    short accel[3];
    short gyro[3];
    short mag[3];
    void Accel_Init();
    void Gyro_Init();
    void Magn_Init();

};

#endif