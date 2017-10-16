
/*Keypad controll example


#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <unistd.h>

#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>


#include "../Motors/motors.h" //motors class
#include "../Battery/battery.h" //Battery measurement class
#include "../Sonar/sonar.h" //distance sensors class


Motors silniki;
Battery lipol;
Sonar hcr;
int speedleft;
int speedright;

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

int main(void)
{

	silniki.disable();

	//if(!lipol.isGood())printf("niski poziom napiecia baterii");

	char c;
	int exit=1;

	while(exit){
		
		
		switch (c){
		case 'w':
			if(speedleft<0)speedleft = 0;
			speedleft+=5;
			if(speedright<0)speedright = 0;
			speedright+=5;
		break;
		case 'a':
			speedleft+=1;
			speedright-=1;
		break;
		case 's':
			if(speedleft<0)speedleft = 0;
			speedleft-=5;
			if(speedright<0)speedright = 0;
			speedright-=5;
		break;
		case 'd':
			speedleft-=1;
			speedright+=1;
		break;
		case 'q':
			silniki.disable();
			exit =0;
			usleep(10000);
		break;
		case 'r':
			speedleft=0;
			speedright=0;
			silniki.disable();
		break;
		}

		silniki.setSpeed(speedleft,speedright,0.0,1);

		c = getch();
		silniki.enable();
	}
	return 0;
}
 */
