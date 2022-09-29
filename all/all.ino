/* pins ready for right and left ultrasonic, right and left color sensor, gyro sensor
 * max servo parameter: f 138, R:107 L:160
 */

#include <tcs3200.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include <Servo.h>

// color sensor parameters and pins
tcs3200 rtcs(44, 42, 46, 48, 52);    // pins for RIGHT color sensor  (S0, S1, S2, S3, output pin)  
tcs3200 ltcs(36, 38, 30, 32, 34);    // pins for LEFT color sensor   (S0, S1, S2, S3, output pin)  

int red, green, blue, white;         // variables for RGB and clear color DATA

int ominred = 70, omingreen = 40, ominblue = 50, ominwhite = 120;  // parameter for minimum RGB values in orange color
int omaxred = 201, omaxgreen = 70, omaxblue = 75, omaxwhite = 200;  // parameter for maximum RGB values in orange color

int bminred = 35, bmingreen = 40, bminblue = 70, bminwhite = 120;  // parameter for minimum RGB values in blue color
int bmaxred = 55, bmaxgreen = 85, bmaxblue = 100, bmaxwhite = 170;  // parameter for maximum RGB values in blue color

// ultrasonic sensor parameters and pins
#define fecho 6                      // input echo pin for FORWARD ultrasonic
#define ftrig 7                      // output trig pin for FORWARD ultrasonic

#define recho 11                     // input echo pin for RIGHT ultrasonic
#define rtrig 10                     // output trig pin for RIGHT ultrasonic

#define lecho 9                      // input echo pin for LEFT ultrasonic
#define ltrig 8                      // output trig pin for LEFT ultrasonic

int fdistance,rdistance,ldistance;
int intfdistance,intrdistance,intldistance;
bool intdistance=true;
long duration;

// gyro sensor parameters and pins
MPU6050 mpu(Wire);
unsigned long timer = 0;

int angle=0;                          // variable for angle DATA - Z Axis-

// DC motor parameters and pins
byte dcmotfpin=5;                     // output pin for FORWARD DC driving motor 
byte dcmotbpin=4;                     // output pin for BACKWARD DC driving motor 
int speedo=200;                       // speed for driving dc motor

// stearing motor parameters and pins
//byte stearRmotpin=4;                  
//byte stearLmotpin =5;               
Servo servo;
int pos = 138 ;                       // servo motor intial angle position in forward position
byte servopin = 2;                    // output pwm pin for servo stearing motor



// start button parameters and pins
byte startpin=24;                     // input pin for start push button
byte startsignal;                     // DATA from button - HIGH or LOW-

bool cw=false,ccw=false, intcolor=true;

int cornerlinecount=0;                // count corner line for achivie 3 laps   if counter= 12 then 3 laps              


void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  byte status = mpu.begin();
  //Serial.print(F("MPU6050 status: "));
  //Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  //Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  //Serial.println("Done!\n");
  pinMode(50,OUTPUT);                      // left color sensor enable
  pinMode(28,OUTPUT);                       // right color sensor enable
  digitalWrite(28,LOW);
  digitalWrite(50,LOW);
  pinMode(ftrig, OUTPUT); 
  pinMode(fecho, INPUT);
  pinMode(rtrig, OUTPUT); 
  pinMode(recho, INPUT);
  pinMode(ltrig, OUTPUT); 
  pinMode(lecho, INPUT);
  pinMode(startpin, INPUT);
  pinMode(dcmotfpin, OUTPUT);
  pinMode(dcmotbpin, OUTPUT);
  digitalWrite(dcmotfpin,LOW);
  digitalWrite(dcmotbpin,LOW);
  servo.attach(servopin);
  servo.write(pos);
  intdistance = true;
  readdistance(ftrig, fecho, 'F');
  delay(50);
  readdistance(rtrig, recho, 'R');
  delay(50);
  readdistance(ltrig, lecho, 'L');
  //intdistance = false;
  //readgyro();
  Serial.print("fdis= ");
  Serial.println(fdistance);
  Serial.print("rdis= ");
  Serial.println(rdistance);
  Serial.print("ldis= ");
  Serial.println(ldistance);
  /*while(true)
  {
    startsignal=digitalRead(startpin);
    if(startsignal == HIGH)
    {
      break;
    }
  }*/
   
  
}

void loop() 
{
  // fnS: readgyro(), readRcolor(), readLcolor(), readdistance(trigPin, echoPin, 'F' || 'R' || 'L')
  // fnS: forward(int speedo), backward(int speedo), left(), right()
  //readLcolor();
  /*left();
  delay(1500);
  straight();
  delay(1500);
  right();
  delay(1500);
  straight();*/
  /*left();
  readgyro();
  Serial.println(angle);
  delay(1000);
  forward(200);
  while(true)
   {
    readgyro();
    if(angle > 87)
    {
      stopdriving();
      Serial.println(angle);
      Serial.println("more than 87 deg");
    }
    readdistance(rtrig, recho, 'R');
    Serial.print(rdistance);
    if(rdistance < 20)
    {
      stopdriving();
      delay(50);
      right();
      backward(200);
      delay(2000);
      stopdriving();
      delay(50);
      left();
      forward(200);
    }
    //break;

   }
  while(true)
  {
    
  }*/
  /*forward(250);
  delay(1000);
  right();
  forward(250);
  delay(800);
  straight();
  forward(250);
  delay(700);
  forward(230);
  delay(1500);
  straight();
  //stopdriving();
  delay(1500);
  forward(250);
  left();
  delay(1500);
  straight();
  stopdriving();
  while(true)
  {
    
  }*/
  // #########################################################
  forward(220);
  readdistance(ftrig, fecho, 'F');
  if(fdistance <=30)
  {
    stopdriving();
  }
  if(fdistance <= 105 && fdistance >33)
  {
    //left();
    while(true)
    {
      forward(175);
      Serial.println(fdistance);
      if ( intcolor == true)
      {
        readRcolor();
        readLcolor();
      }
      if(cw == true)         // turn right
      {
        Serial.println("CW");
        cornerlinecount=cornerlinecount+1;
        if(cornerlinecount == 4)
        {
          stopdriving();
        }
        right();
        //delay(1000);
        //stopdriving();
        while(true)
        {
          readul();
          readgyro();
          Serial.println(fdistance);
          Serial.println(ldistance);
          Serial.println("     ");
          if(fdistance < 20 || angle < -65)
          {
            straight();
            //stopdriving();
            Serial.println(angle);
            break;
          }
        }
        //break;
      }
      else if( ccw == true)    // turn left
      {
        Serial.println("CCW");
        left();
        delay(1000);
        stopdriving();
        //break;
        while(true)
        {
          
        }
        /*while(true)
        {
          forward(250);
          left();
          delay(350);
          straight();
          delay(350);
          left();
          readdistance(ftrig, fecho, 'F');
          readdistance(ltrig, lecho, 'L');
          readgyro();
          if( angle >= 90)
          {
            stopdriving();
          }
          if( fdistance <= 45)
          {
            stopdriving();
            straight();
            delay(50);
            backward(250);
            delay(1200);
            right();
            delay(1200);
            stopdriving();
            forward(250);
            delay(800);
            left();
            if( angle >= 90)
            {
              stopdriving();
            }
          }
          Serial.println(angle);
          Serial.println(fdistance);
          Serial.println(ldistance);
          Serial.println("    ");
          delay(3000);
          stopdriving();
            while(true)
            {
              
            }*/
            
        }
        break;
        
      }
    }
    
    
  

 

  /*if( ccw == true )
  {
    angle=0;
    left();
    
  }

  if( cw == true )
  {
    right();
    digitalWrite(2,LOW);
    while(true)
    {
        
    }
  }*/

  // ######################################################

  
 
 /* readRcolor();
  delay(650);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  while (true)
  {
    
  }*/
  /*readdistance(ftrig, fecho, 'F');
  Serial.println(fdistance);
  readdistance(rtrig, recho, 'R');
  Serial.println(rdistance);
  readdistance(ltrig, lecho, 'L');
  Serial.println(ldistance);
  Serial.println("    ");*/
  //readLcolor();
  //readRcolor();
  /*while( fdistance < 100 )
  {
    forward(150);
    readRcolor();
    readLcolor();
    if( cw == true || ccw == true)
    {
      break;
    }
    //else
    //search for blue or orange line
  }*/

  
  //delay(400);
  

  /*while(true)
  {
    
  }*/
  
  

}

void straight()
{
  pos= 138;
  servo.write(pos);
}

void left()
{
  pos=175;
  servo.write(pos);
}

void right()
{
  pos=107;
  servo.write(pos);
}

void stopdriving()
{
  digitalWrite(dcmotfpin,LOW);
  digitalWrite(dcmotbpin,LOW);
}

void backward(int speedo)
{
  analogWrite(dcmotbpin,speedo);
  digitalWrite(dcmotfpin,LOW);
}

void forward(int speedo)
{
  //digitalWrite(dcmotfpin,HIGH); // moving FORWARD
  analogWrite(dcmotfpin,speedo);
  digitalWrite(dcmotbpin,LOW);
  //delay(500);
  //digitalWrite(dcmotfpin,LOW);
  //digitalWrite(dcmotbpin,LOW);
}


void readgyro()
{
  mpu.update();
  
  if((millis()-timer)>10)
  { 
    angle=mpu.getAngleZ();
    timer = millis();  
  }
}

void readRcolor()
{
  red = rtcs.colorRead('r');
  /*Serial.print("R= ");
  Serial.print(red);
  Serial.print("        ");
  //delay(5);*/
  green = rtcs.colorRead('g');
  /*Serial.print("G= ");
  Serial.print(green);
  Serial.print("        ");
  //delay(5);*/
  blue = rtcs.colorRead('b');
  /*Serial.print("B= ");
  Serial.print(blue);
  Serial.print("        ");
  //delay(5);*/
  white = rtcs.colorRead('c');
  /*Serial.print("c= ");
  Serial.println(white);
  //delay(5);*/
  if(ominred <= red && red <= omaxred && omingreen <= green && green <= omaxgreen && ominblue <= blue && blue <= omaxblue)
  {
    Serial.println("orange line detected. clock wise direction");
    cw=true;
    ccw=false;
    cornerlinecount++;
    intcolor=false;
  }
  else
  Serial.println("go forward");

}

void readLcolor()
{
  red = ltcs.colorRead('r');
  /*Serial.print("R= ");
  Serial.print(red);
  Serial.print("        ");
  //delay(5);*/
  green = ltcs.colorRead('g');
  /*Serial.print("G= ");
  Serial.print(green);
  Serial.print("        ");
  //delay(5);*/
  blue = ltcs.colorRead('b');
  /*Serial.print("B= ");
  Serial.print(blue);
  Serial.print("        ");
  //delay(5);*/
  white = ltcs.colorRead('c');
  /*Serial.print("c= ");
  Serial.println(white);
  //delay(5);*/
  if(bminred <= red && red <= bmaxred && bmingreen <= green && green <= bmaxgreen && bminblue <= blue && blue <= bmaxblue)
  {
    Serial.println("blue detected. counter clock wise direction");
    ccw=true;
    cw=false;
    cornerlinecount++;
    intcolor=false;
  }
  else
  Serial.println("go forward");
}


void readdistance(int trigPin, int echoPin, char u)
{
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if(u=='F')
  {
    fdistance = duration * 0.034 / 2;
    if(intdistance == true)
    {
      intfdistance = duration * 0.034 / 2;
    }
  }
  if(u=='R')
  {
    rdistance = duration * 0.034 / 2;
    if(intdistance == true)
    {
      intrdistance = duration * 0.034 / 2;
    }
  }
  if(u=='L')
  {
    ldistance = duration * 0.034 / 2;
    if( intdistance == true)
    {
      intldistance = duration * 0.034 / 2;
      intdistance = false;
    }
  }

}

void readul()
{
  readdistance(ftrig, fecho, 'F');
  delay(10);
  readdistance(rtrig, recho, 'R');
  delay(10);
  readdistance(ltrig, lecho, 'L');
}
