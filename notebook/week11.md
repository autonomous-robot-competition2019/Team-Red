# Week 11 Journal

For this final week we focused on putting the finishing touches onto our robot before the competition.
In order to improve our ability to score once a goal was located we replaced our IR sensor with a button that 
triggers when the robot drives into the goal. We found that this method was much more consistent than the IR sensor 
which would often not trigger quickly enough for the goal to be recognized. 

In order to improve our tracking, we also remounted the pixy camera and manipulated our rotation constant in order to decrease the 
speed the robot turns at while it is searching. We found that this was beneficial because it allowed use to remain on track with the quaffle.
and lose it much less often. We also reduced the follow range for the yellow goals to prevent the robot from veering to far to the left or the right.

Finally, we implemented a variable that determines our starting zone and ending zone, allowing us to prevent scoring on our own goal.
The variable updates if the robot passes over the lines in the middle of the arena so the robot will always know what zone it is occupying at any given 
moment.

