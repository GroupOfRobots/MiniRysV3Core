#include "Stepper.h"
#include "mbed.h"

stepper::stepper(PinName _en, PinName _stepPin, PinName dir):en(_en),
    stepPin(_stepPin),
    direction(dir)
{
}

void stepper::step(float speed){
    if (speed > 0) {
        direction = 0;
    }
    else{
        direction = 1;
    }
    if(speed==0){
        toggler.detach(); 
        toggler.attach(this,&stepper::toggle_step,0);
    }
    else{
    toggler.detach(); 
    toggler.attach(this,&stepper::toggle_step,1/abs(speed)); 
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