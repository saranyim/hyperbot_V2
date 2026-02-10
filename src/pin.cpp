
#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;
#define pinGrapSpeed 10
#define pinArmDegree 280    

bool fSetDropPin;



void Set_Drop_Pin(){
    fSetDropPin = true;
}
// User defined function
void Drop_Down_Pin_Grab_Up() {
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
   
    wait (0.1, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtLeft.setVelocity(50, percent);
    mot_dtRight.setVelocity(50, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(0.3, seconds);
    

    
    
    Drop_Down_Pin();
    
    mot_dtLeft.stop();
    mot_dtRight.stop();
    
    printf("drop and up");
    printf("\n");
    mg_pin.setVelocity(50.0, percent);
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);

    wait(0.4, seconds);
    // GrabPin;      
    // wait(0.1, seconds);
    
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
    Grab_then_up();
   
}


// User defined function
void Drop_Pin_Arm() {
    printf("drop pin arm");
    printf("\n");
   
    mg_pin.setMaxTorque(10.0, percent);
    mg_pin.setStopping(brake);
    mg_pin.setVelocity(40.0, percent);
    mg_pin.spin(forward);
    pinGuidIn;
    
    wait(0.3, seconds);
    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
 
    mg_pin.stop();
    
   
}


// User defined function
void Drop_Down_Pin() {
    printf("drop down pin");
    printf("\n");
    // OverRideDriveTrain = true;
    wait(0.1, seconds);
    printf("stop mov");
    printf("\n");
    mot_dtRight.stop();
    mot_dtLeft.stop();
    mg_pin.setMaxTorque(40.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(40.0, percent);
    mg_pin.spin(forward);
 
    
    wait(0.2, seconds);
    // pneuVGrabber.extend(cylinder1);
    
    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
    ReleasePin; 

    
    wait(0.2, seconds);
    
    while(mg_pin.velocity(percent) > 5.0) {
        wait(20, msec);
    }
    
    mg_pin.setStopping(brake);
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
    GrabPin;
    mg_pin.setStopping(hold);
    wait(0.3, seconds);
   
    mg_pin.setTimeout(0.5, seconds);
    mg_pin.spinFor(reverse, pinArmDegree , degrees, true);
    pinGuidOut;
    // wait(0.3, seconds);
   
    // mg_pin.stop();
   
    printf("end Grab_then_up\n");
}

// User defined function
void Flip_Pin_Over() {
    printf("start flip\n");
    GrabPin;
// set beam arm free to move a little bit
    mg_beam.setStopping(coast);
    mg_beam.stop();
// move pin over
    pinGuidIn;
    mg_beam.spin(forward);
    wait(0.2, seconds);
    mg_beam.stop();
    GrabPin;
    mg_beam.setTimeout(1.0, seconds);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.spin(reverse);
    wait(1.5, seconds);
    printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));
       
    while((int16_t)mg_pin.velocity(percent) < 0) {
        printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));
        wait(20, msec);
    }
    mg_pin.stop();
    ReleasePin;
    mg_pin.setTimeout(1.0, seconds);
    mg_pin.setStopping(coast);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spin(forward);
    wait(0.4, seconds);
    if(fBeamMovingUp == false){
        mg_beam.setMaxTorque(100, percent);
        mg_beam.setVelocity(100, percent);
        mg_beam.setStopping(hold);
        mg_beam.spinFor(reverse, 50, degrees, false);
    }   


    while(mg_pin.velocity(percent) > 5.0) {
        wait(20, msec);
    }
    mg_pin.stop(brake);

    pinPos = bottom;
    

}



// "when Controller ButtonFUp pressed" hat block
void Grab_Release_Pin() {
    // Pinclaw
    Brain.Timer.reset();
    if (bottom == pinPos) {
        if (grab == pinGraber) {
            ReleasePin;
        }
        else {
            GrabPin;
        }
    }
    else if(mid == pinPos) {
        ReleasePin;
    }
    else {
        GrabPin;
    }
}

   
// "when started" hat block
int TaskPin() {
  //  pin
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(hold);

    mg_pin.stop();
    ReleasePin;
    mg_pin.spin(reverse);
    wait(0.2, seconds);
    mg_pin.stop();
    pinGuidIn;
    Drop_Down_Pin();
    pinPos = bottom;
    pinGuidOut;
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
            if (beamPos == bottom) {
                Flip_Pin_Over();
            }
            fBtnEdownPressed = false;
        }
        else if(fBtnFupPressed) {
            // check flip only if pin is at bottom
            
            if(pinPos == top) {
                pinGuidIn;
                // pneuVGuide.retract(pneuCPinGuide);
                wait(0.2, seconds);
                mg_pin.setMaxTorque(100.0, percent);
                pinPos = mid;
                mg_pin.spinFor(forward, 160 , degrees, false);
            }
            else{
                Grab_Release_Pin();
            }
            
            fBtnFupPressed = false;
        }
        else if(fSetDropPin == true) {
            Drop_Pin_Arm();
            fSetDropPin = false;
            pinPos = bottom;
        }
        // else if(Controller.AxisC.position() > 80) {
        //     GrabBeam;
        // }
        // else if(Controller.AxisC.position() < -80){
        //     ReleaseBeam;
        // }


        wait(5, msec);
    }
    return 0;
   
} 

void Grab_From_Starting() {

    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setStopping(hold);
    wait(0.3, seconds);
    GrabPin;
    mg_pin.setTimeout(0.5, seconds);
    mg_pin.spinFor(reverse, 100 , degrees, false);
    wait(0.3, seconds);
    pinGuidOut;

}




                                                                                                                                                                                                    







