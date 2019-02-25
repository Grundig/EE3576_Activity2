#ifndef Act2_1_h
#define Act2_1_h

#include<Arduino.h>
#include<Basic_Input.h> 
#include<PushButton.h>
#include<IntervalCheckTimer.h>
#include<DCmotor.h>
#include <InterruptBasedSpeedMeasure.h>

class Act2_1{
	
	protected:
		in_analog motor_potentiometer;
		inputs motor_pushbuttons;
		HBridgeDCmotor motor;
		in_analogWmapping potent_map;
		InterruptSpeedMeasure rotation_counter;

		IntervalCheckTimer check_inp_time;
		IntervalCheckTimer speed_check;
		
		int map_min_val = 0;
		int map_max_val = 100;
		int speed_control_ms = 2000;
		
		bool pushbuttons_enabled = false, potentiometer_enabled = false, time_enabled = false, motor_enabled = false, speed_enabled = false;
		bool enabled;
		
		
	public:
		
		Act2_1()
		{
			enabled = false
		}
		
		bool isEnabled()
		{
			if (pushbuttons_enabled && potentiometer_enabled && time_enabled && motor_enabled && speed_enabled)
			{
				enabled = true;
				return enabled;
			}
			
			return enabled;
		}
		
		void setup_pushbuttons(int start_pin, int stop_pin, int reverse_pin)
		{
			if (!isEnabled())
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
		}
		
		void setup_potentiometer(int analog_pin)
		{
			if(!isEnabled())
			{
				// setup potentiometer
				motor_potentiometer.setup_in_analog(analog_pin);
			
				// setup map
				potent_map.setup_map(map_min_val, map_max_val);
			
				potentiometer_enabled = true;
			}
		}
		
		void set_time_between_input_checks(int check_time)
		{
			if(!isEnabled())
			{
				//buttons
  				check_inp_time.setInterCheck(check_time);
  			
  				time_enabled = true;
  			}
		}
		
		void setup_motor(int motor_pin, int direction_pin)
		{
			if(!isEnabled())
			{
				motor.setup_HBridgeDCmotor(motor_pin, direction_pin);
			
				motor_enabled = true;
			}
		}
		
		void setup_speed_measure(int speed_pin)
		{
			if(!isEnabled())
			{
				rotation_counter.setupSpeedMeasure(speed_pin);
				speed_check.setInterCheck(speed_control_ms);
				
				speed_enabled = true;
			}
		}
		
		void motor_direction(command_list_enum in_smpl_cmd)
		{
			switch (in_smpl_cmd)
			{
        		case start:
        		Serial.println(" Start button pressed");
        		motor.start();
        		break;
        		
        		case stop:
        		Serial.println("    Stop button pressed");  
        		motor.stop();
        		break;
        		
        		case reverse:
        		Serial.println("        Reverse button pressed");
        		motor.changedir();
        		break;
        		
        		default:
          		Serial.println("Unknown button pressed");
          		break;

		}
		
		void motor_speed(int val)
		{
			motor.setSpeedPWM(val);
		}
		
		void read_motor_speed()
		{
			double RPM=rotation_counter.getRPMandUpdate();
    			if(RPM>0)
    			{
    				Serial.print("revs per min = ");
    				Serial.println(RPM);
    			}
    					
    			else
    			{
    				Serial.println("Reading speed failed");
    			}
		}
		
		system_execute()
		{
			if (isEnabled())
			{
				// check buttons as often as needed
  				if(check_inp_time.isMinChekTimeElapsedAndUpdate()) 
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
  					
  					if (speed_check.isMinChekTimeElapsedAndUpdate())
  					{
  						read_motor_speed();
  					}
  				}
  			}
		}
};









#endif
