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


// Send output commands to ESC´s
void motor_output()
{
  int throttle;
  byte throttle_mode=0;
 
  throttle = ch_throttle;
 
  if ((throttle_mode==0)&&(ch_throttle < (MIN_THROTTLE + 100)))  // If throttle is low we disable yaw (neccesary to arm/disarm motors safely)
    control_yaw = 0; 

  // Copter mix
  if (motorArmed == 1) {   

          // minThrottle= 0 to MIN_THROTTLE = remote value from EEPR
          rightMotor = constrain(throttle - control_roll + control_yaw, minThrottle, 2000);
          leftMotor = constrain(throttle + control_roll + control_yaw, minThrottle, 2000);
          frontMotor = constrain(throttle + control_pitch - control_yaw, minThrottle, 2000);
          backMotor = constrain(throttle - control_pitch - control_yaw, minThrottle, 2000);


  } else {    // MOTORS DISARMED


      //rightMotor = MIN_THROTTLE;
      rightMotor = MIN_THROTTLE;
      leftMotor = MIN_THROTTLE;
      frontMotor = MIN_THROTTLE;
      backMotor = MIN_THROTTLE;

    // Reset_I_Terms();
    roll_I = 0;     // reset I terms of PID controls
    pitch_I = 0;
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
  char sr = map(rightMotor, MIN_THROTTLE, 2000, 0, 200);
  char sl = map(leftMotor, MIN_THROTTLE, 2000, 0, 200);
  char sf = map(frontMotor, MIN_THROTTLE, 2000, 0, 200);
  char sb = map(backMotor, MIN_THROTTLE, 2000, 0, 200);
  
  char start = 0xF5;
  Serial3.write(start);
  Serial3.write(sf);
  Serial3.write(sb);
  Serial3.write(sr);
  Serial3.write(sl);
  
//  
//  Serial1.print(sf);
//  Serial1.print(sb);
//  Serial1.print(sr);
//  Serial1.println(sl);
}



