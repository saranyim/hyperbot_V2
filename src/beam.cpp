#include "vex.h"
#include "main.h"
using namespace vex;
#define spinBeamUp reverse
#define spinBeamDown forward
#define beamArmDownTorque 10
bool f1stLup;

// User defined function
void myblockfunction_Grab_Beam_up() {
    // printf("beam up");
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
    // mg_beam.spin(forward);
    // wait(20, msec);
    // while(mg_beam.velocity(percent) > 0) {
    //     wait(20, msec);
    // }
    // mg_beam.stop();
    pneuVGrabber.extend(cylinder1);
    wait(0.2, seconds);
   
    // mg_beam.setTimeout(2.5, seconds);
    mg_beam.setStopping(hold);
    mg_beam.spin(reverse);
    wait(1, seconds);
    while(mg_beam.velocity(percent) < 0) { // reverse direction velocity is negative
        // printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) * 10));
        wait(20, msec);
    }
    // printf("Stop beam\n");
        
    mg_beam.stop();
    
    beamPos = top;
    beamGraber = grab;
}

// User defined function
void myblockfunction_Place_beam() {

    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.1, seconds);
    // move beam down
    printf("beam down\n");
    mg_beam.setMaxTorque(5.0, percent);
    mg_beam.setVelocity(100, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.stop();
    mg_beam.spin(forward);
    wait(0.5, seconds);

    while(mg_beam.velocity(percent) > 36) {
        wait(20, msec);
        printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) * 10));
    }
    mg_beam.setStopping(hold);
    mg_beam.stop();
    printf("stop\n");
    pneuVGrabber.retract(cylinder1);
    printf("beam relase and move away\n");
    // wait(5,seconds);
    // mg_beam.spinFor(forward, 80.0, degrees, true);
    // release beam
    
    // move robot away
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor(reverse, 60.00000, degrees, true);
    printf("Spinupfinished\n");
    mot_dtLeft.setVelocity(50.0, percent);
    mot_dtRight.setVelocity(50.0, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.8, seconds);
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
    mg_beam.spin(forward);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
   
    // get arm to proprer down position
    mg_beam.setVelocity(beamArmDownTorque, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(forward);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();

    beamPos = bottom;
    beamGraber = release;
}

// User defined function
void myblockfunction_Drop_down_beam() {
    pneuVGuide.retract(cylinder2);
   // put beam arm down 
    mg_beam.setVelocity(100, percent);
    mg_beam.setStopping(coast);
    ReverseDir = true;
    mg_beam.spin(forward);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    // get arm to proprer down position
    mg_beam.setVelocity(beamArmDownTorque, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(forward);
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
        mg_beam.spin(forward);
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
    mg_beam.spin(forward);
    wait(0.2, seconds);
    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();

    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,360,degrees);
    beamPos=mid;

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
                else {ReverseDir = false;}
                
        
                myblockfunction_Grab_Beam_up();
            } else if (beamPos == top) {
                myblockfunction_Place_beam();
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
                myblockfunction_Drop_down_beam();
                beamPos = bottom;
                ReverseDir = true;
            }
            fBtnLdownPressed = false;
        }
        if(Controller.AxisD.position() > 80){
            mg_beam.setMaxTorque(100.0, percent);
            mg_beam.setVelocity(100.0, percent);
            mg_beam.setStopping(hold);
            mg_beam.spin(reverse);
            
        }
        else if(Controller.AxisD.position() < -80){
            mg_beam.setMaxTorque(10.0, percent);
            mg_beam.setVelocity(10.0, percent);
            mg_beam.setStopping(hold);
            mg_beam.spin(forward);
           
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