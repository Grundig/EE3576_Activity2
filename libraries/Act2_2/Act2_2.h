#ifndef Act2_2_h
#define Act2_2_h

#include <Act2_1.h>

#include<PushButton.h>
#include<SerialPrinterPlotter.h>
#include<basic_speed_PID.h>
#include<DCmotor.h>


class Act2_2{
	
	protected:
		// Objects
		Act2_1 task1;
		SerialPrinterPlotter plotter;
		basic_speed_PID pid;
		inputs pushbuttons;
		HBridgeDCmotor HBmotor;
		InterruptSpeedMeasure rotate_count;
		IntervalCheckTimer button_time_check, adjust_speed_interval, target_speed_check;
		
		
				
		// Values
		double motor_low_speed = 77;  // 30% rated speed (6000 rpm)
		double motor_mid_speed = 128;  // 50% rated speed
		double motor_high_speed = 204;  // 80% rated speed
		
		double motor_low_speed_rpm = 1800;  // 30% rated speed (6000 rpm)
		double motor_mid_speed_rpm = 3000;  // 50% rated speed
		double motor_high_speed_rpm = 4800;  // 80% rated speed
		
		int adjust_speed_time=250;
  		int target_speed_time=1000;
 		int buttons_check_time=200;
		
	public:
		
		// Set up direction push buttons
		void setup_motor_pushbuttons(int start_pin, int stop_pin, int reverse_pin)
		{
			// Set long push
			unsigned long int mininterval_ms=2000;
			
			// Labelling and assigning each pin
  			in_push_button start_but(start_pin, start, mininterval_ms);
  			in_push_button stop_but(stop_pin, stop, mininterval_ms);
  			in_push_button reverse_but(reverse_pin, reverse, mininterval_ms);
  			
  			// Add push button into system
  			pushbuttons.add_in_push_button(start_but);
 			pushbuttons.add_in_push_button(stop_but);
  			pushbuttons.add_in_push_button(reverse_but);
  			
		}
		
		// Set up speed push buttons
		void setup_speed_pushbuttons(int low_speed_pin, int mid_speed_pin, int high_speed_pin)
		{
			// Set long push
			unsigned long int mininterval_ms=2000;
			
			// Labelling and assigning each pin
  			in_push_button low_but(low_speed_pin, low, mininterval_ms);
  			in_push_button mid_but(mid_speed_pin, mid, mininterval_ms);
  			in_push_button high_but(high_speed_pin, high, mininterval_ms);
  			
  				// Add push button into system
  			pushbuttons.add_in_push_button(low_but);
 			pushbuttons.add_in_push_button(mid_but);
  			pushbuttons.add_in_push_button(high_but);

		}
		
		// Set up motor
		void setup_motor(int pwm_motor_pin, int direction_pin)
		{
			HBmotor.setup_HBridgeDCmotor(pwm_motor_pin, direction_pin);
		}
		
		// Set up speed measuring unit, hall effect sensor
		void setup_speed_measure(ArduinoInterruptNames speed_pin)  // int_0, pin 2
		{
			rotate_count.setupSpeedMeasure(speed_pin);
		}
		
		// Set times between checks for components and system
		void set_time_intervals(int buttons_check_time, int adjust_speed_time, int target_speed_time)
		{
			button_time_check.setInterCheck(buttons_check_time);
			adjust_speed_interval.setInterCheck(adjust_speed_time);
			target_speed_check.setInterCheck(target_speed_time);
		}
		
		// Determine motor speed command (low, mid, high) from pushbuttons
		void motor_speed_input(command_list_enum in_smpl_cmd)
		{	
			switch (in_smpl_cmd)
			{
				case start:
        		Serial.println(" Start button pressed");
        		HBmotor.start();
        		break;
        		
        		case stop:
        		Serial.println("    Stop button pressed");  
        		HBmotor.stop();
        		break;
        		
        		case reverse:
        		Serial.println("        Reverse button pressed");
        		HBmotor.changedir();
        		break;
        
        		case low:
        		Serial.println(" Low button pressed");
        		HBmotor.setSpeedPWM(motor_low_speed);
        		break;
        		
        		case mid:
        		Serial.println("    Mid button pressed");  
        		HBmotor.setSpeedPWM(motor_mid_speed);
        		break;
        		
        		case high:
        		Serial.println("        High button pressed");
        		HBmotor.setSpeedPWM(motor_high_speed);
        		break;
        		
        		default:
          		Serial.println("Unknown button pressed");
          		break;
			}
			
		}
		
		double read_motor_speed()
		{
			double RPM=rotate_count.getRPMandUpdate();
    			if(RPM>0)
    			{
    				Serial.print("revs per min = ");
    				Serial.println(RPM);
    			}
    					
    			else
    			{
    				Serial.println("Reading speed failed");
    			}
    			
    		return RPM;
		}
		
		void system_execute()
		{
			command_list_enum in_smpl_cmd;				
			bool success_command_direction, success_speed, success_command_speed;
			double curr_speed;
			double pid_out;
			double target_speed;
			
			// Check buttons
			if (button_time_check.isMinChekTimeElapsedAndUpdate())
			{
				
				// Get motor command and output
          		success_command_direction = pushbuttons.check_n_get_command(in_smpl_cmd);
				// Get motor speed command and output
				success_command_speed = pushbuttons.check_n_get_command(in_smpl_cmd);
				
				if (success_command_direction)
				{
					motor_speed_input(in_smpl_cmd);
					
				}
				
				if (success_command_speed)
				{
					motor_speed_input(in_smpl_cmd);
				}
				
			}
				
			// Get current speed, use adjust speed interval
			if(adjust_speed_interval.isMinChekTimeElapsedAndUpdate())
			{
				curr_speed = read_motor_speed();
			}
				
			// PID controller to adjust speed to set point, use target speed check
			if(target_speed_check.isMinChekTimeElapsedAndUpdate())
			{
				pid_out = pid.ComputePID_output(target_speed, curr_speed);
			}
			
			HBmotor.setSpeedPWM(pid_out);
			
			// Send value to screen using serial print plotter
//			plotter.appendval(pid_out, curr_speed, target_speed);
//			plotter.print_the_string();

			
			
		}		
};


#endif
