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

// General Initialization for all APM electronics
void APM_Init() {
  
  disableDebugPorts();  
  
  Serial1.begin(SERIAL1_BAUD);
  Serial2.begin(115200); //spektrum
  Serial3.begin(115200); //ESC
  
  pinMode(LED_Red,OUTPUT);    // Red LED     (PC13) = 21
  
  pinMode(SPEKTRUM_BIND_PIN,OUTPUT);    // Spektrum Bind Pin
  digitalWrite(SPEKTRUM_BIND_PIN, HIGH);
  
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(BEEPER_PIN, OUTPUT);
  
  readUserConfig();          // Load user configurable items from EEPROM
  
  CM3_RC.Init();             // APM Radio initialization
  
//  CM3_RC.OutputCh(0,MIN_THROTTLE);  // Motors stoped
//  CM3_RC.OutputCh(1,MIN_THROTTLE);
//  CM3_RC.OutputCh(2,MIN_THROTTLE);
//  CM3_RC.OutputCh(3,MIN_THROTTLE);
  // we use serial
  char motorzero = 0;
  Serial3.write(0xF5);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  
  // Wiggle LEDs while ESCs are rebooting  
  FullBlink(50,20);
  
  adc.Init();            // APM ADC library initialization
  
  //DataFlash.Init();          // DataFlash log initialization
  
  //flightOrientation = 1;// X Mode // + Mode.
  //flightMode = 1; // Stable Mode.
  
     // Safety measure for Channel mids
  if(roll_mid < 1400 || roll_mid > 1600) roll_mid = 1500;
  if(pitch_mid < 1400 || pitch_mid > 1600) pitch_mid = 1500;
  if(yaw_mid < 1400 || yaw_mid > 1600) yaw_mid = 1500;
  
    // RC channels Initialization (Quad motors)  
//  CM3_RC.OutputCh(0,MIN_THROTTLE);  // Motors stoped
//  CM3_RC.OutputCh(1,MIN_THROTTLE);
//  CM3_RC.OutputCh(2,MIN_THROTTLE);
//  CM3_RC.OutputCh(3,MIN_THROTTLE);  
  // we use serial
  Serial3.write(0xF5);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  Serial3.write(motorzero);
  
  calibrateSensors();         // Calibrate neutral values of gyros  (in Sensors.pde)
  
    //  Neutro_yaw = APM_RC.InputCh(3); // Take yaw neutral radio value
  for(int i=0;i<6;i++)
  {
    SerPri("AN[]:");
    SerPrln(AN_OFFSET[i]);
  }
  SerPri("Yaw neutral value:");
  SerPri(yaw_mid);

  delay(1000);
}



