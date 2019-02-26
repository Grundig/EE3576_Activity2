#include<Act2_2.h>

Act2_2 our_sys;

void setup() {
  // put your setup code here, to run once:
  int start_pin = 4;
  int stop_pin = 5;
  int reverse_pin = 6;
  int motor_pin = 9;
  int direction_pin = 8;
  int speed_pin = int_1;   // Pin 3
  int low_speed_pin = 11;
  int mid_speed_pin = 12;
  int high_speed_pin = 13;
  
  int adjust_speed_time=250;
  int target_speed_time=1000;
  int buttons_check_time=200;
  
  our_sys.setup_motor_pushbuttons(start_pin, stop_pin, reverse_pin);
  our_sys.setup_motor(motor_pin, direction_pin);
  our_sys.setup_speed_measure(speed_pin);
  our_sys.set_time_intervals(buttons_check_time, adjust_speed_time, target_speed_time);
  our_sys.setup_speed_pushbuttons(low_speed_pin, mid_speed_pin, high_speed_pin);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  our_sys.system_execute();
}
