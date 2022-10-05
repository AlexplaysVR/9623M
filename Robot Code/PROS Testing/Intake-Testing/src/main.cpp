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
#define LEFT_FLY_MOTOR 5
#define RIGHT_FLY_MOTOR 6
//Device Declarations
#define LEFT_FLY_ENCODER_PORT 7
#define RIGHT_FLY_ENCODER_PORT 8
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
int FlyLeftMotorRPM = targetLEFTRPM * 0.25;
int FlyRightMotorRPM = targetRIGHTRPM * 0.25;
/**
 * Creates and Updates RPM Guage and Displays it on the V5 Screen
 *
 * USES LVGL, If ActivateGuage is set to false, the guage will be created and displayed, but will not be updated (Useful for debugging)
 * while If ActivateGuage is set to true, the guage will be created and displayed, and will be updated (Used For Data Collection)
 */
	//\Create Action for the increase RPM button
	static lv_res_t increase_rpm_left(lv_obj_t * button)
	{
		targetRIGHTRPM = targetRIGHTRPM + 100;
		targetLEFTRPM = targetLEFTRPM + 100;
		return LV_RES_OK;
	}
void flywheelrpmgauge(bool ActivateGauge){
//Initialize Motors
cout<<"[RPM Guage] Robot Devices Initializing..."<<endl;
	Motor left_front_mtr(LEFT_FRONT_MOTOR);
	Motor left_back_mtr(LEFT_BACK_MOTOR, true);
	Motor right_front_mtr(RIGHT_FRONT_MOTOR, true);
	Motor right_back_mtr(RIGHT_BACK_MOTOR);
	Motor left_fly_mtr(LEFT_FLY_MOTOR);
	Motor right_fly_mtr(RIGHT_FLY_MOTOR, true);
//Initialize Devices
	Rotation left_fly_encoder(LEFT_FLY_ENCODER_PORT);
	Rotation right_fly_encoder(RIGHT_FLY_ENCODER_PORT);
//Initialize Controller
	Controller master(E_CONTROLLER_MASTER);
//Finish Initializing
	delay(100);
	cout<<"[RPM Guage] Robot Devices Initialized!"<<endl;


	//Create a RPM gauge using LVGL
	lv_obj_t * rpm_gaugeleft = lv_gauge_create(lv_scr_act(), NULL);
	lv_obj_set_size(rpm_gaugeleft, 200, 200);
	lv_obj_align(rpm_gaugeleft, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_gauge_set_range(rpm_gaugeleft, 0, 2500);
	lv_gauge_set_value(rpm_gaugeleft, 0, 0);
	lv_obj_set_pos(rpm_gaugeleft, 0,0);

	lv_obj_t * rpm_gaugeright = lv_gauge_create(lv_scr_act(), NULL);
	lv_obj_set_size(rpm_gaugeright, 200, 200);
	lv_obj_align(rpm_gaugeright, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_gauge_set_range(rpm_gaugeright, 0, 2500);
	lv_gauge_set_value(rpm_gaugeright, 0, 0);
	lv_obj_set_pos(rpm_gaugeright, 275,0);

	//create a button that increases the target RPM using LVGL
	lv_obj_t * increase_rpm_button_left = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(increase_rpm_button_left, 200, 25);
	lv_obj_align(increase_rpm_button_left, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(increase_rpm_button_left, 0, 150);
	lv_btn_set_action(increase_rpm_button_left, LV_BTN_ACTION_CLICK, increase_rpm_left);
	//create a button that decreases the target RPM using LVGL
	lv_obj_t * decrease_rpm_button_left = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(decrease_rpm_button_left, 200, 25);
	lv_obj_align(decrease_rpm_button_left, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(decrease_rpm_button_left, 0, 200);
	lv_btn_set_action(decrease_rpm_button_left, LV_BTN_ACTION_CLICK, increase_rpm_left);

	//create a label using LVGL
	lv_obj_t * increase_rpm_label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(increase_rpm_label, "Increase RPM");
	lv_obj_align(increase_rpm_label, increase_rpm_button_left, LV_ALIGN_CENTER, 0, 0);
	//create a label using LVGL
	lv_obj_t * decrease_rpm_label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(decrease_rpm_label, "Decrease RPM");
	lv_obj_align(decrease_rpm_label, decrease_rpm_button_left, LV_ALIGN_CENTER, 0, 0);

	//create a label using LVGL
	lv_obj_t * rpm_label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(rpm_label, "RPM:");
	lv_obj_align(rpm_label, rpm_gaugeleft, LV_ALIGN_CENTER, 0, 0);

	//create a label using LVGL
	lv_obj_t * target_rpm_labelleft = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(target_rpm_labelleft, &"Target RPM:" [ targetLEFTRPM]);
	lv_obj_align(target_rpm_labelleft, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(target_rpm_labelleft, 0, 175);

	//create a label using LVGL
	lv_obj_t * rpm_labelright = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(rpm_labelright, "RPM:");
	lv_obj_align(rpm_labelright, rpm_gaugeright, LV_ALIGN_CENTER, 0, 0);

	//create a label using LVGL
	lv_obj_t * target_rpm_labelright = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(target_rpm_labelright, "Target RPM:");
	lv_obj_align(target_rpm_labelright, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(target_rpm_labelright, 275, 175);

	//create a label using LVGL
	lv_obj_t * rpm_labelleft = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(rpm_labelleft, "RPM:");
	lv_obj_align(rpm_labelleft, rpm_gaugeleft, LV_ALIGN_CENTER, 0, 0);

	int CPSLEFT = 0;
	int CPSRIGHT = 0;
	if(ActivateGauge == false){
		cout<<"[RPM Guage] RPM Guage Created But Will Not Be Updated! (In Debugging Mode"<<endl;
	}
	if(ActivateGauge == true){
		cout<<"[RPM Guage] RPM Guage Created and Activated!(Output will start when Flywheel RPM is higher that 1rpm)"<<endl;
	}
	while (ActivateGauge) {
		//Update RPM gauge
		int CPSLEFT = -(left_fly_encoder.get_velocity());
		int CPSRIGHT = right_fly_encoder.get_velocity();
		int RPMLEFT = CPSLEFT / 6;
		int RPMRIGHT = CPSRIGHT / 6;
		if (RPMLEFT < 1 and RPMRIGHT < 1){
			//Disables Output if Motors are not moving, Reduces Excessive Console Spam
		}
		else{
			//Updates RPM Gauge and Outputs RPM to Console for Data Collection
			lv_gauge_set_value(rpm_gaugeleft, 0, RPMLEFT);
			lv_gauge_set_value(rpm_gaugeright, 0, RPMRIGHT);
			cout<<"L: "<<RPMLEFT<<" "<<"R: "<<RPMRIGHT<<endl;
			delay(20);
		}
		if(master.get_digital(DIGITAL_B)){
			//Stops Flywheels
			left_fly_mtr.move_velocity(0);
			right_fly_mtr.move_velocity(0);
		}
		if(master.get_digital(DIGITAL_A)){
			//Starts Flywheels
			left_fly_mtr.move_velocity(FlyLeftMotorRPM);
			right_fly_mtr.move_velocity(FlyRightMotorRPM);
		}
		delay(datacollectiondelay);
	}
	
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
	Motor right_fly_mtr(RIGHT_FLY_MOTOR, true);
//Initialize Controller
	Controller master(E_CONTROLLER_MASTER);
	delay(1000);
	cout<<"[Robot Initialization] Robot Devices Initialized!"<<endl;

//Toggle Flywheel RPM Guage
	if (DataMode == true){
		cout<<"[Robot Initialization] Data Mode Selected!"<<endl;
		flywheelrpmgauge(false); // Read line 24-29 for usage information
		delay(500);
	}
	else if (DriverMode == true){
		cout<<"[Robot Initialization] Driver Mode Selected!"<<endl;
	}

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
	Motor left_fly_mtr(LEFT_FLY_MOTOR);
	Motor right_fly_mtr(RIGHT_FLY_MOTOR, true);
//Initialize Controller
	Controller master(E_CONTROLLER_MASTER);
	delay(100);
	cout<<"[Driver Control] Robot Devices Initialized!"<<endl;
	delay(100);
	cout<<"[Driver Control] Driver Control Enabled!"<<endl;
	while (true) {
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
	}
}
