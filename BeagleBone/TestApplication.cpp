//TestApplication

/*Pin configuration
 P8 Header (changes in new version from 7 to 17)
 11 GPIO_45 - Motor Left M0 mode7
 13 GPIO_23 - Motor Left M1 mode7
 15 GPIO_47 - Motor Left M2 mode7
 17 GPIO_27 - Motor Right M0 mode7
 19 GPIO_22 - Motor Right M1 mode7
 21 GPIO_62 - Motor Right M2 mode7 //emmc2pin could be problematic

 P9 Header
 11 GPIO_30 - Motor Left Enable mode7
 12 GPIO_60 - Motor Right Direction mode7
 13 GPIO_31 - Motor Left Step mode7
 14 GPIO_50 - Motor Right Step mode7
 15 GPIO_48 - Motor Left Direction mode7
 16 GPIO_51 - Motor Right Enable mode7// end pins for Stepper

 19 GPIO_5 - I2C SCL mode2
 20 GPIO_4 - I2C SDA mode2
 36 AIN5 - cell 1 (1.8V tolerant!!!)
 38 AIN3 - cell2
 40 AIN1 - cell3
 SPI1 not available !
 //rest to configure

*/

#include <iostream>
#include <string>
#include <unistd.h>
#include "SimpleGPIO/SimpleGPIO.h"
#include "StepperDriver/EasyDriver.h"
//#include "MPU6050/MPU6050.h"
using namespace std;


void stepperMotorTest(){


	//gpio_MS1, gpio_MS2, gpio_STEP, gpio_SLP, gpio_DIR, rpm speed, steps per revolution
	EasyDriver motor1(45, 23, 31, 30, 48, 120, 200);
	//EasyDriver motor2(27, 22, 50, 51, 60, 10, 200);

	//In case the motor SLP (sleep) pin is low
	motor1.wake();
	//motor2.wake();

	cout << "*** Start of Motor Test" << endl;
	//cout << "*** Rotating - Forward 360 degrees (full step)" << endl;
	//forward single step
	motor1.step(720);
	//motor2.step(720);
	sleep(1);	//sleep for 1 second between each test stage

	//back half step
	//cout << "*** Rotating - Reverse 360 degrees (half step)" << endl;
	//motor1.setStepMode(STEP_HALF);
	//motor1.reverseDirection();
	//motor1.rotate(720);
	//sleep(1);

	//cout << "*** Set speed to 120rpm" << endl;
	//motor1.setSpeed(10);

	//forward quarter step
	//cout << "*** Rotating - Forward 360 degrees (quarter step)" << endl;
	//motor1.setStepMode(STEP_QUARTER);
	//motor1.reverseDirection();
	//motor1.rotate(360);
	//sleep(1);

	//reverse eight step
	//cout << "*** Rotating - Reverse 360 degrees (eight step)" << endl;
	//motor1.setStepMode(STEP_EIGHT);
	//motor1.rotate(-360);

	//cout << "*** Sleep the motor for 5 seconds" << endl;
	//Sleep the EasyDriver controller
	motor1.sleep();  //easy to move motor shaft while sleep is enabled
	//motor2.sleep();

	sleep(5);
	//motor1.wake(); not necessary in this case as destructor of motor1 calls unexport

	cout << "*** Motor Test Finished" << endl;

	//motor1 object destroyed now (goes out of scope)
}

void imuInitialize(){

	return;
}




int main(int argc, char *argv[]){

	gpio_export(45);
	gpio_set_dir(45, OUTPUT_PIN); //left mo
	gpio_export(23);
	gpio_set_dir(23, OUTPUT_PIN); //left m1
	gpio_export(31);
	gpio_set_dir(31, OUTPUT_PIN); //step
	gpio_export(48);
	gpio_set_dir(48, OUTPUT_PIN); //dir
	gpio_export(30);
	gpio_set_dir(30, OUTPUT_PIN); //enable


	gpio_export(27);
	gpio_set_dir(27, OUTPUT_PIN); //right m0
	gpio_export(22);
	gpio_set_dir(22, OUTPUT_PIN); //right m1
	gpio_export(50);
	gpio_set_dir(50, OUTPUT_PIN); //right step
	gpio_export(60);
	gpio_set_dir(60, OUTPUT_PIN); // right dir
	gpio_export(51);
	gpio_set_dir(51, OUTPUT_PIN); //right enable

	gpio_set_value(30, LOW);
	gpio_set_value(48, LOW);
	gpio_set_value(51, LOW);
	gpio_set_value(60, LOW);

	gpio_set_value(45, LOW);
	gpio_set_value(23, HIGH);
	gpio_set_value(27, LOW);
	gpio_set_value(22, HIGH);


	for(int i=0; i<1000; i++){


		gpio_set_value(50, LOW);
		gpio_set_value(31, LOW);
		usleep(100);
		gpio_set_value(31, HIGH);
		gpio_set_value(50, HIGH);
		usleep(100);
	}

    usleep(100);
	gpio_set_value(51, HIGH);
	gpio_set_value(50, HIGH);
	gpio_set_value(60, HIGH);
	gpio_set_value(30, HIGH);
	gpio_set_value(27, HIGH);
	gpio_set_value(22, HIGH);
	usleep(100);

	gpio_unexport(45);
	gpio_unexport(23);
	gpio_unexport(31);
	gpio_unexport(48);
	gpio_unexport(30);


	gpio_unexport(27);
	gpio_unexport(22);
	gpio_unexport(50);
	gpio_unexport(60);
	gpio_unexport(51);




	return 0;
}

