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
    int tempThrottle = 0;
  
    // Commands from radio Rx
    if (CM3_RC.GetState() > 0)
    {
      // We apply the Radio calibration parameters (from configurator) except for throttle
      ch_roll = channel_filter(CM3_RC.InputCh(CH_ROLL) * ch_roll_slope + ch_roll_offset, ch_roll);
      ch_pitch = channel_filter(CM3_RC.InputCh(CH_PITCH) * ch_pitch_slope + ch_pitch_offset, ch_pitch);
      ch_yaw = channel_filter(CM3_RC.InputCh(CH_RUDDER) * ch_yaw_slope + ch_yaw_offset, ch_yaw);
      ch_aux = CM3_RC.InputCh(CH_5) * ch_aux_slope + ch_aux_offset;
      ch_aux2 = CM3_RC.InputCh(CH_6) * ch_aux2_slope + ch_aux2_offset;   //This is the MODE Channel in Configurator.
  //  Use this channel if you have a 7 or more Channel Radio.
  //  Can be used for PID tuning (see FUNCTIONS) or Camera 3 position tilt (pitch).
   
  
      // special checks for throttle
      tempThrottle = CM3_RC.InputCh(CH_THROTTLE);
    } 
    else // we have spektrum
    {
      // We apply the Radio calibration parameters (from configurator) except for throttle
      ch_roll = channel_filter(reciver.getRoll() * ch_roll_slope + ch_roll_offset, ch_roll);
      ch_pitch = channel_filter(reciver.getPitch() * ch_pitch_slope + ch_pitch_offset, ch_pitch);
      ch_yaw = channel_filter(reciver.getYaw() * ch_yaw_slope + ch_yaw_offset, ch_yaw);
      ch_aux = reciver.getAux1() * ch_aux_slope + ch_aux_offset;
      ch_aux2 = reciver.getAux2() * ch_aux2_slope + ch_aux2_offset;   //This is the MODE Channel in Configurator.
  //  Use this channel if you have a 7 or more Channel Radio.
  //  Can be used for PID tuning (see FUNCTIONS) or Camera 3 position tilt (pitch).
   
  
      // special checks for throttle
      tempThrottle = reciver.getThro();
    }
//    // throttle safety check
//    if( motorSafety == 1 ) {
//        if( motorArmed == 1 ) {
//            if( ch_throttle > MIN_THROTTLE + 100 ) { // if throttle is now over MIN..
//                // if throttle has increased suddenly, disarm motors 
//                if((tempThrottle - ch_throttle) > SAFETY_MAX_THROTTLE_INCREASE && safetyOff == 0) {     
//                    motorArmed = 0;
//                }else if (tempThrottle > SAFETY_HOVER_THROTTLE){ // if it hasn't increased too quickly turn off the safety
//                    motorSafety = 0;
//                    safetyOff = 1;                               // throttle is over Safety Hover Throttle we switch off Safety.
//                }else{  
//                    motorSafety = 0;
//                }
//            }
//        }
//    }else if( ch_throttle < MIN_THROTTLE + 100 ) { // Safety logic: hold throttle low for more than 1 second, safety comes on which stops sudden increases in throttle
//        Safety_counter++;
//        if( Safety_counter > SAFETY_DELAY ) {
//            motorSafety = 1;
//            Safety_counter = 0;
//        }
//     }else {
//       Safety_counter = 0;
//    }   
//    if(motorSafety == 0 && tempThrottle > SAFETY_HOVER_THROTTLE){  // throttle is over MIN so make sure to reset Safety_counter
//       Safety_counter = 0;
//       safetyOff = 1;                                              // throttle is over Safety Hover Throttle we switch off Safety.
//    }   
   
    // normal throttle filtering.  Note: Transmiter calibration not used on throttle
    ch_throttle = channel_filter(tempThrottle, ch_throttle);
        


      
    //if (flightMode == FM_STABLE_MODE)  // IN STABLE MODE we convert stick positions to absolute angles
      //{
      // In Stable mode stick position defines the desired angle in roll, pitch and yaw


        command_rx_roll = (ch_roll - roll_mid) / STICK_TO_ANGLE_FACTOR;       // Convert stick position to absolute angles
        command_rx_pitch = (ch_pitch - pitch_mid) / STICK_TO_ANGLE_FACTOR;



      // YAW
      if (abs(ch_yaw-yaw_mid)>6)   // Take into account a bit of "dead zone" on yaw
        {
        command_rx_yaw += (ch_yaw - yaw_mid) / YAW_STICK_TO_ANGLE_FACTOR;
        command_rx_yaw = Normalize_angle(command_rx_yaw);    // Normalize angle to [-180,180]
        }
      //}
    
    if (ch_yaw > 1400 && ch_yaw < 1600)
    {
      neutralYAWarm = 1;
    }
    // Motor arm logic
    if (ch_throttle < (MIN_THROTTLE + 100)) {
      control_yaw = 0;
      command_rx_yaw = ToDeg(yaw);
      
      // Arm motor output : Throttle down and full yaw left for more than 2 seconds
      if (ch_yaw < 1150 && motorArmed == 0 && neutralYAWarm == 1) {
        if (Arming_counter > ARM_DELAY){
          if(ch_throttle > 800) {
          motorArmed = 1;
          neutralYAWarm = 0;
          minThrottle = MIN_THROTTLE + 60;  // A minimun value for mantain a bit if throttle
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
          minThrottle = MIN_THROTTLE;
          neutralYAWarm = 0;
          safetyOff = 0;
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
}

