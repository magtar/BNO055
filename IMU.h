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
        char data[2];
        static I2C *C;
        char reg;
        //static char accel [6];  
        void start();
        static void gyroloop();
        static mbed::Ticker tic;
        static void ticloop();
        static Semaphore ticsema;
        

    private:
        enum mode {rate, accel, mag};
        
        
};
#endif