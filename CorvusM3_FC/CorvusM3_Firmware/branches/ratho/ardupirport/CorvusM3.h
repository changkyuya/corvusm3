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



#include "WProgram.h"


/*For debugging purposes DCM*/
#define OUTPUTMODE 1  //If value = 1 will print the corrected data, 0 will print uncorrected data of the gyros (with drift), 2 Accel only data

// Radio definition for REDFOX
#define CH_ROLL 5
#define CH_PITCH 1
#define CH_THROTTLE 2
#define CH_RUDDER 0
#define CH_1 0
#define CH_2 1
#define CH_3 2
#define CH_4 3
#define CH_5 3    //AUX
#define CH_6 4    //AUX2
#define CH_7 6
#define CH_8 7
#define CH_9 8    // PL3
#define CH_10 9   // PB5
#define CH_11 10  // PE3

byte errorRadio = 1;

//Axis
#define ROLL 0
#define PITCH 1
#define YAW 2
#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

#define GYROZ 2
#define GYROX 0
#define GYROY 1
#define ACCELX 3
#define ACCELY 4
#define ACCELZ 5
#define LASTSENSOR 6


/* ************************************************************** */
/* CM3 Hardware definitions */

#define LED_Red 21     // C_LED
#define C_LED_PIN   LED_Red
#define SPEKTRUM_BIND_PIN   39
#define DEBUG_PIN  31
#define BEEPER_PIN 3


/* ************************************************** */
/* Serial port definitions */
#define SERIAL1_BAUD 115200

//#define SerBau  115200      // Baud setting moved close next to port selection
#define SerPri  Serial1.print
#define SerPrln Serial1.println
#define SerPriln Serial1.println
#define SerRea  Serial1.read
#define SerAva  Serial1.available
#define SerRea  Serial1.read
#define SerFlu  Serial1.flush
#define SerBeg  Serial1.begin
#define SerP    Serial1.printf_P
#define SerPor  "Telemetry"


/* *********************************************** */
// IMU definitions
// Sensor: GYROX, GYROY, GYROZ, ACCELX, ACCELY, ACCELZ, BAT
//uint8_t 
byte sensors[7] = {0, 1, 2, 3, 4, 5, 6};  // CM3 Layout

// Sensor: GYROX, GYROY, GYROZ,   ACCELX, ACCELY, ACCELZ,     MAGX, MAGY, MAGZ,     BAT
int SENSOR_SIGN[]={
  1, 1, -1,    1, 1, 1,     -1, -1, -1,    1}; 
 //{-1,1,-1,1,-1,1,-1,-1,-1};
 
/* CM3 Hardware definitions, END */

/* *********************************************** */
/* General definitions */
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0

#define LED_ON 2000000000
#define LED_OFF 0
#define LED_BLINK 1200

// ADC : Voltage reference 3.3v / 12bits(4096 steps) => 0.8mV/ADC step
// ADXL335 Sensitivity(from datasheet) => 330mV/g, 0.8mV/ADC step => 330/0.8 = 412
// Tested value : 408
#define GRAVITY 408 //this equivalent to 1G in the raw data coming from the accelerometer 
#define Accel_Scale(x) x*(GRAVITY/9.81)//Scaling the raw data of the accel to actual acceleration in meters for seconds square

#define ROLL_DEF  0      // Level values for roll, used to calculate roll_acc_offset
#define PITCH_DEF 0      // Level values for pitch, used to calculate pitch_acc_offset
#define Z_DEF  GRAVITY   // Stable level value for Z, used to calculate z_acc_offset, same as GRAVITY


// IDG500 Sensitivity (from datasheet) => 2.0mV/º/s, 0.8mV/ADC step => 0.8/3.33 = 0.4
// Tested values : 
#define Gyro_Gain_X 0.9 // i don't know why ... 0.4  //X axis Gyro gain
#define Gyro_Gain_Y 0.9 //Y axis Gyro gain
#define Gyro_Gain_Z 0.9 //Z axis Gyro gain
#define Gyro_Scaled_X(x) x*ToRad(Gyro_Gain_X) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) x*ToRad(Gyro_Gain_Y) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) x*ToRad(Gyro_Gain_Z) //Return the scaled ADC raw data of the gyro in radians for second




int AN[7]; //array that store the 7 ADC channels
int AN_OFFSET[6]; //Array that store the Offset of the gyros and accelerometers

float G_Dt=0.002;                  // Integration time for the gyros (DCM algorithm)
float Accel_Vector[3]= {0, 0, 0}; // Store the acceleration in a vector
float Accel_Vector_unfiltered[3]= {0, 0, 0}; // Store the acceleration in a vector
float Gyro_Vector[3]= {0, 0, 0};  // Store the gyros rutn rate in a vector
float Omega_Vector[3]= {0, 0, 0}; // Corrected Gyro_Vector data
float Omega_P[3]= {0, 0, 0};      // Omega Proportional correction
float Omega_I[3]= {0, 0, 0};      // Omega Integrator
float Omega[3]= {0, 0, 0};
//float Accel_magnitude;
//float Accel_weight;

float errorRollPitch[3] = {0, 0, 0};
float errorYaw[3] = {0, 0, 0};
float errorCourse = 0;

float roll = 0;
float pitch = 0;
float yaw = 0;

unsigned int counter = 0;

float DCM_Matrix[3][3]= {
  { 1,0,0 },
  { 0,1,0 },
  { 0,0,1 }}; 
  
float Update_Matrix[3][3]={
  { 0,1,2 },
  { 3,4,5 },
  { 6,7,8 }}; //Gyros here

float Temporary_Matrix[3][3]={
  { 0,0,0 },
  { 0,0,0 },
  { 0,0,0 }};


// Main timers
long timer=0; 
long timer_old;


// Attitude control variables
float command_rx_roll=0;        // User commands
float command_rx_roll_old;
float command_rx_roll_diff;
float command_rx_pitch=0;
float command_rx_pitch_old;
float command_rx_pitch_diff;
float command_rx_yaw=0;
float command_rx_yaw_diff;
int control_roll;           // PID control results
int control_pitch;
int control_yaw;

// Wind Compensation variables
int control_wind_roll;
int control_wind_pitch;
int max_wind_angle;
int Gyro_drift_error_sum;

// Attitude PID controls
float roll_I=0;
float roll_D;
float err_roll;
float pitch_I=0;
float pitch_D;
float err_pitch;
float yaw_I=0;
float yaw_D;
float err_yaw;

#define BATTERY_VOLTAGE(x) (x*(INPUT_VOLTAGE/4096.0)) * VOLT_DIV_RATIO
float 	battery_voltage 	= LOW_VOLTAGE * 1.05;		// Battery Voltage, initialized above threshold for filter
byte battwarning = 0;

// Mode LED timers and variables, used to blink LED_Green
byte gled_status = HIGH;
long gled_timer;
long gled_speed;


// Radio definitions
int   roll_mid;
int   pitch_mid;
int   yaw_mid;

int   Neutro_yaw;
int   ch_roll;
int   ch_pitch;
int   ch_throttle;
int   ch_yaw;
int   ch_aux;
int   ch_aux2;
int   ch_flightmode;

// Quad motors
int frontMotor;
int backMotor;
int leftMotor;
int rightMotor;


byte  motorArmed = 0;                              // 0 = motors disarmed, 1 = motors armed
byte  motorSafety = 1;                             // 0 = safety off, 1 = on.  When On, sudden increases in throttle not allowed
int   minThrottle = 0;
byte  safetyOff = 0;                              // During normal Flight, the motor Safety feature is switched off.

// Serial communication
char   queryType;
long   tlmTimer = 0;

// Arming/Disarming
//uint8_t 
byte Arming_counter=0;
byte Disarming_counter=0;
byte Safety_counter=0;
byte neutralYAWarm = 0;


// System Timers
// --------------
unsigned long fast_loopTimer		        = 0;		// Time in miliseconds of main control loop
unsigned long medium_loopTimer		        = 0;		// Time in miliseconds of navigation control loop
byte   medium_loopCounter			= 0;		// Counters for branching from main control loop to slower loops
byte   slow_loopCounter				= 0;		// 
unsigned long deltaMiliSeconds 		        = 0;		// Delta Time in miliseconds
unsigned long dTnav				= 0;		// Delta Time in milliseconds for navigation computations
int   mainLoop_count 				= 0;
unsigned long elapsedTime			= 0;		// for doing custom events



// Debug options - set only one of these options to 1 at a time, set the others to 0
#define DEBUG_SUBSYSTEM 0 		// 0 = no debug
					// 1 = Debug the Radio input
					// 2 = Debug the Servo output
					// 3 = Debug the Sensor input
					// 4 = Debug the GPS input
					// 5 = Debug the GPS input - RAW HEX OUTPUT
					// 6 = Debug the IMU
					// 7 = Debug the Control Switch
					// 8 = Debug the Servo DIP switches
					// 9 = Debug the Relay out
					// 10 = Debug the Magnetometer
					// 11 = Debug the ABS pressure sensor
					// 12 = Debug the stored waypoints
					// 13 = Debug the Throttle
					// 14 = Debug the Radio Min Max
					// 15 = Debug the EEPROM - Hex Dump


// Following variables stored in EEPROM
float KP_QUAD_ROLL;
float KI_QUAD_ROLL;
float STABLE_MODE_KP_RATE_ROLL;
float KP_QUAD_PITCH;
float KI_QUAD_PITCH;
float STABLE_MODE_KP_RATE_PITCH;
float KP_QUAD_YAW;
float KI_QUAD_YAW;
float STABLE_MODE_KP_RATE_YAW;
float STABLE_MODE_KP_RATE;       //NOT USED NOW
int acc_offset_x;
int acc_offset_y;
int acc_offset_z;
int gyro_offset_roll;
int gyro_offset_pitch;
int gyro_offset_yaw;
float Kp_ROLLPITCH;
float Ki_ROLLPITCH;
float Kp_YAW;
float Ki_YAW;
float Kp_RateRoll;
float Ki_RateRoll;
float Kd_RateRoll;
float Kp_RatePitch;
float Ki_RatePitch;
float Kd_RatePitch;
float Kp_RateYaw;
float Ki_RateYaw;
float Kd_RateYaw;
float xmitFactor;
float ch_roll_slope          = 1;
float ch_pitch_slope         = 1;
float ch_throttle_slope      = 1;
float ch_yaw_slope           = 1;
float ch_aux_slope           = 1;
float ch_aux2_slope          = 1;
float ch_roll_offset         = 0;
float ch_pitch_offset        = 0;
float ch_throttle_offset     = 0;
float ch_yaw_offset          = 0;
float ch_aux_offset          = 0;
float ch_aux2_offset         = 0;
int MIN_THROTTLE;
//float eeprom_counter;                // reserved for eeprom write counter, 31-10-10, jp
//float eeprom_checker;                // reserved for eeprom checksums, 01-11-10, jp



// This function call contains the default values that are set to the ArduCopter
// when a "Default EEPROM Value" command is sent through serial interface
void defaultUserConfig() {
  KP_QUAD_ROLL               = 4.0;
  KI_QUAD_ROLL               = 0.15;
  STABLE_MODE_KP_RATE_ROLL   = 1.2;
  KP_QUAD_PITCH              = 4.0;
  KI_QUAD_PITCH              = 0.15;
  STABLE_MODE_KP_RATE_PITCH  = 1.2;
  KP_QUAD_YAW                = 3.0;
  KI_QUAD_YAW                = 0.15;
  STABLE_MODE_KP_RATE_YAW    = 2.4;
  STABLE_MODE_KP_RATE        = 0.2;     // NOT USED NOW
  acc_offset_x               = 2057;
  acc_offset_y               = 2044;
  acc_offset_z               = 2052;
  gyro_offset_roll           = 1659;
  gyro_offset_pitch          = 1650;
  gyro_offset_yaw            = 1650;
  Kp_ROLLPITCH               = 0.0014;
  Ki_ROLLPITCH               = 0.00000015;
  Kp_YAW                     = 1.0;
  Ki_YAW                     = 0.00002;
  Kp_RateRoll                = 1.95;
  Ki_RateRoll                = 0.0;
  Kd_RateRoll                = 0.0;
  Kp_RatePitch               = 1.95;
  Ki_RatePitch               = 0.0;
  Kd_RatePitch               = 0.0;  
  Kp_RateYaw                 = 3.2;
  Ki_RateYaw                 = 0.0;
  Kd_RateYaw                 = 0.0;
  xmitFactor                 = 0.32;
  roll_mid                   = 1520;
  pitch_mid                  = 1500;
  yaw_mid                    = 1522;
  ch_roll_slope              = 1;
  ch_pitch_slope             = 1;
  ch_throttle_slope          = 1;
  ch_yaw_slope               = 1;
  ch_aux_slope               = 1;
  ch_aux2_slope              = 1;
  ch_roll_offset             = 0;
  ch_pitch_offset            = 0;
  ch_throttle_offset         = 0;
  ch_yaw_offset              = 0;
  ch_aux_offset              = 0;
  ch_aux2_offset             = 0;
  MIN_THROTTLE               = 1178; // used to be #define but now in EEPROM
}

// EEPROM storage addresses
#define KP_QUAD_ROLL_ADR       0
#define KI_QUAD_ROLL_ADR       8
#define STABLE_MODE_KP_RATE_ROLL_ADR 4
#define KP_QUAD_PITCH_ADR      12
#define KI_QUAD_PITCH_ADR      20
#define STABLE_MODE_KP_RATE_PITCH_ADR 16
#define KP_QUAD_YAW_ADR        24
#define KI_QUAD_YAW_ADR        32
#define STABLE_MODE_KP_RATE_YAW_ADR 28
#define STABLE_MODE_KP_RATE_ADR 36      // NOT USED NOW
#define acc_offset_x_ADR       80
#define acc_offset_y_ADR       84
#define acc_offset_z_ADR       88
#define gyro_offset_roll_ADR   92
#define gyro_offset_pitch_ADR  96
#define gyro_offset_yaw_ADR    100
#define Kp_ROLLPITCH_ADR       104
#define Ki_ROLLPITCH_ADR       108
#define Kp_YAW_ADR             112
#define Ki_YAW_ADR             116
#define XMITFACTOR_ADR         128
#define KP_RATEROLL_ADR        132
#define KI_RATEROLL_ADR        136
#define KD_RATEROLL_ADR        140
#define KP_RATEPITCH_ADR       144
#define KI_RATEPITCH_ADR       148
#define KD_RATEPITCH_ADR       152
#define KP_RATEYAW_ADR         156
#define KI_RATEYAW_ADR         160
#define KD_RATEYAW_ADR         164
#define CHROLL_MID             168
#define CHPITCH_MID            172
#define CHYAW_MID              176
#define ch_roll_slope_ADR      180
#define ch_pitch_slope_ADR     184
#define ch_throttle_slope_ADR  188
#define ch_yaw_slope_ADR       192
#define ch_aux_slope_ADR       196
#define ch_aux2_slope_ADR      200
#define ch_roll_offset_ADR     204
#define ch_pitch_offset_ADR    208
#define ch_throttle_offset_ADR 212
#define ch_yaw_offset_ADR      216
#define ch_aux_offset_ADR      220
#define ch_aux2_offset_ADR     224
#define MIN_THROTTLE_ADR       252

//#define eeprom_counter_ADR     238  // hmm should i move these?!? , 31-10-10, jp
//#define eeprom_checker_ADR     240  // this too... , 31-10-10, jp



// end of file



