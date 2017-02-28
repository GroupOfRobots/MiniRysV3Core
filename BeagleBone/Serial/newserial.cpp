#include <iostream>
#include "serial.h"
#include "newserial.h"

using namespace std;


const string Element::getstring(){ 
	
	return current->first;
}

int Element::getint(){
	
	return current->second;
}

int Status::set(const string input){

    current = value.find(input);
    if(current == value.end())return 1;

    return 0;
}


int Parity::set(const string input){

    current = value.find(input);
    if(current == value.end())return 1;

    return 0;
}

void Parity::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

int BaudRate::set(const string input){

    current=value.find(input);
    if(current == value.end())return 1;

    return 0;

}

void BaudRate::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

int StopBits::set(const string input){
	
     current=value.find(input);
     if(current==value.end())return 1;

     return 0;
}

void StopBits::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

int DataBits::set(const string input){
	
    current=value.find(input);
    if(current==value.end())return 1;

    return 0;
}

void DataBits::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

int FlowControl::set(const string input){
    
    current=value.find(input);
    if(current==value.end())return 1;

    return 0;
}

void FlowControl::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

int PortName::set(const string input){
    
    current=value.find(input);
    if(current==value.end())return 1;

    return 0;
}

void PortName::showoptions(){

    for(current=value.begin(); current!=value.end(); ++current){

        cout<<current->first<<endl;
    }
    cout<<endl;
}

SerialPort::SerialPort(const string baud, const string par, const string stop, const string data, const string port, const string flow){
	
    baudrate.set(baud);
    parity.set(par);
    stopbits.set(stop);
    databits.set(data);
    portname.set(port);
    flowcontrol.set(flow);
    status.set("Disconnected");

}

SerialPort::~SerialPort(){

    this->disconnect();
}

Status SerialPort::connect(){ //functions from rs232.c

	 int baudr,
	        status;

	    if((comport_number>37)||(comport_number<0)){

	        perror("illegal port number\n");
	        return(1);
	    }

	    switch(baudrate){

	        case      50 : baudr = B50;
	                       break;
	        case      75 : baudr = B75;
	                       break;
	        case     110 : baudr = B110;
	                       break;
	        case     134 : baudr = B134;
	                       break;
	        case     150 : baudr = B150;
	                       break;
	        case     200 : baudr = B200;
	                       break;
	        case     300 : baudr = B300;
	                       break;
	        case     600 : baudr = B600;
	                       break;
	        case    1200 : baudr = B1200;
	                       break;
	        case    1800 : baudr = B1800;
	                       break;
	        case    2400 : baudr = B2400;
	                       break;
	        case    4800 : baudr = B4800;
	                       break;
	        case    9600 : baudr = B9600;
	                       break;
	        case   19200 : baudr = B19200;
	                       break;
	        case   38400 : baudr = B38400;
	                       break;
	        case   57600 : baudr = B57600;
	                       break;
	        case  115200 : baudr = B115200;
	                       break;
	        case  230400 : baudr = B230400;
	                       break;
	        case  460800 : baudr = B460800;
	                       break;
	        case  500000 : baudr = B500000;
	                       break;
	        case  576000 : baudr = B576000;
	                       break;
	        case  921600 : baudr = B921600;
	                       break;
	        case 1000000 : baudr = B1000000;
	                       break;
	        case 1152000 : baudr = B1152000;
	                       break;
	        case 1500000 : baudr = B1500000;
	                       break;
	        case 2000000 : baudr = B2000000;
	                       break;
	        case 2500000 : baudr = B2500000;
	                       break;
	        case 3000000 : baudr = B3000000;
	                       break;
	        case 3500000 : baudr = B3500000;
	                       break;
	        case 4000000 : baudr = B4000000;
	                       break;
	        default      : perror("invalid baudrate\n");
	                       return(1);
	                       break;
	  }

	   int cbits=CS8,
	       cpar=0,
	       ipar=IGNPAR,
	       bstop=0;


	   switch(databits){

	     case 8 : cbits = CS8;
	              break;
	     case 7 : cbits = CS7;
	              break;
	     case 6 : cbits = CS6;
	              break;
	     case 5 : cbits = CS5;
	              break;
	     default :perror("invalid number of data-bits '%d'\n", databits);
	              return(1);
	              break;
	   }

	   switch(parity){

	     case 0 : cpar = 0;
	              ipar = IGNPAR;
	              break;
	     case 1 : cpar = PARENB;
	              ipar = INPCK;
	              break;
	     case 2 : cpar = (PARENB | PARODD);
	              ipar = INPCK;
	              break;
	     default :perror("invalid parity '%c'\n", parity);
	              return(1);
	              break;
	    }

	    switch(stopbits)
	    {
	        case 0 : bstop = 0;
	                 break;
	        case 1 : bstop = CSTOPB;
	                 break;
	        default :perror("invalid number of stop bits '%c'\n", stopbits);
	                return(1);
	                break;
	    }

	/*
	http://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html

	http://man7.org/linux/man-pages/man3/termios.3.html
	*/

	    Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
	    if(Cport[comport_number]==-1){

	        perror("unable to open comport ");
	        return(1);
	    }

	    /* lock access so that another process can't also use the port */
	    if(flock(Cport[comport_number], LOCK_EX | LOCK_NB) != 0){

	        close(Cport[comport_number]);
	        perror("Another process has locked the comport.");
	        return(1);
	    }

	    error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
	    if(error==-1){

	        close(Cport[comport_number]);
	        perror("unable to read portsettings ");
	        return(1);
	    }

	    memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

	    new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
	    new_port_settings.c_iflag = ipar;
	    new_port_settings.c_oflag = 0;
	    new_port_settings.c_lflag = 0;
	    new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
	    new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

	    cfsetispeed(&new_port_settings, baudr);
	    cfsetospeed(&new_port_settings, baudr);

	    error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
	    if(error==-1){

	        close(Cport[comport_number]);
	        perror("unable to adjust portsettings ");
	        return(1);
	    }

	    if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1){

	        perror("unable to get portstatus");
	        return(1);
	    }

	    if(flowcontrol){

	        status |= TIOCM_DTR;    /* turn on DTR */
	        status |= TIOCM_RTS;    /* turn on RTS */
	    }

	    if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1){

	        perror("unable to set portstatus");
	        return(1);
	    }

	    return(0);

    return status;
}

Status SerialPort::disconnect(){

	int status;

	    if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1){

	        perror("unable to get portstatus");
	    }

	    status &= ~TIOCM_DTR;    /* turn off DTR */
	    status &= ~TIOCM_RTS;    /* turn off RTS */

	    if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1){

	        perror("unable to set portstatus");
	    }

	    tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
	    close(Cport[comport_number]);

	    flock(Cport[comport_number], LOCK_UN); /* free the port so that others can use it. */
}

int SerialPort::receive(unsigned char * buffer){

	 int n;

	    n = read(Cport[comport_number], buf, size);

	    return(n);
}

int SerialPort::send(unsigned char * buffer){

    int n;

    n = write(Cport[comport_number], &byte, 1);
    if(n<0)  return(1);

    return(0);
}

void SerialPort::flush(){

    tcflush(Cport[comport_number], TCIOFLUSH);
}

