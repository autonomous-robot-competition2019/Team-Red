#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define ARMSERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define ARMSERVOMAX  350 // this is the 'maximum' pulse length count (out of 4096)

#define CLAWSERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define CLAWSERVOMAX  300 // this is the 'maximum' pulse length count (out of 4096)


void setup() {
  Serial.begin(9600);

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  open_claw();
  delay(3000);
  close_claw();
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

// Lower arm of the robot
void open_claw() {
  for (uint16_t pulselen = CLAWSERVOMAX; pulselen > CLAWSERVOMIN; pulselen--) {
    pwm.setPWM(3, 0, pulselen);
  }
}

// Raise arm of the robot
void close_claw() {
  for (uint16_t pulselen = CLAWSERVOMIN; pulselen < CLAWSERVOMAX; pulselen++) {
    pwm.setPWM(3, 0, pulselen);
  }
}
