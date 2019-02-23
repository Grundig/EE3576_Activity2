#ifndef Act2_1_h
#define Act2_1_h

#include<Aduino.h>
#include<Basic_Input.h> 
#include<PushButton.h>
#include<IntervalCheckTimer.h>
#include<DCmotor.h>

class Act2_1{
	
	protected:
		in_analog motor_potentiometer;
		inputs motor_pushbuttons;
		HBridgeDCmotor motor;
		in_analogWmapping potent_map;

		IntervalCheckTimer buttons_check;
		IntervalCheckTimer potentiom_check;
		
		int map_min_val = 0;
		int map_max_val = 100;
		
		bool pushbuttons_enabled = false, potentiometer_enabled = false, time_enabled = false, motor_enabled = false;
		
		
	public:
		
		Act2_1()
		{
			// Constructor, TO BE MADE
		}
		
		bool isEnabled()
		{
			if (pushbuttons_enabled && potentiometer_enabled && time_enabled && motor_enabled)
			{
				return true;
			}
		}
		
		void setup_pushbuttons(int start_pin, int stop_pin, int reverse_pin)
		{
			// Set long push
			unsigned long int mininterval_ms=2000;
			
			// Labelling and assigning each pin
  			in_push_button start_but(start_pin, start, mininterval_ms);
  			in_push_button stop_but(stop_pin, stop, mininterval_ms);
  			in_push_button reverse_but(reverse_pin, reverse, mininterval_ms);
  			
  			// Adding push button into system
  			motor_pushbuttons.add_in_push_button(start_but);
 			motor_pushbuttons.add_in_push_button(stop_but);
  			motor_pushbuttons.add_in_push_button(reverse_but);
  			
  			pushbuttons_enabled = true;
		}
		
		void setup_potentiometer(int analog_pin)
		{
			// setup potentiometer
			motor_potentiometer.setup_in_analog(analog_pin);
			
			// setup map
			potent_map.setup_map(map_min_val, map_max_val);
			
			potentiometer_enabled = true;
		}
		
		void set_time_between_input_checks(int button_time, int potent_time)
		{
			//buttons
  			buttons_check.setInterCheck(button_time);
  			
  			// potentiometer
  			potentiom_check.setInterCheck(potent_time);
  			
  			time_enabled = true;
		}
		
		void setup_motor(int motorpin, int directionpin)
		{
			motor.setup_HBridgeDCmotor(motorpin, directionpin);
			
			motor_enabled = true;
		}
		
		void motor_direction(command_list_enum in_smpl_cmd)
		{
			switch (in_smpl_cmd)
			{
        		case start:
        		//Serial.println(" Start button pressed");
        		motor.start();
        		break;
        		
        		case stop:
        		//Serial.println("    Stop button pressed");  
        		motor.stop();
        		break;
        		
        		case reverse:
        		//Serial.println("        Reverse button pressed");
        		motor.changedir();
        		break;
        		
        		default:
          		//Serial.println("Unknown button pressed");
          		break;

		}
		
		void motor_speed(int val)
		{
			motor.setSpeedPWM(val);
		}
		
		overall_system_execute_task()
		{
			if (isEnabled())
			{
				// check buttons as often as needed
  				if(buttons_check.isMinChekTimeElapsedAndUpdate() && potentiom_check.isMinChekTimeElapsedAndUpdate())
  				{
  					command_list_enum in_smpl_cmd;
    				bool success_command, success_val;
    				int val;
  					
  					success_command = motor_pushbuttons.check_n_get_command(in_smpl_cmd);
  					success_val = potent_map.read_input(val);
  				
  					if (success_command && success_val)
  					{
  						motor_direction(in_smpl_cmd);
  						motor_speed(val);
  					}
  				}
  			}
		}
};









#endif
