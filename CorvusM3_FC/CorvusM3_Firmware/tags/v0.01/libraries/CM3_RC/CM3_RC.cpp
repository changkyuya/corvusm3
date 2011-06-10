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

#include "CM3_RC.h"
#include "WProgram.h"

#define PPMSUM_IN 6 ///TR: Change PA8
volatile unsigned char radio_status_rc=0;
volatile unsigned char sync=0;
static int analogOutPin[4];
volatile unsigned int currentChannel = 0;
static unsigned int last = 0;


// ***PPM SUM SIGNAL***
static uint8_t rcChannel[12] = {PPM_ORDER};
volatile uint16_t rcValue[12] = {1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500}; // interval [1000;2000]


void rxInt(void) {
volatile unsigned int now;
volatile unsigned int diff;

  now = micros();
  diff = now - last;
  last = now;
  // we found a gap - sync
  if(diff>3000  &&  diff<21000) 
      {
      currentChannel = 0;
	  radio_status_rc=0;
	  rcValue[currentChannel] = diff;
      currentChannel++;
	  sync=1;
      }
  else 
  	 // if sync we read
     if (sync==1) 
     {
          rcValue[currentChannel] = diff;
          currentChannel++;
		  if(diff<=2200 && diff>=900)radio_status_rc++;
	}
	 if (currentChannel>8)
	 {	
		 sync=0;
		 radio_status_rc=0;
	 }
}


// Constructors ////////////////////////////////////////////////////////////////
CM3_RC_Class::CM3_RC_Class()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void CM3_RC_Class::Init(void)
{
  analogOutPin[MOTORID1] = 12; ///TR: Change // Connected to PPM output1 // TIMER2
  analogOutPin[MOTORID2] = 13; ///TR: Change // Connected to PPM output2 // TIMER2
  analogOutPin[MOTORID3] = 14; ///TR: Change //  Connected to PPM output3 // TIMER1
  analogOutPin[MOTORID4] = 15; ///TR: Change //  Connected to PPM output4 // TIMER1

  pinMode(analogOutPin[MOTORID1],PWM);
  pinMode(analogOutPin[MOTORID2],PWM);
  pinMode(analogOutPin[MOTORID3],PWM);
  pinMode(analogOutPin[MOTORID4],PWM);

  pinMode(PPMSUM_IN,INPUT);
  attachInterrupt(PPMSUM_IN, rxInt, RISING);
}

void CM3_RC_Class::OutputCh(unsigned char ch, uint16_t pwm)
{
  analogWrite(analogOutPin[ch], pwm);
}

uint16_t CM3_RC_Class::InputCh(unsigned char ch)
{
  uint16_t data;
  data = rcValue[rcChannel[ch]]; 
  return data; // We return the value correctly copied when the IRQ's where disabled
}

unsigned char CM3_RC_Class::GetState(void)
{
  return(radio_status_rc);
}

