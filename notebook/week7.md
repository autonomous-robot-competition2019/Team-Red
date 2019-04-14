## Week 7 Notebook

This week was very exciting because we were not only able to solve multiple problems we were struggling with last week but also complete all of
the remaining milsetones. After class last week we were able to install a new servo for the left motor allowing us to test out our new chassis and see
how it would perform. We were forced to adjust the code slightly to account for the new design but after some fine tuning the robot was faster and more precise 
than it had ever been before. This new design was also now able to accomodate a mount for the arm which we fashioned out of legos at placed at the front of 
the robot. 

With our new completed chassis in place we got to work on refining our color search code in order to implement the chaser role into our robot.
We began by simplifying the prioritization of the color search function  to better fit with the chaser role. The robot now begins in a search state where
it looks for a green quaffle and then transfers to a score state once the quaffle is picked up off the ground by the claw. 
Next we integrated our pre-existing code for the operation of the arm and the claw into the color search code to allow the arm to function in tandem with all the other sensors.
Since the quaffle is so small we had some difficulty positioning the robot in the ideal location to be able to pick up the ball after tracking was complete. The robot would often lose track of 
the ball just as it got into a position where the claw would be able to grab the ball. In order to adress this problem we implemented a very specific maneveur after the robot had tracke
the ball to the pixy cam's ideal distance to get the claw in position. After this maneuveur was completed the robot would always reach the position required to complete
the retrieval of the quaffle. A similar function called score was implemented in addition to fetch ball to allow the robot to score on the goal after it completed its tracking of the yellow color.
This code was ultimately successful and the robot was able to consistently pick up the quaffle and then score. Unfortunately during our testing we ran into a new problem when we attempted to place 
multiple quaffles in the robot's path. The addition of these new variables would often confuse the color tracking on the robot and after a quaffle was retrieved other balls would get in the way of the robot's path
and cause it to get stuck. As we attempted to rectify this problem the servo in our gripper died so we were forced to move on and troubleshoot other functions until we
have another working gripper
``` 
void fetch_ball() {
  state = 1;
  drive(4);
  delay(1000);
  rotation(32, 0);
  drive(0);
  delay(1200);
  drive(4);
  close_claw();
  delay(200);
  raise_arm();
  state = 2;
}

void score() {
  drive(4);
  delay(1000);
  drive(0);
  delay(800);
  drive(4);
  open_claw();
  delay(200);
  stop_robot();
}
``` 

Finally in order to complete the last milestone we implemented reliable tracking for the orange color of the snitch. We utilized our pre-existing color tracking code and
were easily able to apply the same logic to the snitch. With all these tasks completed we were able to complete all of the remaining milestones. Now we can begin to focus on refining our design
as well as implementing additional features and roles to ensure sucess in the competetion.

### Goals for next week:
- Implement the Keeper role
- Fix the claw or attach a new claw
- Find a way to avoid quaffles on the ground
