//TestApplication

/*Pin configuration
 *

*/

#include <iostream>
#include <string>
#include <unistd.h>
#include "SimpleGPIO/SimpleGPIO.h"
#include "StepperDriver/EasyDriver.h"
using namespace std;





void stepperMotorTest(){

	//Set up the mux mode correctly on the P8 Header for the following pins:
	//Set up as output pins 0x07 (Mode 7 on all pins)
	gpio_omap_mux_setup("gpmc_ad7", "07"); 	//gpio 39 P8pin# 4
	gpio_omap_mux_setup("gpmc_ad10", "07"); //gpio 26 P8pin#14
	gpio_omap_mux_setup("gpmc_ad6", "07"); 	//gpio 38 P8pin# 3
	gpio_omap_mux_setup("gpmc_ad2", "07"); 	//gpio 34 P8pin# 5
	gpio_omap_mux_setup("gpmc_ad12", "07"); //gpio 44 p8pin#12

	//gpio_MS1, gpio_MS2, gpio_STEP, gpio_SLP, gpio_DIR, rpm speed, steps per revolution
	EasyDriver motor1(38, 39, 34, 26, 44, 60, 200); //gpio pins (last two arguments not necessary as they are the default values)

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




int main(int argc, char *argv[]){

	stepperMotorTest();

	return 0;
}

