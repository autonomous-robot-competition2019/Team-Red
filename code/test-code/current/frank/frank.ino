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
const int drive_speed = 5;
int isRotating = 0; // 0 is not rotating, 1 is rotating

// Time
const long runtime = 500;

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
const int follow_range = 5;
const int follow_center = 160;
const int follow_rotation_const = 20;

// Yellow variables
int foundYellow = 0; // 0 = no yellow, 1 = yes yellow
int closestYellowX = 0;
int closestYellowY = 0;
int closestYellowSizeX = 0;
int closestYellowSizeY = 0;

int yellowSizeX = 50;
int yellowSizeY = 34;
int yellowRangeX = 5;
int yellowRangeY = 5;
int yellowMinY = 0;
int yellowMinX = 0;

// Green variables
int foundGreen = 0; // 0 = no green, 1 = yes green
int closestGreenX = 0;
int closestGreenY = 0;
int closestGreenSizeX = 0;
int closestGreenSizeY = 0;
int greenMinX = 0;
int greenMinY = 0;

int greenSizeX = 20;
int greenSizeY = 10;
int greenRangeX = 5;
int greenRangeY = 5;

// Orange variables
int foundOrange = 0; // 0 = no orange, 1 = yes orange
int closestOrangeX = 0;
int closestOrangeY = 0;

// State
int state = 0; // 0 = searching, 1 = found, 2 = score

// Arm variables

#define ARMSERVOMIN  240 // this is the 'minimum' pulse length count (out of 4096)
#define ARMSERVOMAX  400 // this is the 'maximum' pulse length count (out of 4096)
#define ARM2SERVOMIN  250 // this is the 'minimum' pulse length count (out of 4096)
#define ARM2SERVOMAX  475 // this is the 'maximum' pulse length count (out of 4096)


#define CLAWSERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define CLAWSERVOMAX  300 // this is the 'maximum' pulse length count (out of 4096)

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
  lower_arm();
  lower_JOHNNY();
  open_claw();
  delay(10);
}

void loop() {
  if (stop_all == 1) {
    drive(4);
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
//    if (distance > 0 && distance <= 2 && state == 0) {
//      Serial.println("I'm too close :(");
//      stop_robot();
//    }
    // Edge detection
    if (colorRange == 3) {
      Serial.println("Uh oh, yellow :(");
      rotation(180, 0);
    }
    else {
      if (state == 0) {
        drive(0);
      } 
      else if (state == 2) {
        focusYellow();
      }   
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
    pwm.setPWM(0, 0, 333 + drive_speed);
    pwm.setPWM(1, 0, 336 - (drive_speed + (drive_speed * .2)));
    isRotating = 0;
  }
  else if (direction == 1) {
    pwm.setPWM(0, 0, 333 - drive_speed);
    pwm.setPWM(1, 0, 336 + (drive_speed + (drive_speed * .2)));
    isRotating = 0;
  }
  else if (direction == 2) {
    pwm.setPWM(0, 0, 333 + drive_speed);
    pwm.setPWM(1, 0, 336 + drive_speed);
    isRotating = 1;
  }
  else if (direction == 3) {
    pwm.setPWM(0, 0, 333 - drive_speed);
    pwm.setPWM(1, 0, 336 - (drive_speed + (drive_speed * .2)));
    isRotating = 1;
  }
  else if (direction == 4) {
    pwm.setPWM(0, 0, 333);
    pwm.setPWM(1, 0, 336);
    isRotating = 1;
  }
  else {
    int randRotate = random(1,2);
    int randomDirection = random(0,1);
    int randomDistance = random(0, 5);
    rotation(randRotate * 90, randomDirection);
    drive(0);
    delay(randomDistance * 1000);
  }
}

void rotation(int degree, int direction) {
  // Turn left
  if (direction == 0) {
    drive(3);
    delay(degree * 25);
    // Turn right
  } else {
    drive(2);
    delay(degree * 25);
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
  } else if (r >= (yel_r - range) && r <= (yel_r + range) && b >= (yel_b - range) && b <= (yel_b + range) && g >= (yel_g - range) && g <= (yel_g + range)) {
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
    int smallestYOrangeindex = -1;
    int smallestYGreenindex = -1;

    for (j = 0; j < blocks; j++) {
      // Yellow tracking
      if (pixy.blocks[j].signature == 1 && pixy.blocks[j].height >= yellowMinY && pixy.blocks[j].width >= yellowMinX) {
        if (smallestYindex == -1) {
          smallestYindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYindex].y) {
          smallestYindex = j;
        }
      }
      // Green tracking
      else if (pixy.blocks[j].signature == 2 && pixy.blocks[j].height >= greenMinY && pixy.blocks[j].width >= greenMinX) {
        if (smallestYGreenindex == -1) {
          smallestYGreenindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYGreenindex].y) {
          smallestYGreenindex = j;
        }
      }
      // Orange tracking
      else if (pixy.blocks[j].signature == 3) {
        if (smallestYOrangeindex == -1) {
          smallestYOrangeindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYOrangeindex].y) {
          smallestYOrangeindex = j;
        }
      }
    }

    // Update yellow global variables
    if (smallestYindex >= 0) {
      foundYellow = 1;
      closestYellowY = pixy.blocks[smallestYindex].y;
      closestYellowX = pixy.blocks[smallestYindex].x;
      closestYellowSizeX = pixy.blocks[smallestYindex].width;
      closestYellowSizeY = pixy.blocks[smallestYindex].height;
    } else {
      foundYellow = 0;
    }

    // Update orange global variables
    if (smallestYOrangeindex >= 0) {
      foundOrange = 1;
      closestOrangeY = pixy.blocks[smallestYOrangeindex].y;
      closestOrangeX = pixy.blocks[smallestYOrangeindex].x;
    } else {
      foundOrange = 0;
    }

    // Update green global variables
    if (smallestYGreenindex >= 0) {
      foundGreen = 1;
      closestGreenY = pixy.blocks[smallestYGreenindex].y;
      closestGreenX = pixy.blocks[smallestYGreenindex].x;
      closestGreenSizeX = pixy.blocks[smallestYGreenindex].width;
      closestGreenSizeY = pixy.blocks[smallestYGreenindex].height;
      pixy.blocks[smallestYGreenindex].print();
      delay(10);
    } else {
      foundGreen = 0;
    }
  }
}

int get_distance() {
  // Read values from IR sensor
  float volts = analogRead(sensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1); // worked out from datasheet graph
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
    int yellowDistance = get_distance();
    if (yellowDistance > 0 && yellowDistance <= 4) {
      if (closestYellowSizeX >= 320) {
        score();
      } else {
        rotation(90, 1);
        drive(0);
        delay(1500);
        rotation(90, 0);
      }
    }
    // Object is aligned. Drive straight
    else if ((x_low <= closestYellowX) && (x_high >= closestYellowX)) {
      drive(0);
    }
    // Object is too far right. Turn left.
    else if (closestYellowX >= (follow_center + follow_range)) {
      rotation((closestYellowX - follow_center) / follow_rotation_const, 1);
      drive(0);
    }
    // Object is too far left. Turn right.
    else if (closestYellowX <= (follow_center - follow_range)) {
      rotation((follow_center - closestYellowX) / follow_rotation_const, 0);
      drive(0);
    }
  }
}
void focusOrange() {
  int x_low = follow_center - follow_range;
  int x_high = follow_center + follow_range;

  // Spin until orange is found
  if (foundOrange == 0) {
    drive(2);
  }
  // If orange is found
  else if (foundOrange == 1) {
    // If rotating, go straight
    if (isRotating == 1) {
      drive(0);
      delay(500);
    }
    int orangeDistance = get_distance();
    if (orangeDistance > 0 && orangeDistance <= 4) {
      stop_robot();
    }
    // Object is aligned. Drive straight
    else if ((x_low <= closestOrangeX) && (x_high >= closestOrangeX)) {
      drive(0);
      delay(3000);
    }
    // Object is too far right. Turn left.
    else if (closestOrangeX >= (follow_center + follow_range)) {
      rotation((closestOrangeX - follow_center) / follow_rotation_const, 1);
      drive(0);
    }
    // Object is too far left. Turn right.
    else if (closestOrangeX <= (follow_center - follow_range)) {
      rotation((follow_center - closestOrangeX) / follow_rotation_const, 0);
      drive(0);
    }
  }
}


void focusGreen() {
  int x_low = follow_center - follow_range;
  int x_high = follow_center + follow_range;

  // Spin until green is found
  if (foundGreen == 0) {
    drive(2);
  }
  // If green is found
  else if (foundGreen == 1) {
    // If rotating, go straight
    if (isRotating == 1) {
      drive(0);
    }
    if ((closestGreenSizeX <= (greenRangeX + greenSizeX)) && (closestGreenSizeX >= (greenSizeX - greenRangeX)) && (closestGreenSizeY <= (greenRangeY + greenSizeY)) && (closestGreenSizeY >= (greenSizeY - greenRangeY))) {
      fetch_ball();
    }
    // Object is aligned. Drive straight
    else if ((x_low <= closestGreenX) && (x_high >= closestGreenX)) {
      drive(0);
    }
    // Object is too far right. Turn left.
    else if (closestGreenX >= (follow_center + follow_range)) {
      rotation((closestGreenX - follow_center) / follow_rotation_const, 1);
      drive(0);
    }
    // Object is too far left. Turn right.
    else if (closestGreenX <= (follow_center - follow_range)) {
      rotation((follow_center - closestGreenX) / follow_rotation_const, 0);
      drive(0);
    }
  }
}

void stop_robot() {
  drive(4);
  stop_all = 1;
}

// Raise arm of the robot
void raise_arm() {
  for (uint16_t pulselen = ARMSERVOMAX; pulselen > ARMSERVOMIN; pulselen--) {
    pwm.setPWM(2, 0, pulselen);
    delay(5);
  }
}

// Lower arm of the robot
void lower_arm() {
  for (uint16_t pulselen = ARMSERVOMIN; pulselen < ARMSERVOMAX; pulselen++) {
    pwm.setPWM(2, 0, pulselen);
    delay(5);
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
// Raise arm of the robot
void raise_JOHNNY() {
  for (uint16_t pulselen = ARM2SERVOMAX; pulselen > ARM2SERVOMIN; pulselen--) {
    pwm.setPWM(4, 0, pulselen);
    delay(5);
  }
}

// Lower arm of the robot
void lower_JOHNNY() {
  for (uint16_t pulselen = ARM2SERVOMIN; pulselen < ARM2SERVOMAX; pulselen++) {
    pwm.setPWM(4, 0, pulselen);
    delay(5);
  }
}

void fetch_ball() {
  state = 1;
  drive(4);
  delay(1000);
  raise_JOHNNY();
  delay(200);
  rotation(8, 0);
  drive(0);
  delay(400);
  drive(4);
  close_claw();
  delay(200);
  raise_arm();
  delay(200);
  lower_JOHNNY();
  state = 2;
 
}

void score() {
  drive(4);
  delay(1000);
  raise_JOHNNY();
  delay(200);
  drive(0);
  delay(1200);
  drive(4);
  open_claw();
  delay(200);
  lower_JOHNNY();
  stop_robot();
}
