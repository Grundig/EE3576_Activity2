#include <DCmotor.h>
//#include <InterruptBasedInputs.h>

HBridgeDCmotor motor;
IntervalCheckTimer reverse_speed_check;

void setup()
{
 // motor to be controlled: it does not have pushbuttons
  int motorpin=11;
  int directionpin=12;
  // enable motor pin
  motor.setup_HBridgeDCmotor(motorpin, directionpin);
  
  
  // timer to reverse the speed measurement at given interval:
  // set the time between inversions
  int speed_reverse_ms=12000;  
  reverse_speed_check.setInterCheck(speed_reverse_ms);
  
  // to print to screen
 // Serial.begin(9600);
  
  
  int stablespeed=55;
  // this is to kick-start the motor at every change of direction
  motor.set_jumpstart(true);
  motor.start();
  motor.setSpeedPWM(stablespeed);
  
}

void loop()
{
  // reverse the speed every ;
  
  if(reverse_speed_check.isMinChekTimeElapsedAndUpdate())
  {
    //Serial.println("Reversing Speed");
    motor.changedir();
    //Serial.println("Reversed!");
    //Serial.println( motor.getSpeedPWM() );
    //Serial.println("");
    
  }
}
