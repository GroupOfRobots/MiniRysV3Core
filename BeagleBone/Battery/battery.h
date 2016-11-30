#ifndef _BATTERY_
#define _BATTERY_

#include <fstream>

class Battery{
private:
	std::ifstream fs;
	int cellVoltage[3];
	char buffer[4];

public:

	void readState();
	int isGood();
	int getState();
	int getRaw();
};

#endif

