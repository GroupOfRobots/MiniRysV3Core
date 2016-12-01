#include "battery.h"

void Battery::readState(){

	fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_voltage5_raw",std::ios::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[0] = atoi(buffer);

	/*fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_voltage3_raw",std::ios::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[1] = atoi(buffer);

	fs.open("/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_voltage1_raw",std::ios::in);
	if(fs.fail())throw 5;

	fs.read(buffer, 4);
	fs.close();

	cellVoltage[2] = atoi(buffer);
	*/

}
int Battery::isGood(){

	this->readState();
	if(cellVoltage[0]<2500)return 0;
	else return 1;
}
int Battery::getState(){

	//calculate battery state in %;
	return 0;
}

int Battery::getRaw(){
	this->readState();
	return cellVoltage[0];
}
