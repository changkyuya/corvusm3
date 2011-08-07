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

//******************************************************************************
// Radio definition
//******************************************************************************
#define RC_ROLL 0
#define RC_NICK 1
#define RC_YAW 2
#define RC_PITCH 3
#define RC_AUX1 4
#define RC_AUX2 5
#define RC_AUX3 6
#define RC_AUX4 7
int remote_assignment[]={0, 1, 2, 3};
  
//#define CH_ROLL 3
//#define CH_NICK 1
//#define CH_PITCH 2
//#define CH_YAW 0
//#define CH_AUX1 4
//#define CH_AUX2 5
//#define CH_AUX3 6
//#define CH_AUX4 7

byte errorRadio = 1;

//******************************************************************************
// Axis
//******************************************************************************
#define ROLL 0 
#define NICK 1 
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


//******************************************************************************
// CM3 Hardware definitions 
//******************************************************************************
#define LED_Red 21     // C_LED
#define SPEKTRUM_BIND_PIN   39
#define DEBUG_PIN  31
#define BEEPER_PIN 3


//******************************************************************************
// Serial port definitions 
//******************************************************************************
#define SERIAL1_BAUD 115200
#define SerPri  Serial1.print
#define SerPrln Serial1.println
#define SerPriln Serial1.println
#define SerRea  Serial1.read
#define SerAva  Serial1.available
#define SerRea  Serial1.read
#define SerFlu  Serial1.flush
#define SerBeg  Serial1.begin



//******************************************************************************
// IMU definitions
// Sensor: GYROX, GYROY, GYROZ, ACCELX, ACCELY, ACCELZ, BAT
//******************************************************************************
byte sensors[7] = {0, 1, 2, 3, 4, 5, 6};  // CM3 Layout

// Sensor: GYROX, GYROY, GYROZ,   ACCELX, ACCELY, ACCELZ,     MAGX, MAGY, MAGZ,     BAT
int SENSOR_SIGN[]={
  1, 1, -1,    1, 1, 1,     -1, -1, -1,    1}; 
 //{-1,1,-1,1,-1,1,-1,-1,-1};

//******************************************************************************
// General definitions 
//******************************************************************************
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0

#define LED_ON 2000000000
#define LED_OFF 0
#define LED_BLINK 1200
#define LED_FLASH 120

#define Accel_Scale(x) x*(GRAVITY/9.81)//Scaling the raw data of the accel to actual acceleration in meters for seconds square

#define ROLL_DEF  0      // Level values for roll, used to calculate roll_acc_offset
#define NICK_DEF 0      // Level values for nick, used to calculate nick_acc_offset
#define Z_DEF  GRAVITY   // Stable level value for Z, used to calculate z_acc_offset, same as GRAVITY

#define ToRad(x) (x*0.01745329252)	// *pi/180
#define ToDeg(x) (x*57.2957795131)	// *180/pi

//#define Gyro_Scaled_X(x) x*ToRad(Gyro_Gain_X) //Return the scaled ADC raw data of the gyro in radians for second
//#define Gyro_Scaled_Y(x) x*ToRad(Gyro_Gain_Y) //Return the scaled ADC raw data of the gyro in radians for second
//#define Gyro_Scaled_Z(x) x*ToRad(Gyro_Gain_Z) //Return the scaled ADC raw data of the gyro in radians for second

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

float errorRollNick[3] = {0, 0, 0};
float errorYaw[3] = {0, 0, 0};
float errorCourse = 0;

float roll = 0;
float nick = 0;
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
float command_rx_nick=0;
float command_rx_nick_old;
float command_rx_nick_diff;
float command_rx_yaw=0;
float command_rx_yaw_diff;
int control_roll;           // PID control results
int control_nick;
int control_yaw;

// Attitude PID controls
float roll_I=0;
float roll_D;
float err_roll;
float nick_I=0;
float nick_D;
float err_nick;
float yaw_I=0;
float yaw_D;
float err_yaw;

#define BATTERY_VOLTAGE(x) (x*(INPUT_VOLTAGE/4096.0)) * VOLT_DIV_RATIO
float 	battery_voltage 	= 11;		// Battery Voltage, initialized above threshold for filter
byte battwarning = 0;

// Mode LED timers and variables, used to blink LED_Green
byte gled_status = 1;
long gled_timer;
long gled_speed;

int   Neutro_yaw;
int   ch_roll;
int   ch_nick;
int   ch_pitch;
int   ch_yaw;
int   ch_aux1;
int   ch_aux2;
int   ch_aux3;
int   ch_aux4;

// Quad motors
int frontMotor;
int backMotor;
int leftMotor;
int rightMotor;

byte  motorArmed = 0;                              // 0 = motors disarmed, 1 = motors armed
byte  motorSafety = 1;                             // 0 = safety off, 1 = on.  When On, sudden increases in throttle not allowed
int   minPitch = 0;
//byte  safetyOff = 0;                              // During normal Flight, the motor Safety feature is switched off.

// Serial communication
char   queryType;
long   tlmTimer = 0;

// Arming/Disarming
//uint8_t 
byte Arming_counter=0;
byte Disarming_counter=0;
byte Safety_counter=0;
byte neutralYAWarm = 0;

//******************************************************************************
// System Timers
//******************************************************************************
unsigned long fast_loopTimer		        = 0;		// Time in miliseconds of main control loop
unsigned long medium_loopTimer		        = 0;		// Time in miliseconds of navigation control loop
byte   medium_loopCounter			= 0;		// Counters for branching from main control loop to slower loops
byte   slow_loopCounter				= 0;		// 
unsigned long deltaMiliSeconds 		        = 0;		// Delta Time in miliseconds
unsigned long dTnav				= 0;		// Delta Time in milliseconds for navigation computations
int   mainLoop_count 				= 0;
unsigned long elapsedTime			= 0;		// for doing custom events


//******************************************************************************
// EEPROM storage addresses
//******************************************************************************
#define KP_QUAD_ROLL                    1
#define KI_QUAD_ROLL                    2
#define STABLE_MODE_KP_RATE_ROLL        3
#define KP_QUAD_NICK                    4
#define KI_QUAD_NICK                    5
#define STABLE_MODE_KP_RATE_NICK        6
#define KP_QUAD_YAW                     7
#define KI_QUAD_YAW                     8
#define STABLE_MODE_KP_RATE_YAW         9
#define STABLE_MODE_KP_RATE             10      // NOT USED NOW
#define acc_offset_x                    11
#define acc_offset_y                    12
#define acc_offset_z                    13
#define Kp_ROLLNICK                     14
#define Ki_ROLLNICK                     15
#define Kp_YAW                          16
#define Ki_YAW                          17
#define XMITFACTOR                      18
#define Kp_RateRoll                     19
#define Ki_RateRoll                     20
#define Kd_RateRoll                     21
#define Kp_RateNick                     22
#define Ki_RateNick                     23
#define Kd_RateNick                     24
#define Kp_RateYaw                      25
#define Ki_RateYaw                      26
#define Kd_RateYaw                      27
#define roll_mid                        28
#define nick_mid                        29
#define yaw_mid                         30
#define MIN_PITCH                       31
#define BAT_WARNING                     32
#define STICK_TO_ANGLE_FACTOR           33
#define YAW_STICK_TO_ANGLE_FACTOR       34
#define TUNING_PARA                     35
#define TUNING_MIN                      36
#define TUNING_MAX                      37
#define CHANNEL_ASSIGNMENT              38
#define MIN_GAS                         39
#define GYRO_GAIN_X                     40
#define GYRO_GAIN_Y                     41
#define GYRO_GAIN_Z                     42

#define LAST_PARAMETER                  42

float parameter[LAST_PARAMETER + 1];

//******************************************************************************
// default values 
//******************************************************************************
void defaultUserConfig() 
{
  parameter[KP_QUAD_ROLL]                 = 4.0;
  parameter[KI_QUAD_ROLL]                 = 0.15;
  parameter[STABLE_MODE_KP_RATE_ROLL]     = 1.2;
  parameter[KP_QUAD_NICK]                 = 4.0;
  parameter[KI_QUAD_NICK]                 = 0.15;
  parameter[STABLE_MODE_KP_RATE_NICK]     = 1.2;
  parameter[KP_QUAD_YAW]                  = 3.0;
  parameter[KI_QUAD_YAW]                  = 0.15;
  parameter[STABLE_MODE_KP_RATE_YAW]      = 2.4;
  parameter[STABLE_MODE_KP_RATE]          = 0.2;     // NOT USED NOW
  parameter[acc_offset_x]                 = 2057;
  parameter[acc_offset_y]                 = 2044;
  parameter[acc_offset_z]                 = 2052;
  parameter[Kp_ROLLNICK]                  = 0.14;
  parameter[Ki_ROLLNICK]                  = 0.15;
  parameter[Kp_YAW]                       = 1.0;
  parameter[Ki_YAW]                       = 0.2;
  parameter[XMITFACTOR]                   = 0.32;
  parameter[Kp_RateRoll]                  = 1.95;
  parameter[Ki_RateRoll]                  = 0.0;
  parameter[Kd_RateRoll]                  = 0.0;
  parameter[Kp_RateNick]                  = 1.95;
  parameter[Ki_RateNick]                  = 0.0;
  parameter[Kd_RateNick]                  = 0.0;  
  parameter[Kp_RateYaw]                   = 3.2;
  parameter[Ki_RateYaw]                   = 0.0;
  parameter[Kd_RateYaw]                   = 0.0;
  parameter[roll_mid]                     = 1520;
  parameter[nick_mid]                     = 1500;
  parameter[yaw_mid]                      = 1522;
  parameter[MIN_PITCH]                    = 1178; 
  parameter[BAT_WARNING]                  = 10.0;
  parameter[STICK_TO_ANGLE_FACTOR]        = 7.0;
  parameter[YAW_STICK_TO_ANGLE_FACTOR]    = 1150.0;
  parameter[TUNING_PARA]                  = 0;
  parameter[TUNING_MIN]                   = 0;
  parameter[TUNING_MAX]                   = 0;
  parameter[CHANNEL_ASSIGNMENT]           = 3102;
  parameter[MIN_GAS]                      = 60;
  parameter[GYRO_GAIN_X]                  = 0.9;
  parameter[GYRO_GAIN_Y]                  = 0.9;
  parameter[GYRO_GAIN_Z]                  = 0.9;
}





