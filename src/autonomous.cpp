#include "vex.h"
#include "main.h"
#include "pin.h"
using namespace vex;

#define IS_IN_RANGE(value, min, max) ((value >= min) && (value <= max))

uint16_t driveSpeed = 70;
double distanceToGo;
void Setup();
void Step1(); // 1st 3colors stack
void Step2_1();
void Step2_2();
void Step2_3();
void Step3();
void Step4();
void pushStackAway();
// 1 wheel rotation = 8 inches
int TaskAutonomous() {
   
    Brain.Screen.setCursor(2, 1);
  
    OverRideDriveTrain = false;
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent);
    mot_dtLeft.setPosition(0.0, degrees);
    mot_dtRight.setPosition(0.0, degrees);
    Setup();
    while(TouchLED12.pressing() == false){
        wait(20, msec);
    }
  
    Step1();
    Step2_1();
    Step2_2();
    Step2_3();
    Step3();
    Step4();
    while (true) {
        if(!OverRideDriveTrain) {
            if(Controller.AxisA.position() > 80){
                mot_dtLeft.spin(forward);
                mot_dtRight.spin(forward);
            }
            else if(Controller.AxisA.position() < -80){
                mot_dtLeft.spin(reverse);
                mot_dtRight.spin(reverse);
            }
            else if(Controller.AxisB.position() > 80){
                mot_dtLeft.spin(forward);
                mot_dtRight.spin(reverse);
            }
            else if(Controller.AxisB.position() < -80){
                mot_dtLeft.spin(reverse);
                mot_dtRight.spin(forward);
            
            }
            else {
                mot_dtLeft.stop();
                mot_dtRight.stop();
            }
        
        }
    wait(20, msec);
    }
    return 0;
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
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(100), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(100), degrees, true);
    // release stack
    // fBtnEupPressed = true;
    // wait(1, sec);
    //move backward to prevuious position
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(100), degrees, true);
   
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
    myblockfunction_Drop_Down_Grab_Up();
   
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
    myblockfunction_Drop_down();
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
    myblockfunction_Grab_then_up();
   
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
    
    

    myblockfunction_Drop_down();

    // move forward to red
    mot_dtRight.setVelocity(50, percent);
    mot_dtLeft.setVelocity(50, percent); 
    
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    // grab red
    myblockfunction_Grab_then_up();
   

}

void Step2_2(){
    printf("Serching for Blue\n");
    // dis lef 185, righ 240
    // turn right to blue
    // turn to dis left < 240 righ > 500
    mot_dtLeft.spin(forward,10, percent);
    mot_dtRight.spin(reverse,10, percent);
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
    myblockfunction_Drop_Down_Grab_Up();

    
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
    myblockfunction_Drop_down();
    pushStackAway();

    printf("Step2 End\n");
}

void Step3(){
    // move backward
    // distance left 225, right >500
    // grab red on left craw
    printf("Step3 Start\n");
    printf("turning left to Red\n");
    myblockfunction_Grab_then_up();
    // while(1){
    //     //PrintDistance();
    //     wait(0.5, sec);
    // }
    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    wait(2, sec);
    while(1){       
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),240,330) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),500,700) 
        ){
            break;
        }
        wait(5, msec);
        //PrintDistance();
    }

    mot_dtLeft.stop();
    mot_dtRight.stop();
    printf("red 3 found ");
    PrintDistance();
    distanceToGo = dis_left.objectDistance(mm) + 40;
    myblockfunction_Drop_down();
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    myblockfunction_Grab_then_up();
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
    myblockfunction_Drop_Down_Grab_Up();

    printf("turning left to yellow\n");
    mot_dtLeft.spin(reverse,15, percent);
    mot_dtRight.spin(forward,15, percent);
    wait(2, sec);
    while(1){       
         if(IS_IN_RANGE((uint16_t)dis_left.objectDistance(mm),90,200) 
            && IS_IN_RANGE((uint16_t)dis_right.objectDistance(mm),450,1000) 
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
    myblockfunction_Drop_down();
    pushStackAway();
   // push stack away
   

    printf("Step3 End\n");
}

void Step4(){
    myblockfunction_Grab_then_up();
    printf("turning left to Red\n");
    // while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
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
    distanceToGo = dis_left.objectDistance(mm) + 40;
    
    //  while(TouchLED12.pressing() == false){
    //     wait(20, msec);
    // }
    myblockfunction_Drop_down();
    printf("Moving to Red\n");
    // move forward to red
    mot_dtRight.setVelocity(driveSpeed, percent);
    mot_dtLeft.setVelocity(driveSpeed, percent); 
    mot_dtLeft.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, false);
    mot_dtRight.spinFor(forward, Distance_MM_to_Degrees(distanceToGo), degrees, true);
    mot_dtLeft.stop();
    mot_dtRight.stop();
    // grab red
    myblockfunction_Grab_then_up();
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
    myblockfunction_Drop_Down_Grab_Up();
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
    myblockfunction_Drop_down();
    mot_dtLeft.spinFor(reverse, Distance_MM_to_Degrees(200), degrees, false);
    mot_dtRight.spinFor(reverse, Distance_MM_to_Degrees(200), degrees, true);
    // wait(2, sec);
    printf("Step4 End\n");
    Brain.programStop();
    
}