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
// Utilities for writing and reading from the EEPROM
float readEEPROM(int address) {
  union floatStore {
    byte floatByte[4];
    float floatVal;
  } floatOut;
  
  for (int i = 0; i < 4; i++) 
    floatOut.floatByte[i] = EEPROM.read((address * 4) + i);
  return floatOut.floatVal;
}

void writeEEPROM(float value, int address) {
  union floatStore {
    byte floatByte[4];
    float floatVal;
  } floatIn;
  
  floatIn.floatVal = value;
  for (int i = 0; i < 4; i++) 
    EEPROM.write((address * 4) + i, floatIn.floatByte[i]);
}

void readUserConfig() {

  for (int i = 0; i <= LAST_PARAMETER; i++)
  {
    parameter[i] = readEEPROM(i);
  }  
}

void writeUserConfig() {

  for (int i = 0; i <= LAST_PARAMETER; i++)
  {
    writeEEPROM(parameter[i], i);
  }  
  SerPrln("write OK");
}

