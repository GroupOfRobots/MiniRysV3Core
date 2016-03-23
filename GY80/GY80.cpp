#include "GY80.h"

Serial pc2(USBTX, USBRX);

GY80::GY80() : Wire(D14,D15)
{
    Wire.frequency(I2C_FREQ);
    Accel_Init();
    Gyro_Init();
    Magn_Init();
}
GY80::~GY80()
{
}
void GY80::Accel_Init()
{    
    byte data[2];
    data[0] = 0x2D; // Power register
    data[1] = 0x08; //Measurement mode
    Wire.write(ACCEL_ADDRESS, data, 2);
    wait_ms(1);

    data[0] = 0x31; // Data format register
    data[1] = 0x08; //Set to full resolution
    Wire.write(ACCEL_ADDRESS, data, 2);
    wait_ms(1);

    // 
    data[0] = 0x2C; // Rate
    data[1] = 0x0D; //Set to 800Hz, normal operation, 0x0A 100hz 
    Wire.write(ACCEL_ADDRESS, data, 2);
    wait_ms(1);
}

void GY80::Gyro_Init()
{
    byte data[2];

    data[0] = 0x20; //L3G4200D_CTRL_REG1
    data[1] = 0xCF; // normal power mode, all axes enable, 8:20 9:25 A:50 B:110 
    Wire.write(GYRO_ADDRESS, data, 2);
    wait_ms(1);


    data[0] = 0x23; // L3G4200D_CTRL_REG4
    data[1] = 0x20; //2000 dps full scale 
    Wire.write(GYRO_ADDRESS, data, 2);
    wait_ms(1);


    data[0] = 0x24; // L3G4200D_CTRL_REG5
    data[1] = 0x02; //Low Pass Filter
    Wire.write(GYRO_ADDRESS, data, 2);
}

void GY80::Magn_Init()
{   
    byte data[2];
    data[0] = 0x02;
    data[1] = 0x00; // 00000000 Set continuous mode (default 10Hz)
    Wire.write(MAGN_ADDRESS, data, 2);
    wait_ms(1);

    data[0] = 0x00;
    data[1] = 0x50; // 01010000
    Wire.write(MAGN_ADDRESS, data, 2);
    wait_ms(1);
}

void GY80::Read_Accel(float* accel_v)
{
    byte buff[6];
    buff[0] = 0x32; // Send address to read from
    Wire.write(ACCEL_ADDRESS, buff, 1);

    int accel[3];
    if (Wire.read(ACCEL_ADDRESS, buff,6) == 0)  // All bytes received?
    {
        accel[0] = (short) ((uint16_t) buff[1] << 8 | buff[0]);
        accel[1] = (short) ((uint16_t) buff[3] << 8 | buff[2]);
        accel[2] = (short) ((uint16_t) buff[5] << 8 | buff[4]);
    }
    accel_v[0] = (accel[0] - ACCEL_X_OFFSET) * ACCEL_X_SCALE;
    accel_v[1] = (accel[1] - ACCEL_Y_OFFSET) * ACCEL_Y_SCALE;
    accel_v[2] = (accel[2] - ACCEL_Z_OFFSET) * ACCEL_Z_SCALE;
}


void GY80::Read_Gyro(float* gyro_v)
{
    byte buff[6];

    buff[0] = 0xA8; // 0x28 | (1 << 7) Send address to read from 
    Wire.write(GYRO_ADDRESS, buff, 1);
    // Request 6 bytes
    int gyro[3];
    if (Wire.read(GYRO_ADDRESS, buff,6) == 0)  // All bytes received?
    {
        gyro[0] = (short) ((uint16_t) buff[1] << 8 | buff[0]);
        gyro[1] = (short) ((uint16_t) buff[3] << 8 | buff[2]);
        gyro[2] = (short) ((uint16_t) buff[5] << 8 | buff[4]);
    }
    gyro_v[0] = DEG2RAD((gyro[0] - GYRO_X_OFFSET) * GYRO_GAIN_X); 
    gyro_v[1] = DEG2RAD((gyro[1] - GYRO_Y_OFFSET) * GYRO_GAIN_Y);
    gyro_v[2] = DEG2RAD((gyro[2] - GYRO_Z_OFFSET) * GYRO_GAIN_Z);
}

void GY80::Read_Magn(float* magn_v)
{
    byte buff[6];

    buff[0] = 0x03; // Send address to read from
    Wire.write(MAGN_ADDRESS, buff, 1);

    // Request 6 bytes
    int mag[3];
    if (Wire.read(MAGN_ADDRESS, buff,6) == 0)  // All bytes received?
    {
        mag[0] = (short) ((uint16_t) buff[1] << 8 | buff[0]);
        mag[1] = (short) ((uint16_t) buff[3] << 8 | buff[2]);
        mag[2] = (short) ((uint16_t) buff[5] << 8 | buff[4]);
    }
    magn_v[0] = (mag[0] - MAGN_X_OFFSET) * MAGN_X_SCALE;
    magn_v[1] = (mag[1] - MAGN_Y_OFFSET) * MAGN_Y_SCALE;
    magn_v[2] = (mag[2] - MAGN_Z_OFFSET) * MAGN_Z_SCALE;
}

