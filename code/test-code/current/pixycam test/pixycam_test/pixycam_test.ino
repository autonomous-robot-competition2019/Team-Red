#include <Pixy.h>
#include <PixyI2C.h>
#include <PixySPI_SS.h>
#include <PixyUART.h>
#include <TPixy.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725.h"

// This is the main Pixy object 
Pixy pixy;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333, 336

#define sensor A0

// Speed
const int drive_speed = 10;
int isRotating = 0; // 0 is not rotating, 1 is rotating

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

// Tracking constants
const int follow_step = 1;
const int follow_range = 30;
const int follow_center = 160;
const int follow_rotation_const = 10;

// Yellow variables
int foundYellow = 0; // 0 = no yellow, 1 = yes yellow
int closestYellowX = 0;
int closestYellowY = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  // Initialize pixycam
  pixy.init();

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
    if (get_distance() > 0 && get_distance() <= 4) {
      // rotation(180, 0);
      stop_robot();
    }
    // Edge detection
    else if (colorRange == 3) {
      rotation(180, 0);
    } 
    else {
      focusYellow();
    }
  }
}

// 0 = straight
// 1 = reverse
// 2 = right
// 3 = left
// 4 = stop
void drive(int direction) {
  if (direction == 0) {
    pwm.setPWM(0,0, 333 + drive_speed);
    pwm.setPWM(1,0,336 - (drive_speed + (drive_speed * .55)));   
    isRotating = 0;  
  }
  else if (direction == 1) {
    pwm.setPWM(0,0, 333 - drive_speed);
    pwm.setPWM(1,0,336 + (drive_speed + (drive_speed * .55))); 
    isRotating = 0;       
  }
  else if (direction == 2) {
    pwm.setPWM(0,0, 333 + drive_speed);
    pwm.setPWM(1,0,336 + (drive_speed + (drive_speed * .55)));   
    isRotating = 1;   
  }
  else if (direction == 3) {
    pwm.setPWM(0,0, 333 - drive_speed);
    pwm.setPWM(1,0,336 - (drive_speed + (drive_speed * .55)));
    isRotating = 1;          
  }
  else {
    pwm.setPWM(0,0, 333);
    pwm.setPWM(1,0,336);   
    isRotating = 1;      
  }
}

void rotation(int degree, int direction) {
  // Turn left
  if (direction == 0) {
    drive(3);
    delay(degree * 10);    
  // Turn right
  } else {
    drive(2);
    delay(degree * 10);
  }
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
  Serial.print("Zone: "); Serial.print(colorRange); Serial.println();  
}

void updatePixy() {
  uint16_t blocks;
  char buf[32]; 
  int j;

  // grab blocks!
  blocks = pixy.getBlocks();

  if (blocks) {
    int smallestYindex = -1;
    for (j = 0; j < blocks; j++) {
      if (pixy.blocks[j].signature == 1) {
        if (smallestYindex == -1) {
          smallestYindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYindex].y) {
          smallestYindex = j;
        }
      }
    }
    if (smallestYindex >= 0) {
      foundYellow = 1;
      closestYellowY = pixy.blocks[smallestYindex].y;
      closestYellowX = pixy.blocks[smallestYindex].x;
    } else {
      foundYellow = 0;
    }
    pixy.blocks[j].print();
    Serial.print("foundYellow = ");
    Serial.print(foundYellow);
    Serial.print(", closestYellowX = ");
    Serial.print(closestYellowX);
    Serial.print(", closestYellowY = ");
    Serial.print(closestYellowY);
    Serial.println();
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

void focusYellow() {
    int x_low = follow_center - follow_range;
    int x_high = follow_center + follow_range;
    
    // Spin until yellow is found
    if (foundYellow == 0) {
      drive(2);
    }
    // If yellow is found
    else if (foundYellow == 1) {
      // If rotating, go straight
      if (isRotating == 1) {
        drive(0);
      }
      // Object is aligned. Drive straight
      if ((x_low <= closestYellowX) && (x_high >= closestYellowX)) {
        drive(0);
      }
      // Object is too far right. Turn left.
      else if (closestYellowX >= (follow_center + follow_range)) {
        rotation((closestYellowX - follow_center)/follow_rotation_const, 1);
        drive(0);
      }
      // Object is too far left. Turn right.
      else if (closestYellowX <= (follow_center - follow_range)) {
        rotation((follow_center - closestYellowX)/follow_rotation_const, 0);
        drive(0);
      }
    }
}

void stop_robot() {
  drive(4);
  stop_all = 1;
}
