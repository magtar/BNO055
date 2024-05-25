#ifndef IMU_H
#define IMU_H
#include <mbed.h>
#include <rtos.h>
#include <Arduino.h>
static int IMUadd=0x28;
static int IMUadd8=IMUadd<<1;
using namespace mbed;
using namespace rtos;

class imu
{
    public:
        imu(I2C &com, int);
        ~imu();         
        static constexpr int IMUadd=0x28;
        static constexpr int IMUadd8=IMUadd<<1;   
        static const char gyro;
        static const char ndof;

        static char cmd[2];
        static I2C *C;
        //I2C * C
        char reg;
        //static char accel [6];  
        void start();
        static void gyroloop();
        static void NDOFloop();
        static mbed::Ticker tic;
        static void ticloop();
        static Semaphore ticsema;
        static char data[8];

        static int16_t accelX, accelY, accelZ;
        static int16_t gyrorate[3];
        static int16_t quat[4]; //w,x,y,z order

        void get();
        int _mode;
        

    private:
        enum mode {rate, accel, mag, NDOF};
        
        
};
#endif