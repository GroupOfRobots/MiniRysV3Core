#include "Stepper.h"
#include "mbed.h"

stepper::stepper(PinName _en, PinName _stepPin, PinName dir):en(_en),
    stepPin(_stepPin),
    direction(dir)
{
}

void stepper::step( float speed){
    if (speed > 0) {
        direction = 0;
	 toggler.attach(this,&stepper::toggle_step,1/speed); 
    }
    else{
        direction = 1;
	toggler.attach(this,&stepper::toggle_step,1/-(speed)); 
    }
}

void stepper::enable(){
    en = 0;
}

void stepper::disable(){
    en = 1;
}

void stepper::toggle_step (){
    stepPin = !stepPin; 
}
