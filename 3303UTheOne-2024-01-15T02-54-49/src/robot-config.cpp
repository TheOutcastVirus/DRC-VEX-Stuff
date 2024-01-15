#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftMotorA = motor(PORT19, ratio18_1, false);
motor leftMotorB = motor(PORT15, ratio18_1, false);
motor leftMotorC = motor(PORT17, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT12, ratio18_1, true);
motor rightMotorB = motor(PORT13, ratio18_1, true);
motor rightMotorC = motor(PORT16, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
motor intake = motor(PORT14, false);
inertial Gyro = inertial(PORT11);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, Gyro, 319.19, 260, 270, mm, 1);
motor FlyWheel = motor(PORT1, ratio6_1, false);
digital_out FapJacks = digital_out(Brain.ThreeWirePort.A);
controller Controller1 = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values

  bool flapperActive = false;
  bool previousR1 = false;

  while(true) {

    if(RemoteControlCodeEnabled) {

      if (Controller1.ButtonX.pressing()) {
        FlyWheel.setVelocity(100, percent);
        FlyWheel.spin(reverse);
      } else if (Controller1.ButtonB.pressing()) {
        FlyWheel.setVelocity(0, percent);
        FlyWheel.spin(reverse);
      }

      if (previousR1 == false && Controller1.ButtonR1.pressing()) {
        flapperActive = !flapperActive;
        FapJacks.set(flapperActive);
      } 

      if (Controller1.ButtonA.pressing()) {
        FapJacks.set(true);
      } else if (Controller1.ButtonY.pressing()) {
        FapJacks.set(false);
      }

      if(Controller1.ButtonL1.pressing()) {
        intake.spin(forward, 85, percent);
      } else if (Controller1.ButtonL2.pressing()) {
        intake.spin(reverse, 85, percent);
      } else {
        intake.spin(forward, 0, percent);
      }


      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3
      // right = Axis2
      int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    previousR1 = Controller1.ButtonR1.pressing();
    wait(20, msec);
    
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}