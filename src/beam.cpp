#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;

#define beamArmDownTorque 10
bool f1stLup;
bool fBeamMovingUp;




// User defined function
void Grab_Beam_up() {
    // printf("beam up");
    fBeamMovingUp = true;
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(80.0, percent);
    // mg_beam.spin(spinBeamDown);
    // wait(20, msec);
    // while(mg_beam.velocity(percent) > 0) {
    //     wait(20, msec);
    // }
    // mg_beam.stop();
    pneuVGrabber.extend(cylinder1);
    wait(0.2, seconds);
   
    // mg_beam.setTimeout(2.5, seconds);
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamUp);
    wait(1, seconds);
     printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) ));
    while((mg_beam.velocity(percent)) < -20) { // reverse direction velocity is negative
        // printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) ));
        wait(5, msec);
    }
    // printf("Stop beam\n");
        
    mg_beam.stop();
    
    beamPos = top;
    beamGraber = grab;
    fBeamMovingUp = false;
}

void Place_Beam_2_Stack() {
  
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.setMaxTorque(100, percent);
    mot_dtLeft.setMaxTorque(100, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(0.5, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.1, seconds);
    
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(80, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(spinBeamDown);
    mg_beam.spinFor(spinBeamDown,467,degrees,true);
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    pneuVGrabber.retract(cylinder1);
    wait(0.1,seconds);
    Drop_Y_Arm();
}
// User defined function
void Place_Beam_Stand_Off() {
  
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.1, seconds);
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(50, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(spinBeamDown);
    mg_beam.spinFor(spinBeamDown,75,degrees,true); 
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    pneuVGrabber.retract(cylinder1);
    wait(0.1,seconds);
    OverRideDriveTrain = false;
    Drop_Y_Arm();
}


// User defined function
void Place_Pin_On_Stand_Off() {
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.1, seconds);
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(100, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(spinBeamDown);
    mg_beam.spinFor(spinBeamDown,185,degrees,true);
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");

    pneuVGrabber.retract(cylinder1);
    wait(0.1,seconds);
    Drop_Y_Arm();
}

// User defined function
void Drop_Y_Arm() {
   
    // move arm up a little to release any tension
    mg_beam.setMaxTorque(100, percent);   
    mg_beam.spin(spinBeamUp);
    wait(0.5, seconds);
    printf("Spinupfinished\n");
    mg_beam.stop();
    OverRideDriveTrain = true;

    // move robot away
    mot_dtLeft.setMaxTorque(100.0, percent);
    mot_dtRight.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(100.0, percent);
    mot_dtRight.setVelocity(100.0, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.6, seconds);
    printf("stop moving\n");

    // close pneu guide
    pneuVGuide.retract(cylinder2);
    beamGraber = grab;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
    // put beam arm down 
    printf("beam down\n");
    mg_beam.setVelocity(100, percent);
    mg_beam.setMaxTorque(10, percent);
    mg_beam.setStopping(brake);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 5) {
        wait(20, msec);
    }
    mg_beam.stop();

    beamPos = bottom;
    beamGraber = release;
}


// "when Controller ButtonFDown pressed" hat block
void Grab_Beam() {
  // beam grabber
    Brain.Timer.reset();
    if ((grab == beamGraber) && (bottom == beamPos)) {
        beamGraber = release;
        pneuVGrabber.retract(pneuCBeamGrab);
    }
    else {
        if (release == beamGraber) {
        beamGraber = grab;
        mg_beam.spin(spinBeamDown);
        OverRideDriveTrain = true;
        wait(0.1, seconds);
        mot_dtLeft.setVelocity(60.0, percent);
        mot_dtRight.setVelocity(60.0, percent);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        wait(0.1, seconds);
        pneuVGrabber.extend(pneuCBeamGrab);
        wait(0.1, seconds);
        mg_beam.stop();
        mot_dtLeft.stop();
        mot_dtRight.stop();
        OverRideDriveTrain = false;
        }
    }
}

void Prep_Load_Pin() {
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,500,degrees,true);
    beamPos=top;

}


int TaskBeam() {
//    beam
    
    
    beamPos = bottom;
    beamGraber = release;
    pinPos = bottom;
    pinGraber = release;

    pneuVGuide.retract(cylinder2);
// move arm down
    mg_beam.setMaxTorque(20.0, percent);
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);
    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    wait(0.5,seconds);
    mg_beam.setMaxTorque (50,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,540,degrees,false);
    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    
    beamPos=mid;
    wait(1,seconds);
    printf("Task Beam Start");
    printf("\n");
    f1stLup=true;
    while (true) {
        if (fBtnLupPressed) {
            printf("L UP");
            printf("\n");
            Brain.Timer.reset();
            if ((beamPos == bottom) || (beamPos == mid)) {
                beamPos = top;
                if (f1stLup==true)
                {
                    f1stLup=false;
            
                }
                else {
                    ReverseDir = false;
                    Set_Drop_Pin();
                }
                // drop pin down when lifting beam
                Grab_Beam_up();
            } 
            else if (beamPos == top) {
                Place_Beam_Stand_Off();
                beamPos = bottom;
            } 
            else {
                mg_beam.stop();
            }
            fBtnLupPressed = false;
        }
        else if (fBtnLdownPressed ) {
            if(beamPos == top ){
                printf("L Down");
                printf("\n");
                Brain.Timer.reset();
                Place_Beam_2_Stack();
                
                beamPos = bottom;
                ReverseDir = true;
            }
            fBtnLdownPressed = false;
        }
        else if(fBtnFdownPressed) {
            // printf("F Down\n");
            // printf("\n");
            // Grab_Beam();
            if (grab == beamGraber) {
                beamGraber = release;
                pneuVGrabber.retract(pneuCBeamGrab);
            }
            else {
                beamGraber = grab;
                pneuVGrabber.extend(pneuCBeamGrab);
            }
            fBtnFdownPressed = false;
        }
        else if(Controller.AxisD.position() > 80){
            mg_beam.setVelocity(100, percent);
            mg_beam.setMaxTorque(10, percent);
            mg_beam.setStopping(brake);
            ReverseDir = true;
            mg_beam.spin(spinBeamDown);
            wait(0.2, seconds);

            while(mg_beam.velocity(percent) > 5) {
                wait(20, msec);
            }
            mg_beam.stop();

            beamPos = bottom;
            beamGraber = release;
    
            printf("up vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
            f1stLup=false;
        }
        else if(Controller.AxisD.position() < -80){
            Place_Pin_On_Stand_Off();
            printf("down vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
        }
        
        
       
    wait(20, msec);
    }
    return 0;
}    


