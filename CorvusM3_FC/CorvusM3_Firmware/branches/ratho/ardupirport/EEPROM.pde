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
// Utilities for writing and reading from the EEPROM
float readEEPROM(int address) {
  union floatStore {
    byte floatByte[4];
    float floatVal;
  } floatOut;
  
  for (int i = 0; i < 4; i++) 
    floatOut.floatByte[i] = EEPROM.read(address + i);
  return floatOut.floatVal;
}

void writeEEPROM(float value, int address) {
  union floatStore {
    byte floatByte[4];
    float floatVal;
  } floatIn;
  
  floatIn.floatVal = value;
  for (int i = 0; i < 4; i++) 
    EEPROM.write(address + i, floatIn.floatByte[i]);
}

void readUserConfig() {
//  eeprom_counter = readEEPROM(eeprom_counter_ADR);
//  eeprom_checker = readEEPROM(eeprom_checker_ADR);
  KP_QUAD_ROLL = readEEPROM(KP_QUAD_ROLL_ADR);
  KI_QUAD_ROLL = readEEPROM(KI_QUAD_ROLL_ADR);
  STABLE_MODE_KP_RATE_ROLL = readEEPROM(STABLE_MODE_KP_RATE_ROLL_ADR);
  KP_QUAD_PITCH = readEEPROM(KP_QUAD_PITCH_ADR);
  KI_QUAD_PITCH = readEEPROM(KI_QUAD_PITCH_ADR);
  STABLE_MODE_KP_RATE_PITCH = readEEPROM(STABLE_MODE_KP_RATE_PITCH_ADR);
  KP_QUAD_YAW = readEEPROM(KP_QUAD_YAW_ADR);
  KI_QUAD_YAW = readEEPROM(KI_QUAD_YAW_ADR);
  STABLE_MODE_KP_RATE_YAW = readEEPROM(STABLE_MODE_KP_RATE_YAW_ADR);
  STABLE_MODE_KP_RATE = readEEPROM(STABLE_MODE_KP_RATE_ADR);          // NOT USED NOW
  acc_offset_x = readEEPROM(acc_offset_x_ADR);
  acc_offset_y = readEEPROM(acc_offset_y_ADR);
  acc_offset_z = readEEPROM(acc_offset_z_ADR);
  gyro_offset_roll = readEEPROM(gyro_offset_roll_ADR);
  gyro_offset_pitch = readEEPROM(gyro_offset_pitch_ADR);
  gyro_offset_yaw = readEEPROM(gyro_offset_yaw_ADR);
  Kp_ROLLPITCH = readEEPROM(Kp_ROLLPITCH_ADR);
  Ki_ROLLPITCH = readEEPROM(Ki_ROLLPITCH_ADR);
  Kp_YAW = readEEPROM(Kp_YAW_ADR);
  Ki_YAW = readEEPROM(Ki_YAW_ADR);
  Kp_RateRoll = readEEPROM(KP_RATEROLL_ADR);
  Ki_RateRoll = readEEPROM(KI_RATEROLL_ADR);
  Kd_RateRoll = readEEPROM(KD_RATEROLL_ADR);
  Kp_RatePitch = readEEPROM(KP_RATEPITCH_ADR);
  Ki_RatePitch = readEEPROM(KI_RATEPITCH_ADR);
  Kd_RatePitch = readEEPROM(KD_RATEPITCH_ADR);
  Kp_RateYaw = readEEPROM(KP_RATEYAW_ADR);
  Ki_RateYaw = readEEPROM(KI_RATEYAW_ADR);
  Kd_RateYaw = readEEPROM(KD_RATEYAW_ADR);
  xmitFactor = readEEPROM(XMITFACTOR_ADR);
  roll_mid = readEEPROM(CHROLL_MID);
  pitch_mid = readEEPROM(CHPITCH_MID);
  yaw_mid = readEEPROM(CHYAW_MID);
  ch_roll_slope = readEEPROM(ch_roll_slope_ADR);
  ch_pitch_slope = readEEPROM(ch_pitch_slope_ADR);
  ch_throttle_slope = readEEPROM(ch_throttle_slope_ADR);
  ch_yaw_slope = readEEPROM(ch_yaw_slope_ADR);
  ch_aux_slope = readEEPROM(ch_aux_slope_ADR);
  ch_aux2_slope = readEEPROM(ch_aux2_slope_ADR);
  ch_roll_offset = readEEPROM(ch_roll_offset_ADR);
  ch_pitch_offset = readEEPROM(ch_pitch_offset_ADR);
  ch_throttle_offset = readEEPROM(ch_throttle_offset_ADR);
  ch_yaw_offset = readEEPROM(ch_yaw_offset_ADR);
  ch_aux_offset = readEEPROM(ch_aux_offset_ADR);
  ch_aux2_offset = readEEPROM(ch_aux2_offset_ADR);
  MIN_THROTTLE = readEEPROM(MIN_THROTTLE_ADR);    
}

void writeUserConfig() {
//  eeprom_counter = readEEPROM(eeprom_counter_ADR);
//  if(eeprom_counter > 0) eeprom_counter = 0;
//  eeprom_counter++;
//  writeEEPROM(eeprom_counter, eeprom_counter_ADR);
  writeEEPROM(KP_QUAD_ROLL, KP_QUAD_ROLL_ADR);
  writeEEPROM(KI_QUAD_ROLL, KI_QUAD_ROLL_ADR);
  writeEEPROM(STABLE_MODE_KP_RATE_ROLL, STABLE_MODE_KP_RATE_ROLL_ADR);
  writeEEPROM(KP_QUAD_PITCH, KP_QUAD_PITCH_ADR);
  writeEEPROM(KI_QUAD_PITCH, KI_QUAD_PITCH_ADR);
  writeEEPROM(STABLE_MODE_KP_RATE_PITCH, STABLE_MODE_KP_RATE_PITCH_ADR);
  writeEEPROM(KP_QUAD_YAW, KP_QUAD_YAW_ADR);
  writeEEPROM(KI_QUAD_YAW, KI_QUAD_YAW_ADR);
  writeEEPROM(STABLE_MODE_KP_RATE_YAW, STABLE_MODE_KP_RATE_YAW_ADR);
  writeEEPROM(STABLE_MODE_KP_RATE, STABLE_MODE_KP_RATE_ADR);  // NOT USED NOW
  writeEEPROM(acc_offset_x, acc_offset_x_ADR);
  writeEEPROM(acc_offset_y, acc_offset_y_ADR);
  writeEEPROM(acc_offset_z, acc_offset_z_ADR);
  writeEEPROM(gyro_offset_roll, gyro_offset_roll_ADR);
  writeEEPROM(gyro_offset_pitch, gyro_offset_pitch_ADR);
  writeEEPROM(gyro_offset_yaw, gyro_offset_yaw_ADR);
  writeEEPROM(Kp_ROLLPITCH, Kp_ROLLPITCH_ADR);
  writeEEPROM(Ki_ROLLPITCH, Ki_ROLLPITCH_ADR);
  writeEEPROM(Kp_YAW, Kp_YAW_ADR);
  writeEEPROM(Ki_YAW, Ki_YAW_ADR);
  writeEEPROM(Kp_RateRoll, KP_RATEROLL_ADR);
  writeEEPROM(Ki_RateRoll, KI_RATEROLL_ADR);
  writeEEPROM(Kd_RateRoll, KD_RATEROLL_ADR);
  writeEEPROM(Kp_RatePitch, KP_RATEPITCH_ADR);
  writeEEPROM(Ki_RatePitch, KI_RATEPITCH_ADR);
  writeEEPROM(Kd_RatePitch, KD_RATEPITCH_ADR);
  writeEEPROM(Kp_RateYaw, KP_RATEYAW_ADR);
  writeEEPROM(Ki_RateYaw, KI_RATEYAW_ADR);
  writeEEPROM(Kd_RateYaw, KD_RATEYAW_ADR);
  writeEEPROM(xmitFactor, XMITFACTOR_ADR);
  writeEEPROM(roll_mid, CHROLL_MID);
  writeEEPROM(pitch_mid, CHPITCH_MID);
  writeEEPROM(yaw_mid, CHYAW_MID);
  writeEEPROM(ch_roll_slope, ch_roll_slope_ADR);
  writeEEPROM(ch_pitch_slope, ch_pitch_slope_ADR);
  writeEEPROM(ch_throttle_slope, ch_throttle_slope_ADR);
  writeEEPROM(ch_yaw_slope, ch_yaw_slope_ADR);
  writeEEPROM(ch_aux_slope, ch_aux_slope_ADR);
  writeEEPROM(ch_aux2_slope, ch_aux2_slope_ADR);
  writeEEPROM(ch_roll_offset, ch_roll_offset_ADR);
  writeEEPROM(ch_pitch_offset, ch_pitch_offset_ADR);
  writeEEPROM(ch_throttle_offset, ch_throttle_offset_ADR);
  writeEEPROM(ch_yaw_offset, ch_yaw_offset_ADR);
  writeEEPROM(ch_aux_offset, ch_aux_offset_ADR);
  writeEEPROM(ch_aux2_offset, ch_aux2_offset_ADR);
  writeEEPROM(MIN_THROTTLE, MIN_THROTTLE_ADR);    
}

