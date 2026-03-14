/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Kin                                                 */
/*    Created:      11/4/2025, 3:32:57 PM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "pin.h"
#include "beam.h"
#include "drive.h"
#include "autonomous.h"
using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;
vex::controller  Controller;
vex::inertial Inertial;


motor mot_dtLeft( vex::PORT1);
motor mot_dtRight( vex::PORT2, true);

motor mot_pinLeft( vex::PORT8 );
motor mot_pinRight( vex::PORT7 , true);

motor mot_beamLeft( vex::PORT4 );
motor mot_beamRight( vex::PORT5, true );


motor_group mg_pin( mot_pinLeft, mot_pinRight );
motor_group mg_beam( mot_beamLeft, mot_beamRight );
// define your global instances of motors and other devices here
pneumatic pneuVGuide( vex::PORT11 );
pneumatic pneuVGrabber( vex::PORT12 );

distance dis_rear( vex::PORT10 );
touchled TouchLED12( vex::PORT9 );

position_t pinPos;
position_t beamPos;
graber_t pinGraber;
graber_t beamGraber;

timer BrainTimer;


bool OverRideDriveTrain,ReverseDir;
int Screen_precision = 0, Console_precision = 0;

static vex::mutex gYGuidCmdMutex;
volatile bool gPlaceBeam2StackRunning = false;

// Serialize yGuide retract command so multiple tasks cannot race on the same pneumatic channel.
void YGuidInSafe() {
    printf("YGuidInSafe\n");
    gYGuidCmdMutex.lock();
    yGuidIn;
    gYGuidCmdMutex.unlock();
}

// Block yGuide extend while Place_Beam_2_Stack is running to avoid command fighting.
void YGuidOutSafe() {
    printf("YGuidOutSafe\n");
    timer waitTimeout;
    waitTimeout.reset();
    while(gPlaceBeam2StackRunning) {
        if(waitTimeout.time(msec) >= 1000) {
            printf("[WARN] YGuidOutSafe timeout waiting Place_Beam_2_Stack\n");
            return;
        }
        wait(5, msec);
    }

    gYGuidCmdMutex.lock();
    if(!gPlaceBeam2StackRunning) {
        yGuidOut;
    }
    gYGuidCmdMutex.unlock();
}

int TaskDebug();

// Handle controller L3 press (debug placeholder).
void onevent_ControllerButtonL3_pressed_0() {
    printf("L3 Pressed\n");

}


// Toggle drive direction when EUp is pressed.
void onevent_ControllerButtonEUp_pressed_0() {
    //  Drive Direction
    BrainTimer.reset();
    if (ReverseDir) {
        ReverseDir = false;
    }
    else {
        ReverseDir = true;
    }
}




// Initialize devices and start tasks.
int main() {
   
    mg_beam.setPosition(0.0, degrees);
    
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
   
    mg_beam.setStopping(hold);
    
    mot_dtLeft.setStopping(brake);
    mot_dtRight.setStopping(brake);
    pneuVGrabber.pumpOn();
  // register event handlers
    Controller.ButtonL3.pressed(onevent_ControllerButtonL3_pressed_0);
    Controller.ButtonEUp.pressed(onevent_ControllerButtonEUp_pressed_0);

    wait(15, msec);
    vex::task ws1(TaskPin);  
    vex::task ws2(TaskBeam);
    //  vex::task wsDebug(TaskDebug);
    TaskDriveTrain();
    // TaskAutonomous();
}

// Print rear distance sensor reading.
void PrintDistance(){
    printf("Distance : %u\n", (uint16_t)dis_rear.objectDistance(mm));
    
}

// Periodic debug output task.
int TaskDebug() {
    while(1){
        // printf("Dis Left: %u ", (uint16_t)dis_rear.objectDistance(mm));
        // printf(" Dis Right: %u", (uint16_t)dis_right.objectDistance(mm));
        // printf(" heading: %u\n", (uint16_t)Inertial.angle());
        // printf(" mot_left Pos: %d ", (uint16_t)mot_dtLeft.position(degrees));
        // printf(" mot_right Pos: %d \n", (uint16_t)mot_dtRight.position(degrees));
        PrintDistance();
        wait(500, msec);
    }
}

