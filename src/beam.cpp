#include "vex.h"
#include "main.h"
#include "pin.h"
using namespace vex;
#define spinBeamUp reverse
#define spinBeamDown forward
#define beamArmDownTorque 10
bool f1stLup;

// User defined function
void Grab_Beam_up() {
    // printf("beam up");
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
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
    while(mg_beam.velocity(percent) < -1) { // reverse direction velocity is negative
        // printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) ));
        wait(20, msec);
    }
    // printf("Stop beam\n");
        
    mg_beam.stop();
    
    beamPos = top;
    beamGraber = grab;
}

// User defined function
void Place_beam() {
    double maxSpeed,beamSpeed;
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.1, seconds);
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(10, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(spinBeamDown);
    wait(0.4, seconds);
    // initial max speed
    
    maxSpeed = mg_beam.velocity(percent);
    beamSpeed = maxSpeed;
    printf("1st vel: %d\n", (uint16_t)maxSpeed);
   
    while(1) {
        wait(50, msec);
        // track max speed
        beamSpeed -= 0.1 * (beamSpeed - mg_beam.velocity(percent));
        if(beamSpeed > (maxSpeed)) {
            maxSpeed = beamSpeed;
        }
        // check speed drop 3% stop
        printf("beam vel: %d max : %d\n", (uint16_t)beamSpeed, (uint16_t)maxSpeed);
        if((maxSpeed - beamSpeed) > 5) {
            break;
        }
        
    }
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");
    pneuVGrabber.retract(cylinder1);
    wait(0.1,seconds);

    // move arm up a little to release any tension
    mg_beam.setMaxTorque(100, percent);   
    mg_beam.spin(spinBeamUp);
    wait(0.5, seconds);
    printf("Spinupfinished\n");
    mg_beam.stop();

    // move robot away
    mot_dtLeft.setMaxTorque(100.0, percent);
    mot_dtRight.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(100.0, percent);
    mot_dtRight.setVelocity(100.0, percent);
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
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
   
    // get arm to proprer down position
    mg_beam.setVelocity(beamArmDownTorque, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();

    beamPos = bottom;
    beamGraber = release;
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
    mg_beam.setVelocity(10, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(spinBeamDown);
    mg_beam.spinFor(spinBeamDown,180,degrees,true);
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop and release beam\n");

    pneuVGrabber.retract(cylinder1);
    wait(0.1,seconds);

    // move arm up a little to release any tension
    mg_beam.setMaxTorque(100, percent);   
    mg_beam.spin(spinBeamUp);
    wait(0.5, seconds);
    printf("Spinupfinished\n");
    mg_beam.stop();

    // move robot away
    mot_dtLeft.setMaxTorque(100.0, percent);
    mot_dtRight.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(100.0, percent);
    mot_dtRight.setVelocity(100.0, percent);
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
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
   
    // get arm to proprer down position
    mg_beam.setVelocity(beamArmDownTorque, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();

    beamPos = bottom;
    beamGraber = release;
}

// User defined function
void Drop_Down_Pin_beam() {
    pneuVGuide.retract(cylinder2);
   // put beam arm down 
    mg_beam.setVelocity(100, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    // get arm to proprer down position
    mg_beam.setVelocity(beamArmDownTorque, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(spinBeamDown);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();

    pneuVGrabber.retract(cylinder1);
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

// "when started" hat block
int TaskBeam() {
  //  beam
    
    
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
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,540,degrees,true);
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
                }
                // drop pin down when lifting beam
                Drop_Down_Pin();
                Grab_Beam_up();
            } else if (beamPos == top) {
                Place_beam();
                beamPos = bottom;
            } else {
                mg_beam.stop();
            }
            fBtnLupPressed = false;
        }
        else if (fBtnLdownPressed ) {
            if((beamPos == top) || (beamPos == mid)){
                printf("L Down");
                printf("\n");
                Brain.Timer.reset();
                Drop_Down_Pin_beam();
                
                beamPos = bottom;
                ReverseDir = true;
            }
            fBtnLdownPressed = false;
        }
        if(Controller.AxisD.position() > 80){
            mg_beam.setMaxTorque(100.0, percent);
            mg_beam.setVelocity(100.0, percent);
            mg_beam.setStopping(hold);
            mg_beam.spin(spinBeamUp);
            
        }
        else if(Controller.AxisD.position() < -80){
            Place_Pin_On_Stand_Off();
            printf("down vel: %d\n", (uint16_t)mg_beam.velocity(percent) );
        }
        else{
            mg_beam.stop();
        }
        if(fBtnFdownPressed) {
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
    wait(20, msec);
    }
    return 0;
}