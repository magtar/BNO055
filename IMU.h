#pragma once
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
        imu();
        ~imu();
        I2C com; 
        static constexpr int IMUadd=0x28;
        static constexpr int IMUadd8=IMUadd<<1;

    private:


        
};