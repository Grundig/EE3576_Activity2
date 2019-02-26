#include <MultyUnitClosedLoopSystem.h>

//NOTE:
// the following libraries should be present in the appropriate folders and be up-to-date. 
// in case of compiler error: Download from my aberdeen again 
// PushButton.h
// SystemControlUnit.h
// DCmotor.h
// InterruptBasedInputs.h
// InterruptBasedSpeedMeasure.h
// Basic_Input.h

// this is the main library and your starting point in udnerstanding the system
// MultyUnitClosedLoopSystem.h


// this is the closed loop system
ClosedLoopSystemSImple testsystem;


void setup()
{
  // setup the potentiometr
  // this communicate the target RPM in range [1000 8000]
  testsystem.add_potentiometer(A2);

  // add three pushbuttons (pin and label)
  // change the pin number as appropriate but keep the labels (switch_on, switch_off, change_spin_dir)
  testsystem.add_buttonpin_and_label(4, switch_on);
  testsystem.add_buttonpin_and_label(5, switch_off);
  testsystem.add_buttonpin_and_label(6, change_spin_dir);

  // setup the Hbridgemotor
  int pwmmotorpin=9;
  int directionpin=8;
  testsystem.setup_HBridgeDCmotor(pwmmotorpin, directionpin);
  
  // setup the speed measuring unit on pin 2 (interrupt 0)
  testsystem.setupSpeedMeasure(int_1);

  // adjust the itervals between 
  //two speed-adjustments; two taget-speed checks; two button checks; 
  int adjust_speed_interval_ms=250;
  int target_speed_check_interval_ms=1000;
  int buttons_check_interval_ms=200;
  testsystem.set_interval_vals(adjust_speed_interval_ms,target_speed_check_interval_ms,buttons_check_interval_ms);

  // to enable the verbose mode to print info on screen (off by default, makes program slower)
  testsystem.setVerbose(true);
  
  //Serial.begin(9600);
  
}


void loop()
{
  testsystem.CheckInputsAndControlMotor();
  delay(1000);
}
