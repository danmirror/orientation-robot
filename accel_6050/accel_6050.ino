//www.nyebarilmu.com
 
#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_PCF8574.h>
 
LiquidCrystal_PCF8574 lcd(0x3F); 
// dapat dicek menggunakan i2c finder
 
MPU6050 mpu;
 
byte degree[8] = {
  0b00000,
  0b00110,
  0b01111,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
 
void setup()
{
  Serial.begin(115200);
 
  lcd.begin(16,2);
  lcd.createChar(00, degree);
  Serial.begin(9600);
  Serial.println("Initialize MPU6050");
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("tidak ada sensor MPU6050 yang terpasang!");
    delay(500);
  }
 
  //Kalibrasi gyrometer
  mpu.calibrateGyro();
  mpu.setThreshold(3);
  
  lcd.clear();
  lcd.print("Module MPU6050");
  lcd.setCursor(00,1);
  lcd.print("nyebarilmu");
  delay(2000);
  lcd.clear();
}
 
void loop()
{
  //untuk pembacaan gyrometer
  Vector rawGyro = mpu.readRawGyro();
 
  Serial.println("Gryometer");
  Serial.print("X : ");
  Serial.print(rawGyro.XAxis);
  Serial.print("Y : ");
  Serial.print(rawGyro.YAxis);
  Serial.print("Z : ");
  Serial.println(rawGyro.ZAxis);
 
  lcd.setCursor(00,00);
  lcd.print(rawGyro.XAxis);
  lcd.setCursor(5,00);
  lcd.print(rawGyro.YAxis);
  lcd.setCursor(10,00);
  lcd.print(rawGyro.ZAxis);
 
  //untuk pembacaan accelerometer
  Vector rawAccel = mpu.readRawAccel();
 
  Serial.println("Accelerometer");
  Serial.print("X : ");
  Serial.print(rawAccel.XAxis);
  Serial.print("Y : ");
  Serial.print(rawAccel.YAxis);
  Serial.print("Z : ");
  Serial.println(rawAccel.ZAxis);
 
  lcd.setCursor(00,1);
  lcd.print(rawAccel.XAxis);
  lcd.setCursor(5,1);
  lcd.print(rawAccel.YAxis);
  lcd.setCursor(10,1);
  lcd.print(rawAccel.ZAxis);
 
  float celc = mpu.readTemperature();
 
  Serial.println("temperature ");
  Serial.print(celc);
  Serial.println(" *C");
 
  lcd.print("Temperature");
  lcd.setCursor(13,1);
  lcd.print(celc);
  lcd.write((byte)00);
 
  delay(500);
}
