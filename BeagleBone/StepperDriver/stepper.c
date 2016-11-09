/*! \file stepper.c
\brief Example: control a stepper motor.


Compile by: `gcc -Wall -o stepper stepper.c -lpruio`

*/

//! Message for the compiler.
#define _GNU_SOURCE 1
#include "stdio.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include "../c_wrapper/pruio.h"
#include "../c_wrapper/pruio_pins.h"

/*
#define LEN P8_38
#define LSTEP P8_40
#define LDIR P8_39
#define LM0 P8_41
#define LM1 P8_42
#define REN P8_37
#define RSTEP P8_44
#define RDIR P8_43
#define RM0 P8_45
#define RM1 P8_46
*/

#define LEN P9_11
#define LSTEP P9_13
#define LDIR P9_15
#define LM0 P8_11
#define LM1 P8_13
#define REN P9_16
#define RSTEP P9_14
#define RDIR P9_12
#define RM0 P8_17
#define RM1 P8_19



#define SET_PIN(p, val) \
  if (pruio_gpio_setValue(Io, p, val)) {printf("setValue P4 error (%s)\n", Io->Errr); break;}


void setSpeed(int rightspeed, int leftspeed){

	SET_PIN(LDIR, rightspeed); 
	SET_PIN(RDIR, leftspeed);

}

void enableMotors(){

	SET_PIN(LEN, 0); 
	SET_PIN(REN, 0);
}

void disableMotors(){

	SET_PIN(LEN, 1); 
	SET_PIN(REN, 1);
}

void setMicrostep(int m0){

	SET_PIN(RM0, m0); 
	SET_PIN(RM1, m0);
	SET_PIN(LM0, m0); 
	SET_PIN(LM1, m0);
}


//! The main function.
int main(int argc, char **argv)
{
  pruIo *Io = pruio_new(PRUIO_DEF_ACTIVE, 0x98, 0, 1); //! create new driver structure
  do {
    if (Io->Errr) {
               printf("initialisation failed (%s)\n", Io->Errr); break;}

    //                            initialize pin config

    //' pin config OK, transfer local settings to PRU and start PRU driver
    if (pruio_config(Io, 1, 0x1FE, 0, 4)) {
                       printf("config failed (%s)\n", Io->Errr); break;}

	int i; 
	enableMotors(); 
	setMicrostep(0); 
	setSpeed(1,1); 
	for(i=0; i<10000; i++){

		SET_PIN(LSTEP, 0); 
		SET_PIN(RSTEP, 0);
	    	delay(100); 
		SET_PIN(LSTEP, 1); 
		SET_PIN(RSTEP, 1);
	    	delay(100); 
	}
	disableMotors(); 


    printf("\n");
  } while (0);

  pruio_destroy(Io);       /* destroy driver structure */
	return 0;
}
