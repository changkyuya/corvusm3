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



#ifndef CM3_SpektrumRC_H_
#define CM3_SpektrumRC_H_

#define PPM_ORDER         ROLL,NICK,PITCH,YAW,AUX1,AUX2,AUX3,AUX4,AUX5,AUX6,AUX7,AUX8 

// alias for RC
#define ROLL 3
#define NICK 2
#define YAW 1
#define PITCH 0
#define AUX1 4
#define AUX2 5
#define AUX3 6
#define AUX4 7
#define AUX5 8
#define AUX6 9
#define AUX7 10
#define AUX8 11


class CM3_SpektrumRC{
public:
	void regByte(int byte);
	int InputCh(unsigned char ch);
	unsigned char GetState();
private:
	int spektrumByte[32];
  int spektrumByteCount;
  int channel[8];
  unsigned int time;
  unsigned int time_old;
};


#endif /* CM3_SpektrumRC_H_ */
