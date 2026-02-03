#include "vex.h"
#include "main.h"
using namespace vex;

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
            mot_dtRight.setVelocity((Controller.AxisA.position() - Controller.AxisB.position()), percent);
            mot_dtLeft.setVelocity((Controller.AxisA.position() + Controller.AxisB.position()), percent);
<<<<<<< HEAD
            mot_dtLeft.spin(reverse);
            mot_dtRight.spin(reverse);
=======
            mot_dtLeft.spin(forward);
            mot_dtRight.spin(forward);
            mot_dtLeft.spin(forward);
            mot_dtRight.spin(forward);
>>>>>>> 80f7cb49801084c6fa1771225010f67eb2b20263
        }
        else {
            TouchLED12.setColor(orange);
            mot_dtRight.setVelocity((Controller.AxisA.position() + Controller.AxisB.position()), percent);
            mot_dtLeft.setVelocity((Controller.AxisA.position() - Controller.AxisB.position()), percent);
            mot_dtLeft.spin(reverse);
            mot_dtRight.spin(reverse);
            mot_dtLeft.spin(reverse);
            mot_dtRight.spin(reverse);
        }
        }
    wait(20, msec);
    }
    return 0;
}
