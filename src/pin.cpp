
#include "vex.h"
#include "main.h"
#include "pin.h"
using namespace vex;
#define pinGrapSpeed 50
#define pinArmDegree 155    

bool fSetDropPin;
void Grab_then_up() ;
void ReleasePin() {
    pneuVGrabber.extend(pneuCPinGrab);
    pinGraber = release;
}

void GrabPin() {
    pneuVGrabber.retract(pneuCPinGrab);
    pinGraber = grab;
}   

void Set_Drop_Pin(){
    fSetDropPin = true;
}
// User defined function
void Drop_Down_Pin_Grab_Up() {
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(40, percent);
    mot_dtRight.setVelocity(40, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);

    wait (0.6, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
    
    Drop_Down_Pin();
    printf("drop and up");
    printf("\n");
    mg_pin.setVelocity(50.0, percent);
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(pinGrapSpeed, percent);
    mot_dtRight.setVelocity(pinGrapSpeed, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);

    wait(0.3, seconds);
    GrabPin();   
    wait(0.3, seconds);
    
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
    Grab_then_up();
   
}


// User defined function
void Drop_Pin_Arm() {
    printf("drop down");
    printf("\n");
   
    mg_pin.setMaxTorque(10.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(40.0, percent);
    mg_pin.spin(forward);
    pneuVGuide.retract(cylinder1);
    
    wait(0.3, seconds);
    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
    mg_pin.stop();
    
   
}


// User defined function
void Drop_Down_Pin() {
    printf("drop down");
    printf("\n");
    OverRideDriveTrain = true;
    wait(0.1, seconds);
    printf("stop mov");
    printf("\n");
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(10.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(40.0, percent);
    mg_pin.spin(forward);
    pneuVGuide.retract(cylinder1);
    
    wait(0.3, seconds);
    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
    mg_pin.stop();
    
    ReleasePin();
   
    
    mg_pin.spin(forward);
    wait(0.2, seconds);
    while(mg_pin.velocity(percent) > 5.0) {
        wait(20, msec);
    }
    mg_pin.stop();
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
}

// User defined function
void Grab_then_up() {
    printf("Grab_then_up\n");

    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setVelocity(100.0, percent);
    GrabPin();
    mg_pin.setStopping(hold);
    wait(0.3, seconds);
    mg_pin.setTimeout(0.5, seconds);
    mg_pin.spinFor(reverse, pinArmDegree, degrees, false);
    wait(0.2, seconds);
    pneuVGuide.extend(cylinder1);
   
    // mg_pin.stop();
   
    printf("end Grab_then_up\n");
}

// User defined function
void Flip_Pin_Over() {
    GrabPin();
// set beam arm free to move a little bit
    mg_beam.setStopping(coast);
    mg_beam.stop();
// move pin over
    pneuVGuide.retract(cylinder1);
    mg_beam.spin(forward);
    wait(0.2, seconds);
    mg_beam.stop();
    pneuVGrabber.extend(cylinder1);
    mg_beam.setTimeout(1.0, seconds);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.spin(reverse);
    wait(1.5, seconds);

    while(mg_pin.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_pin.stop();
    ReleasePin();
    mg_pin.setTimeout(1.0, seconds);
    mg_pin.setStopping(coast);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spin(forward);
    wait(0.4, seconds);

    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
    mg_pin.stop();

    pinPos = bottom;
}



// "when Controller ButtonFUp pressed" hat block
void Grab_Release_Pin() {
    // Pinclaw
    Brain.Timer.reset();
    if (bottom == pinPos) {
        if (grab == pinGraber) {
            ReleasePin();
        }
        else {
            GrabPin();
        }
    }
    else if(mid == pinPos) {
        ReleasePin();
    }
    else {
        GrabPin();
    }
}

// "when started" hat block
int TaskPin() {
  //  pin
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(hold);

    mg_pin.stop();
    ReleasePin();
    mg_pin.spinFor(reverse, 10.0, degrees, true);
    pneuVGuide.retract(cylinder1);
    // Drop_Down_Pin();
    while (true) {
        if (fBtnRupPressed) {
            Brain.Timer.reset();
            if (bottom == pinPos) {
                Grab_then_up();
                pinPos = top;
            }
            else {
                Drop_Down_Pin_Grab_Up();
                pinPos = top;
            }
            fBtnRupPressed = false;
        }
        else if (fBtnRdownPressed) {
            Brain.Timer.reset();
            if ((top == pinPos)||(mid == pinPos)) {
                Drop_Down_Pin();
                pinPos = bottom;
            }
            fBtnRdownPressed = false;
        }
        else if (fBtnEdownPressed) {
            Brain.Timer.reset();
            // if (pinPos == bottom) {
                Flip_Pin_Over();
            // }
            fBtnEdownPressed = false;
        }
        else if(fBtnFupPressed) {
            // check flip only if pin is at bottom
            
            if(pinPos == top) {

                pneuVGuide.retract(pneuCPinGuide);
                pinPos = mid;
                mg_pin.spinFor(forward, pinArmDegree/2, degrees, false);
            }
            else{
                Grab_Release_Pin();
            }
            
            fBtnFupPressed = false;
        }
        else if(fSetDropPin == true) {
            Drop_Pin_Arm();
            fSetDropPin = false;
        }
        wait(5, msec);
    }
    return 0;
}
