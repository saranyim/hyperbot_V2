#pragma once
#include "vex.h"

using namespace vex;

typedef enum {
  top = 1,
  bottom = 2,
  mid = 3
} position_t;



typedef enum{
    grab,
    release
} graber_t;

extern position_t pinPos;
extern position_t beamPos;
extern graber_t pinGraber;
extern graber_t beamGraber;
extern bool OverRideDriveTrain;
extern bool ReverseDir;
extern vex::brain Brain;
extern vex::controller Controller;

extern motor mot_dtLeft;
extern motor mot_dtRight;

extern motor mot_pinLeft;
extern motor mot_pinRight;

extern motor mot_beamLeft;
extern motor mot_beamRight;

extern motor_group mg_pin;
extern motor_group mg_beam;
// define your global instances of motors and other devices here
extern pneumatic pneuVGuide;
extern pneumatic pneuVGrabber;
extern distance dis_left;
extern distance dis_right;
extern touchled TouchLED12;

extern bool fBtnFupPressed ;
extern bool fBtnFdownPressed ;
extern bool fBtnEupPressed ;
extern bool fBtnEdownPressed ;
extern bool fBtnRupPressed ;
extern bool fBtnRdownPressed ;
extern bool fBtnLupPressed ; 
extern bool fBtnLdownPressed ;


#define pneuCBeamGrab cylinder1
#define pneuCPinGrab cylinder2

#define pneuCPinGuide cylinder1
#define pneuCBeamGuide cylinder2


void PrintDistance();