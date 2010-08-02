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

#include "pid.h"
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vs32 summI[3];
vs32 diffOld[3];
extern vu16 parameter[0x190]; //parameter

/* calculate the correktion faktor ------------------------------------------*/
void calcPIDCorr(vs32 * PIDCorr, vs32 * copterAngle, vs32 * targetAngle)
{
	/*
	aus http://forum.mikrokopter.de/topic-post76057.html
	Abweichung = Soll - Ist;
	Summe_Abweichungen = Summe_Abweichungen + Abweichung;
	Differenz_zur_letzten_Abweichung = Abweichung - Abweichung_alt;

	Gegensteuern = Verstärkungsfaktor_Prop * Abweichung 
	+ Verstärkungsfaktor_Inte * Zeit * Summe_Abweichungen
	+ (Verstärkungsfaktor_Diff * Differenz_zur_letzten_Abweichung) / Zeit;

	Abweichung_alt = Abweichung;
	*/
	
	vs32 diff;
	vs32 diffLast;
	u8 i;
	
	for (i = 0; i < 3; i++)
	{
		// special for yaw!
		diff = (targetAngle[i] / 1000) - (copterAngle[i] / 1000);
		 
		// calc the shortest way for yaw
		if (i == 2)
		{
			if (diff > 18000)
			{
				diff = diff - 36000;
			}
			if (diff < -18000)
			{
				diff = diff + 36000;
			}
		}
		
		summI[i] += (diff / 10);
		if (diffOld[i] == 0) diffOld[i] = ( copterAngle[i] / 1000 );
		//diffLast = ( copterAngle[i] / 1000 ) - diffOld[i];
		diffLast = diffOld[i] - diff;
		
		PIDCorr[i] = ( parameter[23 + (i * 3)] * diff ) / 10000;
		PIDCorr[i] += ( parameter[24 + (i * 3)] * (summI[i] / 100) ) / 100000;
		PIDCorr[i] -= ( parameter[25 + (i * 3)] * diffLast  ) / 1000;
		
		//limit PID Corr
		//if (PIDCorr[i] > 50) PIDCorr[i] = 20;
		//if (PIDCorr[i] < -0) PIDCorr[i] = -20;
		
		diffOld[i] = diff;
		//diffOld[i] = copterAngle[i] / 1000;
	}
}




