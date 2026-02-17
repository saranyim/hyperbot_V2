#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;




uint16_t driveSpeed = 70;
uint16_t turnSpeed = 15;
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
void trim_heading(uint16_t heading);
double Distance_MM_to_Degrees(double distance_mm);

void SpinLeft(uint16_t heading);
void SpinRight(uint16_t heading);

void turnTo(double targetDeg) ;

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

// 1 wheel rotation = 8 inches
// Main autonomous routine sequence.
int TaskAutonomous() {
    int ledBlinkCount;
    pneuVGuide.retract(pneuCPinGuide);
   
    Brain.Screen.setCursor(2, 1);
    TouchLED12.setColor(red);

    while(TouchLED12.pressing() == false){
        wait(0.02, seconds);
    }
    // debouce
    while(TouchLED12.pressing() == true){

        wait(0.02, seconds);
    
    }

    Inertial.calibrate();
    ledBlinkCount = 5;
    while(ledBlinkCount--){
        TouchLED12.setColor(yellow);
        wait(0.5,seconds);
        TouchLED12.setColor(red);
        wait(0.5,seconds);
    }

    Inertial.setHeading(90, degrees);
    mg_beam.setMaxTorque (100,percent);
    mg_beam.setVelocity (100,percent);
    mg_beam.spinFor (spinBeamUp,540,degrees,true);
    beamPos=mid;
    pneuVGrabber.retract(pneuCBeamGrab);
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
    pneuVGrabber.extend(pneuCBeamGrab);
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
    Grab_then_up();
   

    TouchLED12.setBlink(red_violet, 0.1, 0.1);
    while(TouchLED12.pressing() == false){
    
        wait(0.02, seconds);
        }
    vex::task wsAuto(TaskAutoCnt);
    
    TouchLED12.setColor(yellow_green);
    printf("touch\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    distanceToGo = dis_rear.objectDistance(mm);
    Set_Drop_Pin();
    from_Start_to_Yellow();
    // wait(0.5, seconds);
    // WaitTouchDebug();



    reverse_to_get_Blue();
    // wait(0.5, seconds);
    Grab_Beam_up();
    // WaitTouchDebug();
    printf("touch\n");
    spin_to_get_blue();
    // wait(0.5, seconds);
    // WaitTouchDebug();

    go_forward_to_make_stack();
   
    Auto_Drop_Down_Pin_Grab_Up();


    distanceToGo = 200;
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtLeft.stop();
    // wait(0.5, seconds);
    // WaitTouchDebug();


    reverse_to_set_distance();
    //  wait(0.5, seconds);



    

    go_backwards_to_place_pin_on_stand_off();
    // wait(0.3, seconds);
    // WaitTouchDebug();



    spin_to_get_beam();
    // wait(0.5, seconds);
    // WaitTouchDebug();

    go_backwards_to_get_beam();
    // wait(0.5, seconds);
    // WaitTouchDebug();

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
    distanceToGo = 1050;
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
   
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.spinFor(forward,220,degrees,false);
    mot_dtRight.spinFor(forward,220,degrees,false);
    wait(0.5,seconds);
    Grab_then_up();
    // WaitTouchDebug();
    
}



// Reverse to reach the blue goal position.
void reverse_to_get_Blue(){
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.setVelocity(100, percent);
    distanceToGo = 480;
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtRight.stop();
    // WaitTouchDebug();

      
}

// Turn toward the blue target.
void spin_to_get_blue(){
    turnTo(155);
//    SpinRight(156);
//    WaitTouchDebug();
}

// Drive forward to stack position.
void go_forward_to_make_stack(){
    // WaitTouchDebug();
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtRight.setVelocity(driveSpeed, percent);
    distanceToGo = 850;
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtLeft.stop();
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.6, seconds);
    mot_dtRight.stop();
    mot_dtLeft.stop();
}

// Reverse to a target distance using the rear sensor.
void reverse_to_set_distance(){
// SpinRight(180);
    turnTo(180);

// WaitTouchDebug();

    distanceToGo = dis_rear.objectDistance(mm) - 1080;
    if(distanceToGo < 0) {
        distanceToGo = 0;
    }
    mot_dtLeft.setVelocity(40, percent);
    mot_dtRight.setVelocity(40, percent);
    
    
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtRight.stop();
    wait(0.5, seconds);
    SpinLeft(90);
    // WaitTouchDebug();
      
}


// Back into place and drop the pin on the standoff.
void go_backwards_to_place_pin_on_stand_off(){
    mot_dtRight.setVelocity(85, percent);
    mot_dtLeft.setVelocity(85, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.0, seconds);
    mot_dtRight.stop();
    mot_dtRight.stop(); 
    wait(0.5, seconds);      
    Place_Pin_On_Stand_Off();
}

// Turn to face the beam target.
    void spin_to_get_beam(){
        turnTo(270);
    }



// Reverse to pick up the beam.
    void go_backwards_to_get_beam(){
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.8, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.2, seconds);
    pneuVGrabber.extend(cylinder1);
    
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
    return distance_mm / (8.0* 25.4) * 360.0;
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

    double Kp = 1.4;       // ค่าปกติเริ่มต้น
    double Kd = 0.12;      // ลด overshoot
    double maxPower = 70;  // จำกัดเพื่อความ smooth
    double minPower = 10;  // ป้องกัน stall
    
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
        mot_dtLeft.spin(fwd,  power, pct);
        mot_dtRight.spin(fwd, -power, pct);

        prevError = error;
        wait(10, msec);
    }

    mot_dtLeft.stop(brake);
    mot_dtRight.stop(brake);
}