#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define ARMSERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define ARMSERVOMAX  350 // this is the 'maximum' pulse length count (out of 4096)


void setup() {
  Serial.begin(9600);

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  raise_arm();
  delay(3000);
  lower_arm();
  delay(3000);
}

// Lower arm of the robot
void lower_arm() {
  for (uint16_t pulselen = ARMSERVOMAX; pulselen > ARMSERVOMIN; pulselen--) {
    pwm.setPWM(2, 0, pulselen);
  }
}

// Raise arm of the robot
void raise_arm() {
  for (uint16_t pulselen = ARMSERVOMIN; pulselen < ARMSERVOMAX; pulselen++) {
    pwm.setPWM(2, 0, pulselen);
  }
}
