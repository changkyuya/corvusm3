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
// Read Serial Command
//******************************************************************************		 
void readSerialCommand() 
{
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

//******************************************************************************
// Send Serial Telemetry
//******************************************************************************
void sendSerialTelemetry() 
{
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
    case 'P': 									
      write_Parameter();
      queryType = 'x';
      break;
    case 'p': 		
      show_Parameter();
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
      SerPrln(" # - Reset_Settings");
      SerPrln(" * - writeUserConfig to flash");
      SerPrln(" x - stop output");            
      queryType = 'x';
      break;
    default:
      break;
  }
}

//******************************************************************************
// Show all Parameter - a
//******************************************************************************
void show_all_Parameter() 
{
  for (int i = 1; i <= LAST_PARAMETER; i++)
  {
    SerPri("p");
    SerPri(i);
    SerPri(";");
    SerPrln(readEEPROM(i),4);
  }
}

//******************************************************************************
//  Show Parameter - p
//******************************************************************************
void show_Parameter() {
  int tmpAdr = (int)readFloatSerial();
  SerPri("p");
  SerPri(tmpAdr);
  SerPri(";");
  SerPrln(readEEPROM(tmpAdr),4);				
}

//******************************************************************************
//  write Parameter - P
//******************************************************************************
void write_Parameter() 
{
  int tmpAdr = readFloatSerial();
  float tmpValue = readFloatSerial();
  writeEEPROM(tmpValue, tmpAdr);
  parameter[tmpAdr] = tmpValue;
  SerPri("=");
  SerPri(tmpAdr);
  SerPri(";");
  SerPrln(readEEPROM(tmpAdr),4);
}


//******************************************************************************
//  Show Control Values and Motor - m
//******************************************************************************
void show_Motors() 
{
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


//******************************************************************************
//  SHOW STABLE MODE PIDS - p
//******************************************************************************
void Show_Stable_PIDs() 
{ 
  char buf[20];
    SerPri(parameter[KP_QUAD_ROLL],4);
    comma();
    SerPri(parameter[KI_QUAD_ROLL],4);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_ROLL],4);
    comma();
    SerPri(parameter[KP_QUAD_NICK],4);
    comma();
    SerPri(parameter[KI_QUAD_NICK],4);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_NICK],4);
    comma();
    SerPri(parameter[KP_QUAD_YAW],4);
    comma();
    SerPri(parameter[KI_QUAD_YAW],4);
    comma();
    SerPri(parameter[STABLE_MODE_KP_RATE_YAW],4);
    comma();
    SerPri(parameter[Kp_ROLLNICK],4);
    comma();
    SerPrln(parameter[Ki_ROLLNICK],4);
}


//******************************************************************************
//  CALIBRATE THROTTLE - T
//******************************************************************************
void Calibrate_Pitch() 
{
  int tmp;
  int tmp_ch[4];
  
//  SerPrln("Move all Sticks to mid position!");
//  delay(1000);
//  SerPrln("2. ");
//  delay(1000);
//  SerPrln("1. ");
//  delay(1000);
//  // we read all spektrum bytes
//  while (Serial2.available()) {
//    reciverSpek.regByte(Serial2.read());
//  }
//  read_radio();
//  
//  SerPrln("Move Roll Stick to max/min");
//  delay(1000);
//  SerPrln("2. ");
//  delay(1000);
//  SerPrln("1. ");
//  delay(1000);
//  // we read all spektrum bytes
//  while (Serial2.available()) {
//    reciverSpek.regByte(Serial2.read());
//  }
//  read_radio();
//  
//  if(ch_roll < 1300 || ch_roll > 1600)
//    tmp_ch[0] = 0;
//  if(ch_nick < 1300 || ch_nick > 1600)
//    tmp_ch[0] = 1;
//  if(ch_yaw < 1300 || ch_yaw > 1600)
//    tmp_ch[0] = 2;
//  if(ch_pitch < 1300 || ch_pitch > 1600)
//    tmp_ch[0] = 3;
//   
//  SerPri("Roll Pos: ");  
//  SerPrln(tmp_ch[0]);
//  
//  SerPrln("Move Nick Stick to max/min");
//  delay(1000);
//  SerPrln("2. ");
//  delay(1000);
//  SerPrln("1. ");
//  delay(1000);
//  // we read all spektrum bytes
//  while (Serial2.available()) {
//    reciverSpek.regByte(Serial2.read());
//  }
//  read_radio();
//  
//  if(ch_roll < 1300 || ch_roll > 1600)
//    tmp_ch[1] = 0;
//  if(ch_nick < 1300 || ch_nick > 1600)
//    tmp_ch[1] = 1;
//  if(ch_yaw < 1300 || ch_yaw > 1600)
//    tmp_ch[1] = 2;
//  if(ch_pitch < 1300 || ch_pitch > 1600)
//    tmp_ch[1] = 3;
//    
//  SerPri("Nick Pos: ");
//  SerPrln(tmp_ch[1]);
//  
//  SerPrln("Move Yaw Stick to max/min");
//  delay(1000);
//  SerPrln("2. ");
//  delay(1000);
//  SerPrln("1. ");
//  delay(1000);
//  // we read all spektrum bytes
//  while (Serial2.available()) {
//    reciverSpek.regByte(Serial2.read());
//  }
//  read_radio();
//  
//  if(ch_roll < 1300 || ch_roll > 1600)
//    tmp_ch[2] = 0;
//  if(ch_nick < 1300 || ch_nick > 1600)
//    tmp_ch[2] = 1;
//  if(ch_yaw < 1300 || ch_yaw > 1600)
//    tmp_ch[2] = 2;
//  if(ch_pitch < 1300 || ch_pitch > 1600)
//    tmp_ch[2] = 3;
//    
//  SerPri("Yaw Pos: ");
//  SerPrln(tmp_ch[2]);
//  
//  SerPrln("Move Pitch Stick to max/min");
//  delay(1000);
//  SerPrln("2. ");
//  delay(1000);
//  SerPrln("1. ");
//  delay(1000);
//  // we read all spektrum bytes
//  while (Serial2.available()) {
//    reciverSpek.regByte(Serial2.read());
//  }
//  read_radio();
//  
//  if(ch_roll < 1300 || ch_roll > 1600)
//    tmp_ch[3] = 0;
//  if(ch_nick < 1300 || ch_nick > 1600)
//    tmp_ch[3] = 1;
//  if(ch_yaw < 1300 || ch_yaw > 1600)
//    tmp_ch[3] = 2;
//  if(ch_pitch < 1300 || ch_pitch > 1600)
//    tmp_ch[3] = 3;
//    
//  SerPri("Pitch Pos: ");
//  SerPrln(tmp_ch[3]);
//  
//  remote_assignment[0] = tmp_ch[0];
//  remote_assignment[1] = tmp_ch[1];
//  remote_assignment[2] = tmp_ch[2];
//  remote_assignment[3] = tmp_ch[3];
//  
//  tmp = (remote_assignment[0] * 1000) + (remote_assignment[1] * 100) + (remote_assignment[2] * 10) + remote_assignment[0];
//  
//  SerPri("Parameter [38]: ");
//  SerPrln(tmp);
//  
//  writeEEPROM(tmp, CHANNEL_ASSIGNMENT);

  tmp = 1200;
  SerPrln("Reading mid values! Pitch to min, Roll/Nick/Yaw mid. Starts in 3 seconds");
  delay(1000);
  SerPrln("2. ");
  delay(1000);
  SerPrln("1. ");
  delay(1000);
  // we read all spektrum bytes
  while (Serial2.available()) {
    reciverSpek.regByte(Serial2.read());
  }
  read_radio();
  SerPrln("Reading Pitch value");
  tmp = ch_pitch;
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
  SerPrln("Reading Roll value");
  tmp = ch_roll;
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
  SerPrln("Reading Nick value");
  tmp = ch_nick;
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
  SerPrln("Reading Yaw value, enter x to exit");
  tmp = ch_yaw;
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


//******************************************************************************
//  SHOW RECEIVER VALUES - r
//******************************************************************************
void Show_Receiver_Values() 
{ 
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



//******************************************************************************
//  SHOW SENSOR DATA - s
//******************************************************************************
void Show_Sensor_Data() 
{
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
    SerPri(read_adc(3));
    //SerPri(AN[3]);
    comma();
    SerPri(read_adc(4));
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

//******************************************************************************
//  RESET SETTINGS - #
//******************************************************************************
void Reset_Settings() 
{
  SerPrln("Reseting EEPROM to default!"); 
  defaultUserConfig();
  delay(200);
  SerPrln("Saving to EEPROM");
  writeUserConfig();
  SerPrln("Restart Loop");
  APM_Init();
}

//******************************************************************************
//  CALIBRATE ACCELEROMETER OFFSET - C
//******************************************************************************
void Calibrate_Accel_Offset() 
{
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


//******************************************************************************
//  RECEIVE STABLE MODE PID VALUES - P
//******************************************************************************
void Receive_Stable_PID() 
{
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
    
  SerPrln("Saving to EEPROM");
  writeUserConfig();
}

//******************************************************************************
//  RUN MOTORS 
//******************************************************************************
void RUN_Motors() 
{
  SerPrln("Hit numbers 1 - 4 for corresponding motor.");
  SerPrln("Motor will pulse slowly! (20% Throttle)");
  SerPrln("SAFETY!! Remove all propellers before doing stick movements");
  SerPrln();
  SerPrln("Exit from test by x + Enter");
  SerPrln();

  SerFlu();
  delay(50);
  while(1) 
  {
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


//******************************************************************************
// SUPPORTING FUNCTIONS BELOW 
//******************************************************************************

//******************************************************************************
//  PRINT COMMA OVER SERIAL
//******************************************************************************
void comma() 
{
  SerPri(',');
}

//******************************************************************************
//  PRINT TAB OVER SERIAL
//******************************************************************************
void tab() 
{
  SerPri("\t");
}

//******************************************************************************
//  PRINT COMMA, SPACE OVER SERIAL
//******************************************************************************
void cspc() 
{
  SerPri(", ");
}

//******************************************************************************
//  WAIT FOR SERIAL ENTER
//******************************************************************************
void WaitSerialEnter() 
{
  // Flush serials
  SerFlu();
  delay(50);
  while(1) 
  {
    if(SerAva() > 0)
    {
      break; 
    }
    delay(20);
  }
  delay(250);
  SerFlu();  
}

//******************************************************************************
//  PRINT COMMAND TO CLEAR REMOTE SERIAL CONSOLE
//******************************************************************************
void SerPriClScr() 
{  // Clears the remote serial console
  SerPrln(". . . .");
}

//******************************************************************************
//  READ FLOATING POINT VALUE OVER SERIAL
//******************************************************************************
float readFloatSerial() 
{
  byte index = 0;
  byte timeout = 0;
  char data[128] = "";

  do {
    if (SerAva() == 0) 
    {
      delay(10);
      timeout++;
    } 
    else 
    {
      data[index] = SerRea();
      timeout = 0;
      index++;
    }
  }  
  while ((data[constrain(index-1, 0, 128)] != ';') && (timeout < 5) && (index < 128));
  //Serial1.println(atof(data));
  return atof(data);
}

