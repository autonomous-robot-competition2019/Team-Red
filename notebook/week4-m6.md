# Week 4

We made major progress this week for our robot in terms of its foundation and how it reacts to its environment. We first decided on mounting points for all of our major sensors. The IR sensor is mounted on the front center of the robot and will serve as collision detection for the robot. The color sensor is mounted on the underside of the robot towards the front and will allow the robot to see where it is located in the arena by observing the color of the tape and floor. The light sensor is also mounted on the front of the robot and just stops the robot when a certain amount of light is shined on the robot. The pixycam is also mounted on the front of the robot and will be used to help the robot locate balls and other objects from a distance. 

We started off the week by installing the new battery on the robot so we would not have to follow it with the larger battery. We then created a simple function to stop the robot after a specified period of time to reach milestone 6.

We started by setting up the color sensor to have the robot rotate when it hits the edge of the arena (yellow tape). We started by using the example code to determine what the RGB values of the various colors of the arena were. We then created a function to return the zone the robot is in by seeing if the current color is within a range of values. Below is an example of the logic used to determine the zones.

``` 
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
```
This function worked well, but we had an issue where our robot would identify the color too late and roll off the arena before it had time to adjust. After much troubleshooting and trying to isolate the issue, we found the example code used an additional delay when the sensor was defined. After some online research we found that this definition allowed our robot to react quickly to color changes.
```
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
```
Once the robot could react effectively, we worked on a rotation function to allow the robot to rotate a certain amount of degrees. We tested certain delays with the motors and found a constant that would give us pretty accurate rotation based on the parameters passed.

## New Goals
+ Configure the pixycam to allow the robot to track objects
+ Create a more finalized design for the robot so we can move away from legos
+ Design our ball collection and deposit mechanism
+ Think about upgrading the treads to something wider
+ Code the robot for the keeper function
