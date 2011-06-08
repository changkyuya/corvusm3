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


void read_radio()
{
    int tempPitch = 0;
  
    // Commands from radio Rx
    if (reciverPPM.GetState() > 0)
    {
      // We apply the Radio calibration parameters (from configurator) except for throttle
      ch_pitch = channel_filter(reciverPPM.InputCh(CH_PITCH), ch_pitch);
      ch_roll = channel_filter(reciverPPM.InputCh(CH_ROLL), ch_roll);
      ch_nick = channel_filter(reciverPPM.InputCh(CH_NICK), ch_nick);
      ch_yaw = channel_filter(reciverPPM.InputCh(CH_YAW), ch_yaw);
      ch_aux1 = reciverPPM.InputCh(CH_AUX1);
      ch_aux2 = reciverPPM.InputCh(CH_AUX2);
      ch_aux3 = reciverPPM.InputCh(CH_AUX3);
      ch_aux4 = reciverPPM.InputCh(CH_AUX4);
    } 
    else // we have spektrum
    {
      // We apply the Radio calibration parameters (from configurator) except for throttle
      ch_pitch = channel_filter(reciverSpek.InputCh(CH_PITCH), ch_pitch);
      ch_roll = channel_filter(reciverSpek.InputCh(CH_ROLL), ch_roll);
      ch_nick = channel_filter(reciverSpek.InputCh(CH_NICK), ch_nick);
      ch_yaw = channel_filter(reciverSpek.InputCh(CH_YAW), ch_yaw);
      ch_aux1 = reciverSpek.InputCh(CH_AUX1);
      ch_aux2 = reciverSpek.InputCh(CH_AUX2);
      ch_aux3 = reciverSpek.InputCh(CH_AUX3);
      ch_aux4 = reciverSpek.InputCh(CH_AUX4);
    }
  
    //if (flightMode == FM_STABLE_MODE)  // IN STABLE MODE we convert stick positions to absolute angles
      //{
      // In Stable mode stick position defines the desired angle in roll, pitch and yaw


        command_rx_roll = (ch_roll - parameter[roll_mid]) / parameter[STICK_TO_ANGLE_FACTOR];       // Convert stick position to absolute angles
        command_rx_nick = (ch_nick - parameter[nick_mid]) / parameter[STICK_TO_ANGLE_FACTOR];



      // YAW
      if (abs(ch_yaw-yaw_mid)>6)   // Take into account a bit of "dead zone" on yaw
        {
        command_rx_yaw += (ch_yaw - parameter[yaw_mid]) / parameter[YAW_STICK_TO_ANGLE_FACTOR];
        command_rx_yaw = Normalize_angle(command_rx_yaw);    // Normalize angle to [-180,180]
        }
      //}
    
    if (ch_yaw > 1400 && ch_yaw < 1600)
    {
      neutralYAWarm = 1;
    }
    
    // Motor arm logic
    if (ch_pitch < (parameter[MIN_PITCH] + 100)) {
      control_yaw = 0;
      command_rx_yaw = ToDeg(yaw);
      
      // Arm motor output : Throttle down and full yaw left for more than 2 seconds
      if (ch_yaw < 1150 && motorArmed == 0 && neutralYAWarm == 1) {
        if (Arming_counter > ARM_DELAY){
          if(ch_pitch > 800) {
          motorArmed = 1;
          neutralYAWarm = 0;
          minPitch = parameter[MIN_PITCH] + 60;  // A minimun value for mantain a bit if throttle
          }
        }
        else
          Arming_counter++;
      }
      else
        Arming_counter=0;
        
      // To Disarm motor output : Throttle down and full yaw left for more than 2 seconds
      if (ch_yaw < 1150 && motorArmed == 1 && neutralYAWarm == 1) {
        if (Disarming_counter > DISARM_DELAY){
          motorArmed = 0;
          minPitch = parameter[MIN_PITCH];
          neutralYAWarm = 0;
        }
        else
          Disarming_counter++;
      }
      else
        Disarming_counter=0;
    }
    else{
      Arming_counter=0;
      Disarming_counter=0;
    }
    
    // calibrate sensor
    if (ch_yaw < 1150 && motorArmed == 0 && neutralYAWarm == 1 && ch_pitch > 1850)
   {
     calibrateSensors();
     neutralYAWarm = 0;
   } 
}

