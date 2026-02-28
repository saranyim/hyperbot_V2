#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;
#define pinGrapSpeed 10
#define pinArmDegree 300        

#define pinGrabSpeed 15
bool fSetDropPin;




// Flag the pin arm to drop to the bottom position.
void Set_Drop_Pin(){
    fSetDropPin = true;
}

// Drop pin to stack height and back away.
void DropDownMakeStack(){
      printf("drop down pin");
    printf("\n");
    
    wait(0.1, seconds);
    printf("stop mov");
    printf("\n");
  
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(80.0, percent);
    pinGuidIn;
    mg_pin.spinFor(forward, 120 , degrees, true);
    ReleasePin; 
    wait(0.1, seconds);
    printf("stop mov");
    printf("\n");
    OverRideDriveTrain = true;

    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    // wait(0.3, seconds);
    wait(0.2, seconds);
    handUp;
    // wait(0.1, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop(); 
    OverRideDriveTrain = false;

    Drop_Pin_Arm();

}

// Lower the pin arm to the drop position.
void Drop_Pin_Arm() {
    printf("drop pin arm");
    printf("\n");
   
    mg_pin.setMaxTorque(10.0, percent);
    mg_pin.setStopping(hold);
    mg_pin.setVelocity(80.0, percent);
    mg_pin.spin(forward);
    pinGuidIn;
    
    wait(0.3, seconds);
    while(mg_pin.velocity(percent) > 20.0) {
        wait(20, msec);
    }
 
    mg_pin.stop();
    wait(0.1, seconds);
   
}



// Grab the pin and lift the arm.
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
    handDown;
    // wait(0.3, seconds);
   
    // mg_pin.stop();
   
    printf("end Grab_then_up\n");
}

// Flip the pin over using the beam arm.
void Flip_Pin_Over() {
    printf("start flip\n");
    GrabPin;
// set beam arm free to move a little bit
    mg_beam.setStopping(coast);
    mg_beam.stop();
// move pin over
    pinGuidIn;
    handUp;
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
    mg_pin.setStopping(hold);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spin(forward);
    wait(0.5, seconds);
    if(fBeamMovingUp == false){
        mg_beam.setMaxTorque(100, percent);
        mg_beam.setStopping(hold);
        ReverseDir = true;
        mg_beam.spin(spinBeamUp);
        wait(0.3, seconds);
        mg_beam.stop();
    }   


    while(mg_pin.velocity(percent) > 5.0) {
        wait(20, msec);
    }
    mg_pin.stop(brake);

    pinPos = bottom;
    

}



// Toggle pin grab/release based on current position.
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

   
// Pin subsystem task for button-driven actions.
int TaskPin() {
  //  pin
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(hold);

    mg_pin.stop();
    ReleasePin;
    handUp;
    pinGuidIn;
    Drop_Pin_Arm();
    pinPos = bottom;

    while (true) {
        if (fBtnRupPressed) {
            
            Brain.Timer.reset();
            if (bottom == pinPos) {
                Grab_then_up();
                pinPos = top;
            }
            else {
                DropDownMakeStack();
                pinPos = bottom;
            }
            fBtnRupPressed = false;
        }
        else if (fBtnRdownPressed) {
            Brain.Timer.reset();
            if (bottom == pinPos) {
                GrabPin;
                mg_pin.setStopping(hold);
                mg_pin.setVelocity(100.0, percent);
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setTimeout(0.5, seconds);
                mg_pin.spinFor(reverse, 130 , degrees, true);
                // wait(1, seconds);
                // mg_pin.stop(hold);
                pinPos = mid;
            }
            else{
                handUp;
                Drop_Pin_Arm();
                ReleasePin;
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
            Grab_Release_Pin();     
            fBtnFupPressed = false;
        }
        else if(fSetDropPin == true) {
            Drop_Pin_Arm();
            fSetDropPin = false;
            ReleasePin;
            pinPos = bottom;
        }
        else if(Controller.AxisC.position() > 60) { // grab
            if(pinPos == mid){
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100.0, percent);
                GrabPin;
                mg_pin.setStopping(hold);
                wait(0.3, seconds);
                mg_pin.setTimeout(0.5, seconds);
                mg_pin.spinFor(reverse, 180 , degrees, true);
                pinPos = top;
                handDown;
            }
           
            while (Controller.AxisC.position() > 10)
            {
                /* code */
                wait(20, msec);
            }
        }
        else if(Controller.AxisC.position() < -60){ // up
            if(pinPos == bottom){
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100, percent);
                mg_pin.setStopping(hold);
                mg_pin.spinFor(reverse, 120 , degrees, false);
                pinPos = mid;
            }
            while (Controller.AxisC.position() < -10)
            {
                /* code */
                wait(20, msec);
            }
            
        }
        else if(Controller.AxisD.position() < -60){// place stack on stand off
            // if(pinPos = bottom){
            //     mg_pin.setMaxTorque(100.0, percent);
            //     mg_pin.setVelocity(100.0, percent);
            //     mg_pin.setStopping(hold);
            //     mg_pin.spinFor(reverse, 500 , degrees, false);
            //     wait(0.3, seconds);
            //     pinPos == aboveStandoff;
            //     //spin up from bottom
            // }
            // else if(pinPos == mid){
            //     //spin up from mid
            //     mg_pin.setMaxTorque(100.0, percent);
            //     mg_pin.setVelocity(100.0, percent);
            //     mg_pin.setStopping(hold);
            //     mg_pin.spinFor(reverse, 220 , degrees, false);
            //     wait(0.3, seconds);
            //     pinPos == aboveStandoff;
            // }
            // else if(pinPos == aboveStandoff){
            //     //drop down
            //     handDown;
            //     ReleasePin;
            //     Drop_Pin_Arm();
            //     pinPos = bottom;
            // }
            // while (Controller.AxisD.position() < -10)
            // {
            //     /* code */
            //     wait(20, msec);
            // }

        }

        wait(5, msec);
    }
    return 0;
   
} 

// Initial grab sequence at the start position.
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

