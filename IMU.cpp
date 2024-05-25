#include "IMU.h"
static Thread IMUloop;
I2C * imu::C;
rtos::Semaphore imu::ticsema(0);
mbed::Ticker imu::tic;
char imu::cmd[2]={0x00,0x00};
const char imu::gyro=(0x14);
const char imu::ndof=(0x20);
char imu::data[8];//={0,0,0,0,0,0};

int16_t imu::accelX; 
int16_t imu::accelY;
int16_t imu::accelZ;
int16_t imu::gyrorate[3];
int16_t imu::quat[4];

imu::imu(I2C &com, int mode) //: com(PH_8,PH_7)
{
    C=&com;
    _mode=mode;
    char reg = 0x00;
    char chipID[1];
    C->write(IMUadd8, &reg, 1, true);
    ThisThread::sleep_for(std::chrono::milliseconds(1000) );
    C->read(IMUadd8, chipID, 1, false);    
    
    //set to config mode
    cmd[0]= 0x3D;
    cmd[1]= 0x00;
    C->write(IMUadd8,cmd,2,false);
    ThisThread::sleep_for(std::chrono::milliseconds(7));

    //Use internal osc
    cmd[0] = 0x3F;
    cmd[1] = 0x80;
    C->write(IMUadd8,cmd,2, false);
    ThisThread::sleep_for(std::chrono::milliseconds(20) );

    // Reset all interrupt status bits
    cmd[0] = 0x3F;
    cmd[1] = 0x01;
    C->write(IMUadd8, cmd, 2, false);

    // Configure Power Mode
    cmd[0] = 0x3E;
    cmd[1] = 0x00;
    C->write(IMUadd8, cmd, 2, false);
    ThisThread::sleep_for(std::chrono::milliseconds(20) );

    // gyro: mode=0.
    if(_mode==rate)
    {
        //change to page 1
        cmd[0] = 0x07;
        cmd[1] = 0x01;
        C->write(IMUadd8,cmd,2,false);
        ThisThread::sleep_for(std::chrono::milliseconds(100) );    

        //configure gyro
        cmd[0] = 0x0A;
        cmd[1] = 0b00111100; //0X3C, 2 Hz Bandwidth, 125 dps res=125/2^15 ?
        C->write(IMUadd8,cmd,2,false);
        ThisThread::sleep_for(std::chrono::milliseconds(100) );
        
        //change to page 0
        cmd[0] = 0x07;
        cmd[1] = 0x00;
        C->write(IMUadd8,cmd,2,false);
        ThisThread::sleep_for(std::chrono::milliseconds(100) );  

        //enable  gyro only mode
        cmd[0] = 0x3D;
        //cmd[1] = 0x0C; //NDOF
        //cmd[1] = 0x01; //ACCEL Only
        
        cmd[1] = 0x03; //gyro only
        C->write(IMUadd8, cmd, 2, false);
        ThisThread::sleep_for(std::chrono::milliseconds(100) );    
    }

    else if(_mode==NDOF)
    {
        cmd[0] = 0x3D;
        cmd[1] = 0x0C; //NDOF
        //cmd[1] = 0x01; //ACCEL Only        
        //cmd[1] = 0x03; //gyro only
        C->write(IMUadd8, cmd, 2, false);
        ThisThread::sleep_for(std::chrono::milliseconds(100) );    
    }      
}

imu::~imu()
{


}

void imu::start()
{
    //IMUloop.start(callback(loop));
    if(_mode==rate)
    {
        IMUloop.start(gyroloop);
        tic.attach(ticloop,std::chrono::milliseconds(100));        
    }
    else if(_mode==NDOF)
    {
        IMUloop.start(NDOFloop);
        tic.attach(ticloop,std::chrono::milliseconds(100));
    }
}

 void imu::gyroloop()
 {     
     while(1)
     {  
        ticsema.acquire();       
        //char reg = 0x08;//acceldata
        //char reg = 0x14;//gyrodata        
        //char data[6];
      
        C->write(IMUadd8, &gyro, 1, true);
        ThisThread::sleep_for(std::chrono::milliseconds(10) );
        C->read(IMUadd8, data, 6, false);
  
        gyrorate[0]=((data[1]<<8) | data[0]);
        gyrorate[1]=((data[3]<<8) | data[2]);
        gyrorate[2]=((data[5]<<8) | data[4]);            

        char buff[50];
        sprintf(buff,"%d,%d,%d,\n",gyrorate[0],gyrorate[1],gyrorate[2]);
        Serial.print(buff);
        //ThisThread::sleep_for(std::chrono::milliseconds(100) );        
     }     
 }


 void imu::NDOFloop()
 {
    while(1)
    {
        ticsema.acquire();
        C->write(IMUadd8, &ndof, 1, true);
        ThisThread::sleep_for(std::chrono::milliseconds(10) );
        C->read(IMUadd8, data, 8, false);    
                  
        quat[0]=((data[1]<<8) | data[0]);
        quat[1]=((data[3]<<8) | data[2]);
        quat[2]=((data[5]<<8) | data[4]); 
        quat[3]=((data[7]<<8) | data[6]);  
       
        for(int i=0;i<4;i++)
        {
            Serial.print(quat[i]);
            Serial.print(',');
            if(i==3)
            {            
                Serial.println();
            }
        }        //ThisThread::sleep_for(std::chrono::milliseconds(100) ); 
 
    }
 }

 void imu::ticloop()
 {
    ticsema.release();
 }

 void imu::get()
 {

 }