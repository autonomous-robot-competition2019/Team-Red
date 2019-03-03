#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333

// Define runtime in seconds
#define RUNTIME 60
#define sensor A0

int hasRun;

void setup() {
  Serial.begin(9600);

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  // Stop code
  if (millis() >= RUNTIME * 1000) {
      pwm.setPWM(0,0,333);
      pwm.setPWM(1,0,333);
  } 
  else {
    if (distance <= 5) {
      stop();
      delay(2000);
    } 
    pwm.setPWM(0,0,100);
    pwm.setPWM(1,0,433);
  }
}

void fullRotation() {
    pwm.setPWM(0,0,433);
    pwm.setPWM(1,0,433);
}

//int checkDistance() {
//  long startTime = millis();
//  long endTime = startTime();
//  while ((endTime - startTime) <= 500) {
//    float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
//    int distance = 13*pow(volts, -1); // worked out from datasheet graph
//    if (distance > 5) {
//      return 0;
//    }
//  }
//  return 1;
//}

void stop() {
    pwm.setPWM(0,0,333);
    pwm.setPWM(1,0,333);
}

//void goStraight(int length) {
//  long startTime = millis();
//  long endTime = startTime;
//  while ((endTime - startTime) <= length) {
//      pwm.setPWM(0,0,100);
//      pwm.setPWM(1,0,433);
//  }
//}
