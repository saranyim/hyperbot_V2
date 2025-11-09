#include "vex.h"
#include "main.h"
using namespace vex;

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
    wait(0.6, seconds);
        
    while(mg_beam.velocity(percent) < 0) { // reverse direction velocity is negative
        // printf("beam vel: %d\n", (uint16_t)(mg_beam.velocity(percent) * 10));
        wait(20, msec);
    }
    // printf("Stop beam\n");
        
    mg_beam.stop();
    pneuVGuide.extend(cylinder2);
    beamPos = top;
    beamGraber = grab;
}

// User defined function
void myblockfunction_Place_beam() {
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_beam.setVelocity(50.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setStopping(hold);
    OverRideDriveTrain = true;
    wait(0.1, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_beam.spinFor(forward, 80.0, degrees, true);
    pneuVGrabber.retract(cylinder1);
    mg_beam.spinFor(reverse, 30.0, degrees, true);
    mot_dtLeft.setVelocity(50.0, percent);
    mot_dtRight.setVelocity(50.0, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.8, seconds);
    pneuVGuide.retract(cylinder2);
    beamGraber = grab;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    OverRideDriveTrain = false;

    mg_beam.setVelocity(100.0, percent);
    mg_beam.setStopping(hold);
    ReverseDir = true;
    mg_beam.spin(forward);
    wait(0.2, seconds);

    while(mg_beam.velocity(percent) > 0) {
        wait(20, msec);
    }
    mg_beam.stop();
    mg_beam.setVelocity(5.0, percent);
    mg_beam.spin(forward);
    wait(0.4, seconds);
    mg_beam.stop();
    beamPos = bottom;
    beamGraber = release;
}

// User defined function
void myblockfunction_Drop_down_beam() {
    pneuVGuide.retract(cylinder2);
    mg_pin.setTimeout(2.0, seconds);
    mg_beam.setStopping(hold);
    mg_beam.spin(forward);
    wait(1.0, seconds);
    mg_beam.stop();
    mg_beam.setPosition(0.0, degrees);
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
        wait(0.3, seconds);
        pneuVGrabber.extend(pneuCBeamGrab);
        wait(0.2, seconds);
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
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
    // move pin down
    mg_beam.setStopping(coast);
    printf("Task Beam Start");
    printf("\n");
    while (true) {
        if (fBtnLupPressed) {
            printf("L UP");
            printf("\n");
            Brain.Timer.reset();
            if (beamPos == bottom) {
                beamPos = top;
                ReverseDir = false;
                myblockfunction_Grab_Beam_up();
            } else if (beamPos == top) {
                myblockfunction_Place_beam();
                beamPos = bottom;
            } else {
                mg_beam.stop();
            }
            fBtnLupPressed = false;
        }
        if (fBtnLdownPressed ) {
            if(beamPos == top){
                printf("L Down");
                printf("\n");
                Brain.Timer.reset();
                myblockfunction_Drop_down_beam();
                beamPos = bottom;
                ReverseDir = true;
            }
            fBtnLdownPressed = false;
        }
        if(fBtnFdownPressed) {
            // printf("F Down\n");
            // printf("\n");
            Grab_Beam();
            fBtnFdownPressed = false;
        }
    wait(20, msec);
    }
    return 0;
}