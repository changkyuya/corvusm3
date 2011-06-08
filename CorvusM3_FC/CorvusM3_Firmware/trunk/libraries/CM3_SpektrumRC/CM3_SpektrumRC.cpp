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



#include "CM3_SpektrumRC.h"
#include "WProgram.h"

volatile unsigned char radio_status_spek = 0;
static int rcChannelSpek[12] = {PPM_ORDER};


void CM3_SpektrumRC::regByte(int byte){
    
      time = micros();
      
      spektrumByte[spektrumByteCount] = byte;
      
	//Serial1.write(spektrumByte[spektrumByteCount]);
      
	  // we have new frame
      if ((time - time_old)>5000) {
          if (spektrumByteCount >= 31) {
              spektrumByteCount = 0; 
				
			  //first 2 bytes are unknown
              for (int i = 1; i < 16; i++)
              {
				//2. frame ... first 2 bytes are unknown
                if (i != 8)
				{
					int temp = (spektrumByte[i*2] * 256) + spektrumByte[(i*2)+1];
					if (temp > 0 && temp < 1024) { channel[2] = temp + 988;} // nick
					if (temp > 1024 && temp < 2048) {channel[0] = temp - 12;} // pitch
					if (temp > 2048 && temp < 3072) {channel[1] = temp - 1036;} // roll
					if (temp > 3072 && temp < 4096) {channel[3] = temp - 2060;}  //yaw
					if (temp > 4096 && temp < 5120) {channel[4] = temp - 3084;}
					if (temp > 5120 && temp < 6144) {channel[5] = temp - 4108;}
					if (temp > 7168 && temp < 8192) {channel[6] = temp - 6156;}
					if (temp > 8192 && temp < 9216) {channel[7] = temp - 7180;}
				}
			}
              radio_status_spek = 0;
          }  
      }
      
      spektrumByteCount++;
      radio_status_spek++;      
      time_old = time;
}

int CM3_SpektrumRC::InputCh(unsigned char ch)
{
  int data;
  data = channel[rcChannelSpek[ch]]; 
  return data; 
}

unsigned char CM3_SpektrumRC::GetState(void)
{
  if (micros() - time_old > 50000)
  {
	radio_status_spek = 0;
  }
  return(radio_status_spek);
}