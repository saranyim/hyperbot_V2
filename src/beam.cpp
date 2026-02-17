#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;

#define beamArmDownTorque 10
bool f1stLup;
bool fBeamMovingUp;



/*
degree height
200 10cm
400 25cm
*/


// User defined function
void Grab_Beam_up() {
    // printf("beam up");
    fBeamMovingUp = true;
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
    // mg_beam.spin(spinBeamDown);
    // wait(20, msec);
    // while(mg_beam.velocity(percent) > 0) {
    //     wait(20, msec);
    // }
    // mg_beam.stop();
    GrabBeam;
    wait(0.2, seconds);
   
    // mg_beam.setTimeout(2.5, seconds);
    mg_beam.setStopping(hold);
    mg_beam.spinFor(spinBeamUp,530,degrees);
    
    
    beamPos = top;
    fBeamMovingUp = false;

    
}

void Place_Beam_2_Stack() {
  
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtRight.setVelocity(20, percent);
    mot_dtLeft.setVelocity(20, percent);
    mot_dtRight.setMaxTorque(100, percent);
    mot_dtLeft.setMaxTorque(100, percent);
    if((uint16_t)dis_rear.objectDistance(mm) < 45){
           mot_dtRight.setVelocity(30, percent);
        mot_dtLeft.setVelocity(30, percent);
        mot_dtRight.setMaxTorque(100, percent);
        mot_dtLeft.setMaxTorque(100, percent);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        while(IS_IN_RANGE((uint16_t)dis_rear.objectDistance(mm), 90, 110) == false) {
            wait(2, msec);
        }
        mot_dtLeft.stop(brake);
        mot_dtRight.stop(brake);
    }
    wait(0.1, seconds);
    
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(80, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();

    mg_beam.spinFor(spinBeamDown,230,degrees,true);
    mg_beam.setStopping(hold);
    // mg_beam.stop();
    printf("stop and release beam\n");
    ReleaseBeam;
    wait(0.1,seconds);
    Drop_Y_Arm();
}
// User defined function
void Place_Beam_Stand_Off() {
  
    OverRideDriveTrain = true;
    if((uint16_t)dis_rear.objectDistance(mm) < 45){
        mot_dtRight.setVelocity(20, percent);
        mot_dtLeft.setVelocity(20, percent);
        mot_dtRight.setMaxTorque(100, percent);
        mot_dtLeft.setMaxTorque(100, percent);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        while(IS_IN_RANGE((uint16_t)dis_rear.objectDistance(mm), 40, 50) == false) {
            wait(2, msec);
        }
        mot_dtLeft.stop(brake);
        mot_dtRight.stop(brake);
    }
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

    mg_beam.spinFor(spinBeamDown,100,degrees,true); 
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    ReleaseBeam ;
    wait(0.1,seconds);
    OverRideDriveTrain = false;
    Drop_Y_Arm();
}


// User defined function
void Place_Pin_On_Stand_Off() {
    OverRideDriveTrain = true;
    if((uint16_t)dis_rear.objectDistance(mm) < 45){
        mot_dtRight.setVelocity(20, percent);
        mot_dtLeft.setVelocity(20, percent);
        mot_dtRight.setMaxTorque(100, percent);
        mot_dtLeft.setMaxTorque(100, percent);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        while(IS_IN_RANGE((uint16_t)dis_rear.objectDistance(mm), 25, 35) == false) {
            wait(2, msec);
        }
        mot_dtLeft.stop(brake);
        mot_dtRight.stop(brake);
    }
    wait(0.1, seconds);
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(100, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();

    mg_beam.spinFor(spinBeamDown,130,degrees,true);
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");

    ReleaseBeam;
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
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    
    wait(0.4, seconds);
    printf("stop moving\n");

    // close pneu guide
    // beamGuideIn;

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









int TaskBeam() {
//    beam
    
    
    beamPos = bottom;
    beamGraber = release;
    pinPos = bottom;
    pinGraber = release;

    // beamGuideIn;
    ReleaseBeam;
    // while(1)
    //     wait(1,seconds);
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
    // mg_beam.setMaxTorque (50,percent);
    // mg_beam.setVelocity (100,percent);
    // mg_beam.spinFor (spinBeamUp,250,degrees,false);
    // wait(1, seconds);
    // while(mg_beam.velocity(percent) > 0) {
    //     wait(20, msec);
    // }
    // mg_beam.stop();

    // beamPos=mid;
    beamPos = bottom;
    wait(1,seconds);
    printf("Task Beam Start");
    printf("\n");
    // f1stLup=true;
    while (true) {
        if (fBtnLupPressed) {
            printf("L UP");
            printf("\n");
            Brain.Timer.reset();
            if ((beamPos == bottom) || (beamPos == mid)) {
                beamPos = top;               
                ReverseDir = false;
                Set_Drop_Pin();
                printf("grab beam up\n");
                Grab_Beam_up();
            } 
            else if (beamPos == top) {
                printf("place beam stand off\n");
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
            if (grab == beamGraber) {
                ReleaseBeam;
            }
            else {
                GrabBeam;
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
            
    
            printf("up vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
            f1stLup=false;
        }
        else if(Controller.AxisD.position() < -80){
            Place_Pin_On_Stand_Off();
            printf("down vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
        }
        // else if(Controller.AxisC.position() < -80){
        //     mini_Y();
        // }

        
        
       
    wait(20, msec);
    }
    return 0;
}    

void mini_Y() {
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.setMaxTorque(100, percent);
    mot_dtLeft.setMaxTorque(100, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(1.0, seconds);
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
  
    mg_beam.spinFor(spinBeamDown,400,degrees,true);
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    ReleaseBeam;
    wait(0.1,seconds);
    Drop_Y_Arm();
}


