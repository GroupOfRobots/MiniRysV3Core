#ifndef MBED_STEPPER_H
#define MBED_STEPPER_H
#endif

#include "mbed.h"

class stepper{
public:
    stepper(PinName _en, PinName _stepPin, PinName dir);
    void step(float speed);
    void enable();
    void disable();
    void toggle_step();
private:
    DigitalOut en;
    DigitalOut stepPin;
    DigitalOut direction;
    Ticker toggler;
};