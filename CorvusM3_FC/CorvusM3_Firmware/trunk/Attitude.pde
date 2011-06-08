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


/* ************************************************************ */
// STABLE MODE
// PI absolute angle control driving a P rate control
// Input : desired Roll, Pitch and Yaw absolute angles. Output : Motor commands
void Attitude_control_v3(int command_roll, int command_nick, int command_yaw)
{
  
  float stable_roll,stable_nick,stable_yaw;
  
  // ROLL CONTROL    
  err_roll = command_roll - ToDeg(roll);
  err_roll = constrain(err_roll,-25,25);  // to limit max roll command...
  
  roll_I += err_roll*G_Dt;
  roll_I = constrain(roll_I,-20,20);

  // PID absolute angle control
  stable_roll = parameter[KP_QUAD_ROLL] * err_roll + parameter[KI_QUAD_ROLL] * roll_I;
  
  // PD rate control (we use also the bias corrected gyro rates)
  err_roll = stable_roll - ToDeg(Omega[0]); // Omega[] is the raw gyro reading plus Omega_I, so it´s bias corrected
  control_roll = parameter[STABLE_MODE_KP_RATE_ROLL] * err_roll;
  control_roll = constrain(control_roll,-MAX_CONTROL_OUTPUT,MAX_CONTROL_OUTPUT);
  
  // NICK CONTROL
  err_nick = command_nick - ToDeg(nick);
  err_nick = constrain(err_nick,-25,25);  // to limit max pitch command...
  
  nick_I += err_nick*G_Dt;
  nick_I = constrain(nick_I,-20,20);
 
  // PID absolute angle control
  stable_nick = parameter[KP_QUAD_NICK] * err_nick + parameter[KI_QUAD_NICK] * nick_I;
  
  // P rate control (we use also the bias corrected gyro rates)
  err_nick = stable_nick - ToDeg(Omega[1]);
  control_nick = parameter[STABLE_MODE_KP_RATE_NICK] * err_nick;
  control_nick = constrain(control_nick,-MAX_CONTROL_OUTPUT,MAX_CONTROL_OUTPUT);
  
  // YAW CONTROL
  err_yaw = command_yaw - ToDeg(yaw);
  if (err_yaw > 180)    // Normalize to -180,180
    err_yaw -= 360;
  else if(err_yaw < -180)
    err_yaw += 360;
  err_yaw = constrain(err_yaw,-60,60);  // to limit max yaw command...
  
  yaw_I += err_yaw*G_Dt;
  yaw_I = constrain(yaw_I,-20,20);
 
  // PID absoulte angle control
  stable_yaw = parameter[KP_QUAD_YAW] * err_yaw + parameter[KI_QUAD_YAW] * yaw_I;
  // PD rate control (we use also the bias corrected gyro rates)
  err_yaw = stable_yaw - ToDeg(Omega[2]);
  control_yaw = parameter[STABLE_MODE_KP_RATE_YAW] * err_yaw;
  control_yaw = constrain(control_yaw,-MAX_CONTROL_OUTPUT,MAX_CONTROL_OUTPUT);
}


// ACRO MODE
void Rate_control_v2()
{
  static float previousRollRate, previousNickRate, previousYawRate;
  float currentRollRate, currentNickRate, currentYawRate;
  
  // ROLL CONTROL
  currentRollRate = ToDeg(Omega[0]);  // Omega[] is the raw gyro reading plus Omega_I, so it´s bias corrected
  err_roll = ((ch_roll - parameter[roll_mid]) * parameter[XMITFACTOR]) - currentRollRate;  
  roll_I += err_roll*G_Dt;
  roll_I = constrain(roll_I,-20,20);
  roll_D = (currentRollRate - previousRollRate)/G_Dt;
  previousRollRate = currentRollRate;  
  // PID control
  control_roll = parameter[Kp_RateRoll] * err_roll + parameter[Kd_RateRoll] * roll_D + parameter[Ki_RateRoll] * roll_I; 
  
  // NICK CONTROL
  currentNickRate = ToDeg(Omega[1]);  // Omega[] is the raw gyro reading plus Omega_I, so it´s bias corrected
  err_nick = ((ch_nick - parameter[nick_mid]) * parameter[XMITFACTOR]) - currentNickRate;
  
  nick_I += err_nick*G_Dt;
  nick_I = constrain(nick_I,-20,20);

  nick_D = (currentNickRate - previousNickRate)/G_Dt;
  previousNickRate = currentNickRate;
 
  // PID control
  control_nick = parameter[Kp_RateNick] * err_nick + parameter[Kd_RateNick] * nick_D + parameter[Ki_RateNick] * nick_I; 
  
  // YAW CONTROL
  currentYawRate = ToDeg(Omega[2]);  // Omega[] is the raw gyro reading plus Omega_I, so it´s bias corrected;
  err_yaw = ((ch_yaw - parameter[yaw_mid])* parameter[XMITFACTOR]) - currentYawRate;
  
  yaw_I += err_yaw*G_Dt;
  yaw_I = constrain(yaw_I,-20,20);

  yaw_D = (currentYawRate - previousYawRate)/G_Dt;
  previousYawRate = currentYawRate;
 
  // PID control
  control_yaw = parameter[Kp_RateYaw] * err_yaw + parameter[Kd_RateYaw] * yaw_D + parameter[Ki_RateYaw] * yaw_I; 
}


