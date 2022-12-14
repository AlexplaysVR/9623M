/**
--------- Program Info ---------
Name: FlyWheel RPM Data Logger
Author: AlexplaysVR
Description: This program is used to log the RPM of a flywheel. This also allows for control over the Flywheels
Instructions: 
1. Change Motor Declarations to match your robot
2. Select DataMode or DriverMode (Shown Below)
3. Set Target Flywheel RPMs
--------------------------------
*/

// -------------------------------
//Modes
bool DataMode = true; //Disables Driver Control, Enables Data Collection, Increases Data Collection Speed and Accuracy
bool DriverMode = false; //Enables Driver Control, Enables Data Collection But, At a slower rate. Reducing Accuracy
//Flywheel Speed (RPM)
int targetRIGHTRPM = 2500; //Sets the "target" RPM of the Right Flywheel
int targetLEFTRPM = 2000; //Sets the "target" RPM of the Left Flywheel
int datacollectiondelay = 10; //Sets the delay between data collection (in milliseconds)
// -------------------------------

//includes
#include "main.h"
#include <cstdio>
#include <iostream>
#include "display/lvgl.h"
//namespaces
using namespace std;
using namespace pros;
//Motor Declarations
#define LEFT_FRONT_MOTOR 1
#define LEFT_BACK_MOTOR 2
#define RIGHT_FRONT_MOTOR 3
#define RIGHT_BACK_MOTOR 4
#define RIGHT_FLY_MOTOR 5
#define LEFT_FLY_MOTOR 6
//Device Declarations
#define LEFT_FLY_ENCODER_PORT 7
#define RIGHT_FLY_ENCODER_PORT 8
//3 Wire Device Declarations
#define LAUNCH_PISTON_PORT 'A'
//Controller Inputs
#define TOP_RIGHT_SHOLDER DIGITAL_R1
#define BOTTOM_RIGHT_SHOLDER DIGITAL_R2
#define TOP_LEFT_SHOLDER DIGITAL_L1
#define BOTTOM_LEFT_SHOLDER DIGITAL_L2
#define RIGHT_JOYSTICK_X ANALOG_RIGHT_X
#define RIGHT_JOYSTICK_Y ANALOG_RIGHT_Y
#define LEFT_JOYSTICK_X ANALOG_LEFT_X
#define LEFT_JOYSTICK_Y ANALOG_LEFT_Y
//Flywheel RPM Conversion
int FlyLeftMotorRPM = targetLEFTRPM / 6;
int FlyRightMotorRPM = targetRIGHTRPM / 6;

//LVGL Declarations
char buffer[200]; //max 200 characters
char buffer2[200]; //max 200 characters
static lv_style_t style = lv_style_pretty_color;
static lv_style_t stylepressed = lv_style_btn_pr;
static lv_style_t stylereleased = lv_style_btn_rel;
static lv_style_t styleswbg = lv_style_pretty;

//Other Declarations
bool running = true;

/**
 * Creates and Updates RPM Guage and Displays it on the V5 Screen
 *
 * USES LVGL, If ActivateGuage is set to false, the guage will be created and displayed, but will not be updated (Useful for debugging)
 * while If ActivateGuage is set to true, the guage will be created and displayed, and will be updated (Used For Data Collection)
 */
	//\Create Action for the increase RPM button
	static lv_res_t increase_rpm_left(lv_obj_t * button)
	{
		targetLEFTRPM = targetLEFTRPM + 100;
		return LV_RES_OK;
	}
	//\Create Action for the decrease RPM button
	static lv_res_t decrease_rpm_left(lv_obj_t * button)
	{
		targetLEFTRPM = targetLEFTRPM - 100;
		return LV_RES_OK;
	}
	//\Create Action for the increase RPM button
	static lv_res_t increase_rpm_right(lv_obj_t * button)
	{
		targetRIGHTRPM = targetRIGHTRPM + 100;
		return LV_RES_OK;
	}	
	//\Create Action for the decrease RPM button
	static lv_res_t decrease_rpm_right(lv_obj_t * button)
	{
		targetRIGHTRPM = targetRIGHTRPM - 100;
		return LV_RES_OK;
	}
	static lv_res_t increase_rpm_left_repeat(lv_obj_t * button)
	{
		targetLEFTRPM = targetLEFTRPM + 100;
		delay(10);
		return LV_RES_OK;
	}
	//\Create Action for the decrease RPM button
	static lv_res_t decrease_rpm_left_repeat(lv_obj_t * button)
	{
		targetLEFTRPM = targetLEFTRPM - 100;
		delay(10);
		return LV_RES_OK;
	}
	//\Create Action for the increase RPM button
	static lv_res_t increase_rpm_right_repeat(lv_obj_t * button)
	{
		targetRIGHTRPM = targetRIGHTRPM + 100;
		delay(10);
		return LV_RES_OK;
	}	
	//\Create Action for the decrease RPM button
	static lv_res_t decrease_rpm_right_repeat(lv_obj_t * button)
	{
		targetRIGHTRPM = targetRIGHTRPM - 100;
		delay(10);
		return LV_RES_OK;
	}
	static lv_res_t start(lv_obj_t * button)
	{
		running = true;
		return LV_RES_OK;
	}
	static lv_res_t stop(lv_obj_t * button)
	{
		running = false;	
		return LV_RES_OK;
	}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	//Nice Itialization Message
	cout<<"FlyWheel RPM Guage Program Created By AlexplaysVR"<<endl;
	cout<<"#############################################################"<<endl;
	cout<<"#                    _                                      #"<<endl;  
	cout<<"#                  -=\\`\\                                    #"<<endl;  
	cout<<"#              |\\ ____\\_\\__                                 #"<<endl;  
	cout<<"#            -=\\c`\"\"\"\"\"\"\" \"`)                               #"<<endl;  
	cout<<"#               `~~~~~/ /~~`                                #"<<endl;  
	cout<<"#                 -==/ /                                    #"<<endl;  
	cout<<"#                   '-'                                     #"<<endl;  
	cout<<"#                  _  _                                     #"<<endl;  
	cout<<"#                 ( `   )_                                  #"<<endl;  
	cout<<"#                (    )    `)                               #"<<endl;  
	cout<<"#              (_   (_ .  _) _)                             #"<<endl;  
	cout<<"#                                             _             #"<<endl;  
	cout<<"#                                            (  )           #"<<endl;  
	cout<<"#             _ .                         ( `  ) . )        #"<<endl;  
	cout<<"#           (  _ )_                      (_, _(  ,_)_)      #"<<endl;  
	cout<<"#         (_  _(_ ,)                                        #"<<endl;  
	cout<<"#############################################################"<<endl;
	//Initialize Motors
cout<<"[Robot Initialization] Robot Devices Initializing..."<<endl;
	Motor left_front_mtr(LEFT_FRONT_MOTOR);
	Motor left_back_mtr(LEFT_BACK_MOTOR, true);
	Motor right_front_mtr(RIGHT_FRONT_MOTOR, true);
	Motor right_back_mtr(RIGHT_BACK_MOTOR);
	Motor left_fly_mtr(LEFT_FLY_MOTOR);
	Motor right_fly_mtr(RIGHT_FLY_MOTOR);
//Initialize Devices
	Rotation left_fly_encoder(LEFT_FLY_ENCODER_PORT);
	Rotation right_fly_encoder(RIGHT_FLY_ENCODER_PORT);
	left_fly_encoder.reset(); //Reset Rotation Sensor
	right_fly_encoder.reset(); //Reset Rotation Sensor
//Initialize Controller
	Controller master(E_CONTROLLER_MASTER);
	delay(1000);
	cout<<"[Robot Initialization] Robot Devices Initialized!"<<endl;
}
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

//Initialize Motors
cout<<"[Driver Control] Robot Devices Initializing..."<<endl;
	Motor left_front_mtr(LEFT_FRONT_MOTOR);
	Motor left_back_mtr(LEFT_BACK_MOTOR, true);
	Motor right_front_mtr(RIGHT_FRONT_MOTOR, true);
	Motor right_back_mtr(RIGHT_BACK_MOTOR);
	Motor left_fly_mtr(LEFT_FLY_MOTOR, true);
	Motor right_fly_mtr(RIGHT_FLY_MOTOR);
	left_fly_mtr.set_brake_mode(E_MOTOR_BRAKE_COAST);
	right_fly_mtr.set_brake_mode(E_MOTOR_BRAKE_COAST);
//Initialize Devices
	Rotation left_fly_encoder(LEFT_FLY_ENCODER_PORT);
	Rotation right_fly_encoder(RIGHT_FLY_ENCODER_PORT);
	left_fly_encoder.reset(); //Reset Rotation Sensor
	right_fly_encoder.reset(); //Reset Rotation Sensor
//Initialize 3 Wire Devices
	ADIDigitalOut launch_piston(LAUNCH_PISTON_PORT);
//Initialize Controller
	Controller master(E_CONTROLLER_MASTER);
//Finish Initializing
	delay(100);
	cout<<"[Driver Control] Robot Devices Initialized!"<<endl;


	//Create a RPM gauge using LVGL
	lv_obj_t * rpm_gaugeleft = lv_gauge_create(lv_scr_act(), NULL);
	lv_obj_set_size(rpm_gaugeleft, 200, 200);
	lv_obj_align(rpm_gaugeleft, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_gauge_set_range(rpm_gaugeleft, 0, 3000);
	lv_gauge_set_scale(rpm_gaugeleft, 220, 21, 5);
	lv_obj_set_pos(rpm_gaugeleft, 0,5);
	lv_gauge_set_style(rpm_gaugeleft, &style);

	lv_obj_t * rpm_gaugeright = lv_gauge_create(lv_scr_act(), NULL);
	lv_obj_set_size(rpm_gaugeright, 200, 200);
	lv_obj_align(rpm_gaugeright, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_gauge_set_range(rpm_gaugeright, 0, 3000);
	lv_gauge_set_scale(rpm_gaugeright, 220, 21, 5);
	lv_gauge_set_value(rpm_gaugeright, 0, 0);
	lv_obj_set_pos(rpm_gaugeright, 280,5);
	lv_gauge_set_style(rpm_gaugeright, &style);
	//create a button that increases the target RPM using LVGL
	lv_obj_t * increase_rpm_button_left = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(increase_rpm_button_left, 200, 25);
	lv_obj_align(increase_rpm_button_left, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(increase_rpm_button_left, 0, 150);
	lv_btn_set_action(increase_rpm_button_left, LV_BTN_ACTION_CLICK, increase_rpm_left);
	lv_btn_set_action(increase_rpm_button_left, LV_BTN_ACTION_LONG_PR_REPEAT, increase_rpm_left);
	lv_btn_set_style(increase_rpm_button_left, LV_BTN_STYLE_REL ,&stylereleased);
	lv_btn_set_style(increase_rpm_button_left, LV_BTN_STYLE_PR ,&stylepressed);
	lv_btn_set_style(increase_rpm_button_left, LV_BTN_STYLE_TGL_REL ,&style);
	lv_btn_set_style(increase_rpm_button_left, LV_BTN_STYLE_TGL_PR ,&style);
	lv_btn_set_style(increase_rpm_button_left, LV_BTN_STYLE_INA ,&style);
	//create a button that decreases the target RPM using LVGL
	lv_obj_t * decrease_rpm_button_left = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(decrease_rpm_button_left, 200, 25);
	lv_obj_align(decrease_rpm_button_left, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(decrease_rpm_button_left, 0, 200);
	lv_btn_set_action(decrease_rpm_button_left, LV_BTN_ACTION_CLICK, decrease_rpm_left);
	lv_btn_set_action(decrease_rpm_button_left, LV_BTN_ACTION_LONG_PR_REPEAT, decrease_rpm_left);
	lv_btn_set_style(decrease_rpm_button_left, LV_BTN_STYLE_REL ,&stylereleased);
	lv_btn_set_style(decrease_rpm_button_left, LV_BTN_STYLE_PR ,&stylepressed);
	lv_btn_set_style(decrease_rpm_button_left, LV_BTN_STYLE_TGL_REL ,&style);
	lv_btn_set_style(decrease_rpm_button_left, LV_BTN_STYLE_TGL_PR ,&style);
	lv_btn_set_style(decrease_rpm_button_left, LV_BTN_STYLE_INA ,&style);

	//create a button that increases the target RPM using LVGL
	lv_obj_t * increase_rpm_button_right = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(increase_rpm_button_right, 200, 25);
	lv_obj_align(increase_rpm_button_right, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(increase_rpm_button_right, 280, 150);
	lv_btn_set_action(increase_rpm_button_right, LV_BTN_ACTION_CLICK, increase_rpm_right);
	lv_btn_set_action(increase_rpm_button_right, LV_BTN_ACTION_LONG_PR_REPEAT, increase_rpm_right);
	lv_btn_set_style(increase_rpm_button_right, LV_BTN_STYLE_REL ,&stylereleased);
	lv_btn_set_style(increase_rpm_button_right, LV_BTN_STYLE_PR ,&stylepressed);
	lv_btn_set_style(increase_rpm_button_right, LV_BTN_STYLE_TGL_REL ,&style);
	lv_btn_set_style(increase_rpm_button_right, LV_BTN_STYLE_TGL_PR ,&style);
	lv_btn_set_style(increase_rpm_button_right, LV_BTN_STYLE_INA ,&style);

	//create a button that decreases the target RPM using LVGL
	lv_obj_t * decrease_rpm_button_right = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(decrease_rpm_button_right, 200, 25);
	lv_obj_align(decrease_rpm_button_right, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(decrease_rpm_button_right, 280, 200);
	lv_btn_set_action(decrease_rpm_button_right, LV_BTN_ACTION_CLICK, decrease_rpm_right);
	lv_btn_set_action(decrease_rpm_button_right, LV_BTN_ACTION_LONG_PR_REPEAT, decrease_rpm_right);
	lv_btn_set_style(decrease_rpm_button_right, LV_BTN_STYLE_REL ,&stylereleased);
	lv_btn_set_style(decrease_rpm_button_right, LV_BTN_STYLE_PR ,&stylepressed);
	lv_btn_set_style(decrease_rpm_button_right, LV_BTN_STYLE_TGL_REL ,&style);
	lv_btn_set_style(decrease_rpm_button_right, LV_BTN_STYLE_TGL_PR ,&style);
	lv_btn_set_style(decrease_rpm_button_right, LV_BTN_STYLE_INA ,&style);

	//Increase RPM - Label Left
	lv_obj_t * increase_rpm_label_left = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(increase_rpm_label_left, "Increase RPM");
	lv_obj_align(increase_rpm_label_left, increase_rpm_button_left, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_style(increase_rpm_label_left, &style);
	//Decrease RPM - Label Left
	lv_obj_t * decrease_rpm_label_left = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(decrease_rpm_label_left, "Decrease RPM");
	lv_obj_align(decrease_rpm_label_left, decrease_rpm_button_left, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_style(decrease_rpm_label_left, &style);
	//Increase RPM - Label Right
	lv_obj_t * increase_rpm_label_right = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(increase_rpm_label_right, "Increase RPM");
	lv_obj_align(increase_rpm_label_right, increase_rpm_button_right, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_style(increase_rpm_label_right, &style);
	//Decrease RPM - Label Right
	lv_obj_t * decrease_rpm_label_right = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(decrease_rpm_label_right, "Decrease RPM");
	lv_obj_align(decrease_rpm_label_right, decrease_rpm_button_right, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_style(decrease_rpm_label_right, &style);

	//create a label using LVGL
	lv_obj_t * target_rpm_labelleft = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(target_rpm_labelleft, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(target_rpm_labelleft, 10, 175);

	//create a label using LVGL
	lv_obj_t * target_rpm_labelright = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(target_rpm_labelright, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(target_rpm_labelright, 290, 175);
	sprintf(buffer, "RightTargetRPM: %i", targetRIGHTRPM);
	lv_label_set_text(target_rpm_labelright, buffer);

	//create one start and stop toggle button that controls both flywheels using LVGL
	lv_obj_t * start_stop_switch = lv_sw_create(lv_scr_act(), NULL);
	lv_obj_set_size(start_stop_switch, 50, 60);
	lv_obj_align(start_stop_switch, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(start_stop_switch, 215, 150);


	while (true) {
				//Update RPM gauge
		int CPSLEFT = -(left_fly_encoder.get_velocity());
		int CPSRIGHT = right_fly_encoder.get_velocity();
		int RPMLEFT = CPSLEFT / 6;
		int RPMRIGHT = CPSRIGHT / 6;
		sprintf(buffer2, "LeftTargetRPM: %i", targetLEFTRPM);
		lv_label_set_text(target_rpm_labelleft, buffer2);
		sprintf(buffer, "RightTargetRPM: %i", targetRIGHTRPM);
		lv_label_set_text(target_rpm_labelright, buffer);
		int FlyLeftMotorRPM = targetLEFTRPM / 25;
		int FlyRightMotorRPM = targetRIGHTRPM / 25;
		if(targetLEFTRPM > 3000){
			targetLEFTRPM = 3000;
		}
		if(targetRIGHTRPM > 3000){
			targetRIGHTRPM = 3000;
		}
		if (RPMLEFT < 1 and RPMRIGHT < 1){
			lv_gauge_set_value(rpm_gaugeleft, 0, RPMLEFT);
			lv_gauge_set_value(rpm_gaugeright, 0, RPMRIGHT);
		}

		else{
			//Updates RPM Gauge and Outputs RPM to Console for Data Collection
			lv_gauge_set_value(rpm_gaugeleft, 0, RPMLEFT);
			lv_gauge_set_value(rpm_gaugeright, 0, RPMRIGHT);
			cout<<"L: "<<RPMLEFT<<" "<<"R: "<<RPMRIGHT<<endl;
			delay(20);
		}
		if(lv_sw_get_state(start_stop_switch) == 1){
			//Sets Flywheel Motors to Target RPM & Starts Flywheel Motors
			left_fly_mtr.move_velocity(FlyLeftMotorRPM);	
			right_fly_mtr.move_velocity(FlyRightMotorRPM);
		}
		if(lv_sw_get_state(start_stop_switch) == 0){
			//Stops Flywheel Motors
			left_fly_mtr.move_velocity(0);
			right_fly_mtr.move_velocity(0);
		}

		//Controller Inputs
		if(master.get_digital(DIGITAL_L1)){
			//Increases Target RPM by 25
			targetLEFTRPM += 10;
		}
		if(master.get_digital(DIGITAL_L2)){
			//Decreases Target RPM by 25
			targetLEFTRPM -= 10;
		}
		if(master.get_digital(DIGITAL_R1)){
			//Increases Target RPM by 25
			targetRIGHTRPM += 10;
		}
		if(master.get_digital(DIGITAL_R2)){
			//Decreases Target RPM by 25
			targetRIGHTRPM -= 10;
		}
		if(master.get_digital(DIGITAL_B)){
			lv_sw_toggle(start_stop_switch);
			delay(100);
		}
		if(master.get_digital(DIGITAL_A)){
			launch_piston.set_value(true);
			delay(100);
			launch_piston.set_value(false);
		}
		
		//Strafe Control (Converts Joystick Input to Integer Values)
		int turn = master.get_analog(RIGHT_JOYSTICK_X);
		int power = master.get_analog(LEFT_JOYSTICK_Y);
		int strafe = master.get_analog(LEFT_JOYSTICK_X);
		//Math for Strafe Control (Complicated Math, Don't Touch :D)
		int turnreversed = turn;
		int straferevered = strafe;
		int fl = power + turnreversed + straferevered;
		int rl = power + turnreversed - straferevered;
		int fr = power - turnreversed - straferevered;
		int rr = power - turnreversed + straferevered;
		//Set Motor Speeds (relative to controller input)
		left_front_mtr.move(fl);
		left_back_mtr.move(rl);
		right_front_mtr.move(fr);
		right_back_mtr.move(rr);

		delay(10);
	}
}
