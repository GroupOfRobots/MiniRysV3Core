/*
 * EasyDriver.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.eeng.dcu.ie/~molloyd/
 *
 * YouTube Channel: http://www.youtube.com/derekmolloydcu/
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL I
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "EasyDriver.h"
#include <iostream>
#include <unistd.h>
using namespace std;

EasyDriver::EasyDriver(int gpio_MS1, int gpio_MS2, int gpio_STEP, int gpio_SLP,int gpio_DIR){

	this->gpio_MS1  = gpio_MS1;
	this->gpio_MS2  = gpio_MS2;
	this->gpio_STEP = gpio_STEP;
	this->gpio_SLP  = gpio_SLP;
	this->gpio_DIR  = gpio_DIR;

	//gpio_export(this->gpio_MS1);
	//gpio_set_dir(this->gpio_MS1, OUTPUT_PIN);
	//gpio_export(this->gpio_MS2);
	//gpio_set_dir(this->gpio_MS2, OUTPUT_PIN);
	//gpio_export(this->gpio_STEP);
	//gpio_set_dir(this->gpio_STEP, OUTPUT_PIN);
	//gpio_export(this->gpio_SLP);
	//gpio_set_dir(this->gpio_SLP, OUTPUT_PIN);
	//gpio_export(this->gpio_DIR);
	//gpio_set_dir(this->gpio_DIR, OUTPUT_PIN);

	// default to clockwise direction
	clockwise = true;
	// add microstepping
	// the default speed in rpm
	setSpeed(100.0);

	//wake();
}


void EasyDriver::setSpeed(float rpm) {
	this->speed = rpm;
	float delayPerSec = 100/rpm;    // delay per step in seconds
	this->uSecDelay = (int)(delayPerSec * 1000 * 1000); // in microseconds
}




EasyDriver::~EasyDriver() {
	//gpio_unexport(this->gpio_MS1);
	//gpio_unexport(this->gpio_MS2);
	//gpio_unexport(this->gpio_STEP);
	//gpio_unexport(this->gpio_SLP);
	//gpio_unexport(this->gpio_DIR);

}

