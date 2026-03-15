#pragma once
#include "vex.h"

#define redSide 1
#define blueSide 2

#define side blueSide

#define fixHand 0
using namespace vex;

typedef enum {
  top = 1,
  bottom = 2,
  mid = 3,
  aboveStandoff = 4,
  posFloat = 5
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
extern vex::timer BrainTimer;
extern vex::controller Controller;
extern vex::inertial Inertial;

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
extern distance dis_rear;
extern touchled TouchLED12;

extern volatile bool gPlaceBeam2StackRunning;
void YGuidInSafe();
void YGuidOutSafe();


#define pneuCBeamGrab cylinder1
#define pneuCPinGrab cylinder2

#define pneuCPinGuide cylinder1
#define pneuCBeamGuide cylinder2


#define ReleasePin  pneuVGrabber.retract(pneuCPinGrab); pinGraber = release
#define GrabPin  pneuVGrabber.extend(pneuCPinGrab); pinGraber = grab

#define ReleaseBeam  pneuVGuide.retract(cylinder1); beamGraber = release
#define GrabBeam  pneuVGuide.extend(cylinder1); beamGraber = grab

#define handUp pneuVGuide.extend(cylinder2)
#define handDown pneuVGuide.retract(cylinder2)

#define yGuidIn pneuVGrabber.retract(pneuCBeamGrab)
#define yGuidOut pneuVGrabber.extend(pneuCBeamGrab)
// #define pinGuidOut pneuVGrabber.retract(pneuCBeamGrab)



void PrintDistance();

#define IS_IN_RANGE(value, min, max) ((value >= min) && (value <= max))
