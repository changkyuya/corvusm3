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
using System.IO.Ports;
using System.Windows.Forms;

namespace CorvusM3
{
    public class Parameter
    {
        SerialPort port;

        public Parameter(SerialPort obj)
        {
            port = obj;
            
        }
        public int [] parameter = new int[400];
        public int maxParameter = 19;

        public void fillParameter(string para)
        {
            int counter = Convert.ToInt32(para.Substring(5, 2));
            parameter[counter] = Convert.ToInt32(para.Substring(9));

        }
        public void saveParameter()
        {
            Cursor.Current = Cursors.WaitCursor;
            for (int i = 0; i <= maxParameter; i++)
            {
                port.Write("s" + i.ToString("00") + ":" + parameter[i].ToString() + "\r\n");

                    Application.DoEvents();
                    //System.Threading.Thread.Sleep(100);   

            }
            Cursor.Current = Cursors.Default;
        }
        public void flashParameter()
        {
            for (int i = 0; i <= maxParameter; i++)
            {
                port.Write("f" + i.ToString("00") + ":" + parameter[i].ToString() + "\r\n");
            }
        }

        [CategoryAttribute("Basis"), DisplayName("Parameter Set 00"), DescriptionAttribute("Im Moment nur 0 berücksichtigt. Dieser Parameter gibt das Startset an. Besser gesagt die Verschiebung (wenn 100 dann wäre der Parameter 01 auf 101 zu finden - also Setting 2)")]
        public int ParaSet
        {
            set 
            { 
                parameter[0] = value;
                port.Write("s00:" + value.ToString() + "\r\n");
            }
            get { return parameter[0]; }
        }
        [CategoryAttribute("Basis"), DisplayName("Debugoutput 01"), DescriptionAttribute("0...Debug aus, \r\n1...Empfängersignale, \r\n2...ADC Werte (6x Sensoren, Spannungsmessung), \r\n4...Sensoren (Gyro,ACC), \r\n8...RC und Motoren")]
        public int Debugoutput
        {
            set
            {
                parameter[1] = value;
                port.Write("s01:" + value.ToString() + "\r\n");
            }
            get { return parameter[1]; }
        }
        [CategoryAttribute("Basis"), DisplayName("HAL 02"), DescriptionAttribute("0...RC Empfänger, \r\n1...PC Steuerung")]
        public int HAL
        {
            set
            {
                parameter[2] = value;
                port.Write("s02:" + value.ToString() + "\r\n");
            }
            get { return parameter[2]; }
        }
        [CategoryAttribute("Basis"), DisplayName("Akku 03"), DescriptionAttribute("Einstellung in Punkte vom 12bit ADC. Spannungsteiler (10K auf +, 2K2 auf GND = Spannung/5,55). 1 Volt entspricht 223,64 Punte. Default = 2236 = 10 Volt.")]
        public int Akku
        {
            set
            {
                parameter[3] = value;
                port.Write("s03:" + value.ToString() + "\r\n");
            }
            get { return parameter[3]; }
        }
        [CategoryAttribute("Basis"), DisplayName("SAL 04"), DescriptionAttribute("0...HH Regelung, \r\n1...ACC Regelung")]
        public int SAL
        {
            set
            {
                parameter[4] = value;
                port.Write("s04:" + value.ToString() + "\r\n");
            }
            get { return parameter[4]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-X-ZERO 05"), DescriptionAttribute("Horizontalwert X-Achse für ACC")]
        public int ACCXZERO
        {
            set
            {
                parameter[5] = value;
                port.Write("s05:" + value.ToString() + "\r\n");
            }
            get { return parameter[5]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-Y-ZERO 06"), DescriptionAttribute("Horizontalwert Y-Achse für ACC")]
        public int ACCYZERO
        {
            set
            {
                parameter[6] = value;
                port.Write("s06:" + value.ToString() + "\r\n");
            }
            get { return parameter[6]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC-Z-ZERO 07"), DescriptionAttribute("Horizontalwert Z-Achse für ACC")]
        public int ACCZZERO
        {
            set
            {
                parameter[7] = value;
                port.Write("s07:" + value.ToString() + "\r\n");
            }
            get { return parameter[7]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("SMOOTH_ACC 08 (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHACC
        {
            set
            {
                parameter[8] = value;
                port.Write("s08:" + value.ToString() + "\r\n");
            }
            get { return parameter[8]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("SMOOTH_GYRO 09 (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHGYRO
        {
            set
            {
                parameter[9] = value;
                port.Write("s09:" + value.ToString() + "\r\n");
            }
            get { return parameter[9]; }
        }
        [CategoryAttribute("Basis"), DisplayName("SMOOTH_RC 10 (0-100)"), DescriptionAttribute("Glättung zum letzten Wert (0 wäre immer letzter Wert)")]
        public int SMOOTHRC
        {
            set
            {
                parameter[10] = value;
                port.Write("s10:" + value.ToString() + "\r\n");
            }
            get { return parameter[10]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_X_90 11"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROX90
        {
            set
            {
                parameter[11] = value;
                port.Write("s11:" + value.ToString() + "\r\n");
            }
            get { return parameter[11]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_Y_90 12"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROY90
        {
            set
            {
                parameter[12] = value;
                port.Write("s12:" + value.ToString() + "\r\n");
            }
            get { return parameter[12]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_Z_90 13"), DescriptionAttribute("Gyro Anpassung für 90 Grad Drehung")]
        public int GYROZ90
        {
            set
            {
                parameter[13] = value;
                port.Write("s13:" + value.ToString() + "\r\n");
            }
            get { return parameter[13]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("ACC_FORCE 14 (0-10000)"), DescriptionAttribute("Wirkung vom ACC auf den Gyro (0 ist nur Gyro)")]
        public int ACCFORCE
        {
            set
            {
                parameter[14] = value;
                port.Write("s14:" + value.ToString() + "\r\n");
            }
            get { return parameter[14]; }
        }
        [CategoryAttribute("Sensoren"), DisplayName("GYRO_CORR 15 (0-10000)"), DescriptionAttribute("Berichtigung Gyro-Zero über ACC")]
        public int GYROCORR
        {
            set
            {
                parameter[15] = value;
                port.Write("s15:" + value.ToString() + "\r\n");
            }
            get { return parameter[15]; }
        }
        [CategoryAttribute("RC Channels"), DisplayName("Servo 0 CH"), DescriptionAttribute("Kanla für Servo 0 Ausgang")]
        public int SERVO0
        {
            set
            {
                parameter[16] = value;
                port.Write("s16:" + value.ToString() + "\r\n");
            }
            get { return parameter[16]; }
        }
        [CategoryAttribute("RC Channels"), DisplayName("Servo 1 CH"), DescriptionAttribute("Kanla für Servo 1 Ausgang")]
        public int SERVO1
        {
            set
            {
                parameter[17] = value;
                port.Write("s17:" + value.ToString() + "\r\n");
            }
            get { return parameter[17]; }
        }
        [CategoryAttribute("RC Channels"), DisplayName("Servo 2 CH"), DescriptionAttribute("Kanla für Servo 2 Ausgang")]
        public int SERVO2
        {
            set
            {
                parameter[18] = value;
                port.Write("s18:" + value.ToString() + "\r\n");
            }
            get { return parameter[18]; }
        }
        [CategoryAttribute("RC Channels"), DisplayName("Servo 3 CH"), DescriptionAttribute("Kanla für Servo 3 Ausgang")]
        public int SERVO3
        {
            set
            {
                parameter[19] = value;
                port.Write("s19:" + value.ToString() + "\r\n");
            }
            get { return parameter[19]; }
        }
    }
}
