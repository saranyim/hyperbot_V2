#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;



#define IS_IN_RANGE(value, min, max) ((value >= min) && (value <= max))

uint16_t driveSpeed = 70;
uint16_t turnSpeed = 15;
uint16_t adjustTurnSpeed = 6;
double distanceToGo;
void Auto_Drop_Down_Pin_Grab_Up();
void from_Start_to_Yellow();
void reverse_to_get_Blue();
void spin_to_get_blue();
void spin_to_get_to_position();
void go_forward_to_make_stack();
void Spin_to_place_pin_on_stand_off();
void go_backwards_to_place_pin_on_stand_off();
void spin_to_get_beam();
void go_backwards_to_get_beam();
void go_forward_to_spin_to_stand_off();
void spin_to_get_to_standoff();
void go_reverse_to_stand_off();
void go_backwards_to_get_in_position();
void spin_to_get_beam_2();
double Distance_MM_to_Degrees(double distance_mm);

void SpinLeft(uint16_t heading);
void SpinRight(uint16_t heading);

int TaskAutoCnt(){
    wait(60,sec);
    // Brain.programStop();
     return 0;
}

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

   Inertial.setHeading(0, degrees);
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
        if((uint16_t)Inertial.heading() == 301){
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
        // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    }
    vex::task wsAuto(TaskAutoCnt);
    
    TouchLED12.setColor(yellow_green);
    printf("touch\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    distanceToGo = dis_left.objectDistance(mm);
    Drop_Down_Pin();
    from_Start_to_Yellow();
    wait(0.5, seconds);
    // WaitTouchDebug();

    spin_to_get_to_position();
    wait(0.5, seconds);
    // WaitTouchDebug();
    printf("touch\n");

    reverse_to_get_Blue();
    wait(0.5, seconds);
    Grab_Beam_up();
    // WaitTouchDebug();
    printf("touch\n");
    spin_to_get_blue();
    wait(0.5, seconds);
    // WaitTouchDebug();

    go_forward_to_make_stack();
    Auto_Drop_Down_Pin_Grab_Up();
    wait(0.5, seconds);
    // WaitTouchDebug();


    Spin_to_place_pin_on_stand_off();

    wait(0.5, seconds);
    // WaitTouchDebug();

    go_backwards_to_place_pin_on_stand_off();
    wait(0.3, seconds);
    // WaitTouchDebug();



    spin_to_get_beam();
    wait(0.5, seconds);
    // WaitTouchDebug();

    go_backwards_to_get_in_position();
    wait(0.5, seconds);

    spin_to_get_beam_2();
    wait(0.5, seconds);

    go_backwards_to_get_beam();
    wait(0.5, seconds);
    // WaitTouchDebug();

    Flip_Pin_Over();
    wait(0.5, seconds);
    go_forward_to_spin_to_stand_off();
    // WaitTouchDebug();

    spin_to_get_to_standoff();
    wait(0.3, seconds);
    // WaitTouchDebug();

    Grab_Beam_up();
    wait(0.5, seconds);
    // WaitTouchDebug();

    go_reverse_to_stand_off();
    wait(0.5, seconds);
    // WaitTouchDebug();
    Place_Beam_Stand_Off();
    wait(0.5, seconds);


    
    return 0;
    
}
void from_Start_to_Yellow(){
    distanceToGo = 1050;
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
   
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.spinFor(forward,180,degrees,false);
    mot_dtRight.spinFor(forward,180,degrees,false);
    wait(0.5,seconds);
    Grab_then_up();
    
    // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    
}

void spin_to_get_to_position(){
    SpinLeft(260);
}



void reverse_to_get_Blue(){
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.setVelocity(100, percent);
    distanceToGo = 600;
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtRight.stop();

      
}

void spin_to_get_blue(){
   SpinRight(285);
}

void go_forward_to_make_stack(){
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtRight.setVelocity(driveSpeed, percent);
    distanceToGo = 550;
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtLeft.stop();
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.55, seconds);
    mot_dtRight.stop();
    mot_dtLeft.stop();
}

void Spin_to_place_pin_on_stand_off(){
    SpinLeft(266);
}

void go_backwards_to_place_pin_on_stand_off(){
    mot_dtRight.setVelocity(85, percent);
    mot_dtLeft.setVelocity(85, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.8, seconds);
    mot_dtRight.stop();
    mot_dtRight.stop();       
    Place_Pin_On_Stand_Off();
}

void spin_to_get_beam(){
    SpinRight(50                                              );
}

void go_backwards_to_get_in_position(){
    mot_dtLeft.setVelocity(100, percent);
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(0.5, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();


}

void spin_to_get_beam_2(){
    SpinRight(50                                              );
}

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

void go_forward_to_spin_to_stand_off(){
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.6, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
}

void spin_to_get_to_standoff(){
    SpinRight(266);
}

void go_reverse_to_stand_off(){
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.5, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
}
 



double Distance_MM_to_Degrees(double distance_mm){
    return distance_mm / (8.0* 25.4) * 360.0;
}



void Auto_Drop_Down_Pin_Grab_Up() {

    mot_dtLeft.stop();
    mot_dtRight.stop();
    mg_pin.setMaxTorque(100.0, percent);
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.spinFor(forward,180,degrees,false);
    mot_dtRight.spinFor(forward,180,degrees,true);

    Drop_Down_Pin();
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
    GrabPin();   

    Grab_then_up();
   
}


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
}

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
    printf("Stop dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
   
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
}

void Auto_Flip_Pin_Over() {
    printf("start flip\n");
    GrabPin();
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
    ReleasePin();
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