/* HC-SR04 Sensor
   https://www.dealextreme.com/p/hc-sr04-ultrasonic-sensor-distance-measuring-module-133696
  
   This sketch reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to digital pin 2
  * ECHO connection of the sensor attached to digital pin 4
   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius
   This example code is in the public domain.
 */
#include <SoftwareSerial.h>
SoftwareSerial sigfox(3, 2);

const int trigPin = 13;
const int echoPin = 12;

typedef int bool;
#define TRUE 1
#define FALSE 0

int time_elapsed = 0;
boolean parked = FALSE;
void setup() {
  // initialize serial communication:
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(4800);
  sigfox.begin(9600);
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // If there is a change in the parkingAvailable,
    // if whether proximity is less than 12 inches 
      // time_elapsed++
      //if  tme_elapsed > 3s, 
        // set parkingAvailable = false
    // else if the proximity is more than 12 inches
    // set parkingAvailable = true
    // set time_elapsed = 0
  if(inches < 12){
    if(parked == FALSE){
      parked=TRUE;
      time_elapsed=0;
    }
    time_elapsed++;
    if(time_elapsed==3){
      char* data_to_send="PARKED:TRUE";
      SIGFOX_SEND(data_to_send);
    }
  }
  else{
    if(parked == TRUE){
      parked=FALSE;
      time_elapsed=0;
    }
    time_elapsed++;
    if(time_elapsed==3){
      char* data_to_send="PARKED:FALSE";
      SIGFOX_SEND(data_to_send);
    }
  }
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(1000);
}

String SIGFOX_SEND(char* data){
 Serial.print("sent sigfox");
 String protocol="AT$SF=";
 protocol+=(String)data;
 protocol+="\r";
 sigfox.print(String(protocol));
 delay(1000);
 return "OK";
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
