#include "mbed.h"
#include "math.h"
#include "GY80.h"
#include "Stepper.h"
#include "KalmanFilter.h"


//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------
// PC_10,PC_11 - Enable Motors Drivers
// SDA SCL - IMU I2C
// PB_1, PB_2 - Left motor Step/Dir
// PB_13, PB_14 - Right motor Step/Dir
// -----------------------------------

Serial pc(SERIAL_TX, SERIAL_RX);
GY80 accel;
stepper left(PC_10,PB_1,PB_2); 
stepper right(PC_11,PB_13,PB_14); 
Ticker IMU_Read; 
KalmanFilter angle; 
DigitalOut led1(LED1);
DigitalIn mybutton(USER_BUTTON);

float acc[3]; 
float gyr[3]; 
int i; 
float pitch;
float roll;

void Read_IMU(){
    accel.Read_Accel(acc);
    accel.Read_Gyro(gyr); 
    roll = ((atan2(acc[1],acc[2]))*180.0)/PI;
    roll-=95;
    angle.EstimateAngle(roll,gyr[3],0.01);
}
 
int main() {
    
    left.enable(); 
    right.enable(); 
    IMU_Read.attach(&Read_IMU,0.01); 
    
    while(1) { 
    
        
        //pc.printf("X %f Y %f Z %f \n \r",acc[0],acc[1],acc[2]); 
        led1 = !led1;
        wait(0.2);
        right.step(-(angle.getEstimatedAngle()*15));
        left.step(angle.getEstimatedAngle()*15); 
        pc.printf("roll= %f \n \r",angle.getEstimatedAngle()); 
        //if (mybutton == 0) { // Button is pressed
            //i++; 
        //    pc.printf("Speed: %d\n \r",i); 
            //left.step(0,i);
            //right.step(1,i);  
        //}
  }
}
 