#include "vex.h"
#include "main.h"
using namespace vex;
float speedFactor = 1;
float turnFactor = 0.7;
float stickB;
// Drive task with optional reverse direction and turn scaling.
int TaskDriveTrain() {
    Brain.Screen.setCursor(2, 1);

    OverRideDriveTrain = false;
    ReverseDir = true;
    printf("systen Start");
    printf("\n");
    while (true) {
        if (!OverRideDriveTrain) {
            if(abs(Controller.AxisA.position()) > 90) {
                speedFactor = 1.0;
                turnFactor = 0.7;
            }
            else {
                speedFactor = 0.7;
                turnFactor = 0.5;
            }
            if(abs(Controller.AxisB.position()) < 10){
                stickB = 0;
            }
            else {
                stickB = Controller.AxisB.position();
            }
            // turnFactor = 0.7 * speedFactor;
            if (ReverseDir) {
                TouchLED12.setColor(green);
                mot_dtRight.setVelocity((-Controller.AxisA.position() * speedFactor + (stickB * turnFactor)) , percent);
                mot_dtLeft.setVelocity((-Controller.AxisA.position() * speedFactor - (stickB * turnFactor)) , percent);
            }
            else {
                TouchLED12.setColor(orange);
                mot_dtRight.setVelocity((Controller.AxisA.position() * speedFactor + (stickB * turnFactor)) , percent);
                mot_dtLeft.setVelocity((Controller.AxisA.position() * speedFactor - (stickB * turnFactor)) , percent);
            }
            if(Brain.Battery.capacity() < 70){
                TouchLED12.setColor(red);
            }
   
            mot_dtLeft.spin(forward);
            mot_dtRight.spin(forward);
        }
        wait(20, msec);
    }
    return 0;
}
