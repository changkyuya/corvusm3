/* ************************************************************** */
  
// Declare variables


/*
***************************************************
*												  *
*		R E A D   S E R I A L   C O M M A N D     *
*												  *
***************************************************

Functions within this function may alter data, may
write values to EEPROM. Generally, these functions
should not be used while in flight.
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
// TELEMETRY FUNCTIONS BELOW 
//******************************************************************************************************************
/*
***************************************************
*												  *
*		S E R I A L   T E L E M E T R Y           *
*												  *
*												  *
***************************************************
         					 */

void sendSerialTelemetry() {
	float aux_float[3]; // used for sensor calibration
	switch (queryType) {
  
		// Choices are made here, moved all actual logic into 
		// functions more towards the bottom (kidogo, Feb 6, 2011)
		case 'C': 						
			Calibrate_Accel_Offset();
			queryType = 'x';
			break;
		case 'p': 						
			Show_Stable_PIDs();
			queryType = 'x';
			break;
		case 'P': 						
			Receive_Stable_PID();
			queryType = 'x';
			break;
		case 'r': 						
			Show_Receiver_Values();
			break;
		case 's': 						
			Show_Sensor_Data();
			break;
		case 't': 		
                        Show_Transmitter_Calibration();
			queryType = 'x';
			break;
		case 'T': 		
                        Calibrate_Throttle();
			queryType = 'x';
			break;
		case 'V': 						// Receive transmitter calibration values
			Receive_Transmitter_Calibration();
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
                
                        SerPrln("C - Calibrate_Accel_Offset");
                        SerPrln("M - Test Motors");
                        SerPrln("m - Show Control and Motors");
                        SerPrln("p - Show_Stable_PIDs");
                        SerPrln("P - Receive_Stable_PID");
                        SerPrln("r - Show_Receiver_Values()");
                        SerPrln("V - Receive_Receiver_Values()");
                        SerPrln("s - Show_Sensor_Data");
                        SerPrln("t - Show_Transmitter_Calibration");
                        SerPrln("T - Calibrate_Throttle");
                        SerPrln("w - Show Angles");
                        SerPrln("# - Reset_Settings");
                        SerPrln("* - writeUserConfig to flash");
                        SerPrln("x - stop output");
                        
			queryType = 'x';
                        break;
                default:
                        break;
	}
}



/****************************************************
  Show Control Values and Motor - m
****************************************************/
void show_Motors() {
    SerPri(control_roll);
    comma();
    SerPri(control_pitch);
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
  RECEIVE TRANSMITTER CALIBRATION VALUES - v
****************************************************/
void Receive_Transmitter_Calibration() {
    SerPrln("input roll slope:");
    while (!SerAva()) {}
    ch_roll_slope = readFloatSerial();
    SerPrln("input roll offset:");
    while (!SerAva()) {}
    ch_roll_offset = readFloatSerial();
    SerPrln("input pitch/nick slope:");
    while (!SerAva()) {}
    ch_pitch_slope = readFloatSerial();
    SerPrln("input pich/nick offset:");
    while (!SerAva()) {}
    ch_pitch_offset = readFloatSerial();
    SerPrln("input yaw slope:");
    while (!SerAva()) {}
    ch_yaw_slope = readFloatSerial();
    SerPrln("input yaw offset:");
    while (!SerAva()) {}
    ch_yaw_offset = readFloatSerial();
    SerPrln("input throttle slope:");
    while (!SerAva()) {}
    ch_throttle_slope = readFloatSerial();
    SerPrln("input throttle offset:");
    while (!SerAva()) {}
    ch_throttle_offset = readFloatSerial();
//    SerPrln("input aus:");
//    ch_aux_slope = readFloatSerial();
//    SerPrln("input roll slope:");
//    ch_aux_offset = readFloatSerial();
//    SerPrln("input roll slope:");
//    ch_aux2_slope = readFloatSerial();
//    SerPrln("input roll slope:");
//    ch_aux2_offset = readFloatSerial();
//    SerPrln("input roll slope:");
	SerPrln("Saving to EEPROM");
	writeUserConfig();
}

/****************************************************
  SHOW STABLE MODE PIDS - p
****************************************************/
void Show_Stable_PIDs() {
    
    SerPri(KP_QUAD_ROLL);
    comma();
    SerPri(KI_QUAD_ROLL);
    comma();
    SerPri(STABLE_MODE_KP_RATE_ROLL);
    comma();
    SerPri(KP_QUAD_PITCH);
    comma();
    SerPri(KI_QUAD_PITCH);
    comma();
    SerPri(STABLE_MODE_KP_RATE_PITCH);
    comma();
    SerPri(KP_QUAD_YAW);
    comma();
    SerPri(KI_QUAD_YAW);
    comma();
    SerPrln(STABLE_MODE_KP_RATE_YAW);
//    comma();
//    SerPrln(STABLE_MODE_KP_RATE, 3);    // NOT USED NOW
}


/****************************************************
  CALIBRATE THROTTLE - T
****************************************************/
void Calibrate_Throttle() {
  int tmpThrottle = 1200;

  SerPrln("Move your throttle to MIN, reading starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  SerPrln("Reading Throttle value, enter x to exit");

  SerFlu();
  delay(50);
  for (int i = 0; i < 50; i++) {
     if (CM3_RC.GetState() > 0)
     {
       ch_throttle = CM3_RC.InputCh(CH_THROTTLE);
     }
     else
     {
         // we read all spektrum bytes
        while (Serial2.available()) {
          reciver.regByte(Serial2.read());
        }
       ch_throttle = reciver.getThro();
     }
     
    SerPri("Throttle channel: ");
    SerPrln(ch_throttle);
    if(tmpThrottle > ch_throttle) tmpThrottle = ch_throttle;
    delay(50);
    if(SerAva() > 0){
      break; 
    }
  }

  SerPrln();
  SerPri("Lowest throttle value: ");
  SerPrln(tmpThrottle);
  SerPrln();
  SerPrln("Saving MIN_THROTTLE to EEPROM");
  writeEEPROM(tmpThrottle, MIN_THROTTLE_ADR);
  delay(200);
  SerPrln("Saved..");
  SerPrln();
}


/****************************************************
  SHOW RECEIVER VALUES - r
****************************************************/
void Show_Receiver_Values() { 
    SerPri(ch_roll); // Aileron
    comma();
    SerPri(ch_pitch); // Elevator
    comma();
    SerPri(ch_yaw); // Yaw
    comma();
    SerPri(ch_throttle); // Throttle
    comma();
    SerPri(ch_aux); // AUX1 (Mode)
    comma();
    SerPri(ch_aux2); // AUX2 
    comma();
    SerPri(roll_mid); // Roll MID value
    comma();
    SerPri(pitch_mid); // Pitch MID value
    comma();
    SerPrln(yaw_mid); // Yaw MID Value
}

/****************************************************
  SHOW ANGLE - w
****************************************************/
void Show_Angle() {

    SerPri(degrees(roll));
    comma();
    SerPri(degrees(pitch));
    comma();
    SerPri(degrees(yaw));
    comma();
    SerPri(err_roll);
    comma();
    SerPri(err_pitch);
    comma();
    SerPrln(err_yaw);

}


/****************************************************
  SHOW SENSOR DATA - s
****************************************************/
void Show_Sensor_Data() {
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
    SerPri(degrees(pitch));
    comma();
    SerPrln(degrees(yaw));
	
}

/****************************************************
  RESET SETTINGS - #
****************************************************/
byte Reset_Settings() {
	
  
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

  acc_offset_y = xy;
  acc_offset_x = xx;
  acc_offset_z = xz;

  AN_OFFSET[3] = acc_offset_x;
  AN_OFFSET[4] = acc_offset_y;
  AN_OFFSET[5] = acc_offset_z;

  SerPrln();
  SerPrln("Offsets as follows: ");  
  SerPri("  ");
  tab();
  SerPri(acc_offset_x);
  tab();
  SerPri(acc_offset_y);
  tab();
  SerPrln(acc_offset_z);

  SerPrln("Final results as follows: ");  
  SerPri("  ");
  tab();
  SerPri(adc.Ch(3) - acc_offset_x);
  tab();
  SerPri(adc.Ch(4) - acc_offset_y);
  tab();
  SerPrln(adc.Ch(5) - acc_offset_z);
  SerPrln();
  SerPrln("Final results should be close to 0, 0, 408 if they are far (-+10) from ");
  SerPrln("those values, redo initialization or use Configurator for finetuning");
  SerPrln();
  SerPrln("Saving values to EEPROM");
  writeEEPROM(acc_offset_x, acc_offset_x_ADR);
  writeEEPROM(acc_offset_y, acc_offset_y_ADR);
  writeEEPROM(acc_offset_z, acc_offset_z_ADR);
  delay(200);
  SerPrln("Saved..");
  SerPrln();
}

/****************************************************
  SHOW TRANSMITTER CALIBRATION VALUES - t
****************************************************/
void Show_Transmitter_Calibration() {
    SerPri(ch_roll_slope);
    comma();
    SerPri(ch_roll_offset);
    comma();
    SerPri(ch_pitch_slope);
    comma();
    SerPri(ch_pitch_offset);
    comma();
    SerPri(ch_yaw_slope);
    comma();
    SerPri(ch_yaw_offset);
    comma();
    SerPri(ch_throttle_slope);
    comma();
    SerPri(ch_throttle_offset);
    comma();
    SerPri(ch_aux_slope);
    comma();
    SerPri(ch_aux_offset);
    comma();
    SerPri(ch_aux2_slope);
    comma();
    SerPrln(ch_aux2_offset);
}

/****************************************************
  RECEIVE STABLE MODE PID VALUES - P
****************************************************/
void Receive_Stable_PID() {
    SerPrln("P Roll:");
    while (!SerAva()) {}
    KP_QUAD_ROLL = readFloatSerial();
    SerPrln("I Roll:");
    while (!SerAva()) {}
    KI_QUAD_ROLL = readFloatSerial();
    SerPrln("P Rate Roll:");
    while (!SerAva()) {}
    STABLE_MODE_KP_RATE_ROLL = readFloatSerial();
    SerPrln("P Pitch/Nick:");
    while (!SerAva()) {}
    KP_QUAD_PITCH = readFloatSerial();
    SerPrln("I Pitch/Nick:");
    while (!SerAva()) {}
    KI_QUAD_PITCH = readFloatSerial();
    SerPrln("P Rate Pitch/Nick:");
    while (!SerAva()) {}
    STABLE_MODE_KP_RATE_PITCH = readFloatSerial();
    SerPrln("P Yaw:");
    while (!SerAva()) {}
    KP_QUAD_YAW = readFloatSerial();
    SerPrln("I Yaw:");
    while (!SerAva()) {}
    KI_QUAD_YAW = readFloatSerial();
    SerPrln("P Rate Yaw:");
    while (!SerAva()) {}
    STABLE_MODE_KP_RATE_YAW = readFloatSerial();
//    SerPrln("P Roll:");
//    STABLE_MODE_KP_RATE = readFloatSerial();   // NOT USED NOW
    
	SerPrln("Saving to EEPROM");
	writeUserConfig();

}

/****************************************************
  RUN MOTORS WITH STICKS
****************************************************/
void RUN_Motors() {
	long run_timer;
	byte motor;

	SerPrln("Hit numbers 1 - 4 for corresponding motor.");
	SerPrln("Motor will pulse slowly! (20% Throttle)");
	SerPrln("SAFETY!! Remove all propellers before doing stick movements");
	SerPrln();
	SerPrln("Exit from test by x + Enter");
	SerPrln();

	SerFlu();
	delay(50);
	while(1) {

//	        if (CM3_RC.GetState() > 0)
//                 {
//                     ch_roll = CM3_RC.InputCh(CH_ROLL);
//		    ch_pitch = CM3_RC.InputCh(CH_PITCH);
//                 }
//                 else
//                 {
//                     // we read all spektrum bytes
//                    while (Serial2.available()) {
//                      reciver.regByte(Serial2.read());
//                    }
//                     ch_roll = reciver.getRoll();
//    		      ch_pitch = reciver.getPitch();
//                      SerPri(ch_roll);
//                      SerPri("-");
//                      SerPrln(ch_pitch);
//                 }

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

