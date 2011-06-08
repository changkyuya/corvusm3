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

#ifndef CM3_RC_h
#define CM3_RC_h

#define NUM_CHANNELS 12
#define MIN_PULSEWIDTH 900
#define MAX_PULSEWIDTH 2100

#define MOTORID1 0		
#define MOTORID2 1		
#define MOTORID3 2		
#define MOTORID4 3		

#define PPM_ORDER         ROLL,NICK,PITCH,YAW,AUX1,AUX2,AUX3,AUX4,AUX5,AUX6,AUX7,AUX8 

// alias for RC
#define ROLL 1
#define NICK 2
#define YAW 4
#define PITCH 3
#define AUX1 5
#define AUX2 6
#define AUX3 7
#define AUX4 8
#define AUX5 9
#define AUX6 10
#define AUX7 11
#define AUX8 12


#include <inttypes.h>

class CM3_RC_Class
{
  private:
  public:
	CM3_RC_Class();
	void Init();
	void OutputCh(unsigned char ch, uint16_t pwm);
	uint16_t InputCh(unsigned char ch);
	unsigned char GetState();
};


#endif
