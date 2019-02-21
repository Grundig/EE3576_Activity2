#include <DCmotor.h>
#include <InterruptBasedSpeedMeasure.h>

// h-bridge motor
HBridgeDCmotor motor;


// timer used to measure speed at given intervals
IntervalCheckTimer speed_check;


// timer used to change speed (PWM) at a given intervals
IntervalCheckTimer change_of_speed_check;


// Objects that measure speed (RPM)

// First option: direct speed calculation
InterruptSpeedMeasure speedometer;



// Second option: comment the one above and use this one instead
// to derive a smoothed version of measured speed (RPM).
// This  derived class shares all functions with the parent (above)
// but it implements a "moving average" smoothing of the speed. 

//InterruptSpeedMeasure_SMA speedometer;


int array_index=0;

void setup()
{
 // motor 
  int motorpin=11;
  int directionpin=12;
  // enable motor pin
  motor.setup_HBridgeDCmotor(motorpin, directionpin);
  
  
  // initialize the speedometer
  // input pullup is enabled internally now
  // int_0 operates on pin 2
  speedometer.setupSpeedMeasure(int_0);
  
  // timer to change target speed at given interval:
  // set the time between speed changes 
  int speed_change_ms=15000;  
  change_of_speed_check.setInterCheck(speed_change_ms);
 
  // timer to measure speed at given interval:
  // set the time between inversions
  int speed_check_ms=1000;  
  speed_check.setInterCheck(speed_check_ms);
  
  int sma_windowsie=4;
  // If you're using the speedometer with moving average
  // you can uncomment the line below to set the window size; 
  // (otherwise default size 10 is used)
  //speedometer.setupSMAarray(sma_windowsie);
  
  
  // to print to screen
  Serial.begin(9600);
  
  
  int stablespeed=55;
  // this is to kick-start the motor
  //motor.set_jumpstart(true);
  motor.start();
  motor.setSpeedPWM(stablespeed);
  
}

void loop()
{
  static const int ARRAY_SIZE=5;
  int pwmspeed_array[ARRAY_SIZE]={64, 128, 80, 164, 200};
  // reverse the speed every ;
  
  // change speed when needed
  if(change_of_speed_check.isMinChekTimeElapsed())
  {
    int target_speed;
    change_of_speed_check.updateCheckTime();
    
    target_speed=pwmspeed_array[array_index];
    array_index++;
    if(array_index>=ARRAY_SIZE)
      array_index=0;
    
   
    //Serial.println("Reversing Speed");
    motor.setSpeedPWM(target_speed);
    
    Serial.println("");
    Serial.print("target pwm = ");
    Serial.println(target_speed);
    Serial.println("");
  }
  
  // measure speed when needed
  if(speed_check.isMinChekTimeElapsed())
  {
    speed_check.updateCheckTime();    
    Serial.print("RPM = ");
    Serial.println(speedometer.getRPMandUpdate());
  }
 
}
