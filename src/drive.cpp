#include "vex.h"
#include "main.h"
using namespace vex;

float turnFactor = 0.7;
int TaskDriveTrain() {
   
    Brain.Screen.setCursor(2, 1);
  
    OverRideDriveTrain = false;
    ReverseDir = true;
    printf("systen Start");
    printf("\n");
    while (true) {
        if (!OverRideDriveTrain) {
            
            if (ReverseDir) {
                TouchLED12.setColor(green);
                mot_dtRight.setVelocity((-Controller.AxisA.position() + (Controller.AxisB.position() * turnFactor)), percent);
                mot_dtLeft.setVelocity((-Controller.AxisA.position() - (Controller.AxisB.position() * turnFactor)), percent);
               
                // mot_dtLeft.spin(reverse);
                // mot_dtRight.spin(reverse);
            }
            else {
                TouchLED12.setColor(orange);
                mot_dtRight.setVelocity((Controller.AxisA.position() + (Controller.AxisB.position() * turnFactor)), percent);
                mot_dtLeft.setVelocity((Controller.AxisA.position() - (Controller.AxisB.position() * turnFactor)), percent);
               
                

            }
            mot_dtLeft.spin(forward);
            mot_dtRight.spin(forward);
        }
        wait(20, msec);
    }
    return 0;
}
