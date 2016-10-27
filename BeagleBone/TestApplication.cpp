//TestApplication //something is wrong with libs try to use blacklib


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
#include "Gpio/memGPIO.hpp"
//#include "MPU6050/MPU6050.h"
using namespace std;



void imuInitialize(){

	return;
}




int main(int argc, char *argv[]){

	easyBlack::memGPIO Irys;
	// Get PINs data for better performance.
	easyBlack::memGPIO::gpioPin usr0 = Irys.getPin ("USR0");
	easyBlack::memGPIO::gpioPin left_m0 = Irys.getPin ("P8_11");
	easyBlack::memGPIO::gpioPin left_m1 = Irys.getPin ("P8_13");
	easyBlack::memGPIO::gpioPin left_m2 = Irys.getPin ("P8_15");
	easyBlack::memGPIO::gpioPin right_m0 = Irys.getPin ("P8_17");
	easyBlack::memGPIO::gpioPin right_m1 = Irys.getPin ("P8_19");
	easyBlack::memGPIO::gpioPin right_m2 = Irys.getPin ("P8_21");

	easyBlack::memGPIO::gpioPin left_en = Irys.getPin ("P9_11");
	easyBlack::memGPIO::gpioPin right_dir = Irys.getPin ("P9_12");
	easyBlack::memGPIO::gpioPin left_stp = Irys.getPin ("P9_13");
	easyBlack::memGPIO::gpioPin right_stp = Irys.getPin ("P9_14");
	easyBlack::memGPIO::gpioPin left_dir = Irys.getPin ("P9_15");
	easyBlack::memGPIO::gpioPin right_en = Irys.getPin ("P9_16");

	const unsigned char output = Irys.OUTPUT;
	const unsigned char low = Irys.LOW;
	const unsigned char high = Irys.HIGH;

	Irys.pinMode (usr0, output);
	Irys.pinMode (left_m0, output);
	Irys.pinMode (left_m1, output);
	Irys.pinMode (left_m2, output);
	Irys.pinMode (right_m0, output);
	Irys.pinMode (right_m1, output);
	Irys.pinMode (right_m2, output);
	Irys.pinMode (left_en, output);
	Irys.pinMode (left_dir, output);
	Irys.pinMode (left_stp, output);
	Irys.pinMode (right_en, output);
	Irys.pinMode (right_dir, output);
	Irys.pinMode (right_stp, output);


	Irys.digitalWrite (left_en, low);
	Irys.digitalWrite (right_en, low);
	Irys.digitalWrite (left_dir, low);
	Irys.digitalWrite (right_dir, low);

	Irys.digitalWrite (left_m0, low);
	Irys.digitalWrite (left_m1, low);
	Irys.digitalWrite (left_m2, low);
	Irys.digitalWrite (right_m0, low);
	Irys.digitalWrite (right_m1, low);
	Irys.digitalWrite (right_m2, low);


	for(int i=0; i<10000; i++){


		Irys.digitalWrite(right_stp,low);
		Irys.digitalWrite(left_stp,low);
		Irys.digitalWrite(usr0,low);
		usleep(51);
		Irys.digitalWrite(right_stp,high);
		Irys.digitalWrite(left_stp,high);
		Irys.digitalWrite(usr0,high);
		usleep(51);
	}

    usleep(100);
	Irys.digitalWrite (left_en, high);
	Irys.digitalWrite (right_en, high);


	Irys.resetLEDPin0ToDefault ();
	Irys.~memGPIO ();

	  exit (EXIT_SUCCESS);



	return 0;
}

