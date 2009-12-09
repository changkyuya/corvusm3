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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_H
#define __COMM_H

#include "main.h"

/* Define --------------------------------------------------------------------*/
#define ERROR	0
#define OK		1
#define DEFAULT 2
#define HELP	3

/* Function prototypes -------------------------------------------------------*/
void getComm(void);
void doComm(void);
void dolComm(void);
void dodComm(void);
void dorComm(void);
void dosComm(void);
void dopComm(void);
void dofComm(void);
void docComm(void);
void doRCComm(void);
u16 readInt(u8);
void send(u8);
void print_para (u8 para, u16 value);


#endif /* __COMM_H */