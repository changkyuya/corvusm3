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
using System.IO.Ports;
using System.Globalization;

namespace CorvusM3
{
    class Serial
    {
        SerialPort serPort = new SerialPort();
        
        NewDataEventArgs dataEventArgs = new NewDataEventArgs();

        public Serial() 
        {
            serPort.BaudRate = 115200;
        }

        public string [] getPorts()
        {
            return SerialPort.GetPortNames();                
        }

        public void openPort(string port)
        {
            if (serPort.IsOpen == false)
            {
                try
                {
                    serPort.PortName = port;
                    serPort.Open();
                    serPort.DataReceived += new SerialDataReceivedEventHandler(serPort_DataReceived);

                }
                catch (Exception e)
                {
                    throw new System.InvalidOperationException("Can't open Port!");
                }
            }
        }

        void serPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                while (serPort.BytesToRead > 0)
                {
                    dataEventArgs.Dataline = serPort.ReadLine().ToString();
                    RaiseNewDataEvent();
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }
        }

        // Declare the delegate (if using non-generic pattern).
        public delegate void NewDataHandler(object sender, NewDataEventArgs e);
        // Declare the event.
        public event NewDataHandler NewDataEvent;
        // Wrap the event in a protected virtual method
        // to enable derived classes to raise the event.
        protected virtual void RaiseNewDataEvent()
        {
            // Raise the event by using the () operator.
            if (NewDataEvent != null)
                NewDataEvent(this,dataEventArgs );
        }

        public void sendData(string line)
        {
            if (serPort.IsOpen == true)
                serPort.WriteLine(line);
        }

        public void setParameter(int pos, float para)
        {
            if (serPort.IsOpen == true)
                serPort.WriteLine("P" + pos.ToString() + ";" + para.ToString(CultureInfo.InvariantCulture));
        }

        public void closePort()
        {
            if (serPort.IsOpen == true)
            {
                try
                {
                    serPort.Close();
                }
                catch (Exception e)
                {
                    throw new System.InvalidOperationException("Can't close Port!");
                }
            }
        }

    }

    public class NewDataEventArgs : EventArgs
    {
        public NewDataEventArgs()
        {
        }

        public string Dataline
        {
            get
            {
                return (dataline);
            }
            set
            {
                this.dataline = value;
            }
        }

        string dataline;
    }

}
