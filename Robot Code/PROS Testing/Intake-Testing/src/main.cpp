#include "main.h"

//Motor Declarations
#define LEFT_FRONT_MOTOR 1
#define LEFT_BACK_MOTOR 2
#define RIGHT_FRONT_MOTOR 3
#define RIGHT_BACK_MOTOR 4
//Controller Inputs
#define TOP_RIGHT_SHOLDER DIGITAL_R1
#define BOTTOM_RIGHT_SHOLDER DIGITAL_R2
#define TOP_LEFT_SHOLDER DIGITAL_L1
#define BOTTOM_LEFT_SHOLDER DIGITAL_L2
#define RIGHT_JOYSTICK_X ANALOG_RIGHT_X
#define RIGHT_JOYSTICK_Y ANALOG_RIGHT_Y
#define LEFT_JOYSTICK_X ANALOG_LEFT_X
#define LEFT_JOYSTICK_Y ANALOG_LEFT_Y
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Initializing...");
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
	pros::Motor left_front_mtr(LEFT_FRONT_MOTOR);
	pros::Motor left_back_mtr(LEFT_BACK_MOTOR, true);
	pros::Motor right_front_mtr(RIGHT_FRONT_MOTOR, true);
	pros::Motor right_back_mtr(RIGHT_BACK_MOTOR);
//Initialize Controller
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {
		//Strafe Control
		int turn = master.get_analog(RIGHT_JOYSTICK_X);
		int power = master.get_analog(LEFT_JOYSTICK_Y);
		int strafe = master.get_analog(LEFT_JOYSTICK_X);
		//Math for Strafe Control
		int turnreversed = turn;
		int straferevered = strafe;
		int fl = power + turnreversed + straferevered;
		int rl = power + turnreversed - straferevered;
		int fr = power - turnreversed - straferevered;
		int rr = power - turnreversed + straferevered;

		left_front_mtr.move(fl);
		left_back_mtr.move(rl);
		right_front_mtr.move(fr);
		right_back_mtr.move(rr);
	}
}
