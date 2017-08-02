#include<Wire.h> 
#include<SPI.h> 
#include<SparkFunLSM9DS1.h>
#include "Ultrasonic.h" 

LSM9DS1 imu;
float AX, AY, AZ, MX, MY, MZ;
double roll, pitch,heading, croll, cpitch, cheading;
int quadColor;
//I commented
//Lawrence Commented
double Obstacle1;

Ultrasonic ultrasonic1(13,12); //X sensor
Ultrasonic ultrasonic2(11,10); //Y Sensor
Ultrasonic ultrasonic3(9,8); //Z sensor

#define LSM9DS1_M  0x1E
#define LSM9DS1_AG  0x6B
//define PRINT_CALCULATED 
#define PRINT_RAW
//define PRINT_SPEED 250 
#define DECLINATION -12 

void setup() {


Serial.begin(9600);
imu.settings.device.commInterface = IMU_MODE_I2C;
imu.settings.device.mAddress = LSM9DS1_M;
imu.settings.device.agAddress = LSM9DS1_AG;
if (!imu.begin())
{
   Serial.println("Failed to communicate with LSM9DS1.");
   Serial.println("Double-check wiring.");
   Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1);
}
//print gyro values
imu.readGyro();
 
//print accel values
imu.readAccel();

//print mag values
imu.readMag();
   


//calculate attitude values
AX = imu.ax;
AY = imu.ay;
AZ = imu.az;
MX = imu.mx;
MY = imu.my;
MZ = imu.mz;

roll = atan2(AY, AZ);
roll *= 180.0 / PI;
pitch = atan2(-AX, sqrt(AY * AY + AZ * AZ));
pitch *= 180.0 / PI;
heading = atan2(MY, MX);
heading -= DECLINATION * PI / 180;
heading *= 180.0 / PI;

Serial.print("Pitch, Roll: ");
Serial.print(pitch, 2);
Serial.print(", ");
Serial.println(roll, 2);
Serial.print("Heading: "); Serial.println(heading, 2);

}
void loop() {

//print gyro values
imu.readGyro();
//print accel values
imu.readAccel();
//print mag values
imu.readMag();

    //calculate attitude values
AX = imu.ax;
AY = imu.ay;
AZ = imu.az;
MX = imu.mx;
MY = imu.my;
MZ = imu.mz;

croll = atan2(AY, AZ);
croll *= 180.0 / PI;
  
cpitch = atan2(-AX, sqrt(AY * AY + AZ * AZ));
cpitch *= 180.0 / PI;

cheading = atan2(MY, MX);
  
cheading -= DECLINATION * PI / 180;
cheading *= 180.0 / PI;
//Recently commented below
/*
Serial.print("cPitch, cRoll: ");
Serial.print(cpitch, 2);
Serial.print(", ");
Serial.println(croll, 2);
Serial.print("cHeading: "); Serial.println(cheading, 2);
*/
float heading1;
float heading2 ;
heading1 = abs(heading);
heading2 = abs(cheading);

float delta_pitch = cpitch - pitch;
//Recently commented below
//Serial.print("delta_pitch: ");Serial.println(delta_pitch);

float delta_roll = croll - roll;
//Recently commented below
//Serial.print("delta_roll: ");Serial.println(delta_roll);

float delta_heading = cheading - heading ;
//Serial.print("delta_heading: ");Serial.println(delta_heading);

//float delta_heading = abs(heading2-heading1);
//Recently commented below
//Serial.print("delta_heading: "); Serial.println(delta_heading);


if(ultrasonic3.Ranging(CM)>= 45 && ultrasonic3.Ranging(CM)<= 46){
  Serial.println("Obstacle 1");
  delay(100);
  }
else if(ultrasonic3.Ranging(CM)>= 91 && ultrasonic3.Ranging(CM)<= 92){
  Serial.println("Obstacle 2");
  delay(100);
  }
else if(ultrasonic3.Ranging(CM)>= 137 && ultrasonic3.Ranging(CM)<= 138){
  Serial.println("Obstacle3");
  delay(100);
  }
}
