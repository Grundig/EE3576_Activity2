#include<Act2_1.h>

Act2_1 sys;

void setup() {
  // put your setup code here, to run once:
  int start_pin = 3;
  int stop_pin = 4;
  int reverse_pin = 2;
  int analog_pin = A0;
  int motor_pin = 9;
  int direction_pin = 8;
  int speed_pin = 7;
  
  int check_time = 10;
  
  sys.setup_pushbuttons(start_pin, stop_pin, reverse_pin);
  sys.setup_potentiometer(analog_pin);
  sys.set_time_between_input_checks(check_time);
  sys.setup_motor(motor_pin, direction_pin);
  sys.setup_speed_measure(speed_pin);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  sys.system_execute();
}
