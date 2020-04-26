/*
 * gyro scope orientation KRSBI RSCUAD 
 * AHMAD DAHLAN UNIVERSITY INDONESA
 * Author : JIhad Rahmawan
 * Develop :Danu Andrean
 */

//============== i2c inisialisasi======================= 
#include <Wire.h>


//===============DAC module=========================
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
#define DAC_RESOLUTION    (9)
//===============gyro module=========================
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 mpu;
bool dmpReady = false;  
uint8_t mpuIntStatus;  
uint8_t devStatus;     
uint16_t packetSize;   
uint16_t fifoCount;     
uint8_t fifoBuffer[64]; 
// orientation
Quaternion q;           // [w, x, y, z]                   
VectorFloat gravity;    // [x, y, z]           
float euler[3];         // [psi, theta, phi]    
float ypr[3];           // [yaw, pitch, roll]  
int yaws;
int yaw;
int zero_offset_yaw;

volatile bool mpuInterrupt = false; 
bool startsend=false;    
void dmpDataReady() {
    mpuInterrupt = true;   
}

//===============led module=========================
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN            3
#define NUMPIXELS      16
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int y,limit;
int k,yfrist,ysecond, plus;
int j=0;
float vol;
uint32_t notsend;
bool start;
void setup() {
   Serial.begin(9600);
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(255,0,0));
    pixels.show();
  // addres dac 
  dac.begin(0x62);


   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock.
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    mpu.initialize();
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); 
    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
       
    }
  
    k=1;
    j=0;
   dac.setVoltage(0, false); 
   start=false;
}


void (* reset) (void)= 0;
void loop() {
  if (!dmpReady) return;
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
        
            while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
            k++;
            if(k>10)k=1;
            mpu.getFIFOBytes(fifoBuffer, packetSize);
            fifoCount -= packetSize;
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            y=ypr[0]* 180/M_PI;
            
            if(k==2){
              yfrist=y;
              ysecond=ysecond;
              if(!start){
               pixels.setPixelColor(0, pixels.Color(150,75,0));
               pixels.show();}
            } else if(k==9){
              ysecond=y;
              yfrist=yfrist;
              if(!start){
              pixels.setPixelColor(0, pixels.Color(75,0,75));
              pixels.show();}
            }
               
              
            if(yfrist==ysecond){
               plus++;

              /*if(plus>60){
                plus=48;
               }*/
               Serial.print("plus = ");
              Serial.println(plus);
               if(plus==45){ //setelah falid take nilai limit untuk mencari nilia zero point
                limit=y;
                start=true;
                Serial.println("done");
                Serial.println("\n");
               }
            }
            if(start){
              if(limit>=0){
              yaw=y-limit;
              }else{
                yaw=y+limit;
              }
             if(yaw<0){
                yaws=360+yaw;
                 
              }else{
                yaws=yaw;
              }
              if((yaws<=10 && yaws>=0) || (yaws<=360 && yaws>=350))
              {
                  pixels.setPixelColor(0, pixels.Color(0,0,100));
                  pixels.show();}
              else
                  {
                  pixels.setPixelColor(0, pixels.Color(100,0,0));
                  pixels.show(); 
                  }   
                   vol=yaws*3.3;
                   dac.setVoltage(vol, false); 
            }
            else{  
             if (ysecond < 0 ) {
               pixels.setPixelColor(0, pixels.Color(0,100,0));
               pixels.show();
               //Serial.print("reset \n");
               delay (800);
                reset();
                
              }
              dac.setVoltage(0, false); 
            }
            delay(50);
            //Serial.print("yaws \t");
            Serial.print(yaws); 
            Serial.print("\n");   
             
           delay(100);
           
            
    }
 
}
