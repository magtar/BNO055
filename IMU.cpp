#include "IMU.h"

imu::imu(I2C &com) //: com(PH_8,PH_7)
{
    C=&com;
    char reg = 0x00;
    char chipID[1];
    C->write(IMUadd8, &reg, 1, true);
    ThisThread::sleep_for(std::chrono::milliseconds(1000) );
    C->read(IMUadd8, chipID, 1, false);    
    
    //Use internal osc
    data[0] = 0x3F;
    data[1] = 0x40;
    C->write(IMUadd8,data,2, false);
    ThisThread::sleep_for(std::chrono::milliseconds(20) );

    // Reset all interrupt status bits
    data[0] = 0x3F;
    data[1] = 0x01;
    C->write(IMUadd8, data, 2, false);

    // Configure Power Mode
    data[0] = 0x3E;
    data[1] = 0x00;
    C->write(IMUadd8, data, 2, false);
    ThisThread::sleep_for(std::chrono::milliseconds(20) );

    // Defaul Axis Configuration
    data[0] = 0x41;
    data[1] = 0x24;
    C->write(IMUadd8, data, 2, true);

    // Default Axis Signs
    data[0] = 0x42;
    data[1] = 0x00;
    C->write(IMUadd8, data, 2, true);
    

    // Set units to m/s^2
    data[0] = 0x3B;
    data[1] = 0b0001000;
    C->write(IMUadd8, data, 2, true);
    

    // Set operation to acceleration only
    data[0] = 0x3D;
    data[1] = 0x0C;
    C->write(IMUadd8, data, 2, true);
    ThisThread::sleep_for(std::chrono::milliseconds(100) );    

}

imu::~imu()
{


}

void imu::start()
{
    //IMUloop.start(callback(loop));
    IMUloop.start(loop);
}

 void imu::loop()
 {
     
     while(1)
     {
        Serial.print("IMUloop ");
        char reg = 0x00;
        //char chipID[1];
        C->write(IMUadd8, &reg, 1, true);
        ThisThread::sleep_for(std::chrono::milliseconds(1000) );
        //C->read(IMUadd8, chipID, 1, false);    
        ThisThread::sleep_for(std::chrono::milliseconds(500) ); 
     }     
 }