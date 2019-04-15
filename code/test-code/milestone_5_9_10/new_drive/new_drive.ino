#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Speed
const int drive_speed = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  drive(0);
}

// 0 = straight
// 1 = reverse
// 2 = right
// 3 = left
void drive(int direction) {
  if (direction == 0) {
    pwm.setPWM(0,0, 333 + drive_speed);
    pwm.setPWM(1,0,336 - (drive_speed + (drive_speed * .2)));   
  }
  else if (direction == 1) {
    pwm.setPWM(0,0, 333 - drive_speed);
    pwm.setPWM(1,0,336 + (drive_speed + (drive_speed * .55)));      
  }
  else if (direction == 2) {
    pwm.setPWM(0,0, 333 + drive_speed);
    pwm.setPWM(1,0,336 + (drive_speed + (drive_speed * .55)));      
  }
  else if (direction == 3) {
    pwm.setPWM(0,0, 333 - drive_speed);
    pwm.setPWM(1,0,336 - (drive_speed + (drive_speed * .55)));        
  }
  else {
    pwm.setPWM(0,0, 333);
    pwm.setPWM(1,0,336);       
  }
}
