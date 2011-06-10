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

#include <CM3_ADC.h> 

unsigned long timer;

CM3_ADC adc;

void setup()
{  
  adc.Init();   // CM3 ADC initialization
  Serial1.begin(115200);
  Serial1.println("CM3 ADC library test");
  delay(1000);
  timer = millis();
}

void loop()
{
  int ch;
  
  if((millis()- timer) > 20)
    {
    timer = millis();
    for (ch=0;ch<7;ch++)
      {
      Serial1.print(adc.Ch(ch));
      Serial1.print(",");
      }
    Serial1.println();
    }
}

