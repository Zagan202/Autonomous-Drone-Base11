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

Ultrasonic ultrasonic1(13,12); //X sensor
Ultrasonic ultrasonic2(11,10); //Y Sensor
Ultrasonic ultrasonic3(9,8); //Z sensor

#define LSM9DS1_M  0x1E
#define LSM9DS1_AG  0x6B
//define PRINT_CALCULATED 
#define PRINT_RAW
//define PRINT_SPEED 250 
#define DECLINATION -12 

bool isLevel(float pitch, float roll);
int quadCheck(bool level, Ultrasonic us1, Ultrasonic us2);
int obstacleCheck(Ultrasonic us3);

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

//Testing isLevel function
bool Leveled = isLevel(delta_pitch,delta_roll);

int obs = obstacleCheck(ultrasonic3);

if (quadCheck(Leveled,ultrasonic1,ultrasonic2)>0){
    switch (quadCheck(Leveled,ultrasonic1,ultrasonic2)){
        case 1:
          Serial.println("Quadrant 1");
          if(obs>0){
          Serial.print(" Obstacle: ");Serial.println(obs);
          }
          Serial.println("-------------------");
          break;
        case 2:
          Serial.println("Quadrant 2");
          if(obs>0){
          Serial.print("Obstacle: ");Serial.println(obs); 
          }
          Serial.println("-------------------");
          break;
        case 3:
           Serial.println("Quadrant 3");
           if(obs>0){
           Serial.print("Obstacle: ");Serial.println(obs); 
           }
           Serial.println("-------------------");
           break;
        case 4:
           Serial.println("Quadrant 4");
           if(obs>0){
           Serial.print("Obstacle: ");Serial.println(obs);
           }
           Serial.println("-------------------"); 
           break;
        default: 
          // if nothing else matches, do the default
          // default is optional
        break;
      }
      delay(150);
    } 
}

bool isLevel(float pitch, float roll){
  if(pitch>=-10 && pitch<=10){
     if(roll>=-10 && roll<=10){
      return true;
      }
      else
      {
        return false;
      }
    }
    else return false;
}

int quadCheck(bool level, Ultrasonic us1, Ultrasonic us2){
  if(level){
    if(us1.Ranging(CM)<= 304.8 && us2.Ranging(CM)>= 304.8)
    { //You are in the grid 
      if(us1.Ranging(CM)<= 152.4){ //You are in left-half of grid
        if(us2.Ranging(CM) <= 457.2){ //Y is reading from bottom wall 457.2 is 15 ft
            //Serial.println("Quadrant 3");
            return 3;
            delay(100);}
        else if(us2.Ranging(CM)<= 609.6){ //Y reading from bottom wall and 609.6 is 20 ft
            //Serial.println("Quadrant 2");
            return 2;
            delay(100);}     
      }
    else if(us1.Ranging(CM) > 152.4){ //You are in right-half of grid
          if(us2.Ranging(CM) <= 457.2){ //Y reading distance 15 ft from bottom
              //Serial.println("Quadrant 4");
              return 4;
              delay(100);}
          else if(us2.Ranging(CM)<= 609.6){ //Y reading distance 20 ft from bottom half
                    //Serial.println("Quadrant 1");
                    return 1;
                    delay(100);}       
      }
    }
  }
  else{
      return 0;  
      }
}

int obstacleCheck(Ultrasonic us3){
  if(us3.Ranging(CM)>= 153 && us3.Ranging(CM)<= 198){
  //Serial.println("Obstacle 1");
  return 1;
  delay(100);
  }
else if(us3.Ranging(CM)>= 107 && us3.Ranging(CM)<= 152){
  //Serial.println("Obstacle 2");
  return 2;
  delay(100);
  }
else if(us3.Ranging(CM)>= 60 && us3.Ranging(CM)<= 106){
  //Serial.println("Obstacle 3");
  return 3;
  delay(100);
  }
else{
  return 0;
  }
}


//Test Code Below DONT UNCOMMENT
/*
if(isLevel(delta_pitch,delta_roll)){
  Serial.println("We are level!");
  }
  else{
    Serial.println("We are NOT level");
    }
*/
//Testing quadCheck function
/*
 if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==1){
    Serial.println("Quadrant 1"); 
    Serial.println(quadCheck(Leveled,ultrasonic1,ultrasonic2));

  }
 else if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==2){
    Serial.println("Quadrant 2");
    Serial.println(quadCheck(Leveled,ultrasonic1,ultrasonic2));

  }
  else if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==3){
    Serial.println("Quadrant 3");
    Serial.println(quadCheck(Leveled,ultrasonic1,ultrasonic2));

  }
  else if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==4){
    Serial.println("Quadrant 4");
    Serial.println(quadCheck(Leveled,ultrasonic1,ultrasonic2));

  }
  else if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==0){
    Serial.println("No Quadrant");
  }
  */
  
/*
if(ultrasonic1.Ranging(CM)<= 304.8 && ultrasonic2.Ranging(CM)>= 304.8){ //You are in the grid 
  if(ultrasonic1.Ranging(CM)<= 152.4){ //You are in left-half of grid
    if(ultrasonic2.Ranging(CM) <= 457.2){ //Y is reading from bottom wall 457.2 is 15 ft
     Serial.println("Quadrant 3");
     delay(100); 
    }
    else if(ultrasonic2.Ranging(CM)<= 609.6){ //Y reading from bottom wall and 609.6 is 20 ft
     Serial.println("Quadrant 2");
     delay(100);
    }     
  }
  else if(ultrasonic1.Ranging(CM) > 152.4){ //You are in right-half of grid
    if(ultrasonic2.Ranging(CM) <= 457.2){ //Y reading distance 15 ft from bottom
      Serial.println("Quadrant 4");
      delay(100);  
    }
    else if(ultrasonic2.Ranging(CM)<= 609.6){ //Y reading distance 20 ft from bottom half
      Serial.println("Quadrant 1");
      delay(100);
    }       
  }
}


if(ultrasonic3.Ranging(CM)>= 45 && ultrasonic3.Ranging(CM)<= 46){
  Serial.println("Obstacle 1");
  delay(100);
  }
else if(ultrasonic3.Ranging(CM)>= 91 && ultrasonic3.Ranging(CM)<= 92){
  Serial.println("Obstacle 2");
  delay(100);
  }
else if(ultrasonic3.Ranging(CM)>= 137 && ultrasonic3.Ranging(CM)<= 138){
  Serial.println("Obstacle 3");
  delay(100);
  }
}

*/
/*
if(quadCheck(Leveled,ultrasonic1,ultrasonic2)==0){
    Serial.println("NOT in any quadrant");
    delay(300);
  }
if (quadCheck(Leveled,ultrasonic1,ultrasonic2)>= 1 || quadCheck(Leveled,ultrasonic1,ultrasonic2)<= 4){
      
      Serial.print("Were in quadrant ");Serial.print(quadCheck(Leveled,ultrasonic1,ultrasonic2));
          if (obstacleCheck(ultrasonic3)== 0){
              Serial.println(" I dont see an obstacle");
                  delay(300);
            }
           else if(obstacleCheck(ultrasonic3)>= 1 || obstacleCheck(ultrasonic3)<= 3){
              Serial.print(" I see obstacle ");Serial.println(obstacleCheck(ultrasonic3));
              delay(300);
            }
    }
*/
