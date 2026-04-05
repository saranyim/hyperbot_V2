#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;




uint16_t driveSpeed = 70;
uint16_t turnSpeed = 15;
uint16_t adjustTurnSpeed = 6;
double distanceToGo;

void trim_heading(uint16_t heading);
double Distance_MM_to_Degrees(double distance_mm);
void MoveForDistance(directionType dir, uint16_t distance_mm, uint16_t speed_pct);

void SpinLeft(uint16_t heading);
void SpinRight(uint16_t heading);

void turnTo(double targetDeg) ;
void turnBy(double deltaDeg);

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


void GotoDistance(uint16_t distance_mm){
    const double maxSpeedPct = 75.0;
    const double minDriveSpeedPct = 14.0;
    const double maxTrimSpeedPct = 22.0;
    const double slowDownDistanceMm = 400.0;
    const double coarseToleranceMm = 25.0;
    const double fineToleranceMm = 6.0;
    const double accelStepPct = 3.0;
    const double decelStepPct = 5.0;
    const double moveTimeoutSec = 4.0;
    const double trimTimeoutSec = 1.2;

    auto setDrive = [](double signedSpeedPct) {
        mot_dtLeft.spin(fwd, signedSpeedPct, pct);
        mot_dtRight.spin(fwd, signedSpeedPct, pct);
    };

    auto clampSpeed = [](double value, double low, double high) {
        if(value < low){
            return low;
        }
        if(value > high){
            return high;
        }
        return value;
    };

    printf("Goto distance %d\n", distance_mm);
    printf("current distance %d\n", (uint16_t)dis_rear.objectDistance(mm));

    mot_dtLeft.setStopping(brake);
    mot_dtRight.setStopping(brake);
    mot_dtLeft.setPosition(0, degrees);
    mot_dtRight.setPosition(0, degrees);

    timer moveTimer;
    moveTimer.reset();
    double commandedSpeedPct = 0.0;

    while(moveTimer.time(seconds) < moveTimeoutSec){
        const double currentDistanceMm = dis_rear.objectDistance(mm);
        const double errorMm = distance_mm - currentDistanceMm;
        const double absErrorMm = fabs(errorMm);

        if(absErrorMm <= coarseToleranceMm){
            break;
        }

        double targetSpeedPct = minDriveSpeedPct +
            ((maxSpeedPct - minDriveSpeedPct) * absErrorMm / slowDownDistanceMm);
        targetSpeedPct = clampSpeed(targetSpeedPct, minDriveSpeedPct, maxSpeedPct);

        if(commandedSpeedPct < targetSpeedPct){
            commandedSpeedPct += accelStepPct;
            if(commandedSpeedPct > targetSpeedPct){
                commandedSpeedPct = targetSpeedPct;
            }
        }
        else{
            commandedSpeedPct -= decelStepPct;
            if(commandedSpeedPct < targetSpeedPct){
                commandedSpeedPct = targetSpeedPct;
            }
        }

        const double signedSpeedPct = errorMm > 0.0 ? -commandedSpeedPct : commandedSpeedPct;
        setDrive(signedSpeedPct);
        wait(20, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
    wait(100, msec);

    timer trimTimer;
    trimTimer.reset();
    while(trimTimer.time(seconds) < trimTimeoutSec){
        const double currentDistanceMm = dis_rear.objectDistance(mm);
        const double errorMm = distance_mm - currentDistanceMm;
        const double absErrorMm = fabs(errorMm);

        if(absErrorMm <= fineToleranceMm){
            break;
        }

        double trimSpeedPct = minDriveSpeedPct + (absErrorMm * 0.15);
        trimSpeedPct = clampSpeed(trimSpeedPct, minDriveSpeedPct, maxTrimSpeedPct);
        const double signedTrimSpeedPct = errorMm > 0.0 ? -trimSpeedPct : trimSpeedPct;
        setDrive(signedTrimSpeedPct);
        wait(15, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
    printf("final distance %d\n", (uint16_t)dis_rear.objectDistance(mm));
    printf("left/right deg %u %u\n", (uint16_t)fabs(mot_dtLeft.position(degrees)), (uint16_t)fabs(mot_dtRight.position(degrees)));
}
// 1 wheel rotation = 8 inches
// Main autonomous routine sequence.
int TaskAutonomous() {
    int ledBlinkCount;
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
    Inertial.setHeading(90, degrees);
    pneuVGrabber.pumpOn();
    TouchLED12.setColor(green);

    WaitTouchDebug();
    mg_beam.setStopping(hold);
    MoveForDistance(reverse, 300, 50);
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,120,degrees,true);
    GrabPin;
    
    GotoDistance(1320);
    turnTo(180);
    wait(0.5, seconds);
    turnTo(180);
    wait(0.5, seconds);
    // mot_dtLeft.setVelocity(70, percent);
    // mot_dtRight.setVelocity(70, percent);
    // mot_dtLeft.spin(forward);
    // mot_dtRight.spin(forward);
    // wait(1.5, seconds);
    // mot_dtLeft.stop();
    // mot_dtRight.stop();
    ReleasePin;
    GotoDistance(630);
    GrabPin;
    Grab_then_up();
    GotoDistance(950);
    turnTo(200);
    wait(0.2, seconds);
    turnTo(200);
    // WaitTouchDebug();
    GotoDistance(750);
    DropDownMakeStack();
    WaitTouchDebug();
    return 0;
    
}

void MoveForDistance(directionType dir, uint16_t distance_mm, uint16_t speed_pct){
    mot_dtLeft.setVelocity(speed_pct, percent);
    mot_dtRight.setVelocity(speed_pct, percent);
    mot_dtLeft.spin(dir);
    mot_dtRight.spin(dir);

    const double targetDegrees = Distance_MM_to_Degrees(distance_mm);
    while(fabs(mot_dtLeft.position(degrees)) < targetDegrees && fabs(mot_dtRight.position(degrees)) < targetDegrees){
        wait(20, msec);
    }
    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
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
    mg_pin.setTimeout(1.0, seconds);
    mg_pin.setStopping(coast);
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(100.0, percent);
    mg_pin.spin(forward);
    wait(0.4, seconds);


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

    double Kp = 1.0;       // ค่าปกติเริ่มต้น
    double Kd = 0.08;      // ลด overshoot
    double maxPower = 35;  // จำกัดเพื่อความ smooth
    double minPower = 5;  // ป้องกัน stall
    
    double error, prevError = 0;
    double derivative;
    double power;

    while(true) {
        double angle = Inertial.angle();
        error = targetDeg - angle;

        // normalize wrap-around
        if (error > 180) error -= 360;
        if (error < -180) error += 360;

        if (fabs(error) < 0.5) break; // deadband

        derivative = error - prevError;
        power = Kp * error + Kd * derivative;

        // limit power
        if (fabs(power) > maxPower) power = copysign(maxPower, power);
        if (fabs(power) < minPower) power = copysign(minPower, power);

        // turn
        mot_dtLeft.spin(fwd,  -power, pct);
        mot_dtRight.spin(fwd, power, pct);

        prevError = error;
        wait(10, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
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