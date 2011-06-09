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

#include <CM3_SpektrumRC.h>

CM3_SpektrumRC reciver;
int RCPin = 39; //spektrum bind pin

void setup(){
  disableDebugPorts();  
  pinMode(RCPin, OUTPUT);   
  digitalWrite(RCPin, HIGH);
  
  Serial1.begin(115200); 
  Serial2.begin(115200); 
}

void loop()
{
  while (Serial2.available()) {
    reciver.regByte(Serial2.read());
  }
  
  delay(2); // simulate mainloop
  
    Serial1.print(reciver.getThro());
    Serial1.print("\t");
    Serial1.print(reciver.getRoll());
    Serial1.print("\t");
    Serial1.print(reciver.getPitch());
    Serial1.print("\t");
    Serial1.print(reciver.getYaw());
    Serial1.println();

  

  
}