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


		 
void readSerialCommand() {
		//if(!ShowMenu) Show_Menu();
	// Check for serial message
	if (SerAva()) {
	    byte tmp = 	queryType;
            queryType = SerRea();
            if (queryType == '\r' || queryType == '\n')
            {
              queryType = tmp;
            }
        }
}

//******************************************************************************************************************

void sendSerialTelemetry() {
	float aux_float[3]; // used for sensor calibration
	switch (queryType) {
  
		// Choices are made here, moved all actual logic into 
		// functions more towards the bottom (kidogo, Feb 6, 2011)
                case 'a': 						
			show_all_Parameter();
			queryType = 'x';
			break;
                case 'C': 						
			Calibrate_Accel_Offset();
			queryType = 'x';
			break;
		case 'p': 						
			//Show_Stable_PIDs();
			show_Parameter();
                        queryType = 'x';
			break;
		case 'P': 						
			//Receive_Stable_PID();						
			write_Parameter();
			queryType = 'x';
			break;
		case 'r': 						
			Show_Receiver_Values();
			break;
		case 's': 						
			Show_Sensor_Data();
			break;
		case 'T': 		
                        Calibrate_Pitch();
			queryType = 'x';
			break;
		case 'w': 		
                        Show_Angle();
			break;
		case '#': 						
			Reset_Settings();
			queryType = 'x';
			break;
		case '*': 						
			writeUserConfig();
			queryType = 'x';
			break;
		case 'M': 						
		        RUN_Motors();
			queryType = 'x';
			break;
		case 'm': 						
		        show_Motors();
			break;
                case '?':
                
                        SerPrln(" a - Show all Parameter");
                        SerPrln(" C - Calibrate_Accel_Offset");
                        SerPrln(" M - Test Motors");
                        SerPrln(" m - Show Control and Motors");
                        SerPrln(" p - Show Parameter px");
                        SerPrln(" P - Receive Parameter P1;xx.x");
                        SerPrln(" r - Show_Receiver_Values()");
                        SerPrln(" s - Show_Sensor_Data");
                        SerPrln(" T - Calibrate_Remote");
                        SerPrln(" w - Show Angles");
                        SerPrln(" # - Reset_Settings");
                        SerPrln(" * - writeUserConfig to flash");
                        SerPrln(" x - stop output");
                        
			queryType = 'x';
                        break;
                default:
                        break;
	}
}


/****************************************************
  Show all Parameter - a
****************************************************/
void show_all_Parameter() {
  for (int i = 0; i <= LAST_PARAMETER; i++)
  {
    SerPri("p");
    SerPri(i);
    SerPri(";");
    SerPrln(readEEPROM(i));
  }
}

/****************************************************
  Show Parameter - p
****************************************************/
void show_Parameter() {
  float tmpAdr =   readFloatSerial();
  SerPri("p");
  SerPri(tmpAdr);
  SerPri(";");
  SerPrln(readEEPROM(tmpAdr));
}
/****************************************************
  write Parameter - p
****************************************************/
void write_Parameter() {
  float tmpAdr = readFloatSerial();
  float tmpValue = readFloatSerial();
  writeEEPROM(tmpValue, tmpAdr);
  readUserConfig();
  SerPri("p");
  SerPri(tmpAdr);
  SerPri(";");
  SerPrln(readEEPROM(tmpAdr));
}


/****************************************************
  Show Control Values and Motor - m
****************************************************/
void show_Motors() {
    SerPri(control_roll);
    comma();
    SerPri(control_nick);
    comma();
    SerPri(control_yaw);
    comma();
    SerPri(frontMotor);
    comma();
    SerPri(backMotor);
    comma();
    SerPri(leftMotor);
    comma();
    SerPrln(rightMotor);
    comma();
}


/****************************************************
  SHOW STABLE MODE PIDS - p
****************************************************/
void Show_Stable_PIDs() {
    
    SerPri(parameter[KP_QUAD_ROLL]);
    comma();
    SerPri(parameter[KI_QUAD_ROLL]);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_ROLL]);
    comma();
    SerPri(parameter[KP_QUAD_NICK]);
    comma();
    SerPri(parameter[KI_QUAD_NICK]);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_NICK]);
    comma();
    SerPri(parameter[KP_QUAD_YAW]);
    comma();
    SerPri(parameter[KI_QUAD_YAW]);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_YAW]);
    comma();
    SerPri(parameter[Kp_ROLLNICK]);
    comma();
    SerPrln(parameter[Ki_ROLLNICK]);
//    comma();
//    SerPrln(STABLE_MODE_KP_RATE, 3);    // NOT USED NOW
}


/****************************************************
  CALIBRATE THROTTLE - T
****************************************************/
void Calibrate_Pitch() {
  int tmp = 1200;

  SerPrln("Move your pitch to MIN, reading starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  SerPrln("Reading Pitch value, enter x to exit");

  SerFlu();
  delay(50);
  for (int i = 0; i < 50; i++) {
     if (reciverPPM.GetState() > 0)
     {
       ch_pitch = reciverPPM.InputCh(CH_PITCH);
     }
     else
     {
         // we read all spektrum bytes
        while (Serial2.available()) {
          reciverSpek.regByte(Serial2.read());
        }
       ch_pitch = reciverSpek.InputCh(CH_PITCH);
     }
     
    SerPri("Pitch channel: ");
    SerPrln(ch_pitch);
    if(tmp > ch_pitch) tmp = ch_pitch;
    delay(50);
    if(SerAva() > 0){
      break; 
    }
  }

  SerPrln();
  SerPri("Lowest pitch value: ");
  SerPrln(tmp);
  SerPrln();
  SerPrln("Saving MIN_PITCH to EEPROM");
  writeEEPROM(tmp, MIN_PITCH);
  delay(200);
  SerPri("p");
  SerPri(MIN_PITCH);
  SerPri(";");
  SerPrln(tmp);
  SerPrln("Saved..");
  SerPrln();
  
  // READ ROLL MID
  tmp = 1500;
  SerPrln("Move your roll to MID, reading starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  SerPrln("Reading Roll value, enter x to exit");

  SerFlu();
  delay(50);
  for (int i = 0; i < 50; i++) {
     if (reciverPPM.GetState() > 0)
     {
       ch_roll = reciverPPM.InputCh(CH_ROLL);
     }
     else
     {
         // we read all spektrum bytes
        while (Serial2.available()) {
          reciverSpek.regByte(Serial2.read());
        }
       ch_roll = reciverSpek.InputCh(CH_ROLL);
     }
     
    SerPri("Roll channel: ");
    SerPrln(ch_roll);
    tmp += ch_pitch;
    delay(50);
    if(SerAva() > 0){
      break; 
    }
  }
  tmp = tmp / 50;
  SerPrln();
  SerPri("Mid roll value: ");
  SerPrln(tmp);
  SerPrln();
  SerPrln("Saving roll_mid to EEPROM");
  writeEEPROM(tmp, roll_mid);
  delay(200);
  SerPri("p");
  SerPri(roll_mid);
  SerPri(";");
  SerPrln(tmp);
  SerPrln("Saved..");
  SerPrln();
  
  // READ Nick MID
  tmp = 1500;
  SerPrln("Move your nick to MID, reading starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  SerPrln("Reading Nick value, enter x to exit");

  SerFlu();
  delay(50);
  for (int i = 0; i < 50; i++) {
     if (reciverPPM.GetState() > 0)
     {
       ch_nick = reciverPPM.InputCh(CH_NICK);
     }
     else
     {
         // we read all spektrum bytes
        while (Serial2.available()) {
          reciverSpek.regByte(Serial2.read());
        }
       ch_nick = reciverSpek.InputCh(CH_NICK);
     }
     
    SerPri("Nick channel: ");
    SerPrln(ch_nick);
    tmp += ch_nick;
    delay(50);
    if(SerAva() > 0){
      break; 
    }
  }
  tmp = tmp / 50;
  SerPrln();
  SerPri("Mid nick value: ");
  SerPrln(tmp);
  SerPrln();
  SerPrln("Saving nick_mid to EEPROM");
  writeEEPROM(tmp, nick_mid);
  delay(200);
  SerPri("p");
  SerPri(nick_mid);
  SerPri(";");
  SerPrln(tmp);
  SerPrln("Saved..");
  SerPrln();
  
  // READ Yaw MID
  tmp = 1500;
  SerPrln("Move your yaw to MID, reading starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  SerPrln("Reading Yaw value, enter x to exit");

  SerFlu();
  delay(50);
  for (int i = 0; i < 50; i++) {
     if (reciverPPM.GetState() > 0)
     {
       ch_yaw = reciverPPM.InputCh(CH_YAW);
     }
     else
     {
         // we read all spektrum bytes
        while (Serial2.available()) {
          reciverSpek.regByte(Serial2.read());
        }
       ch_yaw = reciverSpek.InputCh(CH_YAW);
     }
     
    SerPri("Yaw channel: ");
    SerPrln(ch_yaw);
    tmp += ch_yaw;
    delay(50);
    if(SerAva() > 0){
      break; 
    }
  }
  tmp = tmp / 50;
  SerPrln();
  SerPri("Mid yaw value: ");
  SerPrln(tmp);
  SerPrln();
  SerPrln("Saving yaw_mid to EEPROM");
  writeEEPROM(tmp, yaw_mid);
  delay(200);
  SerPri("p");
  SerPri(yaw_mid);
  SerPri(";");
  SerPrln(tmp);
  SerPrln("Saved..");
  SerPrln();
}


/****************************************************
  SHOW RECEIVER VALUES - r
****************************************************/
void Show_Receiver_Values() { 
    SerPri("r");
    comma();
    SerPri(ch_roll); // Aileron
    comma();
    SerPri(ch_nick); // Elevator
    comma();
    SerPri(ch_yaw); // Yaw
    comma();
    SerPri(ch_pitch); // Throttle
    comma();
    SerPri(ch_aux1); // AUX1 (Mode)
    comma();
    SerPri(ch_aux2); // AUX2 
    comma();
    SerPri(ch_aux3); 
    comma();
    SerPrln(ch_aux4); 
}

/****************************************************
  SHOW ANGLE - w
****************************************************/
void Show_Angle() {

    SerPri(degrees(roll));
    comma();
    SerPri(degrees(nick));
    comma();
    SerPri(degrees(yaw));
    comma();
    SerPri(err_roll);
    comma();
    SerPri(err_nick);
    comma();
    SerPrln(err_yaw);

}


/****************************************************
  SHOW SENSOR DATA - s
****************************************************/
void Show_Sensor_Data() {
    SerPri("s");
    comma();
    SerPri(read_adc(0));
    //SerPri(AN[0]);
    comma();
    SerPri(read_adc(1));
    //SerPri(AN[1]);
    comma();
    SerPri(read_adc(2));
    //SerPri(AN[2]);
    comma();
    SerPri(read_adc(4));
    //SerPri(AN[3]);
    comma();
    SerPri(read_adc(3));
    //SerPri(AN[4]);
    comma();
    SerPri(read_adc(5));
    comma();
    SerPri(adc.Ch(sensors[6])); //BAT
    //SerPri(AN[5]);
    comma();
    SerPri(degrees(roll));
    comma();
    SerPri(degrees(nick));
    comma();
    SerPrln(degrees(yaw));
	
}

/****************************************************
  RESET SETTINGS - #
****************************************************/
void Reset_Settings() {
	SerPrln("Reseting EEPROM to default!"); 
	delay(500);
	SerPrln("Reseting to factory settings!");
	defaultUserConfig();
	delay(200);
	SerPrln("Saving to EEPROM");
	writeUserConfig();
	SerPrln("Done..");
}

/****************************************************
  CALIBRATE ACCELEROMETER OFFSET - C
****************************************************/
void Calibrate_Accel_Offset() {

  int loopy;
  long xx = 0, xy = 0, xz = 0; 

  SerPrln("Initializing Accelerometer..");
  adc.Init();            // APM ADC library initialization
  //  delay(250);                // Giving small moment before starting

  calibrateSensors();         // Calibrate neutral values of gyros  (in Sensors.pde)

  SerPrln();
  SerPrln("Sampling 50 samples from Accelerometers, don't move your ArduCopter!");
  SerPrln("Sample:\tAcc-X\tAxx-Y\tAcc-Z");

  for(loopy = 1; loopy <= 50; loopy++) {
    SerPri("  ");
    SerPri(loopy);
    SerPri(":");
    tab();
    SerPri(xx += adc.Ch(3));
    tab();
    SerPri(xy += adc.Ch(4));
    tab();
    SerPrln(xz += adc.Ch(5));
    delay(20);
  }

  xx = xx / (loopy - 1);
  xy = xy / (loopy - 1);
  xz = xz / (loopy - 1);
  xz = xz - 407;               // Z-Axis correction
  //  xx += 42;

  parameter[acc_offset_y] = xy;
  parameter[acc_offset_x] = xx;
  parameter[acc_offset_z] = xz;

  AN_OFFSET[3] = parameter[acc_offset_x];
  AN_OFFSET[4] = parameter[acc_offset_y];
  AN_OFFSET[5] = parameter[acc_offset_z];

  SerPrln();
  SerPrln("Offsets as follows: ");  
  SerPri("  ");
  tab();
  SerPri(parameter[acc_offset_x]);
  tab();
  SerPri(parameter[acc_offset_y]);
  tab();
  SerPrln(parameter[acc_offset_z]);

  SerPrln("Final results as follows: ");  
  SerPri("  ");
  tab();
  SerPri(adc.Ch(3) - parameter[acc_offset_x]);
  tab();
  SerPri(adc.Ch(4) - parameter[acc_offset_y]);
  tab();
  SerPrln(adc.Ch(5) - parameter[acc_offset_z]);
  SerPrln();
  SerPrln("Final results should be close to 0, 0, 408 if they are far (-+10) from ");
  SerPrln("those values, redo initialization or use Configurator for finetuning");
  SerPrln();
  SerPrln("Saving values to EEPROM");
  writeEEPROM(parameter[acc_offset_x], acc_offset_x);
  writeEEPROM(parameter[acc_offset_y], acc_offset_y);
  writeEEPROM(parameter[acc_offset_z], acc_offset_z);
  delay(200);
  SerPri("p");
  SerPri(acc_offset_x);
  SerPri(";");
  SerPrln(parameter[acc_offset_x]);
  SerPri("p");
  SerPri(acc_offset_y);
  SerPri(";");
  SerPrln(parameter[acc_offset_y]);
  SerPri("p");
  SerPri(acc_offset_z);
  SerPri(";");
  SerPrln(parameter[acc_offset_z]);
  SerPrln("Saved..");
  SerPrln();
}


/****************************************************
  RECEIVE STABLE MODE PID VALUES - P
****************************************************/
void Receive_Stable_PID() {
    SerPrln("P Roll:");
    while (!SerAva()) {}
    parameter[KP_QUAD_ROLL] = readFloatSerial();
    SerPrln("I Roll:");
    while (!SerAva()) {}
    parameter[KI_QUAD_ROLL] = readFloatSerial();
    SerPrln("P Rate Roll:");
    while (!SerAva()) {}
    parameter[STABLE_MODE_KP_RATE_ROLL] = readFloatSerial();
    SerPrln("P Pitch/Nick:");
    while (!SerAva()) {}
    parameter[KP_QUAD_NICK] = readFloatSerial();
    SerPrln("I Nick:");
    while (!SerAva()) {}
    parameter[KI_QUAD_NICK] = readFloatSerial();
    SerPrln("P Rate Nick:");
    while (!SerAva()) {}
    parameter[STABLE_MODE_KP_RATE_NICK] = readFloatSerial();
    SerPrln("P Yaw:");
    while (!SerAva()) {}
    parameter[KP_QUAD_YAW] = readFloatSerial();
    SerPrln("I Yaw:");
    while (!SerAva()) {}
    parameter[KI_QUAD_YAW] = readFloatSerial();
    SerPrln("P Rate Yaw:");
    while (!SerAva()) {}
    parameter[STABLE_MODE_KP_RATE_YAW] = readFloatSerial();
    SerPrln("P Roll/Nick ACC:");
    while (!SerAva()) {}
    parameter[Kp_ROLLNICK] = readFloatSerial();
    SerPrln("I Roll/Nick ACC:");
    while (!SerAva()) {}
    parameter[Ki_ROLLNICK] = readFloatSerial();
//    SerPrln("P Roll:");
//    STABLE_MODE_KP_RATE = readFloatSerial();   // NOT USED NOW
    
	SerPrln("Saving to EEPROM");
	writeUserConfig();

}

/****************************************************
  RUN MOTORS 
****************************************************/
void RUN_Motors() {

	SerPrln("Hit numbers 1 - 4 for corresponding motor.");
	SerPrln("Motor will pulse slowly! (20% Throttle)");
	SerPrln("SAFETY!! Remove all propellers before doing stick movements");
	SerPrln();
	SerPrln("Exit from test by x + Enter");
	SerPrln();

	SerFlu();
	delay(50);
	while(1) {


                if(SerAva() > 0)
                {
                  byte motorNr = SerRea();
                  
                  char frontMotor;
                  char backMotor;
                  char leftMotor;
                  char rightMotor;
                  
                  switch (motorNr)
                  {
                    case '1':
                      frontMotor = 20;
                      backMotor = 1;
                      leftMotor = 1;
                      rightMotor = 1;
                      break;
                    case '2':
                      frontMotor = 1;
                      backMotor = 20;
                      leftMotor = 1;
                      rightMotor = 1;
                      break;
                    case '3':
                      frontMotor = 1;
                      backMotor = 1;
                      leftMotor = 20;
                      rightMotor = 1;
                      break;
                    case '4':
                      frontMotor = 1;
                      backMotor = 1;
                      leftMotor = 1;
                      rightMotor = 20;
                      break;
                    case 'x':
                      return;
                      break;
                    default:
                      frontMotor = 0;
                      backMotor = 0;
                      leftMotor = 0;
                      rightMotor = 0;
                      break;
                      
                  }
  
            	  char start = 0xF5;
          
                    Serial3.write(start);
                    Serial3.write(frontMotor);
                    Serial3.write(backMotor);
                    Serial3.write(rightMotor);
                    Serial3.write(leftMotor);
                }
	} 
}


//******************************************************************************************************************
// SUPPORTING FUNCTIONS BELOW 
//******************************************************************************************************************

/****************************************************
  PRINT COMMA OVER SERIAL
****************************************************/
void comma() {
	SerPri(',');
}

/****************************************************
  PRINT TAB OVER SERIAL
****************************************************/
void tab() {
	SerPri("\t");
}

/****************************************************
  PRINT COMMA, SPACE OVER SERIAL
****************************************************/
void cspc() {
	SerPri(", ");
}

/****************************************************
  WAIT FOR SERIAL ENTER
****************************************************/
void WaitSerialEnter() {
	// Flush serials
	SerFlu();
	delay(50);
	while(1) {
		if(SerAva() > 0){
			break; 
		}
		delay(20);
	}
	delay(250);
	SerFlu();  
}

/****************************************************
  PRINT COMMAND TO CLEAR REMOTE SERIAL CONSOLE
****************************************************/
void SerPriClScr() {  // Clears the remote serial console
	SerPrln(". . . .");
}

/****************************************************
  READ FLOATING POINT VALUE OVER SERIAL
****************************************************/
float readFloatSerial() {
	byte index = 0;
	byte timeout = 0;
	char data[128] = "";

	do {
		if (SerAva() == 0) {
			delay(10);
			timeout++;
		} else {
			data[index] = SerRea();
			timeout = 0;
			index++;
		}
	}  
	while ((data[constrain(index-1, 0, 128)] != ';') && (timeout < 5) && (index < 128));
	//Serial1.println(atof(data));
        return atof(data);
}

