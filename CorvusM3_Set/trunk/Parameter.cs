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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace CorvusM3
{
    public class Parameter
    {
        public int [] parameter = new int[255];
        public int maxParameter = 15;

        [CategoryAttribute("Basis"), DisplayName("Parameter Set"), DescriptionAttribute("Im Moment nur 0 berücksichtigt. Dieser Parameter gibt das Startset an. Besser gesagt die Verschiebung (wenn 100 dann wäre der Parameter 01 auf 101 zu finden - also Setting 2)")]
        public int ParaSet
        {
            set { parameter[0] = value; }
            get { return parameter[0]; }
        }
        [CategoryAttribute("Basis"), DisplayName("Debugoutput"), DescriptionAttribute("0...Debug aus, \r\n1...Empfängersignale, \r\n2...ADC Werte (6x Sensoren, Spannungsmessung), \r\n4...Sensoren (Gyro,ACC), \r\n8...RC und Motoren")]
        public int Debugoutput
        {
            set { parameter[1] = value; }
            get { return parameter[1]; }
        }
        [CategoryAttribute("Basis"), DisplayName("HAL"), DescriptionAttribute("0...RC Empfänger, \r\n1...PC Steuerung")]
        public int HAL
        {
            set { parameter[2] = value; }
            get { return parameter[2]; }
        }
        [CategoryAttribute("Basis"), DisplayName("Akku"), DescriptionAttribute("Einstellung in Punkte vom 12bit ADC. Spannungsteiler (10K auf +, 2K2 auf GND = Spannung/5,55). 1 Volt entspricht 223,64 Punte. Default = 2236 = 10 Volt.")]
        public int Akku
        {
            set { parameter[3] = value; }
            get { return parameter[3]; }
        }
        [CategoryAttribute("Basis"), DisplayName("SAL"), DescriptionAttribute("0...HH Regelung, \r\n1...ACC Regelung")]
        public int SAL
        {
            set { parameter[4] = value; }
            get { return parameter[4]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-X-ZERO"), DescriptionAttribute("Horizontalwert X-Achse für ACC")]
        public int ACCXZERO
        {
            set { parameter[5] = value; }
            get { return parameter[5]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-Y-ZERO"), DescriptionAttribute("Horizontalwert Y-Achse für ACC")]
        public int ACCYZERO
        {
            set { parameter[6] = value; }
            get { return parameter[6]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-Z-ZERO"), DescriptionAttribute("Horizontalwert Z-Achse für ACC")]
        public int ACCZZERO
        {
            set { parameter[7] = value; }
            get { return parameter[7]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("SMOOTH_ACC (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHACC
        {
            set { parameter[8] = value; }
            get { return parameter[8]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("SMOOTH_GYRO (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHGYRO
        {
            set { parameter[9] = value; }
            get { return parameter[9]; }
        }
        [CategoryAttribute("Basis"), DisplayName("SMOOTH_RC (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHRC
        {
            set { parameter[10] = value; }
            get { return parameter[10]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_X_90"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROX90
        {
            set { parameter[11] = value; }
            get { return parameter[11]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_Y_90"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROY90
        {
            set { parameter[12] = value; }
            get { return parameter[12]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_Z_90"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROZ90
        {
            set { parameter[13] = value; }
            get { return parameter[13]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC_FORCE (0-10000)"), DescriptionAttribute("Wirkung vom ACC auf den Gyro (0 ist nur Gyro)")]
        public int ACCFORCE
        {
            set { parameter[14] = value; }
            get { return parameter[14]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_CORR (0-10000)"), DescriptionAttribute("Berichtigung Gyro-Zero über ACC")]
        public int GYROCORR
        {
            set { parameter[15] = value; }
            get { return parameter[15]; }
        }
    }
}
