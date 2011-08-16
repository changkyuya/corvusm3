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
// Motor Stop
//******************************************************************************
void motor_stop()
{  
  char start = 0xF5;
  char stop = 0x00;
  Serial3.write(start);
  Serial3.write(stop);
  Serial3.write(stop);
  Serial3.write(stop);
  Serial3.write(stop);
}

//******************************************************************************
// Send output commands to ESC´s
//******************************************************************************
void motor_output()
{
  int pitch;
  byte pitch_mode=0;
 
  pitch = ch_pitch;
 
  if ((pitch_mode==0)&&(ch_pitch < (parameter[MIN_PITCH] + 100)))  // If throttle is low we disable yaw (neccesary to arm/disarm motors safely)
    control_yaw = 0; 

  // Copter mix
  if (motorArmed == 1) 
  {   
    switch ((int)parameter[MOT_CONFIG])
    {
      case 0:
      case 2:
        // For + mode - front pointing towards front motor
        // For X mode - front pointing towards front motor 
        rightMotor = constrain(pitch - control_roll + control_yaw, minPitch, 2000);
        leftMotor = constrain(pitch + control_roll + control_yaw, minPitch, 2000);
        frontMotor = constrain(pitch + control_nick - control_yaw, minPitch, 2000);
        backMotor = constrain(pitch - control_nick - control_yaw, minPitch, 2000);
        break;
      case 1:
        // For X mode - front between front and right motor 
        rightMotor = constrain(pitch - control_roll + control_nick + control_yaw, minPitch, 2000); // Right motor
        leftMotor = constrain(pitch + control_roll - control_nick + control_yaw, minPitch, 2000);  // Left motor
        frontMotor = constrain(pitch + control_roll + control_nick - control_yaw, minPitch, 2000); // Front motor
        backMotor = constrain(pitch - control_roll - control_nick - control_yaw, minPitch, 2000);  // Back motor
        break;
    }
    /// TEST START
//    int tmpRightMotor = pitch - control_roll + control_yaw;
//    int tmpLeftMotor = pitch + control_roll + control_yaw;
//    int tmpFrontMotor = pitch + control_nick - control_yaw;
//    int tmpBackMotor = pitch - control_nick - control_yaw;
//    //limit max Motor output
//    rightMotor = ((tmpRightMotor - rightMotor) > parameter[MAX_MOTOR_OUTPUT]) ? (rightMotor + parameter[MAX_MOTOR_OUTPUT]) : tmpRightMotor;
//    leftMotor = ((tmpLeftMotor - leftMotor) > parameter[MAX_MOTOR_OUTPUT]) ? (leftMotor + parameter[MAX_MOTOR_OUTPUT]) : tmpLeftMotor;
//    frontMotor = ((tmpFrontMotor - frontMotor) > parameter[MAX_MOTOR_OUTPUT]) ? (frontMotor + parameter[MAX_MOTOR_OUTPUT]) : tmpFrontMotor;
//    backMotor = ((tmpBackMotor - backMotor) > parameter[MAX_MOTOR_OUTPUT]) ? (backMotor + parameter[MAX_MOTOR_OUTPUT]) : tmpBackMotor;
//
//    // DTC Dynamic thrust control
//    int minMotor;
//    int maxMotor;
//   
//    //search min max Motor values
//    minMotor = (rightMotor < minMotor) ? rightMotor : minMotor;
//    maxMotor = (rightMotor > maxMotor) ? rightMotor : maxMotor;
//    minMotor = (leftMotor < minMotor) ? leftMotor : minMotor;
//    maxMotor = (leftMotor > maxMotor) ? leftMotor : maxMotor;
//    minMotor = (frontMotor < minMotor) ? frontMotor : minMotor;
//    maxMotor = (frontMotor > maxMotor) ? frontMotor : maxMotor;
//    minMotor = (backMotor < minMotor) ? backMotor : minMotor;
//    maxMotor = (backMotor > maxMotor) ? backMotor : maxMotor;
//   
//    //correkt all motors
//    minMotor = minMotor - parameter[MIN_GAS];
//    maxMotor = maxMotor - 2000;
//    
//    if (maxMotor > 0)
//    {
//      rightMotor = ((rightMotor - maxMotor) < parameter[MIN_GAS]) ? parameter[MIN_GAS] : (rightMotor - maxMotor);
//      leftMotor = ((leftMotor - maxMotor) < parameter[MIN_GAS]) ? parameter[MIN_GAS] : (leftMotor - maxMotor);
//      frontMotor = ((frontMotor - maxMotor) < parameter[MIN_GAS]) ? parameter[MIN_GAS] : (frontMotor - maxMotor);
//      backMotor = ((backMotor - maxMotor) < parameter[MIN_GAS]) ? parameter[MIN_GAS] : (backMotor - maxMotor);
//    }
//    else if (minMotor < 0)
//    {
//      rightMotor = ((rightMotor - minMotor) > 2000) ? 2000 : (rightMotor - minMotor);
//      leftMotor = ((leftMotor - minMotor) > 2000) ? 2000 : (leftMotor - minMotor);
//      frontMotor = ((frontMotor - minMotor) > 2000) ? 2000 : (frontMotor - minMotor);
//      backMotor = ((backMotor - minMotor) > 2000) ? 2000 : (backMotor - minMotor);
//    }
  /// TEST END
  } 
  else  // MOTORS DISARMED
  {   
    //rightMotor = MIN_THROTTLE;
    rightMotor = parameter[MIN_PITCH];
    leftMotor = parameter[MIN_PITCH];
    frontMotor = parameter[MIN_PITCH];
    backMotor = parameter[MIN_PITCH];

    // Reset_I_Terms();
    roll_I = 0;     // reset I terms of PID controls
    nick_I = 0;
    yaw_I = 0; 
    
    // Initialize yaw command to actual yaw when throttle is down...
    command_rx_yaw = ToDeg(yaw);
  }

  // Send commands to motors
  //    CM3_RC.OutputCh(0, rightMotor);   // Right motor
  //    CM3_RC.OutputCh(1, leftMotor);    // Left motor
  //    CM3_RC.OutputCh(2, frontMotor);   // Front motor
  //    CM3_RC.OutputCh(3, backMotor);    // Back motor   
  
  //we use 10 as mingas
  char sr = map(rightMotor, parameter[MIN_PITCH], 2000, 0, parameter[MAX_MOTOR_OUTPUT]);
  char sl = map(leftMotor, parameter[MIN_PITCH], 2000, 0, parameter[MAX_MOTOR_OUTPUT]);
  char sf = map(frontMotor, parameter[MIN_PITCH], 2000, 0, parameter[MAX_MOTOR_OUTPUT]);
  char sb = map(backMotor, parameter[MIN_PITCH], 2000, 0, parameter[MAX_MOTOR_OUTPUT]);
  
  char start = 0xF5;
  Serial3.write(start);
  Serial3.write(sf);
  Serial3.write(sb);
  Serial3.write(sr);
  Serial3.write(sl);
}



