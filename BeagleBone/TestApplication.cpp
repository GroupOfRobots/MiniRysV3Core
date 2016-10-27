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
#include "blacklib/BlackLib.h"
#include "blacklib/BlackGPIO/BlackGPIO.h"
//#include "MPU6050/MPU6050.h"
using namespace std;



void imuInitialize(){

	return;
}


int main(int argc, char *argv[]){


	BlackLib::BlackGPIO left_m0(BlackLib::GPIO_45,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO left_m1(BlackLib::GPIO_23,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO left_m2(BlackLib::GPIO_47,BlackLib::output, BlackLib::FastMode);

	BlackLib::BlackGPIO right_m0(BlackLib::GPIO_27,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO right_m1(BlackLib::GPIO_22,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO right_m2(BlackLib::GPIO_62,BlackLib::output, BlackLib::FastMode);

	BlackLib::BlackGPIO left_en(BlackLib::GPIO_30,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO left_dir(BlackLib::GPIO_48,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO left_step(BlackLib::GPIO_31,BlackLib::output, BlackLib::FastMode);

	BlackLib::BlackGPIO right_en(BlackLib::GPIO_51,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO right_dir(BlackLib::GPIO_60,BlackLib::output, BlackLib::FastMode);
	BlackLib::BlackGPIO right_step(BlackLib::GPIO_50,BlackLib::output, BlackLib::FastMode);

	left_en.setValue(BlackLib::low);
	right_en.setValue(BlackLib::low);
	left_dir.setValue(BlackLib::low);
	right_dir.setValue(BlackLib::low);

	right_m0.setValue(BlackLib::low);
	right_m1.setValue(BlackLib::low);
	right_m2.setValue(BlackLib::low);

	left_m0.setValue(BlackLib::low);
	left_m1.setValue(BlackLib::low);
	left_m2.setValue(BlackLib::low);


	for(int i=0; i<10000; i++){


		left_step.setValue(BlackLib::low);
		right_step.setValue(BlackLib::low);
		usleep(51);
		left_step.setValue(BlackLib::high);
		right_step.setValue(BlackLib::high);
		usleep(51);
	}

    usleep(100);
	left_en.setValue(BlackLib::high);
	right_en.setValue(BlackLib::high);


	return 0;
}

