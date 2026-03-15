#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;
#define pinGrapSpeed 10
#define pinArmDegree 260        

#define pinGrabSpeed 15
bool fSetDropPin;

// Wait until pin speed is below threshold, or exit if speed is unchanged for 0.1s.
static bool WaitPinStopOrNoSpeedChange(double stopThresholdPct) {
    timer stableTimer;
    stableTimer.reset();
    double prevSpeed = abs(mg_pin.velocity(percent));

    while(abs(mg_pin.velocity(percent)) > stopThresholdPct) {
        double currSpeed = abs(mg_pin.velocity(percent));
        if(abs(currSpeed - prevSpeed) < 0.5) {
            if(stableTimer.time(msec) >= 100) {
                printf("[WARN] Pin speed unchanged for 0.1s, break wait loop\n");
                return false;
            }
        }
        else {
            prevSpeed = currSpeed;
            stableTimer.reset();
        }
        wait(20, msec);
    }
    return true;
}




// Flag the pin arm to drop to the bottom position.
void Set_Drop_Pin(){
    fSetDropPin = true;
}

void WaitRup() {
    while(Controller.ButtonRUp.pressing() == false) {
        wait(20, msec);
    }
    while(Controller.ButtonRUp.pressing() == true) {
        wait(20, msec);
    }
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
   
    mg_pin.spinFor(forward, 80 , degrees, true);
    ReleasePin; 
    wait(0.1, seconds);
    YGuidInSafe();
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
    YGuidInSafe();
    mg_pin.setMaxTorque(10.0, percent);
    mg_pin.setStopping(hold);
    mg_pin.setVelocity(80.0, percent);
    mg_pin.spin(forward);

    
    wait(0.3, seconds);
    WaitPinStopOrNoSpeedChange(10.0);
    wait(0.1, seconds);
    mg_pin.stop();
    // wait(0.1, seconds);
    printf("done\n");
   
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
    mg_pin.spinFor(reverse, pinArmDegree , degrees, false);
    wait(0.2, seconds);

    handDown;
    wait(0.2, seconds);
    YGuidOutSafe();
    // wait(0.3, seconds);
   
    // mg_pin.stop();
   
    printf("end Grab_then_up\n");
}

// Flip the pin over using the beam arm.
void Flip_Pin_Over() {
    printf("start flip\n");
    GrabPin;
    YGuidInSafe();
// set beam arm free to move a little bit
    mg_beam.setStopping(coast);
    mg_beam.stop();
// move pin over

    handUp;
    mg_beam.spin(forward);
    wait(0.2, seconds);
    mg_beam.stop();
    GrabPin;
    mg_beam.setTimeout(1.0, seconds);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.spin(reverse);
    YGuidInSafe();
    wait(0.5, seconds);
    printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));

    timer flipStableTimer;
    flipStableTimer.reset();
    double prevFlipSpeed = abs(mg_pin.velocity(percent));

    while(abs(mg_pin.velocity(percent)) > 10) {
        double currSpeed = abs(mg_pin.velocity(percent));

        YGuidInSafe();
        printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));

        if(abs(currSpeed - prevFlipSpeed) < 0.5) {
            if(flipStableTimer.time(msec) >= 100) {
                printf("[WARN] Pin speed unchanged for 0.1s, break wait loop\n");
                break;
            }
        }
        else {
            prevFlipSpeed = currSpeed;
            flipStableTimer.reset();
        }
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
#if side == redSide
    // wait(1,seconds);
    Grab_Beam_up();
#else
    if(fBeamMovingUp == false){
        mg_beam.setMaxTorque(100, percent);
        mg_beam.setStopping(hold);
        ReverseDir = true;
        mg_beam.spin(spinBeamUp);
        wait(0.3, seconds);
        mg_beam.stop();
    }   

#endif
    WaitPinStopOrNoSpeedChange(5.0);
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
        bool btnRUpPressed = false;
        bool btnRDownPressed = false;
        bool btnEDownPressed = false;
        bool btnFUpPressed = false;

  //  pin
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(hold);

    mg_pin.stop();
    ReleasePin;
    handUp;
  
    Drop_Pin_Arm();
    pinPos = bottom;

    while (true) {
        const bool rUpNow = Controller.ButtonRUp.pressing();
        const bool rDownNow = Controller.ButtonRDown.pressing();
        const bool eDownNow = Controller.ButtonEDown.pressing();
        const bool fUpNow = Controller.ButtonFUp.pressing();

        const bool rUpEvent = rUpNow && !btnRUpPressed;
        const bool rDownEvent = rDownNow && !btnRDownPressed;
        const bool eDownEvent = eDownNow && !btnEDownPressed;
        const bool fUpEvent = fUpNow && !btnFUpPressed;

        btnRUpPressed = rUpNow;
        btnRDownPressed = rDownNow;
        btnEDownPressed = eDownNow;
        btnFUpPressed = fUpNow;

        if (rUpEvent) {
            printf("R UP\n");
            Brain.Timer.reset();
            if (bottom == pinPos) {
                
                Grab_then_up();
                pinPos = top;
                YGuidOutSafe();
            }
            else {
             
                DropDownMakeStack();
                
                pinPos = bottom;
            }
            
        }
        else if (rDownEvent) {
            Brain.Timer.reset();
            if(beamPos != top)
                YGuidInSafe();
            if (bottom == pinPos) {
                GrabPin;
                wait(0.2, seconds);
                mg_pin.setStopping(hold);
                mg_pin.setVelocity(100.0, percent);
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setTimeout(0.5, seconds);
                mg_pin.spinFor(reverse, 110 , degrees, true);
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
        }   
        else if (eDownEvent) {
            Brain.Timer.reset();
            if (pinPos == bottom){
                Flip_Pin_Over();
            }
        }
        else if(fUpEvent) {
            // check flip only if pin is at bottom
            Grab_Release_Pin();     
        }
        else if(fSetDropPin == true) {
            Drop_Pin_Arm();
            fSetDropPin = false;
            ReleasePin;
            pinPos = bottom;
        }
        else if(Controller.AxisC.position() > 60) { // right grab
            if(pinPos == mid){
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100.0, percent);
                GrabPin;
                YGuidOutSafe();
                mg_pin.setStopping(hold);
                wait(0.3, seconds);
                mg_pin.setTimeout(0.5, seconds);
                mg_pin.spinFor(reverse, 140 , degrees, true);
                pinPos = top;
                handDown;
                YGuidOutSafe();
            }
            while (Controller.AxisC.position() > 10)
            {
                /* code */
                wait(20, msec);
            }
            YGuidOutSafe();
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
        else if((Controller.AxisD.position() < -60)
            && (abs(Controller.AxisC.position()) < 20)) { // place stack on stand off
            if(pinPos == bottom){
                GrabPin;
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100.0, percent);
                mg_pin.setStopping(hold);
                mg_pin.spinFor(reverse,380 , degrees, false);
                
                
                wait(0.3, seconds);
                handDown;
                pinPos = top;
                //spin up from bottom
            }
            
            else if(pinPos == top){
                //drop down
                OverRideDriveTrain = true;
                
                mot_dtLeft.setStopping(hold);
                mot_dtRight.setStopping(hold);
                mot_dtLeft.stop();
                mot_dtRight.stop(); 
                // mot_dtLeft.setVelocity(60, percent);
                // mot_dtRight.setVelocity(60, percent);
                // mot_dtLeft.spin(reverse);
                // mot_dtRight.spin(reverse);
                // wait(0.5, seconds);
                
        
                // wait(0.1, seconds);
                mg_pin.spinFor(forward,85 , degrees, false);
                // handDown;
                wait(0.3, seconds);
                ReleasePin;
                // handUp;
                OverRideDriveTrain = false;
                wait(0.2, seconds);
                handUp;
                Drop_Pin_Arm();
               
                pinPos = bottom;
                
                
                
                
            }
            
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


}

