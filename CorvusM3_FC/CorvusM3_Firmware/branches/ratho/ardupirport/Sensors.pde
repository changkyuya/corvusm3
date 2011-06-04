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
/* ******* ADC functions ********************* */
// Read all the ADC channles
void Read_adc_raw(void)
{
  //int temp;
  
  for (int i=0;i<7;i++)
    AN[i] = adc.Ch(sensors[i]);
}

// Returns an analog value with the offset
int read_adc(int select)
{
  if (SENSOR_SIGN[select]<0)
    return (AN_OFFSET[select]-AN[select]);
  else
    return (AN[select]-AN_OFFSET[select]);
}

void calibrateSensors(void) {
  int i;
  int j = 0;
  byte gyro;
  float aux_float[3];

  Read_adc_raw();     // Read sensors data
  delay(5);

  // Offset values for accels and gyros...
  AN_OFFSET[3] = acc_offset_x;                // Accel offset values are taken from external calibration (In Configurator)
  AN_OFFSET[4] = acc_offset_y;
  AN_OFFSET[5] = acc_offset_z;
  aux_float[0] = gyro_offset_roll;
  aux_float[1] = gyro_offset_pitch;
  aux_float[2] = gyro_offset_yaw;

  // Take the gyro offset values
  for(i=0;i<600;i++)
  {
    Read_adc_raw();   // Read sensors
    for(gyro = 0; gyro <= 2; gyro++)   
      aux_float[gyro] = aux_float[gyro] * 0.8 + AN[gyro] * 0.2;     // Filtering  

    delay(5);

    RunningLights(j);   // (in Functions.pde)
    // Runnings lights effect to let user know that we are taking mesurements
    if((i % 5) == 0) j++;
    if(j >= 3) j = 0;
  }
  
  // Switch off all ABC lights
  LightsOff();

  for(gyro = 0; gyro <= 2; gyro++)  
    AN_OFFSET[gyro] = aux_float[gyro];    // Update sensor OFFSETs from values read
}



void read_battery(void)
{
  battery_voltage = BATTERY_VOLTAGE(adc.Ch(sensors[6])) * .1 + battery_voltage * .9;
  
  if(battery_voltage < LOW_VOLTAGE)
  {	
    //SerPrln("Battery LOW!");
    battwarning = 1;
  }
  else 
  {
    battwarning = 0;
  }
}


