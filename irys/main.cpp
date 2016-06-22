#include "mbed.h"
#include "math.h"
#include "Stepper.h"
#include "MPU6050.h"
#include "PID.h"

//------------------------------------
// Terminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------
//      PINOUT DESCRIPTION
// MOTORS OUTPUT 
// LEFT: EN-A1 STEP-A0 DIR-D13
// RIGHT: EN-D12 STEP-D11 DIR-D10
//
// IMU MPU MPU6050
// SDA-D4 SCL-D5
//
// DISTANCE SENSORS
// D6, D3
//
// ESP8266 WIFI MODULE
// TX-D1 RX-D2 EN-D7
//
// WS2812B LED 
// D2 - digital output
// -----------------------------------

Serial pc(SERIAL_TX, SERIAL_RX);
stepper left(A1,A0,D13); 
stepper right(D12,D11,D10); 
DigitalOut led1(LED1);
DigitalOut wifiEnable(D7); 
AnalogIn shfront(D6); 
MPU6050 imu; 
Ticker loop;
PID controll(2.0,1.25,0.25,0.1);

int i; 
float pitch;
float roll;
float frontdistance; 
double speed; 

void pid_thread() {
    
        imu.complementaryFilter(&pitch, &roll);
        controll.setProcessValue(pitch); 
        speed = controll.compute(); 
        //pc.printf("%f\r\n",pitch); 
        pc.printf("%f\r\n",speed); 
        left.step(speed); 
        right.step(-speed);    
}

int main() {
    
 
    wifiEnable = 1; 
    imu.init();   
    imu.calibrate(&pitch,&roll);
    controll.setMode(AUTO_MODE); 
    loop.attach(pid_thread,0.05); 
    controll.setSetPoint(0.0);

    while(1) { 
    
        
        /*frontdistance = shfront.read(); 
        left.enable(); 
        right.enable();
        if (frontdistance > 0.75f){
    
            wait(2);
            for(i=300; i<2400; i+=10){
                        left.step(-i); 
                        right.step(i);
                        wait(0.01); 
            }
            wait(2);
            i =800; 
            left.step(i); 
            right.step(-i);
            wait(0.5);
            for(i=800; i<2000; i+=10){
                        left.step(i); 
                        right.step(-i);
                        wait(0.01); 
            }
            wait(2);

        }
        else{
            left.disable(); 
            right.disable();
        }
        //pc.printf("%f\r\n",frontdistance);*/
  }
}
 