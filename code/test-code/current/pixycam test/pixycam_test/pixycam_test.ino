#include <Pixy.h>
#include <PixyI2C.h>
#include <PixySPI_SS.h>
#include <PixyUART.h>
#include <TPixy.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333

#define sensor A0

// Speed
const int drive_speed = 100;

// Time
const long runtime = 60;

// Stop
int stop_all = 0;

// Zone
int zone = 0; // 0 = red zone, 1 = blue zone

// Color Range (0 = gray, 1 = red, 2 = blue, 3 = yellow, 4 = goal)
int colorRange = 0;

// Color Constants
const int range = 20;

const int gry_r = 78;
const int gry_g = 95;
const int gry_b = 77;

const int red_r = 167;
const int red_g = 51;
const int red_b = 49;

const int blu_r = 39;
const int blu_g = 103;
const int blu_b = 124;

const int yel_r = 145;
const int yel_g = 166;
const int yel_b = 64;

const int goal_r = 123;
const int goal_g = 115;
const int goal_b = 45;

// Motor values
int motorRight = 333;
int motorLeft = 333;

// PixyCam

// yellow = 1
// blue = 2
// red = 3
// green = 4

int closestYellowX = 0;
int closestYellowY = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  // Check for rgb sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  
  delay(10);
}

void loop() {
  if (stop_all == 1) {
    // do nothing 
  }
  // Time stop code
  else if (millis() >= runtime * 1000) {
    stop_robot();
  }  
  // Main code
  else {
    // Check for color
    colorUpdate();
    // Check for blocks
    updatePixy();
    // Obstactle stop
    int distance = get_distance();
    if (distance >= 0 && distance <= 7) {
      rotation(180, 0);
    }
    // Edge detection
    if (colorRange == 3) {
      rotation(180, 0);
    } 
    // Object following
    focusObject();
    updateMotor(333 - drive_speed, 333 + drive_speed);
  }
}

void updateMotor(int motor1, int motor0) {
  motorRight = motor0;
  motorLeft = motor1;
  pwm.setPWM(0,0,motorRight);
  pwm.setPWM(1,0,motorLeft);
}


// 1300 = 90

void rotation(int degree, int direction) {
  // Turn left
  if (direction == 0) {
    updateMotor(333 - drive_speed,333 - drive_speed);
    delay(degree * 13.25);    
  // Turn right
  } else {
    updateMotor(333 + drive_speed,333 + drive_speed);
    delay(degree * 13.25);
  }
}

// Stop function
void stop() {
  updateMotor(333,333);
}

void colorUpdate() {
  float r, g, b;

  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read 
  tcs.getRGB(&r, &g, &b);
//  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R: "); Serial.print(r); Serial.print(", ");
  Serial.print("G: "); Serial.print(g); Serial.print(", ");
  Serial.print("B: "); Serial.print(b); 
  Serial.println();

  if (r >= (gry_r - range) && r <= (gry_r + range) && b >= (gry_b - range) && b <= (gry_b + range) && g >= (gry_g - range) && g <= (gry_g + range)) {
    colorRange = 0;
  } else if (r >= (red_r - range) && r <= (red_r + range) && b >= (red_b - range) && b <= (red_b + range) && g >= (red_g - range) && g <= (red_g + range)) {
    colorRange = 1;
    // Update zone
    zone = 0;
  } else if (r >= (blu_r - range) && r <= (blu_r + range) && b >= (blu_b - range) && b <= (blu_b + range) && g >= (blu_g - range) && g <= (blu_g + range)) {
    colorRange = 2;
    // Update zone
    zone = 1; 
  } else {
    colorRange = 3;
  }
//  } else if (r >= (yel_r - range) && r <= (yel_r + range) && b >= (yel_b - range) && b <= (yel_b + range) && g >= (yel_g - range) && g <= (yel_g + range)) {
//    colorRange = 3;
//  }
  
  
  Serial.print("Zone: "); Serial.print(colorRange); Serial.println();
  
}

void updatePixy() {
  uint16_t blocks;
  char buf[32]; 
  int j;

  // grab blocks!
  blocks = pixy.getBlocks();

  if (blocks) {
    int closestYellow = 0;
    int closestYellowDistance = pixy.blocks[0].width + pixy.blocks[0].height;
    for (j = 0; j < blocks; j++) {
      int width = pixy.blocks[j].width;
      int height = pixy.blocks[j].height;
      if (pixy.blocks[j].signature == 1) {
        if (closestYellowDistance < (width + height)) {
          closestYellow = j;
          closestYellowX = pixy.blocks[j].x;
          closestYellowY = pixy.blocks[j].y;
        }
      }
    }
  }
}

int get_distance() {
  // Read values from IR sensor
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  if (distance <= 80 && distance != 0) {
    Serial.print("Distance: "); Serial.print(distance); Serial.println();
    return distance;
  } else {
    return -1;
  }
}

void focusObject(int sig) {
    if (closestYellowX - 20
    if (closestYellowX >= 160) {
      updateMotor(motorLeft, motorRight - 5);
    }
    if (closestYellowX >= 160) {
      updateMotor(motorLeft - 5, motorRight);
    }
}

void stop_robot() {
  updateMotor(333,333);
  stop_all = 1;
}
