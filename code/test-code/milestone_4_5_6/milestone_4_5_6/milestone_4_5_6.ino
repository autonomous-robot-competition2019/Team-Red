#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// Right motor is 0
// Left motor is 1
// Neutral PWM is 333

#define sensor A0

// Time
const long runtime = 30;

// Color Range (0 = gray, 1 = red, 2 = blue, 3 = yellow)
int colorRange = 0;

// Color Constants
const int range = 10;

const int gry_r = 124;
const int gry_g = 80;
const int gry_b = 61;

const int red_r = 150;
const int red_g = 65;
const int red_b = 54;

const int blu_r = 92;
const int blu_g = 86;
const int blu_b = 85;

const int yel_r = 130;
const int yel_g = 87;
const int yel_b = 45;

// Motor values
int motorRight = 333;
int motorLeft = 333;

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
  // Read values from IR sensor
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  
  // Stop code
  if (millis() >= runtime * 1000) {
    updateMotor(333,333);
  }  
  // Main code
  else {
    // Check for color
    colorUpdate();
    // Obstactle stop
    if (distance <= 5) {
      stop();
      delay(2000);
    }
    // Edge detection
    if (colorRange == 3) {
      updateMotor(383,283);
      delay(1000);
      rotation(3000);
    }
    updateMotor(283,383);
  }
}

void updateMotor(int motor0, int motor1) {
  motorRight = motor0;
  motorLeft = motor1;
  pwm.setPWM(0,0,motorRight);
  pwm.setPWM(1,0,motorLeft);
}

void rotation(int degree) {
    updateMotor(383,383);
    delay(1500);
}

// Stop function
void stop() {
  updateMotor(333,333);
}

void colorUpdate() {
  uint16_t clear, red, green, blue, colorTemp, lux;

  tcs.setInterrupt(false);      // turn on LED

//  delay(60);  // takes 50ms to read 

  tcs.getRawData(&red, &green, &blue, &clear);
  colorTemp = tcs.calculateColorTemperature(red, green, blue);
  lux = tcs.calculateLux(red, green, blue);
  tcs.setInterrupt(true);  // turn off LED

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  Serial.print("R: "); Serial.print(r); Serial.print(", ");
  Serial.print("G: "); Serial.print(g); Serial.print(", ");
  Serial.print("B: "); Serial.print(b); 
  Serial.println();

  if (r >= (gry_r - range) && r <= (gry_r + range) && b >= (gry_b - range) && b <= (gry_b + range) && g >= (gry_g - range) && g <= (gry_g + range)) {
    colorRange = 0;
  } else if (r >= (red_r - range) && r <= (red_r + range) && b >= (red_b - range) && b <= (red_b + range) && g >= (red_g - range) && g <= (red_g + range)) {
    colorRange = 1;
  } else if (r >= (blu_r - range) && r <= (blu_r + range) && b >= (blu_b - range) && b <= (blu_b + range) && g >= (blu_g - range) && g <= (blu_g + range)) {
    colorRange = 2;
  } else if (r >= (yel_r - range) && r <= (yel_r + range) && b >= (yel_b - range) && b <= (yel_b + range) && g >= (yel_g - range) && g <= (yel_g + range)) {
    colorRange = 3;
  }
  
  Serial.print("Zone: "); Serial.print(colorRange); Serial.println();
  
}
