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

	//in new kernel only device tree manager is available instead of omap_mux
	//just create dto file slot for irys cape :)


	//gpio_MS1, gpio_MS2, gpio_STEP, gpio_SLP, gpio_DIR, rpm speed, steps per revolution
	EasyDriver motor1(45, 23, 31, 30, 48, 120, 200); //gpio pins (last two arguments not necessary as they are the default values)

	//In case the motor SLP (sleep) pin is low
	motor1.wake();

	cout << "*** Start of Motor Test" << endl;
	cout << "*** Rotating - Forward 360 degrees (full step)" << endl;
	//forward single step
	motor1.rotate(360);
	sleep(1);	//sleep for 1 second between each test stage

	//back half step
	cout << "*** Rotating - Reverse 360 degrees (half step)" << endl;
	motor1.setStepMode(STEP_HALF);
	motor1.reverseDirection();
	motor1.rotate(360);
	sleep(1);

	cout << "*** Set speed to 120rpm" << endl;
	motor1.setSpeed(120);

	//forward quarter step
	cout << "*** Rotating - Forward 360 degrees (quarter step)" << endl;
	motor1.setStepMode(STEP_QUARTER);
	motor1.reverseDirection();
	motor1.rotate(360);
	sleep(1);

	//reverse eight step
	cout << "*** Rotating - Reverse 360 degrees (eight step)" << endl;
	motor1.setStepMode(STEP_EIGHT);
	motor1.rotate(-360);

	cout << "*** Sleep the motor for 10 seconds" << endl;
	//Sleep the EasyDriver controller
	motor1.sleep();  //easy to move motor shaft while sleep is enabled
					 //unexport of SLP pin can set this low, meaning shaft
					 //torque is high when finished
	sleep(10);
	//motor1.wake(); not necessary in this case as destructor of motor1 calls unexport

	cout << "*** Motor Test Finished" << endl;

	//motor1 object destroyed now (goes out of scope)
}

void imuInitialize(){

	return;
}




int main(int argc, char *argv[]){

	stepperMotorTest();

	return 0;
}
