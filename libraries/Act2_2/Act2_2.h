#ifndef Act2_2_h
#define Act2_2_h

#include <Act2_1.h>

#include<PushButton.h>
#include<SerialPrinterPlotter.h>
#include<basic_speed_PID.h>
#include<DCmotor.h>

<<<<<<< Updated upstream
command_list_enum command;
=======
//command_list_enum direction;
command_list_enum speed;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
double target_speed = 2000;
double pid_out;
=======
int target_speed;
>>>>>>> Stashed changes


class Act2_2{
	
	protected:

		// Objects
		Act2_1 task1;
		SerialPrinterPlotter plotter;
		basic_speed_PID pid;
		inputs pushbuttons;
		HBridgeDCmotor HBmotor;
		InterruptSpeedMeasure rotate_count;
		IntervalCheckTimer button_time_check, target_speed_check, plot_time_check;
			
		// Values
		double motor_low_speed = 77;  // 30% rated speed (6000 rpm)
		double motor_mid_speed = 128;  // 50% rated speed
		double motor_high_speed = 204;  // 80% rated speed
		
		double motor_low_speed_rpm = 1800;  // 30% rated speed (6000 rpm)
		double motor_mid_speed_rpm = 3000;  // 50% rated speed
		double motor_high_speed_rpm = 4800;  // 80% rated speed
		
		// Global variabled
  		int target_speed_time;
 		int buttons_check_time;
 		
		
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
		void set_time_intervals(int buttons_check_time, int target_speed_time)
		{
			button_time_check.setInterCheck(buttons_check_time);
			target_speed_check.setInterCheck(target_speed_time);
			plot_time_check.setInterCheck(200);
		}
		        
		// Determine motor speed command (low, mid, high) from pushbuttons
		void motor_speed_input(command_list_enum command)
		{	
			switch (command)
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
        		target_speed = motor_low_speed;
        		break;
        		
        		case mid:
        		Serial.println("    Mid button pressed");  
        		HBmotor.setSpeedPWM(motor_mid_speed);
        		target_speed = motor_mid_speed;
				break;
        		
        		case high:
        		Serial.println("        High button pressed");
        		HBmotor.setSpeedPWM(motor_high_speed);
        		target_speed = motor_high_speed;
        		break;
        		
        		default:
          		break;
			}
			
		}
		
		// Get motor speed from Hall effect sensor (RPM)
		double read_motor_speed()
		{
			double RPM=rotate_count.getRPMandUpdate();
    		return RPM;
		}
		
		// Execute the system task
		void system_execute()
		{		
			bool success_command;
			double curr_speed;
<<<<<<< Updated upstream
			
<<<<<<< Updated upstream
=======
=======
			int pid_out;
>>>>>>> Stashed changes
			
			
			
>>>>>>> Stashed changes
			// Check buttons
			if (button_time_check.isMinChekTimeElapsedAndUpdate())
			{
				
				// Get push button command
				success_command = pushbuttons.check_n_get_command(command);
			
				if (success_command)
				{
					motor_speed_input(command);
				}
				
			}
				
				
			// PID controller to adjust speed to set point, use target speed check
			if(target_speed_check.isMinChekTimeElapsedAndUpdate())
			{
				curr_speed = read_motor_speed();
<<<<<<< Updated upstream
				if(HBmotor.isStarted())
				{
					pid_out = pid.ComputePID_output(target_speed, curr_speed);
			
			
					
					HBmotor.setSpeedPWM(pid_out);
				}
				else
					pid.reset_pidcontrol();
				
			
=======
				curr_speed = map(curr_speed, 0, 6000, 0, 255);
				pid_out = pid.ComputePID_output(target_speed, curr_speed);
				
>>>>>>> Stashed changes
			}
			
			// Plot in serial plotter
			if(plot_time_check.isMinChekTimeElapsedAndUpdate())
				{
					Serial.print(0);  // To freeze the lower limit
					Serial.print(" ");
					Serial.print(6000);  // To freeze the upper limit
					Serial.print(" ");
					Serial.print(target_speed);
					Serial.print(" ");
					Serial.println(curr_speed);      // X axis is number of samples, i.e, sampled every 0.2 ms, so 100 samples taken in 20 s
				}
		}		
};


#endif
