#include <Arduino.h>
#include <Servo.h> //inkludowanie biblioteki servo
#include "Adafruit_VL53L0X.h"
#include <SharpDistSensor.h>


#define trigPin 22
#define echoPin 23
#define trigPin2 24
#define echoPin2 25
#define sharp1Pin A8
#define sharp2Pin A9

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const byte medianFilterWindowSize = 5;
const byte medianFilterWindowSize2 = 5;
Servo myservo;  // tworzenie obiektu servo
Servo myservo2; // tworzenie obiektu servo2
SharpDistSensor sensor1(sharp1Pin, medianFilterWindowSize);
SharpDistSensor sensor2(sharp2Pin, medianFilterWindowSize2);

int RightMotor = 0;
int LeftMotor = 0;
int SpeedValue = 0;
int iloszprzejazdow = 0;

unsigned int distance;

void turn_right();
void turn_left();
void forward();
void mStop();
void errorXD();
int hcsr04Read(int a);
int sharpRead(int a);

void setup()
{
  myservo.attach(2);  // podlaczenie serva do pinu 2
  myservo2.attach(3); // podlaczenie serva2 do pinu 3

  for (int i; i < 9; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(A2, INPUT);
  pinMode(A4, INPUT);
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);

  analogWrite(9, 0);
  analogWrite(10, 0);
  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);
  Serial.begin(9600);

  if (!lox.begin())
  {
    errorXD();
  }
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(7, OUTPUT);
  pinMode(6, INPUT);

  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  Serial.println("Ready");
}

void loop()
{
  VL53L0X_RangingMeasurementData_t measure;

  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4)
  { // phase failures have incorrect data
    //   Serial.print("Distance (mm): ");
    // Serial.println(measure.RangeMilliMeter);
  }
  else
  {
    //   Serial.println(" out of range ");
  }

  if (Serial.available())
  {
    if (Serial.read() == 'f')
    {
      forward();
      Serial.println("ok");
    }
    else if (Serial.read() == 's')
    {
      mStop();
      Serial.println("ok");
    }
    if (Serial.read() == 'r')
    {
      int ch3 = 0;
      Serial.println("ok");
      while (1)
      {
        ch3 = map(pulseIn(A3, HIGH), 1100, 1900, 0, 100);
        SpeedValue = map(pulseIn(A4, HIGH), 1100, 1900, 0, 255);
        RightMotor = SpeedValue - map(pulseIn(A2, HIGH), 1000, 2000, -ch3, ch3);
        LeftMotor = SpeedValue - map(pulseIn(A2, HIGH), 1000, 2000, +ch3 + 20, -ch3);
        //LeftMotor = LeftMotor + 20;

        if (RightMotor < 0)
        {
          RightMotor = 0;
        }
        if (LeftMotor < 0)
        {
          LeftMotor = 0;
        }
        Serial.println(pulseIn(A3, HIGH));
        if (pulseIn(A0, HIGH) <= 1300)
        {

          digitalWrite(11, LOW);
          digitalWrite(13, LOW);

          digitalWrite(8, HIGH);
          digitalWrite(12, HIGH);
          analogWrite(10, LeftMotor);
          analogWrite(9, RightMotor);
        }
        else
        {
          digitalWrite(11, HIGH);
          digitalWrite(13, HIGH);

          digitalWrite(8, LOW);
          digitalWrite(12, LOW);
          analogWrite(9, LeftMotor);
          analogWrite(10, RightMotor);
        }
      }
    }
  }

  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(12, LOW);
  if ((hcsr04Read(2) <= 1500) && (iloszprzejazdow < 2))
  {
    turn_right();
    iloszprzejazdow++;
    if (iloszprzejazdow > 1)
    {
      delay(80);
      digitalWrite(6, HIGH);
      /*
      if (measure.RangeMilliMeter <= 150)
      {

        digitalWrite(7, HIGH);

        digitalWrite(11, HIGH);
        digitalWrite(13, LOW);
        digitalWrite(8, LOW);
        digitalWrite(12, LOW);

      }
      else
      {
        digitalWrite(7, LOW);
      }

      if (measure.RangeMilliMeter >= 150)
      {
        digitalWrite(11, LOW);
        digitalWrite(13, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(12, LOW); 
      }
      
    }
    else
    {
      digitalWrite(6, LOW);
    }
    */
    }
    
  }
  LeftMotor = 100;
  RightMotor = 100;
  analogWrite(9, RightMotor);
  analogWrite(10, LeftMotor);
  if (iloszprzejazdow < 2)
  {

    if (measure.RangeMilliMeter <= 150)
    {
      /*
    LeftMotor += 1000;
    if(LeftMotor >= 255){
      LeftMotor = 255;
    }
    analogWrite(9,  RightMotor);
    analogWrite(10, LeftMotor);
    delay(3);
    LeftMotor = RightMotor;
    analogWrite(9,  RightMotor);
    analogWrite(10, LeftMotor); 
    */
      digitalWrite(7, HIGH);

      digitalWrite(11, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      /*
    delay(10);
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    */
    }
    else
    {
      // LeftMotor = LeftMotor;
      // analogWrite(9,  RightMotor);
      // analogWrite(10, LeftMotor);
      digitalWrite(7, LOW);
    }

    if (measure.RangeMilliMeter >= 150)
    {
      /*
    RightMotor += 50;
    if(RightMotor >= 255){
      RightMotor = 255;
    }
    analogWrite(9,  RightMotor);
    analogWrite(10, LeftMotor);
    //delay(3);
    RightMotor = LeftMotor;
    analogWrite(9,  RightMotor);
    analogWrite(10, LeftMotor);
    */
      digitalWrite(11, LOW);
      digitalWrite(13, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      /*
    delay(10);
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    */
    }
    else
    {
      // RightMotor = RightMotor;
      //analogWrite(9,  RightMotor);
      //analogWrite(10, LeftMotor);
    }
    if ((hcsr04Read(2) <= 1500) && (iloszprzejazdow < 2))
    {
      turn_right();
      iloszprzejazdow++;
    }
  }
  else
  {
    // digitalWrite(11, LOW);
    // digitalWrite(13, LOW);
    // digitalWrite(8, LOW);
    // digitalWrite(12, LOW);
  }
  if ((hcsr04Read(2) <= 1500) && (iloszprzejazdow >= 2))
  {
    turn_left();
    iloszprzejazdow++;
  }

  // forward();
  //Serial.print("sharp: ");
  //Serial.println(sharpRead(2));
  Serial.println(iloszprzejazdow);
  //delay(10);
  //Serial.println(hcsr04Read(2));
  //forward();
  //turn_left();
  //delay(500);
  



//end loop
  
}
void turn_right()
{
  digitalWrite(11, HIGH);
  digitalWrite(13, LOW);

  digitalWrite(8, LOW);
  digitalWrite(12, HIGH);
  analogWrite(9, 150);
  analogWrite(10, 150);
  delay(450);
  analogWrite(10, 0);
  analogWrite(9, 0);
}

void turn_left()
{
  digitalWrite(11, LOW);
  digitalWrite(13, HIGH);

  digitalWrite(8, HIGH);
  digitalWrite(12, LOW);
  analogWrite(9, 150);
  analogWrite(10, 150);
  delay(420);
  analogWrite(10, 0);
  analogWrite(9, 0);
}

void forward()
{

  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);

  digitalWrite(8, LOW);
  digitalWrite(12, LOW);
}

void mStop()
{
  digitalWrite(8, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  analogWrite(10, 0);
  analogWrite(9, 0);
}
int hcsr04Read(int a)
{
  switch (a)
  {
  case 1:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    return pulseIn(echoPin, HIGH);
    break;

  case 2:
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    return pulseIn(echoPin2, HIGH);
    break;

  default:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    return pulseIn(echoPin, HIGH);
    break;
  }
}

int sharpRead(int a)
{
  switch (a)
  {
  case 1:
    distance = sensor1.getDist();
    return distance;
    break;
  case 2:
    distance = sensor2.getDist();
    return distance;
    break;
  }
}

void errorXD()
{
  Serial.println("error XD");
  digitalWrite(7, HIGH);
  while (1)
    ;
}
