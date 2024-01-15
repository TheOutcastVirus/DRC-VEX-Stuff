/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\robol                                            */
/*    Created:      Sun Jan 07 2024                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    19, 15, 12, 13  
// FlyWheel             motor         1               
// FapJacks             digital_out   A               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
 

competition Competition;

void pre_auton () {

}

void autonomous () {
  intake.spinFor(2, timeUnits::sec);
  Drivetrain.driveFor(55, inches, 100, velocityUnits::pct);
  Drivetrain.turnFor(90, rotationUnits::deg, 75, velocityUnits::pct);
  Drivetrain.driveFor(10, inches, 100, velocityUnits::pct);
}


int main() {
  // Set up callbacks for autonomous and driver control periods. Vikram
  Competition.autonomous(autonomous);
  Competition.drivercontrol(vexcodeInit);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}