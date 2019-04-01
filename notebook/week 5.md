## Week 5
We began this week by doing a complete overhaul of the design of our chassis in order to make it more stable and reliable. 
By moving a few things around and adding a foamcore base at the top of the robot we were able to create more space and reduce wire clutter.
In addition to this we now have two treads which utilize a gear system operated by our servo motors in order to function. These new
treads have increased the speed of our robot and also allowed the robot to drive straight much more consistently than the previous set up.

Next we chose to focus on the addition of a color tracking function using the pixy cam as a sensor to detect blocks of color within its field 
of view. In order to achieve this we coded a function in arduino to recognize the block with the smallest y value and orient the robot towards this block of color.
In theory this block will always be the most desirable target because it ensures the robot will focus on the block that takes up the largest portion of its 
field of view. Based on the x value of this block the robot will either rotate in the desired direction or drive forward if it is already alligned with the block of color.
We expanded on this basic functionality to track one specifc color by implementing an additional function of the light sensor, using it as a switch to change the color
the robot was focusing on. 
``` 
if (blocks) {
    int smallestYOrangeindex = -1;
    for (j = 0; j < blocks; j++) {
      if (pixy.blocks[j].signature == 2) {
        if (smallestYOrangeindex == -1) {
          smallestYOrangeindex = j;
        } else if (pixy.blocks[j].y < pixy.blocks[smallestYOrangeindex].y) {
          smallestYOrangeindex = j;
        }
      }
    }
    if (smallestYOrangeindex >= 0) {
      foundOrange = 1;
      closestOrangeY = pixy.blocks[smallestYOrangeindex].y;
      closestOrangeX = pixy.blocks[smallestYOrangeindex].x;
    } else {
      foundOrange = 0;
    }
    pixy.blocks[j].print();
    Serial.print("foundOrange = ");
    Serial.print(foundOrange);
    Serial.print(", closestOrangeX = ");
    Serial.print(closestOrangeX);
    Serial.print(", closestOrangeY = ");
    Serial.print(closestOrangeY);
    Serial.println();
    delay(10);
    
void focusYellow() {
    int x_low = follow_center - follow_range;
    int x_high = follow_center + follow_range;
    
    // Spin until yellow is found
    if (foundYellow == 0) {
      if (isRotating == 0) { 
        constantRotation(1);
      }
    }
    // If yellow is found
    else if (foundYellow == 1) {
      // If rotating, go straight
      if (isRotating == 1) {
        constantRotation(0);
      }
      // Object is aligned. Drive straight
      if ((x_low <= closestYellowX) && (x_high >= closestYellowX)) {
        updateMotor(333 - drive_speed, 333 + drive_speed);
      }
      // Object is too far right. Turn left.
      else if (closestYellowX >= (follow_center + follow_range)) {
        rotation((closestYellowX - follow_center)/follow_rotation_const, 0);
        updateMotor(333 - drive_speed, 333 + drive_speed);
      }
      // Object is too far left. Turn right.
      else if (closestYellowX <= (follow_center - follow_range)) {
        rotation((follow_center - closestYellowX)/follow_rotation_const, 1);
        updateMotor(333 - drive_speed, 333 + drive_speed);
      }
    }
}
``` 

Finally we constructed an arm with a claw at the end to allow the robot to manipulate quaffles and eventually score goals. At the moment the arm only has basic
functionality but it was able to successfully pick up a ball placed in front of it and drop it into the goal. The arm is not yet fully attached to the chassis because
it would require a more sturdy base to allow a full range of motion. We are considering another redesign to the chassis made via 3d printing to reinforce the arm and allow
a full range of motion.
\![alt text](https://github.com/autonomous-robot-competition2019/Team-Red/blob/master/images/IMG_1405 (1).mov)

## New Goals
+ Mount the arm in a more effective way
+ 3D print a new chassis
+ Improve the color switching of the light sensor
+ Implement the keeper role
+ 
