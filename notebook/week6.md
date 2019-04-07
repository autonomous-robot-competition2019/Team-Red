## Week 6 Notebook

This week was a frustrating one for progress with our robot. We decided early on that we would need to modify our chassis to accommodate the arm we constructed to pick of quaffles. We initially thought we could use our existing design, but move to a 3d printed chassis. In the interest of time, we decided this may not be worthwhile. Next, we considered the possibility of using a flat Plexiglas chassis for simplicity, but given the time constraints once again we decided against this. Seeing that the materials are so available and versatile, we decided it may be best to stick with legos.

Our main issues we wanted to address in the new chassis were the following:

 - Add rigidity to our treads so the axles would not bend
 - Add a space for our robotic arm
 - Create a better space for housing the battery and wires

As we tried to model new designs, we decided it may be more sturdy and simplistic if we stop using the treads. We decided we could construct a simple chassis using two wheels instead. After modeling some designs, we decided on a rather square chassis with two side mounted wheels with front and rear supports. This allowed us to better mount the sensors, arm, and battery. We also gained significant speed benefits from this design.

Once the chassis was built, we reassembled everything to make sure it would work with the previous code. However, we could no longer get the left servo to turn. We are not sure what the cause of the issue was, but we determined we would need to replace the servo which cost us some time.

Once this was replaced, we began to have an issue with our color sensor. After some testing, we learned the connections we were using were not consistent and we redid the connections and remount the color sensor. With the new chassis, we needed to update our drive functions as the new motor was a little bit slower than the old one. We completely redid the drive functions for simplicity.

Once this was complete, we tried to run our color tracking code. However, the robot would spin and not seem to detect the yellow objects. After quite a long while of diagnosing, we discovered the chassis reversed the orientation of the motors and they were spinning in the wrong direction. The tracking function worked correctly.

### Goals for next week:
- Attach the arm to the chassis
- Add tracking functions for all objects
- Pick up a quaffle 
