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

#include "motorhal.h"
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0x190]; //parameter


/* send Motorcommands to BLMC over UART3 ------------------------------------*/
void sendMotor(volatile char * motor)
{
	motor[0] = 0xF5;
	u8 i;
	for (i = 1; i < 5; i++)
	{
		motor[i] = (motor[i] < parameter[PARA_MIN_GAS]) ? parameter[PARA_MIN_GAS] : motor[i];
	}
	print_uart3(motor);
}


/* stop all motors ----------------------------------------------------------*/
void stopAllMotors(volatile char * motor)
{
	motor[0] = 0xF5;
	motor[1] = 0x00;
	motor[2] = 0x00;
	motor[3] = 0x00;
	motor[4] = 0x00;
	print_uart3(motor);
}


