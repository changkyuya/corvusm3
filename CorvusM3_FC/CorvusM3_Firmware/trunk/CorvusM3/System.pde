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

//******************************************************************************
// General Initialization for all APM electronics
//******************************************************************************
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
  
  reciverPPM.Init();             // APM Radio initialization
  // read also channel mapping new
  remoteChannelMapping();
  
  
  motor_stop();
  
  // Wiggle LEDs while ESCs are rebooting  
  FullBlink(50,20);
  
  adc.Init();            // APM ADC library initialization
  
  // Safety measure for Channel mids
  if(parameter[roll_mid] < 1400 || parameter[roll_mid] > 1600) parameter[roll_mid] = 1500;
  if(parameter[nick_mid] < 1400 || parameter[nick_mid] > 1600) parameter[nick_mid] = 1500;
  if(parameter[yaw_mid] < 1400 || parameter[yaw_mid] > 1600) parameter[yaw_mid] = 1500;
  
  motor_stop();
  
  calibrateSensors();         // Calibrate neutral values of gyros  (in Sensors.pde)

  delay(1000);
  
  SerPrln("Ready");
}



