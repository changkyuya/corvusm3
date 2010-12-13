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

#include "filterdcm.h"
#include "parameter.h"
#include "sensor.h"
#include <math.h>
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 gyroZero[3]; // sensor
extern vu16 parameter[0x190]; //parameter
extern vs32 compassAngle; //statemachine
extern vs32 targetAngle[3]; //statemachine

vs32 Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
vs32 Gyro_Vector[3]= {0,0,0};//Store the gyros rutn rate in a vector
vs32 Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
vs32 Omega_P[3]= {0,0,0};//Omega Proportional correction
vs32 Omega_I[3]= {0,0,0};//Omega Integrator
vs32 Omega[3]= {0,0,0};
vs32 errorRollPitch[3]= {0,0,0};
//vs32 errorYaw[3]= {0,0,0};



vs32 DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
}; 
vs32 Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here


vs32 Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};


/* init filter --------------------------------------------------------------*/
void initFilterDCM(vs32 * gyroAngle, vs32 * copterAngle)
{
	
	// setup compass only if HW_Setup is 4
	if (getParameter(PARA_HW) & PARA_HW_COMP)
	{
		initCompass();
	}
	zeroGyro();
	setAngleFilterDCM(gyroAngle, copterAngle);
}


/* set gyro start angle -----------------------------------------------------*/
void setAngleFilterDCM(vs32 * gyroAngle, vs32 * copterAngle)
{
	DCM_Matrix[0][0]=1;
	DCM_Matrix[0][1]=0;
	DCM_Matrix[0][2]=0;
	DCM_Matrix[1][0]=0;
	DCM_Matrix[1][1]=1;
	DCM_Matrix[1][2]=0;
	DCM_Matrix[2][0]=0;
	DCM_Matrix[2][1]=0;
	DCM_Matrix[2][2]=1;

	for (int i = 0; i < 3; i++)
	{
		Omega_Vector[i]= 0; //Corrected Gyro_Vector data
		Omega_P[i]= 0;//Omega Proportional correction
		Omega_I[i]= 0;//Omega Integrator
		Omega[i]= 0;
		errorRollPitch[i]= 0;
	}

	
	copterAngle[X] = gyroAngle[X] = 18000000;
	copterAngle[Y] = gyroAngle[Y] = 18000000;
	copterAngle[Z] = gyroAngle[Z] = targetAngle[Z] = 18000000;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n", gyroAngle[X], gyroAngle[Y], gyroAngle[Z]);
	print_uart1(x);
}




/* mix Gyro and ACC for Copter-Angel ----------------------------------------*/
void getCopterAnglesFilterDCM(vs32 * copterAngle)
{

	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	getRawValues(gyroRawValues, accRawValues);
	
	// we read compass only if HW_Bit is set
	if (getParameter(PARA_HW) & PARA_HW_COMP)
	{
		getCompassAngle();
	}
	
	// IMU DCM Algorithm
    /* Original code from Jordi Muñoz and William Premerlani                  */
		/*
	gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100);
	ToRad *0.01745329252  // *pi/180
	ToDeg *57.2957795131  // *180/pi
	*/

	//Gyro_Vector[0] = Gyro_Scaled_X(read_adc(0)); //gyro x roll
	// ADC Values mit 3 nachkomma - DCM mit 4 Nachkomma
	
	Gyro_Vector[0] = (vs32) ((gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 10) * 0.01745329252); // * parameter[PARA_GYRO_X_90];
	Gyro_Vector[1] = (vs32) ((gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 10) * 0.01745329252); // * parameter[PARA_GYRO_X_90];
	Gyro_Vector[2] = (vs32) ((gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 10) * 0.01745329252); // * parameter[PARA_GYRO_X_90];

	Accel_Vector[0] = -accRawValues[Y]; // acc Y
	Accel_Vector[1] = -accRawValues[X]; // acc X
	Accel_Vector[2] = accRawValues[Z]; // acc z

	//char xx [80];
	//sprintf(xx,"gv:%f:%f:%f\r\n",Gyro_Vector[0],Gyro_Vector[1],Gyro_Vector[2]);
	//print_uart1(xx);
	
	Matrix_update(); 
    Normalize();
    //Drift_correction();
    Euler_angles(copterAngle);
    // *****************

}






/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterDCM(vu16 * receiverChannel)
{
	// 180 = neutral
	// max is 110 to 250° - this are 70° for 500 points
	targetAngle[X] = 11000000 + ((14000) * (receiverChannel[ROLL] - 1000));
	targetAngle[Y] = 11000000 + ((14000) * (receiverChannel[NICK] - 1000));
	// only use yaw if stick is more than 5 points out of center and pitch not min
	if (receiverChannel[YAW] < 1495 || receiverChannel[YAW] > 1505)
	{
		if(receiverChannel[PITCH] > 1005)
		{
			targetAngle[Z] -= (receiverChannel[YAW] - 1500) * parameter[PARA_STICK_FACTOR];
		}
	}
	
	if (targetAngle[Z] >= 36000000) 
	{
		targetAngle[Z] -= 36000000;
	}
	if (targetAngle[Z] < 0) 
	{
		targetAngle[Z] += 36000000;
	}
}

// ***********************************************************************************************

//Computes the dot product of two vectors
s32 Vector_Dot_Product(vs32 vector1[3],vs32 vector2[3])
{
	vs32 op=0;

	for(int c=0; c<3; c++)
	{
		op+=vector1[c]*vector2[c] / 10000;
	}

	return op; 
}

//Computes the cross product of two vectors
void Vector_Cross_Product(vs32 vectorOut[3], vs32 v1[3],vs32 v2[3])
{
	vectorOut[0] = (v1[1]*v2[2]) / 10000 - (v1[2]*v2[1]) / 10000;
	vectorOut[1] = (v1[2]*v2[0]) / 10000 - (v1[0]*v2[2]) / 10000;
	vectorOut[2] = (v1[0]*v2[1]) / 10000 - (v1[1]*v2[0]) / 10000;
}

//Multiply the vector by a scalar. 
void Vector_Scale(vs32 vectorOut[3],vs32 vectorIn[3], vs32 scale2)
{
	for(int c=0; c<3; c++)
	{
		vectorOut[c] = (vectorIn[c] * scale2) / 10000; 
	}
}

void Vector_Add(vs32 vectorOut[3],vs32 vectorIn1[3], vs32 vectorIn2[3])
{
	for(int c=0; c<3; c++)
	{
		vectorOut[c] = vectorIn1[c] + vectorIn2[c];
	}
}

/**************************************************/
//Multiply two 3x3 matrixs. This function developed by Jordi can be easily adapted to multiple n*n matrix's. (Pero me da flojera!). 
void Matrix_Multiply(vs32 a[3][3], vs32 b[3][3],vs32 mat[3][3])
{
	vs32 op[3]; 
	for(int x=0; x<3; x++)
	{
		for(int y=0; y<3; y++)
		{
			for(int w=0; w<3; w++)
			{
				op[w] = (a[x][w] * b[w][y]) / 10000;
			} 
			mat[x][y] = 0;
			mat[x][y] = op[0] + op[1] + op[2];

		}
	}
}


/**************************************************/
void Normalize(void)
{
	vs32 error=0;
	vs32 temporary[3][3];
	vs32 renorm=0;

	error = -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19

	Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
	Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19

	Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
	Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19

	Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20

	renorm = (30000 - Vector_Dot_Product(&temporary[0][0],&temporary[0][0])) / 2; //eq.21
	Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);

	renorm = (30000 - Vector_Dot_Product(&temporary[1][0],&temporary[1][0])) / 2; //eq.21
	Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);

	renorm = (30000 - Vector_Dot_Product(&temporary[2][0],&temporary[2][0])) / 2; //eq.21
	Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

/**************************************************/
void Drift_correction(void)
{
	//Compensation the Roll, Pitch and Yaw drift. 
	//float mag_heading_x;
	//float mag_heading_y;
	//float errorCourse;
	//static float Scaled_Omega_P[3];
	static vs32 Scaled_Omega_I[3];
	//float Accel_magnitude;
	vs32 Accel_weight;

	//*****Roll and Pitch***************

	// Calculate the magnitude of the accelerometer vector
	//Accel_magnitude = sqrt(Accel_Vector[0]*Accel_Vector[0] + Accel_Vector[1]*Accel_Vector[1] + Accel_Vector[2]*Accel_Vector[2]);
	//Accel_magnitude = Accel_magnitude / GRAVITY; // Scale to gravity.
	// Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
	//Accel_weight = constrain(1 - 2*abs(1 - Accel_magnitude),0,1);
	Accel_weight = parameter[PARA_ACC_FORCE] / 10000.0;

	Vector_Cross_Product(&errorRollPitch[0],&Accel_Vector[0],&DCM_Matrix[2][0]); //adjust the ground of reference
	Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);

	Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
	Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);

	//*****YAW***************
	// We make the gyro YAW drift correction based on compass magnetic heading
	/*	
	mag_heading_x = cos(MAG_Heading);  // Optimización, usar valores precalculados...
	mag_heading_y = sin(MAG_Heading);
	errorCourse= (DCM_Matrix[0][0]*mag_heading_y) - (DCM_Matrix[1][0]*mag_heading_x);  //Calculating YAW error
	Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

	Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);
	Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.

	Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);
	Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I
	*/
}
/**************************************************/

void Matrix_update()
{


	Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);//adding integrator
	Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]);//adding proportional

	//char xx [80];
	//sprintf(xx,"go:%f:%f:%f\r\n",Omega_Vector[0],Omega_Vector[1],Omega_Vector[2]);
	//print_uart1(xx);

	//Accel_adjust();//adjusting centrifugal acceleration. // Not used for quadcopter

	Update_Matrix[0][0] = 0;
	Update_Matrix[0][1] = -MAINLOOPTIME * Omega_Vector[2];//-z
	Update_Matrix[0][2] =  MAINLOOPTIME * Omega_Vector[1];//y
	Update_Matrix[1][0] =  MAINLOOPTIME * Omega_Vector[2];//z
	Update_Matrix[1][1] = 0;
	Update_Matrix[1][2] = -MAINLOOPTIME * Omega_Vector[0];//-x
	Update_Matrix[2][0] = -MAINLOOPTIME * Omega_Vector[1];//-y
	Update_Matrix[2][1] =  MAINLOOPTIME * Omega_Vector[0];//x
	Update_Matrix[2][2] = 0;

	Matrix_Multiply(DCM_Matrix,Update_Matrix,Temporary_Matrix); //a*b=c

	//char xx [80];
	//print_uart1("G");
	
	for(int x=0; x<3; x++)  //Matrix Addition (update)
	{
		for(int y=0; y<3; y++)
		{
			DCM_Matrix[x][y] += Temporary_Matrix[x][y];
			
			//sprintf(xx,"%f-",DCM_Matrix[x][y]);
			//print_uart1(xx);
		} 
	}
	
	//print_uart1("\r\n");
}

void Euler_angles(vs32 * copterAngle)
{
// Euler angles from DCM matrix
    //5 nachkommastellen (intern mit 4 in DCM)
	copterAngle[Y] = (vs32) ((asin(DCM_Matrix[2][0] / 10000.0) * 5729578 ) + 18000000);
    copterAngle[X] = (vs32) ((atan2(DCM_Matrix[2][1],DCM_Matrix[2][2]) * 5729578) + 18000000);
    copterAngle[Z] = (vs32) ((atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]) * 5729578) + 18000000);
	
	//char xx [80];
	//sprintf(xx,"x:%f:%d\r\n",DCM_Matrix[2][0],copterAngle[Y]);
	//print_uart1(xx);
 }