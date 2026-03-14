#include "vex.h"
#include "main.h"
using namespace vex;
float speedFactor = 1;
float turnFactor = 0.7;
float stickB;
// Drive task with optional reverse direction and turn scaling.
int TaskDriveTrain() {
    Brain.Screen.setCursor(2, 1);
    bool btnR3Pressed = false;

    OverRideDriveTrain = false;
    ReverseDir = true;
    BrainTimer.reset();
    printf("systen Start");
    printf("\n");
    while (true) {
        const int axisA = Controller.AxisA.position();
        const int axisB = Controller.AxisB.position();

        // Emergency-stop sequence triggered from drivetrain task on R3 edge.
        const bool r3Now = Controller.ButtonR3.pressing();
        if (r3Now && !btnR3Pressed) {
            YGuidInSafe();
            ReleaseBeam;
            ReleasePin;
            handUp;
            wait(500, msec);
            Brain.programStop();
        }
        btnR3Pressed = r3Now;

        // Reset 120s auto-stop timer when either drive axis moves beyond deadband.
        if ((abs(axisA) >= 20) || (abs(axisB) >= 20)) {
            BrainTimer.reset();
        }
        if (BrainTimer.value() > 120) {
            Brain.programStop();
        }

        if (!OverRideDriveTrain) {
            if(abs(axisA) > 90) {
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
                stickB = axisB;
            }
            // turnFactor = 0.7 * speedFactor;
            if (ReverseDir) {
#if side == redSide
                TouchLED12.setColor(red);
#else
                TouchLED12.setColor(blue);
#endif
                mot_dtRight.setVelocity((-axisA * speedFactor + (stickB * turnFactor)) , percent);
                mot_dtLeft.setVelocity((-axisA * speedFactor - (stickB * turnFactor)) , percent);
            }
            else {
                TouchLED12.setColor(orange);
                mot_dtRight.setVelocity((axisA * speedFactor + (stickB * turnFactor)) , percent);
                mot_dtLeft.setVelocity((axisA * speedFactor - (stickB * turnFactor)) , percent);
            }
            if(Brain.Battery.capacity() < 70){
                TouchLED12.setColor(purple);
            }
   
            mot_dtLeft.spin(forward);
            mot_dtRight.spin(forward);
        }
        wait(20, msec);
    }
    return 0;
}
