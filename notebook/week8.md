
## Week 8 Journal

This week we made a few major structural changes to the robot including reattaching the new claw and adding a counterweight to keep the robot more stable. Attaching the claw was very straightforward as the new one was exactly the same as the old. 

We then attempted to test the successful code for find a ball and scoring a goal. However, the robot stuttered and failed to do so. We realized the robot was bobbing too much and it needed a counterweight for it to drive level and straight. We attached a lego "basket" on the back that we could weight with various objects. After doing so, the robot was much more stable. However, because of the two different servo motors, it still did not drive straight. To fix this, we re calibrated our drive function so that the right motor had additional drive than the left. Now the robot was able to track objects much more easily and it could once again score a goal.

    else {
	    int randRotate = random(1,2);
	    int randomDirection = random(0,1);
	    int randomDistance = random(0, 5);
	    rotation(randRotate * 90, randomDirection);
	    drive(0);
	    delay(randomDistance * 1000);
    }

We then realized the robot has a hard time finding objects if they are far away due to the angle of the pixycam. To fix this, we wanted to change the search behavior so that the robot would not just spin in place until it finds an object. To do this, we implemented a function that will make the robot rotate and drive forward in a random direction at a random distance. By wandering this way, the robot has a better ability to find objects in the arena. 

However, when we were testing this the pixycam fell off the front of the robot, and then it was having trouble detecting the signatures we programmed. We spent a lot of time troubleshooting this, and our robot is still unable to differentiate between green and yellow signatures at this point. We have been researching the pixycam and our first goal is fix the pixycam so it can better identify different signatures. Our next goal is to then implement the keeper role.


