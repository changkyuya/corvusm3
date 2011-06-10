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
using System.Windows.Forms;
using System.ComponentModel;
using System.IO;

namespace CorvusM3
{
    class Parameter
    {
        public float [] parameter = new float[38];
        private Serial serialComm;
        private PropertyGrid propGrid;

        public Parameter()
        {
        }

        public Serial SerialComm
        {
            set 
            {
                serialComm = value;
                serialComm.NewDataEvent += new Serial.NewDataHandler(serialComm_NewDataEvent);
            }
        }

        public PropertyGrid PropGrid
        {
            set {
                propGrid = value;
            }
        }
        

        void serialComm_NewDataEvent(object sender, NewDataEventArgs e)
        {
            if (e.Dataline.Substring(0, 1) == "p")
            { 
                string [] data = e.Dataline.Split(';');
                data[0] = data[0].Substring(1);
                parameter[Convert.ToInt16(data[0])] = float.Parse(data[1].Replace('.', ','));
                serialComm.setParameter(Convert.ToInt16(data[0]), float.Parse(data[1].Replace('.',',')));
                
            }
        }



        [CategoryAttribute("PID Stable"), DisplayName("ROLL P [1]"), DescriptionAttribute("Default 4.0")]
        public float KP_QUAD_ROLL
        {
            set { parameter[1] = value; serialComm.setParameter(1, value); }
            get { return parameter[1]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("ROLL I [2]"), DescriptionAttribute("Default 0.15")]
        public float KI_QUAD_ROLL
        {
            set { parameter[2] = value; serialComm.setParameter(2, value); }
            get { return parameter[2]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("ROLL RATE P [3]"), DescriptionAttribute("Default 1.2")]
        public float STABLE_MODE_KP_RATE_ROLL
        {
            set { parameter[3] = value; serialComm.setParameter(3, value); }
            get { return parameter[3]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("NICK P [4]"), DescriptionAttribute("Default 4.0")]
        public float KP_QUAD_NICK
        {
            set { parameter[4] = value; serialComm.setParameter(4, value); }
            get { return parameter[4]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("NICK I [5]"), DescriptionAttribute("Default 0.15")]
        public float KI_QUAD_NICK
        {
            set { parameter[5] = value; serialComm.setParameter(5, value); }
            get { return parameter[5]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("NICK RATE P [6]"), DescriptionAttribute("Default 1.2")]
        public float STABLE_MODE_KP_RATE_NICK
        {
            set { parameter[6] = value; serialComm.setParameter(6, value); }
            get { return parameter[6]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("YAW P [7]"), DescriptionAttribute("Default 4.0")]
        public float KP_QUAD_YAW
        {
            set { parameter[7] = value; serialComm.setParameter(7, value); }
            get { return parameter[7]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("YAW I [8]"), DescriptionAttribute("Default 0.15")]
        public float KI_QUAD_YAW
        {
            set { parameter[8] = value; serialComm.setParameter(8, value); }
            get { return parameter[8]; }
        }
        [CategoryAttribute("PID Stable"), DisplayName("YAW RATE P [9]"), DescriptionAttribute("Default 1.2")]
        public float STABLE_MODE_KP_RATE_YAW
        {
            set { parameter[9] = value; serialComm.setParameter(8, value); }
            get { return parameter[9]; }
        }


        [CategoryAttribute("Sensors"), DisplayName("ACC offset X [11]"), DescriptionAttribute("Default 2057")]
        public float acc_offset_x
        {
            set { parameter[11] = value; serialComm.setParameter(10, value); }
            get { return parameter[11]; }
        }
        [CategoryAttribute("Sensors"), DisplayName("ACC offset Y [12]"), DescriptionAttribute("Default 2044")]
        public float acc_offset_y
        {
            set { parameter[12] = value; serialComm.setParameter(11, value); }
            get { return parameter[12]; }
        }
        [CategoryAttribute("Sensors"), DisplayName("ACC offset Z [13]"), DescriptionAttribute("Default 2052")]
        public float acc_offset_z
        {
            set { parameter[13] = value; serialComm.setParameter(12, value); }
            get { return parameter[13]; }
        }


        [CategoryAttribute("DCM"), DisplayName("ROLLNICK P [14]"), DescriptionAttribute("Default 0.14 (*0.01)")]
        public float Kp_ROLLNICK
        {
            set { parameter[14] = value; serialComm.setParameter(13, value); }
            get { return parameter[14]; }
        }
        [CategoryAttribute("DCM"), DisplayName("ROLLNICK I [15]"), DescriptionAttribute("Default 0.15 (*0.000001)")]
        public float Ki_ROLLNICK
        {
            set { parameter[15] = value; serialComm.setParameter(14, value); }
            get { return parameter[15]; }
        }
        [CategoryAttribute("DCM"), DisplayName("YAW P [16]"), DescriptionAttribute("Default 1.0")]
        public float Kp_YAW
        {
            set { parameter[16] = value; serialComm.setParameter(15, value); }
            get { return parameter[16]; }
        }
        [CategoryAttribute("DCM"), DisplayName("YAW I [17]"), DescriptionAttribute("Default 0.2 (*0.0001)")]
        public float Ki_YAW
        {
            set { parameter[17] = value; serialComm.setParameter(16, value); }
            get { return parameter[17]; }
        }



        [CategoryAttribute("PID HH"), DisplayName("\tStick Factor HH [18]"), DescriptionAttribute("Default 0.32")]
        public float XMITFACTOR
        {
            set { parameter[18] = value; serialComm.setParameter(17, value); }
            get { return parameter[18]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("ROLL P [19]"), DescriptionAttribute("Default 1.95")]
        public float Kp_RateRoll
        {
            set { parameter[19] = value; serialComm.setParameter(18, value); }
            get { return parameter[19]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("ROLL I [20]"), DescriptionAttribute("Default 0.0")]
        public float Ki_RateRoll
        {
            set { parameter[20] = value; serialComm.setParameter(19, value); }
            get { return parameter[20]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("ROLL D [21]"), DescriptionAttribute("Default 0.0")]
        public float Kd_RateRoll
        {
            set { parameter[21] = value; serialComm.setParameter(20, value); }
            get { return parameter[21]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("NICK P [22]"), DescriptionAttribute("Default 1.95")]
        public float Kp_RateNick
        {
            set { parameter[22] = value; serialComm.setParameter(21, value); }
            get { return parameter[22]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("NICK I [23]"), DescriptionAttribute("Default 0.0")]
        public float Ki_RateNick
        {
            set { parameter[23] = value; serialComm.setParameter(23, value); }
            get { return parameter[23]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("NICK D [24]"), DescriptionAttribute("Default 0.0")]
        public float Kd_RateNick
        {
            set { parameter[24] = value; serialComm.setParameter(24, value); }
            get { return parameter[24]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("YAW P [25]"), DescriptionAttribute("Default 3.2")]
        public float Kp_RateYaw
        {
            set { parameter[25] = value; serialComm.setParameter(25, value); }
            get { return parameter[25]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("YAW I [26]"), DescriptionAttribute("Default 0.0")]
        public float Ki_RateYaw
        {
            set { parameter[26] = value; serialComm.setParameter(26, value); }
            get { return parameter[26]; }
        }
        [CategoryAttribute("PID HH"), DisplayName("YAW D [27]"), DescriptionAttribute("Default 0.0")]
        public float Kd_RateYaw
        {
            set { parameter[27] = value; serialComm.setParameter(27, value); }
            get { return parameter[27]; }
        }


        [CategoryAttribute("Remote"), DisplayName("\tCenter ROLL [28]"), DescriptionAttribute("Default 1500")]
        public float CHROLL_MID
        {
            set { parameter[28] = value; serialComm.setParameter(28, value); }
            get { return parameter[28]; }
        }
        [CategoryAttribute("Remote"), DisplayName("\tCenter NICK [29]"), DescriptionAttribute("Default 1500")]
        public float CHNICK_MID
        {
            set { parameter[29] = value; serialComm.setParameter(29, value); }
            get { return parameter[29]; }
        }
        [CategoryAttribute("Remote"), DisplayName("\tCenter YAW [30]"), DescriptionAttribute("Default 1500")]
        public float CHYAW_MID
        {
            set { parameter[30] = value; serialComm.setParameter(30, value); }
            get { return parameter[30]; }
        }
        [CategoryAttribute("Remote"), DisplayName("\tPITCH min [31]"), DescriptionAttribute("Default 1100")]
        public float MIN_PITCH
        {
            set { parameter[31] = value; serialComm.setParameter(31, value); }
            get { return parameter[31]; }
        }

        [CategoryAttribute("Basis"), DisplayName("Batterie warning Volt [32]"), DescriptionAttribute("Default 10")]
        public float BAT_WARNING
        {
            set { parameter[32] = value; serialComm.setParameter(32, value); }
            get { return parameter[32]; }
        }

        [CategoryAttribute("Remote"), DisplayName("\tStick to Angle Divisor ROLL/NICK [33]"), DescriptionAttribute("Default 5.0")]
        public float STICK_TO_ANGLE_FACTOR
        {
            set { parameter[33] = value; serialComm.setParameter(33, value); }
            get { return parameter[33]; }
        }
        [CategoryAttribute("Remote"), DisplayName("\tStick to Angle Divisor YAW [34]"), DescriptionAttribute("Default 1150.0")]
        public float YAW_STICK_TO_ANGLE_FACTOR
        {
            set { parameter[34] = value; serialComm.setParameter(34, value); }
            get { return parameter[34]; }
        }
        [CategoryAttribute("Remote"), DisplayName("PID Tuning VALUE [35]"), DescriptionAttribute("Default 0, can use 2 Parameter 1123 = Parameter 11 and 23")]
        public float PID_TUNING_VALUE
        {
            set { parameter[35] = value; serialComm.setParameter(35, value); }
            get { return parameter[35]; }
        }
        [CategoryAttribute("Remote"), DisplayName("PID Tuning MIN [36]"), DescriptionAttribute("Default 0")]
        public float PID_TUNING_MIN
        {
            set { parameter[36] = value; serialComm.setParameter(36, value); }
            get { return parameter[36]; }
        }
        [CategoryAttribute("Remote"), DisplayName("PID Tuning MAX [37]"), DescriptionAttribute("Default 0")]
        public float PID_TUNING_MAX
        {
            set { parameter[37] = value; serialComm.setParameter(37, value); }
            get { return parameter[37]; }
        }


        public void saveFile()
        {
            SaveFileDialog fd = new SaveFileDialog();
            fd.Filter = "CorvusM3 (.cm3)|*.cm3|All Files (*.*)|*.*";
            fd.FilterIndex = 1;
            fd.ShowDialog();
            string file = fd.FileName;
            if (file != "")
            {
                StreamWriter sw = new StreamWriter(file);
                for (int i = 0; i < parameter.Length; i++)
                {
                    sw.Write("Para[" + i.ToString("000") + "]:" + parameter[i] + "\r\n");
                }
                sw.Close();
            }
        }

        public void loadFile()
        {
            OpenFileDialog of = new OpenFileDialog();
            // Set filter options and filter index.
            of.Filter = "CorvusM3 (.cm3)|*.cm3|All Files (*.*)|*.*";
            of.FilterIndex = 1;
            of.Multiselect = false;
            of.ShowDialog();
            string file = of.FileName;
            try
            {
                StreamReader sr = new StreamReader(file);
                string line;
                int count = 0;
                while ((line = sr.ReadLine()) != null)
                {
                    parameter[count++] = float.Parse((line.Substring(10)));
                }
            }
            catch
            { }
        }

    }        
}
