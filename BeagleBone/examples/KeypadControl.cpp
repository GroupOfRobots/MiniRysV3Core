/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <termios.h>
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

	if(!lipol.isGood())printf("niski poziom napiecia baterii");

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
}*/
