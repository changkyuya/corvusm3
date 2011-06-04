/*	This file is part of the Corvus M3 project

	http://corvusm3.blogspot.com/
	http://code.google.com/p/corvusm3/	

	2009, Thorsten Raab - thorsten.raab@gmx.at

    Corvus M3 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Corvus M3 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corvus M3.  If not, see <http://www.gnu.org/licenses/>.
*/

// voltage (only if you have it _wired_ up!)
#define LOW_VOLTAGE		10.0	// Pack voltage at which to trigger alarm
#define INPUT_VOLTAGE 	3.3		// (Volts) voltage your power regulator is feeding your ArduPilot to have an accurate pressure and battery 
// level readings. (you need a multimeter to measure and set this of course)
#define VOLT_DIV_RATIO 	5.55		//  10K +, 2K2 GND = V / 5,55

/*
= ============================================================================ =
 5. SETUP SAFETY FEATURES
 = ============================================================================ =
 
 This section helps you setup the safety features of your multicopter.
 Defaults are generally okay. We advise you to read through them, but not to
 change anything unless you are very sure about what you are doing.
 */
#define ARM_DELAY 50                      // How long you need to keep rudder to
// max right for arming motors
// (units*0.02, so 50units = 1 second)
#define DISARM_DELAY 25                   // How long you need to keep rudder to
// max left for disarming motors
#define SAFETY_DELAY 25                   // How long you need to keep throttle
// to min before safety activates and
// does not allow sudden throttle
// increases.
#define SAFETY_MAX_THROTTLE_INCREASE 100  // How much of jump in throttle
// (within a single cycle of 5ms) will
// cause motors to disarm.
#define SAFETY_HOVER_THROTTLE 1300        // When we reach Hover Throttle Safely
// we switch of Safety Feature

/*- ---------------------------------------------------------------------------- -
 Finetuning your transmitter (radio) settings
 - ---------------------------------------------------------------------------- -
 */
#define ROLL_MID 1520           // Radio Roll channel mid value
#define PITCH_MID 1500          // Radio Pitch channel mid value
#define YAW_MID 1522            // Radio Yaw channel mid value
#define THROTTLE_MID 1500       // Radio Throttle channel mid value
#define AUX_MID 1500

// from Attitude.pde
#define MAX_CONTROL_OUTPUT 250

//radio filter in functions
#define FILTERON

//radio stick factor
#define STICK_TO_ANGLE_FACTOR 5.0        // To convert stick position to absolute angles
#define YAW_STICK_TO_ANGLE_FACTOR 1150.0

