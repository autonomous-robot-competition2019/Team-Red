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
int isSearching = 0; // 0 is not rotating, 1 is rotating

// Time
const long runtime = 500;

// Stop
int stop_all = 0;

// Zone (Start in red)
int enemyZone = 1;
int zone = 2; // 1 = red zone, 2 = blue zone

// Color Range (0 = gray, 1 = red, 2 = blue, 3 = yellow, 4 = goal)
int colorRange = 0;
int prevColorRange = 0;

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
const int follow_range = 10;
const int follow_center = 160;
const int follow_rotation_const = 10;

// Yellow variables
const int follow_yellow_range = 10;
int foundYellow = 0; // 0 = no yellow, 1 = yes yellow
int closestYellowX = 0;
int closestYellowY = 0;
int closestYellowWidth = 0;
int yellowMinX = 7;
int yellowMinY = 7;

// Green variables
int foundGreen = 0; // 0 = no green, 1 = yes green
int closestGreenX = 0;
int closestGreenY = 0;
int closestGreenSizeX = 0;
int closestGreenSizeY = 0;

int greenSizeX = 30;
int greenSizeY = 25;
int greenRangeX = 5;
int greenRangeY = 5;

// State
int state = 0; // 0 = searching, 1 = found, 2 = score

// Arm variables

#define ARMSERVOMIN  240 // this is the 'minimum' pulse length count (out of 4096)
#define ARMSERVOMAX  390 // this is the 'maximum' pulse length count (out of 4096)

#define CLAWSERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define CLAWSERVOMAX  300 // this is the 'maximum' pulse length count (out of 4096)

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pinMode(41, INPUT_PULLUP);

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
    // Edge detection
    if (colorRange == 3) {
      Serial.println("Uh oh, yellow :(");
      rotation(650, 0);
    } else if (digitalRead(41) != HIGH && state != 2) {
      rotation(650, 0);
    }
    else {
      if (state == 0) {
        focusGreen();
      }
      else if (state == 2) {
        if (digitalRead(41) != HIGH) {
          if (zone != enemyZone) {
            drive(1);
            delay(3000);
            rotation(650, 1);
            drive(0);
            delay(2000);
          } else {
            score();           
          }
        } else {
          focusYellow();
        }
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
    colorUpdate();
    pwm.setPWM(0, 0, 333 + drive_speed);
    pwm.setPWM(1, 0, 336 - (drive_speed + (drive_speed * .55)));
    isSearching = 0;
  }
  else if (direction == 1) {
    pwm.setPWM(0, 0, 333 - drive_speed);
    pwm.setPWM(1, 0, 336 + drive_speed);
    isSearching = 0;
  }
  else if (direction == 2) {
    pwm.setPWM(0, 0, 333 + drive_speed);
    pwm.setPWM(1, 0, 336 + drive_speed);
  }
  else if (direction == 3) {
    pwm.setPWM(0, 0, 333 - drive_speed);
    pwm.setPWM(1, 0, 336 - drive_speed);
  }
  else {
    pwm.setPWM(0, 0, 333);
    pwm.setPWM(1, 0, 336);
    isSearching = 0;
  }
}

void search() {
  isSearching = 1;
  int randomNum = rand() % (100 + 1 - 0) + 0;
  if (randomNum == 1) {
    drive(0);
    delay(2000);
  }
  drive(2);
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
    prevColorRange = colorRange;
    colorRange = 0;
  } else if (r >= (red_r - range) && r <= (red_r + range) && b >= (red_b - range) && b <= (red_b + range) && g >= (red_g - range) && g <= (red_g + range)) {
    prevColorRange = colorRange;
    colorRange = 1;
    // Update zone
    if (prevColorRange == 0) {
      zone = 1;
    }
  } else if (r >= (blu_r - range) && r <= (blu_r + range) && b >= (blu_b - range) && b <= (blu_b + range) && g >= (blu_g - range) && g <= (blu_g + range)) {
    prevColorRange = colorRange;
    colorRange = 2;
    // Update zone
    if (prevColorRange == 0) {
      zone = 2;
    }
  } else if (r >= (yel_r - range) && r <= (yel_r + range) && b >= (yel_b - range) && b <= (yel_b + range) && g >= (yel_g - range) && g <= (yel_g + range)) {
    colorRange = 3;
  }
  Serial.print("Current: "); Serial.print(colorRange); Serial.println();
  Serial.print("Zone: "); Serial.print(zone); Serial.println();
}

void updatePixy() {
  static int i = 0;
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
      if (pixy.blocks[j].signature == 1) {
        if (pixy.blocks[j].signature == 1 && pixy.blocks[j].height >= yellowMinY && pixy.blocks[j].width >= yellowMinX) {
          if (smallestYindex == -1) {
            smallestYindex = j;
          } else if (pixy.blocks[j].width > pixy.blocks[smallestYindex].width) {
            smallestYindex = j;
          }
        }
      }
      // Green tracking
      else if (pixy.blocks[j].signature == 2) {
        if (smallestYGreenindex == -1) {
          smallestYGreenindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYGreenindex].y) {
          smallestYGreenindex = j;
        }
      }
    }

    // Update yellow global variables
    if (smallestYindex >= 0) {
      foundYellow = 1;
      closestYellowY = pixy.blocks[smallestYindex].y;
      closestYellowX = pixy.blocks[smallestYindex].x;
      closestYellowWidth = pixy.blocks[smallestYindex].width;
    } else {
      foundYellow = 0;
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
  } else {
    delay(20);
    blocks = pixy.getBlocks();
    if (blocks) {
      int smallestYindex = -1;
      int smallestYOrangeindex = -1;
      int smallestYGreenindex = -1;
  
      for (j = 0; j < blocks; j++) {
        // Yellow tracking
        if (pixy.blocks[j].signature == 1) {
          if (pixy.blocks[j].signature == 1 && pixy.blocks[j].height >= yellowMinY && pixy.blocks[j].width >= yellowMinX) {
            if (smallestYindex == -1) {
              smallestYindex = j;
            } else if (pixy.blocks[j].width > pixy.blocks[smallestYindex].width) {
              smallestYindex = j;
            }
          }
        }
        // Green tracking
        else if (pixy.blocks[j].signature == 2) {
          if (smallestYGreenindex == -1) {
            smallestYGreenindex = j;
          } else if (pixy.blocks[j].y < pixy.blocks[smallestYGreenindex].y) {
            smallestYGreenindex = j;
          }
        }
      }
  
      // Update yellow global variables
      if (smallestYindex >= 0) {
        foundYellow = 1;
        closestYellowY = pixy.blocks[smallestYindex].y;
        closestYellowX = pixy.blocks[smallestYindex].x;
        closestYellowWidth = pixy.blocks[smallestYindex].width;
      } else {
        foundYellow = 0;
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
}

void focusYellow() {
  int x_low = follow_center - follow_yellow_range;
  int x_high = follow_center + follow_yellow_range;

  // Spin until yellow is found
  if (foundYellow == 0) {
    drive(2);
  }
  // If yellow is found
  else if (foundYellow == 1) {
    // If rotating, go straight
    if (isSearching == 1) {
      drive(0);
    }
    if (closestYellowWidth >= 300) {
      pwm.setPWM(0, 0, 333 + drive_speed);
      pwm.setPWM(1, 0, 336 - (drive_speed + (drive_speed * .2)));
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

void focusGreen() {
  int x_low = follow_center - follow_range;
  int x_high = follow_center + follow_range;

  // Spin until green is found
  if (foundGreen == 0) {
    search();
  }
  // If green is found
  else if (foundGreen == 1) {
    // If rotating, go straight
    if (isSearching == 1) {
      drive(0);
      delay(100);
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
    delay(10);
  }
}

void fetch_ball() {
  state = 1;
  drive(4);
  delay(1000);
  rotation(25, 0);
  pwm.setPWM(0, 0, 333 + 4);
  pwm.setPWM(1, 0, 336 - 4);
  delay(3150);
  drive(4);
  close_claw();
  delay(200);
  raise_arm();
  delay(200);
  state = 2;
}

void score() {
  drive(4);
  open_claw();
  drive(1);
  delay(3000);
  rotation(180, 1);
  lower_arm();
  state = 0;
}
