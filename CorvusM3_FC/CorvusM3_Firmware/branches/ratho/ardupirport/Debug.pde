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


#if DEBUG_SUBSYSTEM == 1
void debug_subsystem()
{
	Serial1.println("Begin Debug: Radio Subsystem ");
	while(1){
		delay(20);
		// Filters radio input - adjust filters in the radio.pde file
		// ----------------------------------------------------------
		read_radio();
		Serial1.print("Radio in ch1: ");
		Serial1.print(radio_in[CH_ROLL], DEC);
		Serial1.print("\tch2: ");
		Serial1.print(radio_in[CH_PITCH], DEC);
		Serial1.print("\tch3: ");
		Serial1.print(radio_in[CH_THROTTLE], DEC);
		Serial1.print("\tch4: ");
		Serial1.print(radio_in[CH_RUDDER], DEC);
		Serial1.print("\tch5: ");
		Serial1.print(radio_in[4], DEC);
		Serial1.print("\tch6: ");
		Serial1.print(radio_in[5], DEC);
		Serial1.print("\tch7: ");
		Serial1.print(radio_in[6], DEC);
		Serial1.print("\tch8: ");
		Serial1.println(radio_in[7], DEC);
	}
}
#endif

#if DEBUG_SUBSYSTEM == 2
void debug_subsystem()
{
	Serial1.println("Begin Debug: Servo Subsystem ");
	Serial1.print("Reverse ROLL - CH1: ");
	Serial1.println(reverse_roll, DEC);
	Serial1.print("Reverse PITCH - CH2: ");
	Serial1.println(reverse_pitch, DEC);
	Serial1.print("Reverse THROTTLE - CH3: ");
	Serial1.println(REVERSE_THROTTLE, DEC);
	Serial1.print("Reverse RUDDER - CH4: ");
	Serial1.println(reverse_rudder, DEC);

	// read the radio to set trims
	// ---------------------------
	trim_radio();

	radio_max[0]				 = 	CH1_MAX;
	radio_max[1]				 = 	CH2_MAX;
	radio_max[2]				 = 	CH3_MAX;
	radio_max[3]				 = 	CH4_MAX;
	radio_max[4]				 = 	CH5_MAX;
	radio_max[5]				 = 	CH6_MAX;
	radio_max[6]				 = 	CH7_MAX;
	radio_max[7]				 = 	CH8_MAX;
	radio_min[0]				 = 	CH1_MIN;
	radio_min[1]				 = 	CH2_MIN;
	radio_min[2]				 = 	CH3_MIN;
	radio_min[3]				 = 	CH4_MIN;
	radio_min[4]				 = 	CH5_MIN;
	radio_min[5]				 = 	CH6_MIN;
	radio_min[6]				 = 	CH7_MIN;
	radio_min[7]				 = 	CH8_MIN;

	while(1){
		delay(20);
		// Filters radio input - adjust filters in the radio.pde file
		// ----------------------------------------------------------
		read_radio();
		update_servo_switches();

		servo_out[CH_ROLL]	= ((radio_in[CH_ROLL]	- radio_trim[CH_ROLL])	* 45.0f	* reverse_roll) / 500;
		servo_out[CH_PITCH] = ((radio_in[CH_PITCH] - radio_trim[CH_PITCH]) * 45.0f	* reverse_roll) / 500;
		servo_out[CH_RUDDER] = ((radio_in[CH_RUDDER] - radio_trim[CH_RUDDER]) * 45.0f	* reverse_rudder) / 500;

		// write out the servo PWM values
		// ------------------------------
		set_servos_4();
		
		
		for(int y = 4; y < 8; y++) { 
			radio_out[y] = constrain(radio_in[y], 	radio_min[y], 	radio_max[y]);	
			APM_RC.OutputCh(y, radio_out[y]); // send to Servos
		}

		/*
		Serial1.print("Servo_out ch1: ");
		Serial1.print(servo_out[CH_ROLL], DEC);
		Serial1.print("\tch2: ");
		Serial1.print(servo_out[CH_PITCH], DEC);
		Serial1.print("\tch3: ");
		Serial1.print(servo_out[CH_THROTTLE], DEC);
		Serial1.print("\tch4: ");
		Serial1.print(servo_out[CH_RUDDER], DEC);
		*/
		///*
		Serial1.print("ch1: ");
		Serial1.print(radio_out[CH_ROLL], DEC);
		Serial1.print("\tch2: ");
		Serial1.print(radio_out[CH_PITCH], DEC);
		Serial1.print("\tch3: ");
		Serial1.print(radio_out[CH_THROTTLE], DEC);
		Serial1.print("\tch4: ");
		Serial1.print(radio_out[CH_RUDDER], DEC);
		Serial1.print("\tch5: ");
		Serial1.print(radio_out[4], DEC);
		Serial1.print("\tch6: ");
		Serial1.print(radio_out[5], DEC);
		Serial1.print("\tch7: ");
		Serial1.print(radio_out[6], DEC);
		Serial1.print("\tch8: ");
		Serial1.println(radio_out[7], DEC);
		
		//*/
	}
}
#endif


#if DEBUG_SUBSYSTEM == 3
void debug_subsystem()
{
	Serial1.println("Begin Debug: Analog Sensor Subsystem ");
	
	Serial1.print("AirSpeed sensor enabled: ");
	Serial1.println(AIRSPEED_SENSOR, DEC);
		
	Serial1.print("Enable Battery: ");
	Serial1.println(BATTERY_EVENT, DEC);
	zero_airspeed();
	
	Serial1.print("Air pressure offset:");
	Serial1.println(airpressure_offset, DEC);

	while(1){
		delay(20);
		read_z_sensor();
		read_XY_sensors();
		read_battery();
		
		Serial1.print("Analog IN:");
		Serial1.print("  0:");
		Serial1.print(analog0, DEC);
		Serial1.print(", 1: ");
		Serial1.print(analog1, DEC);
		Serial1.print(", 2: ");
		Serial1.print(analog2, DEC);
		Serial1.print(", 3: ");
		Serial1.print(airpressure_raw, DEC);
		
		Serial1.print("\t\tSensors:");
		Serial1.print("  airSpeed:");
		Serial1.print(airspeed, DEC);
		Serial1.print("m \tbattV:");
		Serial1.print(battery_voltage, DEC);
		Serial1.println("volts ");
	}
}
#endif


#if DEBUG_SUBSYSTEM == 6
void debug_subsystem()
{
	Serial1.println("Begin Debug: IMU Subsystem ");
	startup_IMU_ground();
	
	while(1){
		delay(20);
		read_AHRS();
		
		// We are using the IMU
		// ---------------------
		Serial1.print("  roll:");
		Serial1.print(roll_sensor / 100, DEC);	
		Serial1.print("  pitch:");
		Serial1.print(pitch_sensor / 100, DEC);
		Serial1.print("  yaw:");
		Serial1.println(yaw_sensor / 100, DEC);
		
	}
}
#endif



#if DEBUG_SUBSYSTEM == 13
void debug_subsystem()
{
	Serial1.println("Begin Debug: Throttle Subsystem ");
	read_radio();
	
	uint16_t low_pwm = radio_in[CH_THROTTLE];
	uint16_t high_pwm = radio_in[CH_THROTTLE];
	
	while(1){
		delay(20);
		// Filters radio input - adjust filters in the radio.pde file
		// ----------------------------------------------------------
		read_radio();
		//update_throttle();
		set_servos_4();
		low_pwm = min(low_pwm, radio_in[CH_THROTTLE]);
		high_pwm = max(high_pwm, radio_in[CH_THROTTLE]);
		
		Serial1.print("Radio_in: ");
		Serial1.print(radio_in[CH_THROTTLE], DEC);
		Serial1.print("\tPWM output: ");
		Serial1.print(radio_out[CH_THROTTLE], DEC);
		Serial1.print("\tThrottle: ");
		Serial1.print(servo_out[CH_THROTTLE], DEC);
		Serial1.print("\tPWM Min: ");
		Serial1.print(low_pwm, DEC);
		Serial1.print("\tPWM Max: ");
		Serial1.println(high_pwm, DEC);
	}
}
#endif


#if DEBUG_SUBSYSTEM == 14
void debug_subsystem()
{
	Serial1.println("Begin Debug: Radio Min Max ");
	uint16_t low_pwm[8];
	uint16_t high_pwm[8];
	uint8_t i;
	
	for(i = 0; i < 100; i++){
		delay(20);
		read_radio();
	}
	
	for(i = 0; i < 8; i++){
		radio_min[i] = 0;
		radio_max[i] = 2400;
	 	low_pwm[i]	= radio_in[i];
 		high_pwm[i]	= radio_in[i];
	}
	
	while(1){
		delay(100);
		// Filters radio input - adjust filters in the radio.pde file
		// ----------------------------------------------------------
		read_radio();
		for(i = 0; i < 8; i++){
			low_pwm[i] = min(low_pwm[i], radio_in[i]);
			high_pwm[i] = max(high_pwm[i], radio_in[i]);
		}

		for(i = 0; i < 8; i++){
			Serial1.print("CH");
			Serial1.print(i + 1, DEC);
			Serial1.print(": ");
			low_pwm[i] = min(low_pwm[i], radio_in[i]);
			Serial1.print(low_pwm[i], DEC);
			Serial1.print("|");
			high_pwm[i] = max(high_pwm[i], radio_in[i]);
			Serial1.print(high_pwm[i], DEC);
			Serial1.print("   ");
		}
		Serial1.println(" ");
	}
}
#endif


#if DEBUG_SUBSYSTEM == 15
void debug_subsystem()
{
	Serial1.println("Begin Debug: EEPROM Dump ");
	uint16_t temp;
	for(int n = 0; n < 512; n++){
		for(int i = 0; i < 4; i++){
			temp = eeprom_read_word((uint16_t *) mem);
			mem += 2;
			Serial1.print(temp, HEX);
			Serial1.print("  ");
		}
		Serial1.print("  ");
		for(int i = 0; i < 4; i++){
			temp = eeprom_read_word((uint16_t *) mem);
			mem += 2;
			Serial1.print(temp, HEX);
			Serial1.print("  ");
		}
	}
}
#endif

