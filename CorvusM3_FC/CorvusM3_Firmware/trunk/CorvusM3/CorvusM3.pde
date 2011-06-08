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
    
   All Code is based on:
   =====================
   www.ArduCopter.com - www.DIYDrones.com
   Copyright (c) 2010.  All rights reserved.
   An Open Source Arduino based multicopter.
 
   Version  : v1.0, 11 October 2010
   Author(s): ArduCopter Team
   Ted Carancho (AeroQuad), Jose Julio, Jordi Muñoz,
   Jani Hirvinen, Ken McEwans, Roberto Navoni,          
   Sandro Benigno, Chris Anderson
  
   Author(s) : ArduPirates deveopment team                                  
               Philipp Maloney, Norbert, Hein, Igor, Emile, Kidogo 
          
*/

/* ************************************************************ */
/* **************** MAIN PROGRAM - INCLUDES ******************* */
/* ************************************************************ */

#include <stdlib.h>
#include <math.h>
#include "Config.h"
#include "CorvusM3.h"
#include <CM3_ADC.h>
#include <CM3_RC.h>
#include <CM3_SpektrumRC.h>
#include <EEPROM.h>             // EEPROM 


/* Software version */
#define VER 0.01    // Current software version 


// Sensors - declare one global instance
CM3_ADC	adc;
CM3_SpektrumRC reciverSpek;
CM3_RC_Class reciverPPM;

/* ************************************************************ */
/* ************* MAIN PROGRAM - DECLARATIONS ****************** */
/* ************************************************************ */

//byte flightMode;

unsigned long currentTime;  // current time in milliseconds
unsigned long currentTimeMicros = 0, previousTimeMicros = 0;  // current and previous loop time in microseconds
unsigned long mainLoop = 0;
unsigned long mediumLoop = 0;
unsigned long slowLoop = 0;

/* ************************************************************ */
/* **************** MAIN PROGRAM - SETUP ********************** */
/* ************************************************************ */
void setup() {
  
  APM_Init();                // APM Hardware initialization (in System.pde)

  mainLoop = millis();       // Initialize timers
  mediumLoop = mainLoop;
  motorArmed = 0;
  
  Read_adc_raw();            // Initialize ADC readings...
  
  
  SerPri("Corvus M3 - v");
  SerPrln(VER);
  SerPrln("Type '?' for Help.");

  delay(10);
  digitalWrite(LED_Red,HIGH);     // Ready to go...  
  digitalWrite(BEEPER_PIN, LOW);
}

/* ************************************************************ */
/* ************** MAIN PROGRAM - MAIN LOOP ******************** */
/* ************************************************************ */

/* ***************************************************** */
// Main loop 
void loop()
{
  currentTimeMicros = micros();
  currentTime = currentTimeMicros / 1000;
  
  // we read all spektrum bytes
  while (Serial2.available()) {
    reciverSpek.regByte(Serial2.read());
  }

  // Main loop at 500Hz (IMU + control)
  if ((currentTime-mainLoop) > 2)    // about 500Hz (every 2ms)
  {
    G_Dt = (currentTimeMicros-previousTimeMicros) * 0.000001;   // Microseconds!!!
    mainLoop = currentTime;
    previousTimeMicros = currentTimeMicros;

    //test loop time
    digitalWrite(DEBUG_PIN, HIGH);
    
    //IMU DCM Algorithm
    Read_adc_raw();       // Read sensors raw data
    Matrix_update(); 
    Normalize();          
    Drift_correction();
    Euler_angles();
    
    //test loop time
    digitalWrite(DEBUG_PIN, LOW);

    // Read radio values (if new data is available)
    if (reciverPPM.GetState() > 0 || reciverSpek.GetState() > 0) {  // New radio frame?
      read_radio();
      errorRadio = FALSE;
    }
    else
    {
      errorRadio = TRUE;
    }

    // Attitude control switch AUX1
   if (ch_aux1 < 1500)
   {   
     // STABLE Mode
      Attitude_control_v3(command_rx_roll,command_rx_nick,command_rx_yaw);
   }
   else
   {
      // ACRO Mode
      Rate_control_v2();
      // Reset yaw, so if we change to stable mode we continue with the actual yaw direction
      command_rx_yaw = ToDeg(yaw);
   }

    // Send output commands to motor ESCs...
    motor_output();
  }


  // Medium loop (about 60Hz) 
  if ((currentTime-mediumLoop)>=17){
    mediumLoop = currentTime;

    

    // Each of the six cases executes at 10Hz
    switch (medium_loopCounter){
    case 0:   // Magnetometer reading (10Hz)
      medium_loopCounter++;
      slowLoop++;
      break;
    case 1:  // Debug
      medium_loopCounter++;
      break;
    case 2:  // Send serial telemetry (10Hz)
      medium_loopCounter++;
      sendSerialTelemetry();
      break;
    case 3:  // Read serial telemetry (10Hz)
      medium_loopCounter++;
      readSerialCommand();
      break;
    case 4:  // second Barometer + RangeFinder reading (2x10Hz = 20Hz)
      medium_loopCounter++;
      break;
    case 5:  //  Battery monitor (10Hz)
      medium_loopCounter=0;
      read_battery();         // Battery monitor
      break;	
    }
  }


    
  //search blink time
  if (battwarning == TRUE || errorRadio == TRUE)
  {
    gled_speed = LED_BLINK;
  }
  else
  {
    gled_speed = LED_ON;
    digitalWrite(LED_Red, HIGH);
    digitalWrite(BEEPER_PIN, LOW);
  }  
  
  // AM and Mode status LED lights
  if(millis() - gled_timer > gled_speed) {
    gled_timer = millis();
    if(gled_status == HIGH) { 
      digitalWrite(LED_Red, LOW);
      digitalWrite(BEEPER_PIN, HIGH);
      gled_status = LOW;
    } 
    else {
      digitalWrite(LED_Red, HIGH);
      digitalWrite(BEEPER_PIN, LOW);
      gled_status = HIGH;
    } 
  }
}
