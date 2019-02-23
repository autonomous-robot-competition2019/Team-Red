# Notebook Entry (Week of 2/18)

This week we made the most significant progress in the development of our robot. We decided to try using treads for our robot and constructed a basic model using legos. We managed to include two servo motors in our lego frame for the gearing of the robot. We used a large gear in the back connected to the motor and a small free spinning gear in the front. To connect the lego gear to the motor, we used a foam core spacer to allow the hub of the motor to attach. We then managed to attach the treads to the gears. 

Once we had the basic model of the robot complete, we began to work with the Arduino code so we could reach the first milestone. We started by finding the pulse length that would cause the motor to stop. This number was 333 for both servos. We then created a basic sketch to and incremented/decremented (since the motors are opposite of each other) the pulse of both motors by 100 so that they would both spin forward. The battery we had was too large to fit on the frame of the robot so we had to hold the battery behind the robot as we tested it for the milestone. The robot moved perfectly straight and we are very pleased with the tread design.

![alt text](https://github.com/autonomous-robot-competition2019/Team-Red/blob/master/images/week2-1.jpg)
![alt text](https://github.com/autonomous-robot-competition2019/Team-Red/blob/master/images/week2-2.jpg)



## Our Goals for Next Week
* Find a smaller battery that can power both the arduino and motor shield.
* Integrate some sort of sensor so our robot can locate the boundries of the arena.
* Construct/research a mechanism of collecting balls. We are thinking of using a plow type structure with two spinning wheels to collect balls. https://www.youtube.com/watch?v=5NLjgErJD-k
* Finalize design of lego model and look in to printing a new frame
