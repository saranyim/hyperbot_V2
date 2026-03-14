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


// Raise beam arm and grab the beam.
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

    mg_beam.setVelocity(90, percent);
    mg_beam.setMaxTorque(100, percent);
    mg_beam.spin(spinBeamUp);
    wait(1.8, seconds);
    mg_beam.stop();
    
    
    beamPos = top;
    fBeamMovingUp = true;

    
}

// Place beam on stack using rear distance alignment.........................
void Place_Beam_2_Stack() {
  
    OverRideDriveTrain = true;
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(50, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtRight.setVelocity(20, percent);
    mot_dtLeft.setVelocity(20, percent);
    mot_dtRight.setMaxTorque(100, percent);
    mot_dtLeft.setMaxTorque(100, percent);
    wait(0.3, seconds);

    mg_beam.spinFor(spinBeamDown,276,degrees,true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_beam.setStopping(hold);
    // mg_beam.stop();
    printf("stop and release beam\n");
    ReleaseBeam;
    wait(0.1,seconds);
   
    
    Drop_Y_Arm();
}
// Place beam on the standoff using rear alignment.
void Place_Beam_Stand_Off() {
  
    

   
    ReleaseBeam ;
    ReverseDir = true;
    // wait(0.2,seconds);
    // Drop_Y_Arm();
}


// Place pin on standoff using rear alignment.
void Place_Pin_On_Stand_Off() {
    OverRideDriveTrain = true;
    if((uint16_t)dis_rear.objectDistance(mm) < 45){
        mot_dtRight.setVelocity(20, percent);
        mot_dtLeft.setVelocity(20, percent);
        mot_dtRight.setMaxTorque(100, percent);
        mot_dtLeft.setMaxTorque(100, percent);
        mot_dtLeft.spin(reverse);
        mot_dtRight.spin(reverse);
        timer rangeTimer;
        rangeTimer.reset();
        while(IS_IN_RANGE((uint16_t)dis_rear.objectDistance(mm), 25, 35) == false) {
            if(rangeTimer.time(msec) > 3000) {
                break;
            }
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

// Lift slightly, back away, and lower the beam arm.
void Drop_Y_Arm() {
   
    // move arm up a little to release any tension
    mg_beam.setMaxTorque(100, percent);   
    mg_beam.spin(spinBeamUp);
    wait(0.5, seconds);
    printf("Spinupfinished\n");
    mg_beam.stop();


    // move robot away

    beamGraber = grab;
    mot_dtLeft.stop();
    mot_dtRight.stop();

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









// Beam subsystem task for button-driven actions.
int TaskBeam() {

    beamPos = bottom;
    beamGraber = release;
    pinPos = bottom;
    pinGraber = release;

    ReleaseBeam;

    mg_beam.setMaxTorque(20.0, percent);
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);
    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    
    wait(0.5,seconds);

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
            }
            else {
                GrabBeam;
                wait(0.5, seconds);
                mg_beam.setVelocity(100, percent);
                mg_beam.setMaxTorque(100, percent);
                mg_beam.setStopping(hold);
                ReverseDir = true;
                mg_beam.spin(spinBeamUp);
                wait(0.3, seconds);
                mg_beam.stop();
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
        // else if(Controller.AxisD.position() < -80){
        //     Place_Pin_On_Stand_Off();
        //     printf("down vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
        // }
  
        
        
    
    wait(20, msec);
    }
    return 0;
}    
