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

// Flash those A,B,C LEDs on IMU Board
// 
// Function: FullBlink(int, int);
//           int 1 = 
void FullBlink(int count, int blinkdelay) {
  for(int i = 0; i <= count; i++) {
    digitalWrite(LED_Red, HIGH);
    delay(blinkdelay);
    digitalWrite(LED_Red, LOW);
    delay(blinkdelay);
  }
}





// Funtion to normalize an angle in degrees to -180,180 degrees
float Normalize_angle(float angle)
{
  if (angle > 180)         
    return (angle - 360.0);
  else if (angle < -180)
    return (angle + 360.0);
  else
    return(angle);
}
// Maximun slope filter for radio inputs... (limit max differences between readings)
int channel_filter(int ch, int ch_old)
{
  int diff_ch_old;

  if (ch_old==0)      // ch_old not initialized
    return(ch);
  diff_ch_old = ch - ch_old;      // Difference with old reading
  if (diff_ch_old < 0)
  {
    if (diff_ch_old <- 60)
      return(ch_old - 60);        // We limit the max difference between readings
  }
  else
  {
    if (diff_ch_old > 60)    
      return(ch_old + 60);
  }
  return((ch + ch_old) >> 1);   // Small filtering

}



// Faster and smaller replacement for contrain() function
int limitRange(int data, int minLimit, int maxLimit) {
  if (data < minLimit) return minLimit;
  else if (data > maxLimit) return maxLimit;
  else return data;
}




void pid_Tuning() {
  
  //we read channel 6 for pid tuning
  if (parameter[TUNING_PARA] > 0)
  {
    //split parameter in 2 values
    int first = (int) parameter[TUNING_PARA] / 100;
    int second = (int) parameter[TUNING_PARA] - (first * 100);
    
    if (first > 0)
    {
      parameter[first] = map(ch_aux2, 1000, 2000, parameter[TUNING_MIN], parameter[TUNING_MIN]);
    }
    
    if (second > 0)
    {
      parameter[second] = map(ch_aux2, 1000, 2000, parameter[TUNING_MIN], parameter[TUNING_MIN]);
    }
  } 
}
