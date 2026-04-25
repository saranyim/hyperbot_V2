#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;



#define beamArmDownTorque 10
bool f1stLup;
bool fBeamMovingUp;

// Wait until beam speed is below threshold, or exit if speed is unchanged for 0.1s.
static bool WaitBeamStopOrNoSpeedChange(double stopThresholdPct) {
    timer stableTimer;
    stableTimer.reset();
    double prevSpeed = abs(mg_beam.velocity(percent));

    while(abs(mg_beam.velocity(percent)) > stopThresholdPct) {
        double currSpeed = abs(mg_beam.velocity(percent));
        if(abs(currSpeed - prevSpeed) < 0.5) {
            if(stableTimer.time(msec) >= 100) {
                printf("[WARN] Beam speed unchanged for 0.1s, break wait loop\n");
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
    GrabBeam;
    wait(0.2, seconds);
   
    // mg_beam.setTimeout(2.5, seconds);
    mg_beam.setStopping(hold);

        mg_beam.spin(spinBeamUp);
        WaitBeamStopOrNoSpeedChange(5);
    // }
    wait(1,seconds);
 
    beamPos = top;
    fBeamMovingUp = false;
    fBeamGuideOut = false;
    YGuidInSafe();
    
}

// Place beam on stack using rear distance alignment.
void Place_Beam_2_Stack() {
//     // Place step needs guide OUT first, then retract guide only after release.
//     YGuidOutSafe();
//     gPlaceBeam2StackRunning = true;
   
//     // move beam down
//     // yGuidIn;
//     wait(0.2,seconds);
//     printf("beam down\n");
//     mg_beam.setMaxTorque(100.0, percent);
//     mg_beam.setVelocity(80, percent);
//     mg_beam.setStopping(coast);
//     ReverseDir = true;
//     mg_beam.stop();
// #if side == redSide
//     mg_beam.spin(spinBeamDown);
// #else
    
// #endif
//     wait(0.3, seconds);
//     mg_beam.setStopping(hold);
//     mg_beam.stop();
//     // WaitBeamStopOrNoSpeedChange(3.0);

//     // mg_beam.stop();
//     printf("stop and release beam\n");
//     ReleaseBeam;
//     wait(0.1,seconds);
    
//     // YGuidInSafe();
//     // wait(0.5, seconds); // wait pnumatic guide to retract finish
    
//     Drop_Y_Arm(); // less priority allow other task to control the guide if needed, since guide is not used in other place except stack placement
//     gPlaceBeam2StackRunning = false;
//     if(pinPos == top) {
//         // If pin already returned to top during stack placement, force guide back out.
//         YGuidOutSafe();
//     }
}

void Place_Beam_Stand_Off() {
    printf("beam down\n");
    OverRideDriveTrain = true;
    mot_dtLeft.setVelocity(0,percent);
    mot_dtRight.setVelocity(0,percent);
    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
    wait(0.2,seconds);
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(50, percent);
    // mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
#if robot_number == 1
    mg_beam.spinFor(spinBeamDown,50,degrees,true); 
#elif robot_number == 6
    mg_beam.spinFor(spinBeamDown,40,degrees,true); 
#endif

    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    wait(0.1,seconds);
    ReleaseBeam ;
  
    OverRideDriveTrain = false;
    Drop_Y_Arm();
}




// Place pin on standoff using rear alignment.
void Place_Pin_On_Stand_Off() {
    // OverRideDriveTrain = true;
    // if((uint16_t)dis_rear.objectDistance(mm) < 45){
    //     mot_dtRight.setVelocity(20, percent);
    //     mot_dtLeft.setVelocity(20, percent);
    //     mot_dtRight.setMaxTorque(100, percent);
    //     mot_dtLeft.setMaxTorque(100, percent);
    //     mot_dtLeft.spin(reverse);
    //     mot_dtRight.spin(reverse);
    //     timer rangeTimer;
    //     rangeTimer.reset();
    //     while(IS_IN_RANGE((uint16_t)dis_rear.objectDistance(mm), 25, 35) == false) {
    //         if(rangeTimer.time(msec) > 3000) {
    //             break;
    //         }
    //         wait(2, msec);
    //     }
    //     mot_dtLeft.stop(brake);
    //     mot_dtRight.stop(brake);
    // }
    // wait(0.1, seconds);
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
    
    // wait(0.5, seconds);
    // move arm up a little to release any tension
    // YGuidInSafe();
        mg_beam.setMaxTorque(100, percent);   
    mg_beam.spin(spinBeamUp);
    wait(0.3, seconds);
    printf("Spinupfinished\n");
    mg_beam.stop();
    OverRideDriveTrain = true;
    // move robot away

    

    printf("stop moving\n");
    // close pneu guide
    // beamGuideIn;
    beamGraber = grab;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;
    // put beam arm down 
    
    // Re-assert guide retract right before lowering in case another task toggled it.
    
    YGuidInSafe();
    wait(0.5, seconds);

    printf("beam down\n");
    mg_beam.setVelocity(100, percent);
    mg_beam.setMaxTorque(10, percent);
    mg_beam.setStopping(brake);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    WaitBeamStopOrNoSpeedChange(5.0);
    mg_beam.setStopping(coast);
    mg_beam.stop();


    beamPos = bottom;
    beamGraber = release;
}









// Beam subsystem task for button-driven actions.
int TaskBeam() {
    bool btnLUpPressed = false;
    bool btnLDownPressed = false;
    bool btnFDownPressed = false;

    YGuidInSafe();
    wait(1,seconds);
    beamPos = bottom;
    beamGraber = release;
    pinPos = bottom;
    pinGraber = release;

    ReleaseBeam;

    mg_beam.setMaxTorque(20.0, percent);
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);
    WaitBeamStopOrNoSpeedChange(0.5);
     mg_beam.setStopping(coast);
    mg_beam.stop();
    
    wait(0.5,seconds);

    beamPos = bottom;
    wait(1,seconds);
    printf("Task Beam Start");
    printf("\n");
    // f1stLup=true;
    while (true) {
        const bool lUpNow = Controller.ButtonLUp.pressing();
        const bool lDownNow = Controller.ButtonLDown.pressing();
        const bool fDownNow = Controller.ButtonFDown.pressing();

        const bool lUpEvent = lUpNow && !btnLUpPressed;
        const bool lDownEvent = lDownNow && !btnLDownPressed;
        const bool fDownEvent = fDownNow && !btnFDownPressed;

        btnLUpPressed = lUpNow;
        btnLDownPressed = lDownNow;
        btnFDownPressed = fDownNow;

        if (lUpEvent) {
            printf("L UP");
            printf("\n");
            Brain.Timer.reset();

            if (beamPos == top) {
                printf("place beam stand off\n");
#if side == redSide
                Place_Beam_Stand_Off();
                
#else
                Place_Beam_Stand_Off();
#endif
                beamPos = bottom;
            } 
            else {
#if side == buleSide
                Grab_Beam_up_121();
#endif
            }
        }
        else if (lDownEvent) {
            if(beamPos == top ){
                printf("L Down");
                printf("\n");
                Brain.Timer.reset();
                Place_Beam_2_Stack();
                
                beamPos = bottom;
                ReverseDir = true;
            }
        }
        else if(fDownEvent) {
            if (grab == beamGraber) {
               beamPos = bottom;
// #if side == redSide
//                 YGuidInSafe();
//                 wait(0.5,seconds);
// #endif

                mg_beam.setVelocity(100, percent);
                mg_beam.setMaxTorque(10, percent);
                mg_beam.setStopping(brake);
                ReverseDir = true;
                mg_beam.spin(spinBeamDown);
                wait(0.2, seconds);

                WaitBeamStopOrNoSpeedChange(5.0);
                mg_beam.stop();
                ReleaseBeam;
            }
            else {
                beamPos = posFloat;
                GrabBeam;
                wait(0.5, seconds);
                mg_beam.setVelocity(100, percent);
                mg_beam.setMaxTorque(100, percent);
                mg_beam.setStopping(hold);
                ReverseDir = true;
                mg_beam.spinFor(spinBeamUp,100,degrees,false);
                wait(0.3, seconds);
                mg_beam.stop();
                
            }
        }
        else if(Controller.AxisD.position() > 80) {
             
    
            
            YGuidInSafe();
            wait(0.1,seconds);
    
            mg_beam.setVelocity(100, percent);
            mg_beam.setMaxTorque(10, percent);
            mg_beam.setStopping(brake);
            ReverseDir = true;
            mg_beam.spin(spinBeamDown);
            wait(0.1, seconds);
            WaitBeamStopOrNoSpeedChange(5.0);
            mg_beam.stop();
            printf("up vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
            f1stLup=false;
            beamPos = bottom;
        }
        // else if(Controller.AxisD.position() < -80){
        //     Place_Pin_On_Stand_Off();
        //     printf("down vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
        // }
  
        
        
    
    wait(20, msec);
    }
    return 0;
}
