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
#define INPUT_VOLTAGE 	3.3		// (Volts) voltage your power regulator is feeding your ArduPilot to have an accurate pressure and battery 
// level readings. (you need a multimeter to measure and set this of course)
#define VOLT_DIV_RATIO 	5.55		//  10K +, 2K2 GND = V / 5,55

//******************************************************************************
// SETUP SAFETY FEATURES
//******************************************************************************
#define ARM_DELAY 50                      // How long you need to keep rudder to
// max right for arming motors
// (units*0.02, so 50units = 1 second)
#define DISARM_DELAY 25                   // How long you need to keep rudder to
// max left for disarming motors

// from Attitude.pde
#define MAX_CONTROL_OUTPUT 250

/*For debugging purposes DCM*/
#define OUTPUTMODE 1  //If value = 1 will print the corrected data, 0 will print uncorrected data of the gyros (with drift), 2 Accel only data

// ADC : Voltage reference 3.3v / 12bits(4096 steps) => 0.8mV/ADC step
// ADXL335 Sensitivity(from datasheet) => 330mV/g, 0.8mV/ADC step => 330/0.8 = 412
// Tested value : 408
#define GRAVITY 408 //this equivalent to 1G in the raw data coming from the accelerometer 

// IDG500 Sensitivity (from datasheet) => 2.0mV/º/s, 0.8mV/ADC step => 0.8/3.33 = 0.4
// Tested values : 
#define Gyro_Gain_X 0.9 // i don't know why ... 0.4  //X axis Gyro gain
#define Gyro_Gain_Y 0.9 //Y axis Gyro gain
#define Gyro_Gain_Z 0.9 //Z axis Gyro gain


