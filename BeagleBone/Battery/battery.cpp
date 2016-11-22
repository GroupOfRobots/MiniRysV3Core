#include "battery.h"

void Battery::readState(){

	fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0",std::ifstream::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[0] = atoi(buffer);

	fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device1",std::ifstream::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[1] = atoi(buffer);

	fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device2",std::ifstream::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[2] = atoi(buffer);

}
int Battery::isGood(){

	this->readState();
	if(cellVoltage[0]<1230||cellVoltage[1]<1230||cellVoltage[2]<1230)return 0;
	else return 1;
}
int Battery::getState(){

	//calculate battery state in %;
	return 0;
}
