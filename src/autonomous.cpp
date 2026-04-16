#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;

 int ledBlinkCount;


uint16_t driveSpeed = 70;
uint16_t turnSpeed = 15;
uint16_t adjustTurnSpeed = 6;
double distanceToGo;

void trim_heading(uint16_t heading);
double Distance_MM_to_Degrees(double distance_mm);
static double NormalizeHeadingError(double targetDeg, double currentDeg);
void GotoDistance(uint16_t distance_mm, double targetHeadingDeg);

void MoveForDistance(directionType dir,
    uint16_t distance_mm,
    uint16_t speed_pct,
    double targetHeadingDeg);
void SpinLeft(uint16_t heading);
void SpinRight(uint16_t heading);

void turnTo(double targetDeg) ;
void turnBy(double deltaDeg);
void PinArmUP();
void PlaceStackOnStandoff();

// Safety timer to stop auton after 60 seconds.
int TaskAutoCnt(){
    wait(60,sec);
    Brain.programStop();
    return 0;
}

// Wait for a touch LED press and release.
void WaitTouchDebug(){
    while(TouchLED12.pressing() == false){
        wait(0.02, seconds);
    }
    // debouce
    while(TouchLED12.pressing() == true){
        wait(0.02, seconds);
    }
}

// Wait for a touch LED press and release.
void WaitEUp(){
    while(fBtnEupPressed == false){
        wait(0.5, seconds);
        printf("heading %u.%u\n", (uint16_t)Inertial.angle(), (uint16_t)(Inertial.angle() * 10) % 10);  
        printf("distance %d\n", (uint16_t)dis_rear.objectDistance(mm));
    }
    
    fBtnEupPressed = false;
}
void GotoDistance(uint16_t distance_mm, double targetHeadingDeg){
    const double maxApproachSpeedPctMin = 45.0;
    const double maxApproachSpeedPctMax = 80.0;
    const double minApproachSpeedPct = 16.0;
    const double creepSpeedPct = 7.0;
    const double correctionSpeedPct = 6.0;
    const double headingKp = 0.35;
    const double minHeadingAdjustPct = 2.0;
    const double maxHeadingAdjustPct = 10.0;
    const double slowDownDistanceMmMin = 250.0;
    const double slowDownDistanceMmMax = 600.0;
    const double creepStartBandMmMin = 35.0;
    const double creepStartBandMmMax = 80.0;
    const double finalToleranceMm = 5.0;
    const double approachTimeoutBaseSec = 1.6;
    const double approachTimeoutMaxSec = 6.0;
    const double creepTimeoutBaseSec = 1.2;
    const double creepTimeoutMaxSec = 3.0;
    const int settleMs = 100;
    const int finalSettleMs = 150;
    const int stableSamplesRequired = 2;
    const int maxCorrectionAttempts = 2;
    const int correctionPulseBaseMs = 45;
    const int correctionPulseGainMsPerMm = 4;
    const int correctionPulseMaxMs = 120;

    auto clampSpeed = [](double value, double low, double high) {
        if(value < low){
            return low;
        }
        if(value > high){
            return high;
        }
        return value;
    };

    auto roundToInt = [](double value) {
        return (int)(value >= 0.0 ? value + 0.5 : value - 0.5);
    };

    auto commandForError = [](double errorMm, double speedPct) {
        return errorMm > 0.0 ? -speedPct : speedPct;
    };

    auto setDrive = [&](double signedSpeedPct) {
        const double headingErrorDeg = NormalizeHeadingError(targetHeadingDeg, Inertial.angle());
        const double headingAdjustLimitPct = clampSpeed(fabs(signedSpeedPct) * 0.25,
            minHeadingAdjustPct,
            maxHeadingAdjustPct);
        const double headingAdjustPct = clampSpeed(headingErrorDeg * headingKp,
            -headingAdjustLimitPct,
            headingAdjustLimitPct);
        const double leftSpeedPct = clampSpeed(signedSpeedPct - headingAdjustPct, -100.0, 100.0);
        const double rightSpeedPct = clampSpeed(signedSpeedPct + headingAdjustPct, -100.0, 100.0);
        mot_dtLeft.spin(fwd, leftSpeedPct, pct);
        mot_dtRight.spin(fwd, rightSpeedPct, pct);
    };

    auto readSettledDistance = [&]() {
        mot_dtLeft.stop(hold);
        mot_dtRight.stop(hold);
        wait(settleMs, msec);
        return dis_rear.objectDistance(mm);
    };

    printf("Goto distance %d\n", distance_mm);
    printf("Goto heading %u.%1u\n",
        (uint16_t)targetHeadingDeg,
        (uint16_t)(targetHeadingDeg * 10) % 10);
    printf("current distance %d\n", (uint16_t)dis_rear.objectDistance(mm));

    mot_dtLeft.setStopping(hold);
    mot_dtRight.setStopping(hold);
    mot_dtLeft.setPosition(0, degrees);
    mot_dtRight.setPosition(0, degrees);

    const double startingDistanceMm = dis_rear.objectDistance(mm);
    const double startingErrorMm = distance_mm - startingDistanceMm;
    const double startingAbsErrorMm = fabs(startingErrorMm);
    const double longMoveScale = clampSpeed(startingAbsErrorMm / 1200.0, 0.0, 1.0);
    const double maxApproachSpeedPct = maxApproachSpeedPctMin +
        ((maxApproachSpeedPctMax - maxApproachSpeedPctMin) * longMoveScale);
    const double slowDownDistanceMm = slowDownDistanceMmMin +
        ((slowDownDistanceMmMax - slowDownDistanceMmMin) * longMoveScale);
    const double creepStartBandMm = creepStartBandMmMin +
        ((creepStartBandMmMax - creepStartBandMmMin) * longMoveScale);
    const double approachTimeoutSec = clampSpeed(
        approachTimeoutBaseSec + (startingAbsErrorMm / 320.0),
        approachTimeoutBaseSec,
        approachTimeoutMaxSec);
    const double creepTimeoutSec = clampSpeed(
        creepTimeoutBaseSec + (startingAbsErrorMm / 1400.0),
        creepTimeoutBaseSec,
        creepTimeoutMaxSec);
    printf("[GotoDistance] start=%d target=%d\n",
        roundToInt(startingDistanceMm),
        distance_mm);
    printf("[GotoDistance] profile vmax=%d slowdown=%d handoff=%d approach_to=%d creep_to=%d\n",
        roundToInt(maxApproachSpeedPct),
        roundToInt(slowDownDistanceMm),
        roundToInt(creepStartBandMm),
        roundToInt(approachTimeoutSec * 1000.0),
        roundToInt(creepTimeoutSec * 1000.0));

    timer approachTimer;
    approachTimer.reset();
    int approachLoopCount = 0;

    while(approachTimer.time(seconds) < approachTimeoutSec){
        approachLoopCount++;
        const double currentDistanceMm = dis_rear.objectDistance(mm);
        const double errorMm = distance_mm - currentDistanceMm;
        const double absErrorMm = fabs(errorMm);

        if(absErrorMm <= creepStartBandMm){
            break;
        }

        double approachSpeedPct = minApproachSpeedPct +
            ((maxApproachSpeedPct - minApproachSpeedPct) * absErrorMm / slowDownDistanceMm);
        approachSpeedPct = clampSpeed(approachSpeedPct, minApproachSpeedPct, maxApproachSpeedPct);
        const double signedApproachSpeedPct = commandForError(errorMm, approachSpeedPct);
        // printf("[GotoDistance][approach %d] dist=%d err=%d cmd=%d\n",
        //     approachLoopCount,
        //     roundToInt(currentDistanceMm),
        //     roundToInt(errorMm),
        //     roundToInt(signedApproachSpeedPct));
        setDrive(signedApproachSpeedPct);
        wait(20, msec);
    }

    double settledDistanceMm = readSettledDistance();
    double settledErrorMm = distance_mm - settledDistanceMm;
    // printf("[GotoDistance] approach done loops=%d elapsed_ms=%d dist=%d err=%d\n",
    //     approachLoopCount,
    //     roundToInt(approachTimer.time(msec)),
    //     roundToInt(settledDistanceMm),
    //     roundToInt(settledErrorMm));

    timer creepTimer;
    creepTimer.reset();
    int creepLoopCount = 0;
    int stableCount = 0;

    while(creepTimer.time(seconds) < creepTimeoutSec){
        creepLoopCount++;
        const double currentDistanceMm = dis_rear.objectDistance(mm);
        const double errorMm = distance_mm - currentDistanceMm;
        const double absErrorMm = fabs(errorMm);

        if(absErrorMm <= finalToleranceMm){
            settledDistanceMm = readSettledDistance();
            settledErrorMm = distance_mm - settledDistanceMm;
            if(fabs(settledErrorMm) <= finalToleranceMm){
                stableCount++;
                // printf("[GotoDistance][creep %d] stable dist=%d err=%d count=%d/%d\n",
                //     creepLoopCount,
                //     roundToInt(settledDistanceMm),
                //     roundToInt(settledErrorMm),
                //     stableCount,
                //     stableSamplesRequired);
                if(stableCount >= stableSamplesRequired){
                    break;
                }
            }
            else{
                stableCount = 0;
                // printf("[GotoDistance][creep %d] settle-miss dist=%d err=%d\n",
                //     creepLoopCount,
                //     roundToInt(settledDistanceMm),
                //     roundToInt(settledErrorMm));
            }
            continue;
        }

        stableCount = 0;
        const double signedCreepSpeedPct = commandForError(errorMm, creepSpeedPct);
        // printf("[GotoDistance][creep %d] dist=%d err=%d cmd=%d\n",
        //     creepLoopCount,
        //     roundToInt(currentDistanceMm),
        //     roundToInt(errorMm),
        //     roundToInt(signedCreepSpeedPct));
        setDrive(signedCreepSpeedPct);
        wait(20, msec);
    }

    settledDistanceMm = readSettledDistance();
    settledErrorMm = distance_mm - settledDistanceMm;
    // printf("[GotoDistance] creep done loops=%d elapsed_ms=%d dist=%d err=%d\n",
    //     creepLoopCount,
    //     roundToInt(creepTimer.time(msec)),
    //     roundToInt(settledDistanceMm),
    //     roundToInt(settledErrorMm));

    int correctionAttempt = 0;
    while(fabs(settledErrorMm) > finalToleranceMm && correctionAttempt < maxCorrectionAttempts){
        correctionAttempt++;
        int correctionPulseMs = correctionPulseBaseMs + (roundToInt(fabs(settledErrorMm)) * correctionPulseGainMsPerMm);
        correctionPulseMs = (int)clampSpeed(correctionPulseMs, correctionPulseBaseMs, correctionPulseMaxMs);
        const double signedCorrectionSpeedPct = commandForError(settledErrorMm, correctionSpeedPct);
        // printf("[GotoDistance][trim %d] start dist=%d err=%d cmd=%d pulse_ms=%d\n",
        //     correctionAttempt,
        //     roundToInt(settledDistanceMm),
        //     roundToInt(settledErrorMm),
        //     roundToInt(signedCorrectionSpeedPct),
        //     correctionPulseMs);
        setDrive(signedCorrectionSpeedPct);
        wait(correctionPulseMs, msec);
        settledDistanceMm = readSettledDistance();
        settledErrorMm = distance_mm - settledDistanceMm;
        // printf("[GotoDistance][trim %d] result dist=%d err=%d\n",
        //     correctionAttempt,
        //     roundToInt(settledDistanceMm),
        //     roundToInt(settledErrorMm));
    }

    mot_dtLeft.stop(hold);
    mot_dtRight.stop(hold);
    wait(finalSettleMs, msec);
    settledDistanceMm = dis_rear.objectDistance(mm);
    settledErrorMm = distance_mm - settledDistanceMm;
    printf("[GotoDistance] final dist=%d err=%d trims=%d\n",
        roundToInt(settledDistanceMm),
        roundToInt(settledErrorMm),
        correctionAttempt);
    // printf("final distance %d\n", (uint16_t)settledDistanceMm);
    // printf("left/right deg %u %u\n", (uint16_t)fabs(mot_dtLeft.position(degrees)), (uint16_t)fabs(mot_dtRight.position(degrees)));
}
// 1 wheel rotation = 8 inches
// Main autonomous routine sequence.
void InitAutonomous() {

    pneuVGuide.retract(pneuCPinGuide);
   
    Brain.Screen.setCursor(2, 1);
    TouchLED12.setColor(red);

    pneuVGrabber.pumpOff();


    while(TouchLED12.pressing() == false){
        wait(0.02, seconds);
    }
    // debouce
    while(TouchLED12.pressing() == true){

        wait(0.02, seconds);
    
    }

    // Inertial.calibrate();
    // ledBlinkCount = 5;
    Inertial.startCalibration();
    while(Inertial.isCalibrating()){
      
        TouchLED12.setColor(yellow);
        wait(0.5,seconds);
        TouchLED12.setColor(red);
        wait(0.5,seconds);
    }
    Inertial.setHeading(0, degrees);
    pneuVGrabber.pumpOn();
    TouchLED12.setColor(green);

    WaitTouchDebug();
     mg_beam.setStopping(hold);
    // MoveForDistance(reverse,200,50);
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);

    
}
void FromStartToMakeU(){
// move forward to lift beam
   
     // go to get 1st blue
     printf("Go get 1st blue\n");
    mg_beam.spinFor (spinBeamUp,140,degrees,true);
    GotoDistance(1555, 0.0);
    GrabPin;
    // WaitEUp();
    // turn to get red yellow to makesstaick
    printf("Turn to get red/yellow\n");
    turnTo(114);
    wait(0.5, seconds);
    turnTo(114);
    Grab_then_up();
    // got get red and yellow
    MoveForDistance(forward, 700, 60, 114);
    // WaitEUp();
    DropDownMakeStack();
    MoveForDistance(forward, 100, 50, 114);
    GrabPin;
    // set position to get beam
    turnTo(90);
    wait(0.5, seconds);
    turnTo(90);
    // WaitEUp();
    GotoDistance(1100, 90.0);

    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spinFor(spinPinUp, 140 , degrees, false);
    // WaitEUp();
    // turn to get beam
    turnTo(180);
    wait(0.5, seconds);
    turnTo(180);
    // WaitEUp();
    mg_beam.spinFor (spinBeamDown,120,degrees,true);
    
    mot_dtLeft.setVelocity(50, percent);
    mot_dtRight.setVelocity(50, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(1.5, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    GrabBeam;
    printf("flip pin over\n");
    Flip_Pin_Over();
}

void FromUToBaseStack(){
    //  printf("start debug from U\n");


    printf("\n\nforward to get statck\n");
    MoveForDistance(forward,50,70, 180);
    mg_beam.spinFor (spinBeamUp,180,degrees,false);
    turnTo(180);
    wait(0.5, seconds);
    GotoDistance(470, 180.0);
    wait(0.5, seconds);
    // WaitEUp();
    printf("turn to get stack\n");
    turnTo(220);
    wait(0.5, seconds);

  
    
    printf("move to blue\n");
    MoveForDistance(forward,250,70,220);
    Grab_then_up();
    // printf("heding %u.%u\n", (uint16_t)Inertial.angle(), (uint16_t)(Inertial.angle() * 10) % 10);
    // WaitEUp();
    // turnTo(220);
    // wait(0.5, seconds);
    // WaitEUp();
    printf("get next yellow\n");
    MoveForDistance(forward,350,70,225);
    DropDownMakeStack();
    MoveForDistance(forward, 100, 70,220);
    GrabPin;
    MoveForDistance(reverse,300,70,220);
    turnTo(180);
    wait(0.5, seconds);
    turnTo(180);
    // WaitEUp();
   
}

void PlaceOnStandoff(){

    printf("\n\nset position to stand off\n");
    GotoDistance(550, 180.0);
    mg_beam.spinFor (spinBeamDown,160,degrees,false);
    // WaitEUp();
    PinArmUP();
    turnTo(155);
    // WaitEUp();
    MoveForDistance(forward, 450, 70,155);
    // WaitEUp();
    PlaceStackOnStandoff();
    MoveForDistance(reverse, 200, 70,155);
    Drop_Pin_Arm();
    Grab_Beam_up_121();
    turnBy(165);
    MoveForDistance(reverse, 250, 70,300);
    Place_Beam_Stand_Off();
}

int TaskAutonomous() {
    InitAutonomous();
    // GotoDistance(1500,0);
    // MoveForDistance(reverse, 500, 50);

    FromStartToMakeU();
    // GrabBeam;
    // wait(1.5, seconds);
    // Inertial.setHeading(180, degrees); // debug
    FromUToBaseStack();
    PlaceOnStandoff();
    return 0;
    
}

void PinArmUP(){

    GrabPin;
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setStopping(hold);
    mg_pin.spinFor(reverse,380 , degrees, false);
    
    
    wait(0.3, seconds);
    handDown;
}

void PlaceStackOnStandoff(){
                
    mot_dtLeft.setStopping(hold);
    mot_dtRight.setStopping(hold);
    mot_dtLeft.stop();
    mot_dtRight.stop(); 

    mg_pin.spinFor(forward,100 , degrees, false);
    // handDown;
    wait(0.3, seconds);
    ReleasePin;
    // handUp;

    wait(0.0, seconds);
    handUp;
    wait(0.2, seconds);
    
                
}

static double NormalizeHeadingError(double targetDeg, double currentDeg){
    double errorDeg = targetDeg - currentDeg;

    while(errorDeg > 180.0){
        errorDeg -= 360.0;
    }
    while(errorDeg < -180.0){
        errorDeg += 360.0;
    }

    return errorDeg;
}

void MoveForDistance(directionType dir,
    uint16_t distance_mm,
    uint16_t speed_pct,
    double targetHeadingDeg){
    const double drivetrainForwardSign = -1.0;
    const double minDriveSpeedPct = 8.0;
    const double headingKp = 0.35;
    const double minHeadingAdjustPct = 3.0;
    const double maxHeadingAdjustPct = 12.0;
    const int rampTimeMs = 180;
    const int loopDelayMs = 20;
    const int stallDetectMs = 500;
    const double stallVelocityPct = 2.0;
    const double minProgressDegrees = 2.0;

    auto clampValue = [](double value, double low, double high) {
        if(value < low){
            return low;
        }
        if(value > high){
            return high;
        }
        return value;
    };

    auto commandDrive = [](double leftSpeedPct, double rightSpeedPct) {
        mot_dtLeft.spin(fwd, leftSpeedPct, pct);
        mot_dtRight.spin(fwd, rightSpeedPct, pct);
    };

    printf("MoveForDistance %d mm\n", distance_mm);
    const double targetDegrees = Distance_MM_to_Degrees(distance_mm);
    printf("target degrees %u\n", (uint16_t)targetDegrees);
   
    printf("keep heading %u.%1u\n",
        (uint16_t)targetHeadingDeg,
        (uint16_t)(targetHeadingDeg * 10) % 10);
    

    mot_dtLeft.setPosition(0, degrees);
    mot_dtRight.setPosition(0, degrees);
    mot_dtLeft.setStopping(brake);
    mot_dtRight.setStopping(brake);
    mot_dtLeft.setTimeout(0.5, seconds);
    mot_dtRight.setTimeout(0.5, seconds);
    mot_dtLeft.setMaxTorque(100.0, percent);
    mot_dtRight.setMaxTorque(100.0, percent);

    const double topSpeedPct = fmax((double)speed_pct, minDriveSpeedPct);
    const double directionSign = (dir == forward) ? 1.0 : -1.0;
    const double headingAdjustLimitPct = clampValue(topSpeedPct * 0.25,
        minHeadingAdjustPct,
        maxHeadingAdjustPct);

    timer moveTimer;
    timer stallTimer;
    moveTimer.reset();
    stallTimer.reset();

    double previousAverageDegrees = 0.0;
    bool stuckDetected = false;

    while(true){
        const double leftDegrees = fabs(mot_dtLeft.position(degrees));
        const double rightDegrees = fabs(mot_dtRight.position(degrees));
        const double averageDegrees = (leftDegrees + rightDegrees) * 0.5;

        if(averageDegrees >= targetDegrees){
            break;
        }

        double rampRatio = (double)moveTimer.time(msec) / rampTimeMs;
        rampRatio = clampValue(rampRatio, 0.0, 1.0);
        const double rampedSpeedPct = minDriveSpeedPct + ((topSpeedPct - minDriveSpeedPct) * rampRatio);
        const double signedBaseSpeedPct = drivetrainForwardSign * directionSign * rampedSpeedPct;

        double headingAdjustPct = 0.0;
        
        const double headingErrorDeg = NormalizeHeadingError(targetHeadingDeg, Inertial.angle());
        headingAdjustPct = clampValue(headingErrorDeg * headingKp,
            -headingAdjustLimitPct,
            headingAdjustLimitPct);
        

        const double leftCommandPct = clampValue(signedBaseSpeedPct - headingAdjustPct,
            -100.0,
            100.0);
        const double rightCommandPct = clampValue(signedBaseSpeedPct + headingAdjustPct,
            -100.0,
            100.0);
        commandDrive(leftCommandPct, rightCommandPct);

        const double leftVelocityPct = fabs(mot_dtLeft.velocity(percent));
        const double rightVelocityPct = fabs(mot_dtRight.velocity(percent));
        const double progressDegrees = averageDegrees - previousAverageDegrees;
        const bool stalled = (leftVelocityPct < stallVelocityPct) &&
            (rightVelocityPct < stallVelocityPct) &&
            (progressDegrees < minProgressDegrees);

        if((moveTimer.time(msec) > rampTimeMs) && stalled){
            if(stallTimer.time(msec) >= stallDetectMs){
                printf("[WARN] MoveForDistance stalled at %u/%u deg avg\n",
                    (uint16_t)averageDegrees,
                    (uint16_t)targetDegrees);
                stuckDetected = true;
                break;
            }
        }
        else{
            stallTimer.reset();
        }

        previousAverageDegrees = averageDegrees;
        wait(loopDelayMs, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);

    printf("final distance %d\n", (uint16_t)dis_rear.objectDistance(mm));
    if(stuckDetected){
        printf("[WARN] MoveForDistance stopped before target\n");
    }
}




// Convert travel distance in mm to wheel degrees.
double Distance_MM_to_Degrees(double distance_mm){
    return distance_mm / (12.0* 25.4) * 360.0;
}

// Drop the pin and return arm to the up position.
void Auto_Drop_Down_Pin_Grab_Up() {

    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.spinFor(forward,180,degrees,false);
    mot_dtRight.spinFor(forward,180,degrees,true);

    Set_Drop_Pin();
    printf("drop and up");
    printf("\n");
    mg_pin.setVelocity(50.0, percent);
    OverRideDriveTrain = true;
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.spinFor(forward,180,degrees,false);
    mot_dtRight.spinFor(forward,180,degrees,false);

    wait(0.8, seconds);
    GrabPin;   

    Grab_then_up();
   
}



// Spin left to a target inertial heading.
void SpinLeft(uint16_t heading){
mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtRight.spin(forward);
    mot_dtLeft.spin(reverse);
    while(1){
        if((uint16_t)Inertial.angle()<=heading)
        {
            break;
        }
        wait(5, msec);
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.5,seconds);
    mot_dtLeft.setVelocity(adjustTurnSpeed,percent);
    mot_dtRight.setVelocity(adjustTurnSpeed,percent);
    
    mot_dtRight.spin(forward);
    mot_dtLeft.spin(reverse);
    while(1){
        if((uint16_t)Inertial.angle()>=heading)
        {
            break;
        }
        wait(5, msec);
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    trim_heading(heading);



}

// Spin right to a target inertial heading.
void SpinRight(uint16_t heading){

    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.spin(reverse);
    mot_dtLeft.spin(forward);

    while(1){  
        if(IS_IN_RANGE((uint16_t)Inertial.angle(),heading - 1, heading + 3))
        {
            break;
        }
        wait(5, msec);      
    }
    
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.5,seconds);
    mot_dtLeft.setVelocity(adjustTurnSpeed,percent);
    mot_dtRight.setVelocity(adjustTurnSpeed,percent);
    
    mot_dtRight.spin(reverse);
    mot_dtLeft.spin(forward);

    while(1){  
        if((uint16_t)Inertial.angle()<=heading)
        {
            break;
        }
        wait(5, msec);      
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();

    trim_heading(heading);
}

// Flip the pin over using the beam arm.
void Auto_Flip_Pin_Over() {
    printf("start flip\n");
    GrabPin;
// set beam arm free to move a little bit
    mg_beam.setStopping(coast);
    mg_beam.stop();
// move pin over
    pneuVGuide.retract(cylinder1);
    mg_beam.spin(forward);
    wait(0.2, seconds);
    mg_beam.stop();
    pneuVGrabber.extend(cylinder1);
    mg_beam.setTimeout(1.0, seconds);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.spin(reverse);
    wait(1.5, seconds);
    printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));
       
    while((int16_t)mg_pin.velocity(percent) < 0) {
        printf("mgpin speed %d\n",(int16_t)mg_pin.velocity(percent));
        wait(20, msec);
    }
    mg_pin.stop();
    ReleasePin;
    wait(0.2, seconds);
    mg_pin.setTimeout(1.0, seconds);
    mg_pin.setStopping(coast);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spin(forward);
    wait(0.2, seconds);


    while(mg_pin.velocity(percent) > 30.0) {
        wait(20, msec);
    }
    mg_pin.stop();

    pinPos = bottom;
}

// Fine trim toward a heading (currently disabled).
void trim_heading(uint16_t heading){
    return;
    wait(0.05,seconds);
    if ((uint16_t)Inertial.angle()>heading){
        mot_dtRight.setVelocity(15, percent);
        mot_dtLeft.setVelocity(15, percent);
        mot_dtRight.spin(reverse);
        mot_dtLeft.spin(forward);

        while(1){  
            if((uint16_t)Inertial.angle() == heading)
            {
                break;
            }
            wait(5, msec);      
        }
    mot_dtLeft.stop();
    mot_dtRight.stop();


    }
    else if ((uint16_t)Inertial.angle() <heading) 
    {
        mot_dtRight.setVelocity(15, percent);
        mot_dtLeft.setVelocity(15, percent);
        mot_dtRight.spin(forward);
        mot_dtLeft.spin(reverse);

        while(1){  
            if((uint16_t)Inertial.angle() == heading)
            {
                break;
            }
            wait(5, msec);      
        }
        mot_dtLeft.stop();
        mot_dtRight.stop();


    }



}

// PD turn controller to a target heading.
void turnTo(double targetDeg) {

    double Kp = 0.8;       // ค่าปกติเริ่มต้น
    double Kd = 0.05;      // ลด overshoot
    double maxPower = 30;  // จำกัดเพื่อความ smooth
    double minPower = 5;  // ป้องกัน stall
    
    double error, prevError = 0;
    double derivative;
    double power;
    printf("turnTo target %u.%1u\n", (uint16_t)targetDeg, (uint16_t)(targetDeg * 10) % 10);
    while(true) {
        double angle = Inertial.angle();
        error = targetDeg - angle;

        // normalize wrap-around
        if (error > 180) error -= 360;
        if (error < -180) error += 360;

        if (fabs(error) < 0.8) break; // deadband

        derivative = error - prevError;
        power = Kp * error + Kd * derivative;

        // limit power
        if (fabs(power) > maxPower) power = copysign(maxPower, power);
        if (fabs(power) < minPower) power = copysign(minPower, power);

        // turn
        mot_dtLeft.spin(fwd,  -power, pct);
        mot_dtRight.spin(fwd, power, pct);

        prevError = error;
        wait(50, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
    printf("final heading %u.%1u\n", (uint16_t)Inertial.angle(), (uint16_t)(Inertial.angle() * 10) % 10);
}

void turnBy(double deltaDeg) {
    double targetDeg = Inertial.angle() + deltaDeg;

    while(targetDeg >= 360.0) {
        targetDeg -= 360.0;
    }
    while(targetDeg < 0.0) {
        targetDeg += 360.0;
    }

    turnTo(targetDeg);
}