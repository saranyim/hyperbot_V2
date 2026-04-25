#pragma once


#include "vex.h"
#include "main.h"
using namespace vex;
#define spinBeamUp reverse
#define spinBeamDown forward

extern bool fBeamMovingUp;
int TaskBeam();
void Drop_Y_Arm();
void Grab_Beam_up_121();
void Grab_Beam_up_91();
void Place_Pin_On_Stand_Off();
void Place_Beam_Stand_Off();
