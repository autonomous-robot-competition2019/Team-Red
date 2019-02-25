#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333

// Define runtime in seconds
#define RUNTIME 60

int hasRun;

void setup() {
  Serial.begin(9600);

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  // Stop code
  if (millis() >= RUNTIME * 1000) {
      pwm.setPWM(0,0,333);
      pwm.setPWM(1,0,333);
  // Main code
  } else {
    pwm.setPWM(0,0,100);
    pwm.setPWM(1,0,433);
    delay(1000);
    pwm.setPWM(0,0,433);
    pwm.setPWM(1,0,433);
    delay(3000);
  }

}

void fullRotation() {
  long startTime = millis();
  long endTime = startTime;
  while ((endTime - startTime) <= 3000) {
    pwm.setPWM(0,0,433);
    pwm.setPWM(1,0,433);
    endTime = millis();
  }
  pwm.setPWM(0,0,333);
  pwm.setPWM(1,0,333); 
}

void goStraight(int length) {
  long startTime = millis();
  long endTime = startTime;
  while ((endTime - startTime) <= length) {
      pwm.setPWM(0,0,100);
      pwm.setPWM(1,0,433);
  }
}
