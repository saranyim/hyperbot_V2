#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;




uint16_t driveSpeed = 70;
uint16_t turnSpeed = 100;
uint16_t adjustTurnSpeed = 6;
double distanceToGo;
void Auto_Drop_Down_Pin_Grab_Up();
void Auto_Flip_Pin_Over();
void from_Start_to_Yellow();
void reverse_to_get_Blue();
void spin_to_get_blue();
void spin_to_get_to_position();
void go_forward_to_make_stack();
void go_backwards_to_place_pin_on_stand_off();
void spin_to_get_beam();
void go_backwards_to_get_beam();
void go_forward_to_spin_to_stand_off(); 
void go_reverse_to_stand_off();
void reverse_to_set_distance();
void spin_to_get_to_standoff();
void auto_place_beam_on_standoff();

void trim_heading(uint16_t heading);
double Distance_MM_to_Degrees(double distance_mm);

void SpinLeft(uint16_t heading);
void SpinRight(uint16_t heading);

void turnTo(double targetDeg);
void spinRelative45(double dir = 1);   // +1 = clockwise, -1 = counter-clockwise
void spinRelative90(double dir = 1);
void spinRelative100(double dir = 1);
void spinRelative110(double dir = 1);
void spinRelative120(double dir = 1);
void spinRelative130(double dir = 1);
void spinRelative140(double dir = 1);
void spinRelative150(double dir = 1);
void spinRelative160(double dir = 1);
void spinRelative170(double dir = 1);
void spinRelative180(double dir = 1);
void driveRamped(directionType dir, double distance_mm, uint16_t maxSpeed = 80, uint16_t minSpeed = 20, double rampFrac = 0.25);
void driveForward(double distance_mm, uint16_t maxSpeed = 0);
void driveReverse(double distance_mm, uint16_t maxSpeed = 0);
void driveNudge(directionType dir = forward, double distance_mm = 80, uint16_t speed = 20, bool waitUntilDone = true);
void driveToWallDistance(double targetDist_mm, uint16_t maxSpeed = 80, uint16_t minSpeed = 20, double rampFrac = 0.25, bool waitUntilDone = true);

// Safety timer to stop auton after 60 seconds.
int TaskAutoCnt(){
    wait(68273648762870,sec);
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

// 1 wheel rotation = 8 inches
// Main autonomous routine sequence.
int TaskAutonomous() {
    pneuVGrabber.pumpOff();
    pneuVGuide.pumpOff();
    
    
    // WaitTouchDebug();
 
    
    int ledBlinkCount;
   
    Brain.Screen.setCursor(2, 1);
    TouchLED12.setColor(red);
    while(TouchLED12.pressing() == false){
        wait(0.02, seconds);
    }
    // debouce
    while(TouchLED12.pressing() == true){

        wait(0.02, seconds);
    
    }

    

    
    TouchLED12.setColor(green);
wait(5, seconds);
    Inertial.setHeading(180, degrees);    
    // WaitTouchDebug();
 
    while(true){
        
        TouchLED12.setColor(green);
        wait(0.1, seconds);
        TouchLED12.setColor(blue_green);
        wait(0.1, seconds);
        if(TouchLED12.pressing()){
            // debouce
            while(TouchLED12.pressing() == true){

                wait(0.02, seconds);
            
            }
            break;
        }
    }
   
    while(true){
        if((uint16_t)Inertial.heading() ==119){
            TouchLED12.setColor(green);
        }
        else{
            TouchLED12.setColor(yellow);
        }
        wait(0.02, seconds);
        if(TouchLED12.pressing()){
            break;
        }
    }
    OverRideDriveTrain = false;
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtLeft.setPosition(0.0, degrees);
    mot_dtRight.setPosition(0.0, degrees);
     pneuVGrabber.pumpOn();
    pneuVGuide.pumpOn();
    fBeamGuideOut = false;
    wait(1, seconds);
    handUp;
     ReleaseBeam;
    ReleasePin;

   

    TouchLED12.setBlink(red_violet, 0.1, 0.1);
    while(TouchLED12.pressing() == false){
        
    
        wait(0.02, seconds);
        }
    vex::task wsAuto(TaskAutoCnt);
       
    wait(0.5, seconds);
    
    TouchLED12.setColor(yellow_green);
    printf("touch\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    distanceToGo = dis_rear.objectDistance(mm);
    Set_Drop_Pin();
    // from_Start_to_Yellow();
    // wait(0.5, seconds);
    // WaitTouchDebug();



    // reverse_to_get_Blue();

    // // wait(0.5, seconds);
    // // Grab_Beam_up();
    // // WaitTouchDebug();
    // // printf("touch\n");
    // spin_to_get_blue();
  
    // // wait(0.5, seconds);
    // // WaitTouchDebug();

    // go_forward_to_make_stack();
 
    // Auto_Drop_Down_Pin_Grab_Up();


   
    // // wait(0.5, seconds);



    reverse_to_set_distance();
    //  wait(0.5, seconds);

// WaitTouchDebug();

    

    go_backwards_to_place_pin_on_stand_off();
    // wait(0.3, seconds);
  



    spin_to_get_beam();
    // WaitTouchDebug();
    // wait(0.5, seconds);
    // WaitTouchDebug();

    go_backwards_to_get_beam();
    // wait(0.5, seconds);
    WaitTouchDebug();

    Auto_Flip_Pin_Over();
    // wait(0.5, seconds);
    go_forward_to_spin_to_stand_off();
    // WaitTouchDebug();


    Grab_Beam_up();
    // wait(0.5, seconds);
    // WaitTouchDebug();
    spin_to_get_to_standoff();
    // wait(0.5, seconds);

    go_reverse_to_stand_off();
    // wait(0.5, seconds);
    // WaitTouchDebug();
    Place_Beam_Stand_Off();
    // wait(0.5, seconds);


    
    return 0;
    
}
// Drive from start to the yellow goal and grab.
void from_Start_to_Yellow(){
    driveToWallDistance(100, 100);
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamUp);
    wait(0.3, seconds);
    mg_beam.stop();
    wait(0.3, seconds);

    mg_pin.setStopping(hold);
    mg_pin.spin(spinPinDown);
    wait(0.3, seconds);
    mg_pin.stop();
    handUp;
   driveToWallDistance(1500, 100);
    

    driveNudge(reverse, 50, 15);
    
    GrabPin;
    Grab_then_up();
    
    
    
}
void auto_place_beam_on_standoff() {
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(80, percent);
    mg_beam.spin(spinBeamDown);
    OverRideDriveTrain = true;
    mot_dtLeft.setVelocity(0, percent);
    mot_dtRight.setVelocity(0, percent);
    wait(0.15, seconds);
    mg_beam.setStopping(hold);
    ReleaseBeam;
    ReverseDir = true;
    mg_beam.setMaxTorque(100.0, percent);
    mg_beam.setVelocity(100, percent);
    mg_beam.spin(spinBeamUp);
    mg_beam.stop();
    wait(0.1, seconds);
    OverRideDriveTrain = false;
    driveRamped(reverse, 10220, 100);
}


// Reverse to reach the blue goal position
void reverse_to_get_Blue(){
    turnTo(111);
    wait(0.5, seconds);
    turnTo(111);
    wait(0.5, seconds);
   
}


// Turn toward the blue target.
void spin_to_get_blue(){
    driveToWallDistance(1100, 100);
    driveNudge(reverse, 100, 20);
   Auto_Drop_Down_Pin_Grab_Up();
   driveNudge(reverse, 250, 20, true);  
  

   GrabPin;

}

// Drive forward to stack position.
void    go_forward_to_make_stack(){
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.4, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();

    turnTo(175);
    // WaitTouchDebug ();
    mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamDown);
    wait(0.3, seconds);
    mg_beam.stop();
//    WaitTouchDebug ();
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(1.2, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    GrabBeam;
    Flip_Pin_Over();
    // WaitTouchDebug();
}

// Reverse to a target distance using the rear sensor.
void reverse_to_set_distance(){
    GrabBeam;
    driveRamped(reverse, 70, 80);
      mg_beam.setStopping(hold);
    mg_beam.spin(spinBeamUp);
    wait(0.65, seconds);
    mg_beam.stop();
driveToWallDistance(434, 80);
// WaitTouchDebug();
   turnTo(221);
   wait(0.8, seconds);
   turnTo(221);
    wait(0.8, seconds);
   

    // WaitTouchDebug();
    
 
}

// WaitTouchDebug();

   


// Back into place and drop the pin on the standoff.
void go_backwards_to_place_pin_on_stand_off(){
    driveRamped(reverse, 170, 50);
    Grab_then_up();
    driveRamped(reverse, 350, 50);
    driveNudge(reverse, 60, 20);
    Auto_Drop_Down_Pin_Grab_Up();
    driveNudge(reverse, 67, 20);

    
    GrabPin;
       driveToWallDistance(1300, 80);

     mg_beam.spin(spinBeamDown);
                wait(1, seconds);
                mg_beam.setTimeout(0.7, seconds);
                mg_beam.stop();
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100.0, percent);
                mg_pin.setStopping(hold);
                mg_pin.spinFor(reverse,80 , degrees, false);

    turnTo(80);
    // WaitTouchDebug();
    // driveRamped(forward, 735, 75);
    // WaitTouchDebug();
    
    // turnTo(195); 
    // wait(0.3, seconds);
    // turnTo(195); 
    // wait(0.3, seconds);
    // GrabPin;
                mg_pin.setMaxTorque(100.0, percent);
                mg_pin.setVelocity(100.0, percent);
                mg_pin.setStopping(hold);
                mg_pin.spinFor(reverse,325 , degrees, false);
                
                
                wait(0.3, seconds);
                handDown;
                pinPos = top;
    //             mg_beam.spin(spinBeamDown);
    //             wait(1, seconds);
    //             mg_beam.setTimeout(0.7, seconds);
    //             mg_beam.stop();
                mot_dtLeft.setVelocity(100, percent);
                mot_dtRight.setVelocity(100, percent);
                mot_dtLeft.spin(reverse);
                mot_dtRight.spin(reverse);
                wait(1.6, seconds);
                mot_dtLeft.stop();
                mot_dtRight.stop();
               
}

// Turn to face the beam target.
    void spin_to_get_beam(){
        //  OverRideDriveTrain = true;
                
        //         mot_dtLeft.setStopping(hold);
        //         mot_dtRight.setStopping(hold);
        //         mot_dtLeft.stop();
        //         mot_dtRight.stop(); 
        //         // mot_dtLeft.setVelocity(60, percent);
        //         // mot_dtRight.setVelocity(60, percent);
        //         // mot_dtLeft.spin(reverse);
        //         // mot_dtRight.spin(reverse);
        //         // wait(0.5, seconds);
                
        
        //         // wait(0.1, seconds);
                mg_pin.spinFor(forward,85 , degrees, false);
                // handDown;
                wait(0.3, seconds);
                ReleasePin;
                // handUp;
                OverRideDriveTrain = false;
                wait(0.2, seconds);
                handUp;
                Drop_Pin_Arm();
               
                pinPos = bottom;
                
                
    }



// Reverse to pick up the beam.
    void go_backwards_to_get_beam(){
        GrabBeam;
    driveRamped(forward, 200, 100);
    spinRelative170();
    Grab_Beam_up();
    mot_dtLeft.setVelocity(100, percent);
                mot_dtRight.setVelocity(100, percent);
                mot_dtLeft.spin(forward);
                mot_dtRight.spin(forward);
                wait(1.6, seconds);
                mot_dtLeft.stop();
                mot_dtRight.stop();
                auto_place_beam_on_standoff();
             


}

// Drive forward before turning to the standoff.
void go_forward_to_spin_to_stand_off(){
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.6, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
}

// Turn to face the standoff.
void spin_to_get_to_standoff(){
    turnTo(90);
}

// Reverse into the standoff placement zone.
void go_reverse_to_stand_off(){
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.5, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
}


// Convert travel distance in mm to wheel degrees.
double Distance_MM_to_Degrees(double distance_mm){
    return distance_mm / (12.0* 25.4) * 360.0;
}

// Drive forward or reverse with a speed ramp-up at the start and ramp-down at
// the end so the bot doesn't bounce back from a hard stop.
//
//   dir        – forward or reverse
//   distance_mm – how far to travel in millimeters
//   maxSpeed   – peak speed in percent (e.g. 80)
//   minSpeed   – starting / ending speed in percent (e.g. 20)
//   rampFrac   – fraction of total distance used for each ramp (0.0–0.5)
//                default 0.25 means 25 % ramp-up, 25 % ramp-down
void driveRamped(directionType dir,
                 double   distance_mm,
                 uint16_t maxSpeed,
                 uint16_t minSpeed,
                 double   rampFrac) {

    double totalDeg    = Distance_MM_to_Degrees(distance_mm);
    double rampUpEnd   = totalDeg * rampFrac;
    double rampDnStart = totalDeg * (1.0 - rampFrac);

    // Reset encoders so position starts at 0.
    mot_dtLeft.setPosition(0.0, degrees);
    mot_dtRight.setPosition(0.0, degrees);

    while (true) {
        double pos = (fabs(mot_dtLeft.position(degrees)) +
                      fabs(mot_dtRight.position(degrees))) / 2.0;

        if (pos >= totalDeg) break;

        double speed;
        if (pos < rampUpEnd) {
            speed = minSpeed + (maxSpeed - minSpeed) * (pos / rampUpEnd);
        } else if (pos > rampDnStart) {
            double fraction = (pos - rampDnStart) / (totalDeg - rampDnStart);
            speed = maxSpeed - (maxSpeed - minSpeed) * fraction;
        } else {
            speed = maxSpeed;
        }

        // spin(dir, speed, pct) sets direction and velocity every tick.
        mot_dtLeft.spin(dir, speed, pct);
        mot_dtRight.spin(dir, speed, pct);

        wait(10, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
}

// Easy wrappers – no need to specify directionType.
// Pass a speed (%) or leave blank to use the global driveSpeed variable.
void driveForward(double distance_mm, uint16_t maxSpeed) {
    driveRamped(forward, distance_mm, maxSpeed == 0 ? driveSpeed : maxSpeed);
}
void driveReverse(double distance_mm, uint16_t maxSpeed) {
    driveRamped(reverse, distance_mm, maxSpeed == 0 ? driveSpeed : maxSpeed);
}

// Slow approach drive to creep up to a pin before grabbing.
// dir         – forward or reverse (default forward)
// distance_mm – how far to creep   (default 80 mm)
// speed       – velocity in %      (default 20 %)
void driveNudge(directionType dir, double distance_mm, uint16_t speed, bool waitUntilDone) {
    double totalDeg = Distance_MM_to_Degrees(distance_mm);

    mot_dtLeft.setPosition(0.0, degrees);
    mot_dtRight.setPosition(0.0, degrees);

    mot_dtLeft.spin(dir, speed, pct);
    mot_dtRight.spin(dir, speed, pct);

    if (!waitUntilDone) return;

    while (true) {
        double pos = (fabs(mot_dtLeft.position(degrees)) +
                      fabs(mot_dtRight.position(degrees))) / 2.0;
        if (pos >= totalDeg) break;
        wait(10, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
}

// Drop the pin and return arm to the up position.
void Auto_Drop_Down_Pin_Grab_Up() {

 printf("drop down pin");
    printf("\n");
    
    wait(0.1, seconds);
    printf("stop mov");
    printf("\n");
  
    mg_pin.setMaxTorque(100.0, percent);
    mg_pin.setStopping(coast);
    mg_pin.setVelocity(80.0, percent);
   
    mg_pin.spinFor(forward, 80 , degrees, true);
    ReleasePin; 
    wait(0.1, seconds);
    if(beamPos != top)
        YGuidInSafe();
    printf("stop mov");
    printf("\n");
    OverRideDriveTrain = true;

    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    // wait(0.3, seconds);
    wait(0.2, seconds);
    handUp;
    // wait(0.1, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop(); 
    OverRideDriveTrain = false;

    Drop_Pin_Arm();
  
   
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
    
    mot_dtRight.spin(reverse);
    mot_dtLeft.spin(forward);
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

    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.spin(forward);
    mot_dtLeft.spin(reverse);

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
    YGuidInSafe();
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

// Drive forward or reverse until the rear distance sensor reads targetDist_mm.
// Reads the sensor once, computes the distance and direction needed, then
// delegates to driveRamped for a smooth ramp-up / ramp-down profile.
// Because the sensor faces the wall behind the bot:
//   current < target  →  drive forward  (bot moves away, reading grows)
//   current > target  →  drive reverse  (bot moves toward wall, reading shrinks)
void driveToWallDistance(double targetDist_mm,
                         uint16_t maxSpeed,
                         uint16_t minSpeed,
                         double   rampFrac,
                         bool     waitUntilDone) {
    double current  = dis_rear.objectDistance(mm);
    double error    = targetDist_mm - current;  // positive = need more distance

    if (fabs(error) <= 20) return;  // already close enough, nothing to do

    directionType dir = (error > 0) ? reverse : forward;  // reverse to get more distance, forward to get closer

    if (!waitUntilDone) {
        mot_dtLeft.spin(dir, maxSpeed, pct);
        mot_dtRight.spin(dir, maxSpeed, pct);
        return;
    }

    driveRamped(dir, fabs(error), maxSpeed, minSpeed, rampFrac);
}

// PD turn controller to a target heading.
void turnTo(double targetDeg) {

    double Kp = 0.8;       // ค่าปกติเริ่มต้น
    double Kd = 0.06;      // ลด overshoot
    double maxPower = 40;  // จำกัดเพื่อความ smooth
    double minPower = 5;   // ป้องกัน stall
    
    double error, prevError = 0;
    double derivative;
    double power;

    while(true) {
        double angle = Inertial.angle();
        error = targetDeg - angle;

        // normalize wrap-around
        if (error > 180) error -= 360;
        if (error < -180) error += 360;

        if (fabs(error) < 0.3) break; // deadband

        derivative = error - prevError;
        power = Kp * error + Kd * derivative;

        // limit power
        if (fabs(power) > maxPower) power = copysign(maxPower, power);
        if (fabs(power) < minPower) power = copysign(minPower, power);

        // turn
        mot_dtRight.spin(fwd,  power, pct);
        mot_dtLeft.spin(fwd, -power, pct);

        prevError = error;
        wait(10, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
}

// Spin relative to current heading with a double-check settle.
// dir: +1 = clockwise, -1 = counter-clockwise
static double _relTarget(double deg, double dir) {
    return Inertial.angle() + deg * dir;
}

void spinRelative45(double dir) {
    double target = _relTarget(45, dir);
    turnTo(target);
    wait(0.5, seconds);
    turnTo(target);
    wait(0.5, seconds);
}

void spinRelative90(double dir) {
    double target = _relTarget(90, dir);
    turnTo(target);
    wait(0.5, seconds);
    turnTo(target);
    wait(0.5, seconds);
}

void spinRelative100(double dir) {
    double target = _relTarget(100, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative110(double dir) {
    double target = _relTarget(110, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative120(double dir) {
    double target = _relTarget(120, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative130(double dir) {
    double target = _relTarget(130, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative140(double dir) {
    double target = _relTarget(140, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative150(double dir) {
    double target = _relTarget(150, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative160(double dir) {
    double target = _relTarget(160, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative170(double dir) {
    double target = _relTarget(170, dir);
    turnTo(target); wait(0.5, seconds);
    turnTo(target); wait(0.5, seconds);
}

void spinRelative180(double dir) {
    double target = _relTarget(180, dir);
    turnTo(target);
    wait(0.5, seconds);
    turnTo(target);
    wait(0.5, seconds);
}