#include "vex.h"
#include "main.h"
#include "pin.h"
#include "beam.h"
using namespace vex;



#define IS_IN_RANGE(value, min, max) ((value >= min) && (value <= max))

uint16_t driveSpeed = 70;
uint16_t turnSpeed = 15;
double distanceToGo;
void Setup();
void Step1(); // 1st 3colors stack
void Step2_1();
void Step2_2();
void Step2_3();
void Step3();
void Step4();
void pushStackAway();
void from_Start_to_Yellow();
void reverse_to_get_Blue();
void spin_to_get_blue();
void spin_to_get_to_position();
void go_forward_to_make_stack();
void Spin_to_place_pin_on_stand_off();
void go_backwards_to_place_pin_on_stand_off();
void spin_to_get_beam();
void go_backwards_to_get_beam();
 double Distance_MM_to_Degrees(double distance_mm);

int TaskAutoCnt(){
    wait(60,sec);
    // Brain.programStop();
     return 0;
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
        if(IS_IN_RANGE((uint16_t)Inertial.heading(),301,302)){
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
   
    // while (1)
    // {
    //     wait(1, seconds);
    //    printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm)); /* code */
    // }
    
    
    // Setup();
    TouchLED12.setBlink(green, 0.1, 0.1);
    while(TouchLED12.pressing() == false){
    
        wait(0.02, seconds);
        // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    }
    TouchLED12.setColor(yellow_green);
    printf("touch\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    distanceToGo = dis_left.objectDistance(mm);
    Drop_Down_Pin();
    from_Start_to_Yellow();
    wait(0.5, seconds);

    // while(TouchLED12.pressing() == false){
    //       wait(0.02, seconds);
          
    //     // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    // }
    spin_to_get_to_position();
    wait(0.5, seconds);
    // while(TouchLED12.pressing() == false){
    //        wait(0.02, seconds);
    //     // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    // }
    // printf("touch\n");
       
    reverse_to_get_Blue();
    wait(0.5, seconds);
    Grab_Beam_up();

    // while(TouchLED12.pressing() == false){
    //       wait(0.02, seconds);
          
    //     // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    // }
    // printf("touch\n");
    spin_to_get_blue();
    wait(0.5, seconds);
    
    
     vex::task wsAuto(TaskAutoCnt);
    // TaskDriveTrain();

    // while(TouchLED12.pressing() == false){
    //       wait(0.02, seconds);
          
    //     // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    // }
    go_forward_to_make_stack();
    Drop_Down_Pin_Grab_Up();
    wait(0.5, seconds);

    Spin_to_place_pin_on_stand_off();

    wait(0.5, seconds);

    go_backwards_to_place_pin_on_stand_off();
    wait(0.3, seconds);
    

    spin_to_get_beam();
    wait(0.5, seconds);

    go_backwards_to_get_beam();
    wait(0.5, seconds);
    Flip_Pin_Over();

    
    return 0;
    
}
void from_Start_to_Yellow(){
    distanceToGo = 1000;
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    mot_dtLeft.setVelocity(30, percent);
    mot_dtRight.setVelocity(30, percent);
    mot_dtLeft.spin(forward);
    mot_dtRight.spin(forward);
    wait(0.80, seconds);
    mot_dtRight.stop();
    mot_dtLeft.stop();
    wait(0.2, seconds);
    Grab_then_up();
    
    // printf("dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
    
}

void spin_to_get_to_position(){
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtRight.spin(forward);
    mot_dtLeft.spin(reverse);

     while(1){
    //      if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),700,2000) 
    //         && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),260,270) 
    
        if(
            (uint16_t)Inertial.angle()<268
        )
        {
            break;
        }

        wait(5, msec);
        
    }
    printf("Stop dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
   
    mot_dtLeft.stop();
    mot_dtRight.stop();
}



void reverse_to_get_Blue(){
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.setVelocity(100, percent);
    distanceToGo = 1000-dis_left.objectDistance(mm);
 mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtRight.stop();

      
}

void spin_to_get_blue(){
    mot_dtRight.setVelocity(turnSpeed, percent);
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtRight.spin(reverse);
    mot_dtLeft.spin(forward);
     
  
    
    while(1){
    //      if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),700,2000) 
    //         && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),260,270) 
    
        if(
            (uint16_t)Inertial.angle()>288
        )
        {
            break;
        }

        wait(5, msec);
        
    }
    printf("Stop dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
   
    mot_dtLeft.stop();
    mot_dtRight.stop();
}

void go_forward_to_make_stack(){
mot_dtLeft.setVelocity(driveSpeed, percent);
mot_dtRight.setVelocity(driveSpeed, percent);
    distanceToGo = dis_left.objectDistance(mm);
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
    mot_dtLeft.setVelocity(turnSpeed, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(forward);

     while(1){
    //      if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),700,2000) 
    //         && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),260,270) 
    
        if(
            (uint16_t)Inertial.angle()<282
        )
        {
            break;
        }

        wait(5, msec);
        
    }
    printf("Stop dis_left = %d dis_right = %d\n",(uint16_t)dis_left.objectDistance(mm), (uint16_t)dis_right.objectDistance(mm));
   
    mot_dtLeft.stop();
    mot_dtRight.stop();
}

void go_backwards_to_place_pin_on_stand_off(){
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1.8, seconds);
    mot_dtRight.stop();
    mot_dtRight.stop();
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(0.5, seconds);
    mot_dtRight.stop();
    mot_dtRight.stop();
    Place_Pin_On_Stand_Off();
}

void spin_to_get_beam(){
    mot_dtRight.setVelocity(100, percent);
    mot_dtLeft.setVelocity(100, percent);
    distanceToGo = 775-dis_left.objectDistance(mm);
 mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtRight.stop();
    mot_dtRight.stop();

}

void go_backwards_to_get_beam(){
    mot_dtLeft.spin(reverse);
    mot_dtRight.spin(reverse);
    wait(1, seconds);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    wait(0.2, seconds);
    pneuVGrabber.extend(cylinder1);
    
}




double Distance_MM_to_Degrees(double distance_mm){
    return distance_mm / (8.0* 25.4) * 360.0;
}

void Setup(){
    TouchLED12.setColor(yellow);
     while(TouchLED12.pressing() == false){
        wait(20, msec);
    }
    fBtnRupPressed = true;
     wait(2, sec);
    while(1){

        if((uint16_t)dis_left.objectDistance(mm) > 680 && dis_right.objectDistance(mm) < 300){
            TouchLED12.setColor(green);
        }
        else{
            TouchLED12.setColor(red);
        }
        wait(50, msec);
        if(TouchLED12.pressing() == true){
            break;
        }
    }
}

void pushStackAway(){
    // fBtnEupPressed = true;
    // wait(1, sec);
    // turn right 
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(150), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(150), degrees, true);
    // move forward
    // mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(100), degrees, false);
    // mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(100), degrees, true);
    // // release stack
    // // fBtnEupPressed = true;
    // // wait(1, sec);
    // //move backward to prevuious position
    // mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, false);
    // mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, true);
   
    mot_dtLeft.stop();
    mot_dtRight.stop();
}

void Step1(){ // grab blue and turn left to yellow
    // move forward for 680 degrees
    printf("Step1 Start\n");
    printf("blue 1 found ");
    PrintDistance();
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    distanceToGo = dis_right.objectDistance(mm);
    
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("grab blue\n");
    Drop_Down_Pin_Grab_Up();
   
    printf("Turning to Yellow\n");
    // turn left
    mot_dtLeft.spin(reverse,5, percent);
    mot_dtRight.spin(forward,15, percent);
    while(1){
        if((uint16_t)dis_left.objectDistance(mm) > 1000 && dis_right.objectDistance(mm) < 160){
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("Yellow 1 Found ");
    PrintDistance();
    distanceToGo = dis_right.objectDistance(mm);
    printf("Go to Yellow\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    Drop_Down_Pin();
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(100), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(100), degrees, true);
    
    
}

void Step2_1(){
    // move backward
    printf("Step2 Start\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    Grab_then_up();
   
    // turn to find red
    // while(1){
    //     //PrintDistance();
    //     wait(0.5, sec);
    // }
    printf("Searching for Red\n");
    mot_dtLeft.spin(forward,15, percent);
    mot_dtRight.spin(reverse,15, percent);
    wait(1, sec);
    while(1){
        if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),175,245) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),450,550) 
        ){
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }
    mot_dtRight.stop();
    mot_dtLeft.stop();
    printf("red 2 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm);
    
    

    Drop_Down_Pin();

    // move forward to red
    mot_dtRight.setVelocity(50, percent);
    mot_dtLeft.setVelocity(50, percent); 
    
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    // grab red
    Grab_then_up();
   

}

void Step2_2(){
    printf("Serching for Blue\n");
    // dis lef 185, righ 240
    // turn right to blue
    // turn to dis left < 240 righ > 500
    // wait(1, sec);
    while(1){
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),200,320) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),800,1500) 
        ){
            break;
        }

        wait(5, msec);
        //PrintDistance();
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("blue 2 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm) + 80;
    printf("Blue Found\n");
   
   
    // move to blue
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("get Blue\n");
    // grab blue
    Drop_Down_Pin_Grab_Up();

    
}

void Step2_3(){
    // left > 800 right 219
    // turn left < 220 right > 800
    printf("turning to Yellow\n");
    mot_dtLeft.spin(forward,15, percent);
    mot_dtRight.spin(reverse,15, percent);
    // wait(1, sec);
    while(1){       
        if((uint16_t)dis_left.objectDistance(mm) < 250 && dis_right.objectDistance(mm) > 800)
        {
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("yellow 2 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm);
    
    printf("yellow Found\n");

    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("get yellow\n");
    Drop_Down_Pin();
    pushStackAway();
    printf("Step2 End\n");
}
    

void Step3(){
    // move backward
    // distance left 225, right >500
    // grab red on left craw
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    printf("Step3 Start\n");
    printf("turning left to Red\n");
    Grab_then_up();
    // while(1){
    //     //PrintDistance();
    //     wait(0.5, sec);
    // }
    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    wait(1.5, sec);
    while(1){       
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),240,330) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),500,1500) 
        ){
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    //     while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    printf("red 3 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm) + 40;
    Drop_Down_Pin();
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    Grab_then_up();
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    printf("turning left to blue\n");
    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    // wait(1, sec);
    while(1){       
        if((uint16_t)dis_left.objectDistance(mm) < 270 && dis_right.objectDistance(mm) > 550)
        {
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("blue 3 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm);
    
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    Drop_Down_Pin_Grab_Up();

    printf("turning left to yellow\n");
    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    wait(1.5, sec);
    while(1){       
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),90,220) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),450,2000) 
        ){
            break;
        }
        wait(5, msec);
        PrintDistance();
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("yellow 3 found ");
    PrintDistance();
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    distanceToGo = dis_left.objectDistance(mm);
    
    printf("Go Get yellow\n");
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    Drop_Down_Pin();
    pushStackAway();
   // push stack away
   

    printf("Step3 End\n");
}

void Step4(){
    Grab_then_up();
    printf("turning left to Red\n");
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }

    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(30), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(30), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();

    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    wait(2, sec);
    while(1){       
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),140,300) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),500,1000) 
        ){
            break;
        }
        wait(5, msec);
        PrintDistance();
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("red 4 found ");
    PrintDistance();
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    distanceToGo = dis_left.objectDistance(mm) + 60;
    
    //  while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    Drop_Down_Pin();
    printf("Moving to Red\n");
    // move forward to red
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    // grab red
    Grab_then_up();
    // wait(2, sec);
    printf("Serching for Blue\n");
    // dis lef 185, righ 240
    // turn right to blue
    // turn to dis left < 240 righ > 500
    mot_dtLeft.spin(reverse,10, percent);
    mot_dtRight.spin(forward,10, percent);
    // wait(1, sec);
    while(1){
        // if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),150,250) 
        //     && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),450,550) 
        // )
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),220,300) 
            && (uint16_t)dis_right.objectDistance(mm) > 1000 
        ) {
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("blue 4 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm) + 80;
    
    printf("Blue Found\n");
   
   
    // move to blue
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("get Blue\n");
    // grab blue
    Drop_Down_Pin_Grab_Up();
    //  while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    // left > 800 right 219
    // turn left < 220 right > 800
    printf("turning to Yellow\n");
    // mot_dtLeft.spin(reverse,10, percent);
    // mot_dtRight.spin(forward,10, percent);
    // // wait(1, sec);

    // while(1){       
    //     if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),240,320) 
    //         && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),600,1500) 
    //     ){
    //         break;
    //     }
    //     wait(5, msec);
    //     //PrintDistance();
    // }
    // mot_dtLeft.stop();
    // mot_dtRight.stop();
    // printf("yellow 4 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm) + 140;
    
    //  while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    printf("yellow Found\n");
    // move to blue
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("get yellow\n");
    Drop_Down_Pin();
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(200), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(200), degrees, true);
    // wait(2, sec);
    printf("Step4 End\n");
    Brain.programStop();
    
}