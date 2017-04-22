/*
Przykład sterowania silnikami robota na podstawie odczytu odległości z czujnika


#include "../Motors/Motors.h"
#include "../Sonar/sonar.h"

Motors silniki;
Sonar sonary;

int main(void)
{
	int dst=0;

	silniki.disable();

	silniki.setSpeed(150.0,150.0,0.0,4); //Ustawienie prędkości silników
	while(1){
		 dst = sonary.getDistance(distance_sensors::top);//odczyt odległości z sonaru górnego
		 if((dst)<150){
			 silniki.enable();
		 }
		 else silniki.disable();
		 usleep(500);
	}
	return 0;
}

*/
