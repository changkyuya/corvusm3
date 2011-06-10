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

#include "wirish.h"
#include "CM3_ADC.h"



CM3_ADC::CM3_ADC()
{
}



void CM3_ADC::Init()
{

pinMode(15,INPUT_ANALOG); //Gyro Z
pinMode(16,INPUT_ANALOG); //Gyro Y
pinMode(17,INPUT_ANALOG); //Gyro X
pinMode(18,INPUT_ANALOG); //ACC Z
pinMode(19,INPUT_ANALOG); //ACC Y
pinMode(20,INPUT_ANALOG); //ACC X
pinMode(27,INPUT_ANALOG); //V Bat
analog_port[0]=17; 
analog_port[1]=16;
analog_port[2]=15;
analog_port[3]=20;
analog_port[4]=19;
analog_port[5]=18;
analog_port[6]=27;

}

int CM3_ADC::Ch(unsigned char ch_num)
{

int ret;
ret=analogRead(analog_port[ch_num]);
return (ret);

}
