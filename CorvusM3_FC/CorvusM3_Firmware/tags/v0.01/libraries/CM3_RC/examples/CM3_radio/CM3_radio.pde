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

#include <CM3_RC.h> 
int RCPin = 39;

void setup()
{
  disableDebugPorts();  
  pinMode(RCPin, OUTPUT);   
  digitalWrite(RCPin, HIGH);
  
	CM3_RC.Init();	 // CM3 Radio initialization
	Serial1.begin(115200);
	Serial1.println("CM3 RC library test");
	delay(1000);
}

void loop()
{
	// New radio frame? (we could use also if((millis()- timer) > 20)
	if (CM3_RC.GetState() == 1){
		Serial1.print("CH:");
		for(int i = 0; i < 8; i++){
			Serial1.print(CM3_RC.InputCh(i));	// Print channel values
			Serial1.print(",");
			CM3_RC.OutputCh(i, CM3_RC.InputCh(i)); // Copy input to Servos
		}
		Serial1.println();
	}
}