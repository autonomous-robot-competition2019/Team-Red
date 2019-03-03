#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333

#define sensor A0

const long runtime = 30;

// Motor values
int motorRight = 333;
int motorLeft = 333;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
}

void loop() {
  // Read values from IR sensor
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  
  // Stop code
  if (millis() >= runtime * 1000) {
    updateMotor(333,333);
  }  
  // Main code
  else {
    if (distance <= 5) {
      rotation(180);
      delay(10000);
    } 
    updateMotor(100,433);
  }
}

void updateMotor(int motor0, int motor1) {
  motorRight = motor0;
  motorLeft = motor1;
  pwm.setPWM(0,0,motorRight);
  pwm.setPWM(1,0,motorLeft);
}

void rotation(int degree) {
    updateMotor(433,433);
    delay(3000);
    stop();
}

// Stop function
void stop() {
  updateMotor(333,333);
}
