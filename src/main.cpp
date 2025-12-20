/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       saranyim                                                  */
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
bool fBeamGuideOut;
motor mot_dtLeft( vex::PORT3);
motor mot_dtRight( vex::PORT9, true);

motor mot_pinLeft( vex::PORT10 );
motor mot_pinRight( vex::PORT11 , true);

motor mot_beamLeft( vex::PORT2 );
motor mot_beamRight( vex::PORT8, true );


motor_group mg_pin( mot_pinLeft, mot_pinRight );
motor_group mg_beam( mot_beamLeft, mot_beamRight );
// define your global instances of motors and other devices here
pneumatic pneuVGuide( vex::PORT1 );
pneumatic pneuVGrabber( vex::PORT6 );

distance dis_left( vex::PORT5 );
distance dis_right( vex::PORT4 );
touchled TouchLED12( vex::PORT12 );

position_t pinPos;
position_t beamPos;
graber_t pinGraber;
graber_t beamGraber;

timer BrainTimer;


bool OverRideDriveTrain,ReverseDir;
int Screen_precision = 0, Console_precision = 0;

bool fBtnFupPressed = false;
bool fBtnFdownPressed = false;
bool fBtnEupPressed = false;
bool fBtnEdownPressed = false;
bool fBtnRupPressed = false;
bool fBtnRdownPressed = false;
bool fBtnLupPressed = false; 
bool fBtnLdownPressed = false;
int TaskControl();
int TaskDebug();

// "when Controller ButtonL3 pressed" hat block
void onevent_ControllerButtonL3_pressed_0() {
    printf("L3 Pressed\n");

    if (beamPos = top) {
        // Beam is at top, allow pneumatic toggle
        if (fBeamGuideOut) {
            pneuVGuide.retract(cylinder2);
            fBeamGuideOut = false;
            
        } else {
            pneuVGuide.extend(cylinder2);
            fBeamGuideOut = true;
        }
    } else {
        // Beam is not at top, do nothing
        pneuVGuide.retract(cylinder2);
        
    }
}


// "when Controller ButtonR3 pressed" hat block
void onevent_ControllerButtonR3_pressed_0() {
    printf("R3 Pressed\n");
    Brain.playSound(siren);
   pneuVGuide.retract(pneuCBeamGuide);
   pneuVGuide.retract(pneuCPinGuide);
    wait(1, seconds);
    Brain.programStop();
}

// "when started" hat block

// "when started" hat block


// "when Controller ButtonEUp pressed" hat block
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




int main() {
    mg_pin.setPosition(0.0, degrees);
    mg_beam.setPosition(0.0, degrees);
    mg_pin.setVelocity(80.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(100.0, percent);
    mg_beam.setMaxTorque(100.0, percent);
    mg_pin.setStopping(hold);
    mg_beam.setStopping(hold);
    
    mot_dtLeft.setStopping(brake);
    mot_dtRight.setStopping(brake);
    pneuVGrabber.pumpOn();
  // register event handlers
    Controller.ButtonL3.pressed(onevent_ControllerButtonL3_pressed_0);
    Controller.ButtonR3.pressed(onevent_ControllerButtonR3_pressed_0);
    Controller.ButtonEUp.pressed(onevent_ControllerButtonEUp_pressed_0);
   
    wait(15, msec);
    vex::task ws1(TaskPin);
    vex::task ws2(TaskBeam);
    // vex::task wsDebug(TaskDebug);
    vex::task ws3(TaskControl);
    TaskDriveTrain();
    TaskAutonomous();
}

void PrintDistance(){
    printf("Dis Left: %u ", (uint16_t)dis_left.objectDistance(mm));
    printf(" Dis Right: %u \n", (uint16_t)dis_right.objectDistance(mm));
}

int TaskDebug() {
    while(1){
        printf("Dis Left: %u ", (uint16_t)dis_left.objectDistance(mm));
        printf(" Dis Right: %u\n", (uint16_t)dis_right.objectDistance(mm));
        printf(" mot_left Pos: %d ", (uint16_t)mot_dtLeft.position(degrees));
        printf(" mot_right Pos: %d \n", (uint16_t)mot_dtRight.position(degrees));
        
        wait(500, msec);
    }
}


int TaskControl() {
    // timer logTimer;
    // logTimer.reset();
    uint32_t logTick = 0; // 10 mS Tick
    bool btnLUpPressed = false;
    bool btnLDownPressed = false;
    bool btnRUpPressed = false;
    bool btnRDownPressed = false;
    bool btnEUpPressed = false;
    bool btnEDownPressed = false;
    bool btnFUpPressed = false;
    bool btnFDownPressed = false;
    bool stickAUp = false;
    bool stickADown = false;
    bool stickBLeft = false;
    bool stickBRight = false;
     BrainTimer.reset();
     fBeamGuideOut = false;
    while (true) {
        if(TouchLED12.pressing()) {
            BrainTimer.reset();
        }
        if(BrainTimer.value() > 120) {
            Brain.programStop();
            
        }
        if(Controller.ButtonLUp.pressing()) {
            if(!btnLUpPressed) {
                 BrainTimer.reset();
                btnLUpPressed = true;
                fBtnLupPressed = true;
                // printf("%u ,L UP Press\n",logTick);
            }
        }
        else {
            if(btnLUpPressed) {
                btnLUpPressed = false;
                // printf("%u ,L UP Release\n",logTick);
            }
        }
        if(Controller.ButtonLDown.pressing()) {
            if(!btnLDownPressed) {
                BrainTimer.reset();
                btnLDownPressed = true;
                fBtnLdownPressed = true;
                // printf("%u ,L Down Press\n",logTick);
            }
        }
        else {
            if(btnLDownPressed) {
                btnLDownPressed = false;
                // printf("%u ,L Down Release\n",logTick);
            }
        }
        if(Controller.ButtonRUp.pressing()) {
            if(!btnRUpPressed) {
                 BrainTimer.reset();
                btnRUpPressed = true;
                fBtnRupPressed = true;
                // printf("%u ,R UP Press\n",logTick);
            }
        }
        else {
            if(btnRUpPressed) {
                btnRUpPressed = false;
                // printf("%u ,R UP Release\n",logTick);
            }
        }
        if(Controller.ButtonRDown.pressing()) {
            if(!btnRDownPressed) {
                 BrainTimer.reset();
                btnRDownPressed = true;
                fBtnRdownPressed = true;
                // printf("%u ,R Down Press\n",logTick);
            }
        }
        else {
            if(btnRDownPressed) {
                btnRDownPressed = false;
                // printf("%u ,R Down Release\n",logTick);
            }
        }
        if(Controller.ButtonEUp.pressing()) {
            if(!btnEUpPressed) {
                 BrainTimer.reset();
                btnEUpPressed = true;
                fBtnEupPressed = true;
                // printf("%u ,E UP Press\n",logTick);/
            }
        }
        else {
            if(btnEUpPressed) {
                btnEUpPressed = false;
                // printf("%u ,E UP Release\n",logTick);
            }
        }
        if(Controller.ButtonEDown.pressing()) {
            if(!btnEDownPressed) {
                 BrainTimer.reset();
                btnEDownPressed = true;
                fBtnEdownPressed = true;
                // printf("%u ,E Down Press\n",logTick);
            }
        }
        else {
            if(btnEDownPressed) {
                btnEDownPressed = false;
                // printf("%u ,E Down Release\n",logTick);
            }
        }
        if(Controller.ButtonFUp.pressing()) {
            if(!btnFUpPressed) {
                 BrainTimer.reset();
                btnFUpPressed = true;
                fBtnFupPressed = true;
                // printf("%u ,F UP Press\n",logTick);
            }
        }
        else {
            if(btnFUpPressed) {
                btnFUpPressed = false;
                // printf("%u ,F UP Release\n",logTick);
            }
        }
        if(Controller.ButtonFDown.pressing()) {
            if(!btnFDownPressed) {
                 BrainTimer.reset();
                btnFDownPressed = true;
                fBtnFdownPressed = true;
                // printf("%u ,F Down Press\n",logTick);
            }
        }
        else {
            if(btnFDownPressed) {
                btnFDownPressed = false;
                // printf("%u ,F Down Release\n",logTick);
            }
        }

        if(Controller.AxisA.position() > 80) {
            if(!stickAUp) {
                 BrainTimer.reset();
                stickAUp = true;
                // printf("%u ,Stick A Up\n",logTick);
            }
        }
        else {
            if(stickAUp) {
                stickAUp = false;
                // printf("%u ,Stick A Center\n",logTick);
            }
        }
        if(Controller.AxisA.position() < -80) {
            if(!stickADown) {
                 BrainTimer.reset();
                stickADown = true;
                // printf("%u ,Stick A Down\n",logTick);
            }
        }
        else {
            if(stickADown) {
                stickADown = false;
                // printf("%u ,Stick A Center\n",logTick);
            }
        }
        if(Controller.AxisB.position() < -80) {
            if(!stickBLeft) {
                 BrainTimer.reset();
                stickBLeft = true;
                // printf("%u ,Stick B Left\n",logTick);
            }
        }
        else {
            if(stickBLeft) {
                stickBLeft = false;
                // printf("%u ,Stick B Center\n",logTick);
            }   
        }
        if(Controller.AxisB.position() > 80) {
            if(!stickBRight) {
                 BrainTimer.reset();
                stickBRight = true;
                // printf("%u ,Stick B Right\n",logTick);
            }
        }
        else {
            if(stickBRight) {
                stickBRight = false;
                // printf("%u ,Stick B Center\n",logTick);
            }   
        }
        // Brain.Screen.setCursor(1,1);
        // Brain.Screen.print("Pin Pos: %d  ", mg_pin.position(degrees));
        // Brain.Screen.setCursor(2,1);
        // Brain.Screen.print("Beam Pos: %d  ", mg_beam.position(degrees));
        // logTimer.reset();
        logTick++;
        wait(10, msec);
    }
}