#include "imu.h"

void Imu::setup() {
    // initialize device
    //printf("Initializing I2C devices...\n");
    mpu.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    // load and configure the DMP
    //printf("Initializing DMP...\n");
    devStatus = mpu.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        //printf("Enabling DMP...\n");
        mpu.setDMPEnabled(true);

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready!\n");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}

void Imu::readData() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    if (fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        printf("FIFO overflow!\n");

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (fifoCount >= 42) {
        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            //printf("r %7.2f \n", ypr[2]);
    }
}

float Imu::getPitch(){

	readData();
	return ypr[1];
}
float Imu::getRoll(){

	readData();
	return ypr[2];
}
float Imu::getYaw(){

	readData();
	return ypr[0];
}
