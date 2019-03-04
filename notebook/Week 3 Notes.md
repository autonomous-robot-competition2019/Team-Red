# Notebook Entry 3 (Week of 2/25)

This week we focused on the wiring and coding of the various required sensors as well as the basic movement of 
the robot. The first sensor that we focused on implenting was the SHARP IR Sensor which we utilizes to detect objects
in front of the robot and stop it before a collision. We were able to mount the IR sensor onto the front of the robot using legos.
The code for the IR Sensor functions by reading the distance from an object outputted by the IR Sensor and
if that distance falls within a certain range stopping the robot. 

Sample of Code for IR Sensor
float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  // Stop code
  if (millis() >= RUNTIME * 1000) {
      pwm.setPWM(0,0,333);
      pwm.setPWM(1,0,333);
  } 
  else {
    if (distance <= 5) {
      stop();
      delay(2000);
    } 
  
 We than began to work with the other three required sensors: Light, RGB and the PixyCam by determining how 
 to wire each onto the servo shield and mount them on our chassis. Utilizing foam core and legos we were able to 
 succesfully mount every required sensor onto the front of our robot. Next we started to work on coding the RGB sensors to recognize
 the three distinct colored lines in the arena as well as the gray color of the ground. By specifying ranges for each of these colors
 we implemented an integer value in the code for each of the colors in the arena. This will allow the robot to perform certain actions such as backing up and turning around when it encounters a specific color. 
 
 Sample of Code for the Color Update Function
  if (r >= (gry_r - range) && r <= (gry_r + range) && b >= (gry_b - range) && b <= (gry_b + range) && g >= (gry_g - range) && g <= (gry_g + range)) {
    colorRange = 0;
  } else if (r >= (red_r - range) && r <= (red_r + range) && b >= (red_b - range) && b <= (red_b + range) && g >= (red_g - range) && g <= (red_g + range)) {
    colorRange = 1;
  } else if (r >= (blu_r - range) && r <= (blu_r + range) && b >= (blu_b - range) && b <= (blu_b + range) && g >= (blu_g - range) && g <= (blu_g + range)) {
    colorRange = 2;
  } else if (r >= (yel_r - range) && r <= (yel_r + range) && b >= (yel_b - range) && b <= (yel_b + range) && g >= (yel_g - range) && g <= (yel_g + range)) {
    colorRange = 3;
  }
  
![alt text](https://github.com/autonomous-robot-competition2019/Team-Red/blob/master/images/IMG_1820 (1).mov)

## Acknowledgements
Coding for IR Sensor and RGB Sensor-Wes
Wiring of Sensors-Ryan
Mounting of Sensors onto the chassis- Wes and Ryan

## Our Goals for Next Week
* Find a smaller battery that can power both the arduino and motor shield.
*Implement code for light sensor and Pixycam
*Construct?reseach a mechanism for collecting balls
*Implement a player role. Most likely the keeper role with some sort of patrol method along the yellow line in 
the arena.
