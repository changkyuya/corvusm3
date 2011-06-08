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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using ZedGraph;
using System.Globalization;
using System.Configuration;
using System.Xml;
using System.Collections;


namespace CorvusM3
{
    public partial class CorvusM3 : Form
    {
        SerialPort serial;
        string dataLine = "";
        double counter = 0;
        bool on = false;
        double[] channelValues = new double[8];
        double[] motorValues = new double[8];
        Parameter parm;
        ArrayList stringBuffer = new ArrayList();
        int bufferCount = 0;
        int updateCount = 0;
        bool debugToFile = false;
        StreamWriter debugWriter;
        int cockpitCount = 0;
        string filename = "";
        

        public CorvusM3()
        {
            InitializeComponent();
            serial = new SerialPort();
            parm = new Parameter(serial);
            CorvusM3.CheckForIllegalCrossThreadCalls = false;
            setupGraph();
            setupGraphReceiver();
            propertyGrid.SelectedObject = parm;
            
            foreach ( string port in Einstellungen.getPorts) {
                comPortToolStripComboBox.Items.Add(port); 
            }
            try
            {
                comPortToolStripComboBox.SelectedItem = Einstellungen.comPort;
                textBoxUpdateTime.Text = Einstellungen.dropFrame;
            }
            catch { }
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            
            try
            {
                serial.PortName = comPortToolStripComboBox.SelectedItem.ToString();
                serial.BaudRate = 115200;
                serial.DtrEnable = true;
                //serial.RtsEnable = true;
                serial.DataReceived += new SerialDataReceivedEventHandler(serial_DataReceived);
                serial.ReadTimeout = 100; 
                serial.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            infoToolStripStatusLabel.Text = "connection open";
        }

        void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                while (serial.BytesToRead > 0)
                {
                    dataLine = serial.ReadLine();
                    dataTextBox.AppendText(dataLine + "\r\n");
                    textBoxLastLine.Text = dataLine;
                    if (debugToFile == true)
                    {
                        debugWriter.Write(dataLine);
                        
                    }
                    if (dataLine.Substring(0, 1) == "P")
                    {
                        parm.fillParameter(dataLine);
                        propertyGrid.Refresh();
                    }
                    else 
                    {
                        addGraph();
                        updateCockpit();
                    }
                }
            }
            catch { }
        }

        private void closeToolStripButton_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                serial.Close();
                serial.Dispose();
                infoToolStripStatusLabel.Text = "connection closed";
                dataTextBox.Text = "";
                textBoxLastLine.Text = "";
            }
        }




        private void commandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (!serial.IsOpen) {
                serial.Open();
            }
            if (e.KeyCode == Keys.Enter)
            {
                serial.Write(commandTextBox.Text + "\r\n");
                dataTextBox.AppendText(">" + commandTextBox.Text + "\r\n");
                stringBuffer.Add(commandTextBox.Text);
                commandTextBox.Text = "";
                bufferCount = stringBuffer.Count;
            }
            try 
            {
                if (e.KeyCode == Keys.Up && bufferCount > 0)
                {
                    commandTextBox.Text = stringBuffer[--bufferCount].ToString();
                }
                if (e.KeyCode == Keys.Down && bufferCount < stringBuffer.Count)
                {
                    commandTextBox.Text = stringBuffer[++bufferCount].ToString();
                }
            }
            catch
            {}
        }



        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //setupGraph();
            commandTextBox.Focus();
        }

        void setupGraph() {
            // 
            // zedGraphControl1
            //  = new GraphPane();
            counter = 0;

            GraphPane myPane = zedGraphControl1.GraphPane;
            
            myPane.CurveList.Clear();
            myPane.Title.Text = "Debug Graph";
            myPane.XAxis.Title.Text = "Time";
            myPane.YAxis.Title.Text = "Debug Data";

            if (checkBox1.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 1", list, Color.Blue, SymbolType.None);
            }
            if (checkBox2.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 2", list, Color.Red, SymbolType.None);
            }
            if (checkBox3.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 3", list, Color.Green, SymbolType.None);
            }
            if (checkBox4.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 4", list, Color.Aqua, SymbolType.None);
            }
            if (checkBox5.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 5", list, Color.Orange, SymbolType.None);
            }
            if (checkBox6.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 6", list, Color.LightGreen, SymbolType.None);
            }
            if (checkBox7.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 7", list, Color.DarkBlue, SymbolType.None);
            }
            if (checkBox8.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 8", list, Color.DarkRed, SymbolType.None);
            }
            if (checkBox9.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 9", list, Color.DarkGreen, SymbolType.None);
            }
            if (checkBox10.Checked)
            {
                RollingPointPairList list = new RollingPointPairList(500);
                LineItem curve = myPane.AddCurve("DBG 10", list, Color.Chocolate, SymbolType.None);
            }


            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 100;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 5;

            // Scale the axes
            zedGraphControl1.AxisChange();
        }

        void setupGraphReceiver() {
            string[] str = { "CH 1", "CH 2", "CH 3", "CH 4", "CH 5", "CH 6", "CH 7", "CH 8" };
            GraphPane myPane = zedGraphControl2.GraphPane;
            myPane.Title.Text = "Receiver";
            myPane.XAxis.Title.Text = "Channel";
            myPane.YAxis.Title.Text = "Value";
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 9;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 1;
            myPane.XAxis.Type = AxisType.Text;
            myPane.XAxis.Scale.TextLabels = str;
            myPane.YAxis.Scale.Min = -600;
            myPane.YAxis.Scale.Max = 600;

            PointPairList list = new PointPairList();
            //Random rand = new Random();

            //for (int i = 0; i < 8; i++)
            //{
            //    double x = i+1;
                //double y = rand.NextDouble() * 1000 - 500;
            //    double y = 0;
            //    list.Add(x, y);
            //}

            BarItem myCurve = myPane.AddBar("", list, Color.Blue);
            
            
            // Tell ZedGraph to calculate the axis ranges
            zedGraphControl2.AxisChange();
            BarItem.CreateBarLabels(myPane, false, "F0");
        }

        private void addGraph()
        {
            if (Convert.ToInt16(textBoxUpdateTime.Text) > updateCount)
            {
                updateCount++;
                return;
            }
            updateCount = 0;
            
            
            NumberFormatInfo nfi = new NumberFormatInfo();
            nfi.NumberDecimalSeparator = "."; 
            
            string[] data = dataLine.Split(':');
            double y = 0;
            double y1 = 0;
            double y2 = 0;
            double y3 = 0;
            double y4 = 0;
            double y5 = 0;
            double y6 = 0;
            double y7 = 0;
            double y8 = 0;
            double y9 = 0;

            // if debug data are not for graph
            try
            {
                if (data[0].Substring(0, 1) != "G" && data[0].Substring(0, 1) != "R")
                {
                    return;
                }
            }
            catch (Exception ex) { }

            try
            {
                if (data[0].Substring(0, 1) == "G")
                {
                    if (checkBox1.Checked)
                    {
                        y = Convert.ToDouble(data[2], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[0] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y);

                    }
                    if (checkBox2.Checked)
                    {
                        int index = 1;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        y1 = Convert.ToDouble(data[3], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y1);
                    }
                    if (checkBox3.Checked)
                    {
                        int index = 2;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        y2 = Convert.ToDouble(data[4], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y2);
                    }
                    if (checkBox4.Checked)
                    {
                        int index = 3;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        y3 = Convert.ToDouble(data[5], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y3);
                    }
                    if (checkBox5.Checked)
                    {
                        int index = 4;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        y4 = Convert.ToDouble(data[6], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y4);
                    }
                    if (checkBox6.Checked)
                    {
                        int index = 5;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox5.Checked == false)
                        {
                            index--;
                        }
                        y5 = Convert.ToDouble(data[7], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y5);
                    }
                    if (checkBox7.Checked)
                    {
                        int index = 6;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox5.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox6.Checked == false)
                        {
                            index--;
                        }
                        y6 = Convert.ToDouble(data[8], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y6);
                    }
                    if (checkBox8.Checked)
                    {
                        int index = 7;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox5.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox6.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox7.Checked == false)
                        {
                            index--;
                        }
                        y7 = Convert.ToDouble(data[9], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y7);
                    }
                    if (checkBox9.Checked)
                    {
                        int index = 8;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox5.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox6.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox7.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox8.Checked == false)
                        {
                            index--;
                        }
                        y8 = Convert.ToDouble(data[10], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y8);
                    }
                    if (checkBox10.Checked)
                    {
                        int index = 9;
                        if (checkBox1.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox2.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox3.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox4.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox5.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox6.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox7.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox8.Checked == false)
                        {
                            index--;
                        }
                        if (checkBox9.Checked == false)
                        {
                            index--;
                        }
                        y9 = Convert.ToDouble(data[11], nfi);
                        LineItem curve = zedGraphControl1.GraphPane.CurveList[index] as LineItem;
                        IPointListEdit list = curve.Points as IPointListEdit;
                        list.Add(counter, y9);
                    }
                }
                if (data[0].Substring(0, 1) == "R") {
                    GraphPane myPane = zedGraphControl2.GraphPane; 
                    BarItem bar = zedGraphControl2.GraphPane.CurveList[0] as BarItem;
                    
                    PointPairList list = new PointPairList();
                    //Random rand = new Random();

                    for (int i = 0; i < 8; i++)
                    {
                        if (Convert.ToDouble(data[i + 2]) == 0)
                        {
                            y = 0;
                        }
                        else
                        {
                            y = Convert.ToDouble(data[i + 2]) - 1500;
                        }
                        list.Add(i+1, y);
                    }
                    myPane.CurveList.Clear();
                    bar = myPane.AddBar("", list, Color.Blue);
                }
            }
            catch (Exception ex) {
                return;
            }

            counter++;

            
            // Keep the X scale at a rolling 30 second interval, with one
            // major step between the max X value and the end of the axis
            Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
            if (counter > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = counter + xScale.MajorStep;
                xScale.Min = xScale.Max - 100.0;
            }

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();
            // Force a redraw
            zedGraphControl1.Invalidate();
            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl2.AxisChange();
            // Force a redraw
            zedGraphControl2.Invalidate();
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            setupGraph();
            zedGraphControl1.Refresh();
        }



        private void comPortToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            Einstellungen.comPort = comPortToolStripComboBox.SelectedItem.ToString();
            
        }

        private void nickRollPanel_MouseDown(object sender, MouseEventArgs e)
        {
            mouseClick(sender, e);
        }


        private void nickRollRadioButton_MouseDown(object sender, MouseEventArgs e)
        {
            mouseClick(sender, e);
        }


        void mouseClick(object sender, MouseEventArgs e) {

            if (e.Button == MouseButtons.Middle)
            {
                on = !on;
                if (on == true)
                {
                    infoToolStripStatusLabel.Text = "Steering ON";
                    nickRollRadioButton.Location = new Point(168, 168);
                    nickRollRadioButton.Checked = true;
                    yawTrackBar.Value = 1500;
                    pitchTrackBar.Value = 1000;
                    channelValues[0] = 1000;
                    channelValues[1] = 1500;
                    channelValues[2] = 1500;
                    channelValues[3] = 1500;
                    steeringTimer.Start();
                }
                else
                {
                    infoToolStripStatusLabel.Text = "Steering OFF";
                    //radioButton1.Location = new Point(168, 168);
                    nickRollRadioButton.Location = new Point(168, 168);
                    nickRollRadioButton.Checked = false;
                    yawTrackBar.Value = 1500;
                    pitchTrackBar.Value = 1000;
                    steeringTimer.Stop();
                }
            } 
            if (on == true)
            {
                if (e.Button == MouseButtons.Left)
                {
                    yawTrackBar.Value += 50;
                    if (yawTrackBar.Value > 1700) yawTrackBar.Value = 1700;
                    
                }
                if (e.Button == MouseButtons.Right)
                {
                    yawTrackBar.Value -= 50;
                    if (yawTrackBar.Value < 1300) yawTrackBar.Value = 1300;
                }
                channelValues[3] = yawTrackBar.Value;
            }
        }

        private void nickRollPanel_MouseMove(object sender, MouseEventArgs e)
        {
            mouseMove(sender, e);
        }

        private void nickRollRadioButton_MouseMove(object sender, MouseEventArgs e)
        {
            
            //mouseMove(sender, e);
        }

        void mouseMove(object sender, MouseEventArgs e) {

            if (on == true)
            {
                nickRollRadioButton.Location = new Point(e.X-5, e.Y-5);
                channelValues[1] = 2000 - Math.Round(e.X * 2.8 + 10);
                channelValues[2] = 2000 - Math.Round(e.Y * 2.8 + 10);
                
            }
        }

        private void nickRollPanel_MouseWheel(object sender, MouseEventArgs e) {

            if (on == true)
            {
                int newPitch = pitchTrackBar.Value + e.Delta/2-10;
                if (newPitch < 1000) newPitch = 1000;
                if (newPitch > 2000) newPitch = 2000;
                pitchTrackBar.Value = newPitch;
                channelValues[0] = newPitch;
            }

        }

        private void steeringTimer_Tick(object sender, EventArgs e)
        {
            commandTextBox.Text = "~" + channelValues[0].ToString() + ":" + channelValues[1].ToString() + ":" + channelValues[2].ToString() + ":" + channelValues[3].ToString();
            try
            {
                if (serial.IsOpen)
                {
                    serial.Write("~" + channelValues[0].ToString() + ":" + channelValues[1].ToString() + ":" + channelValues[2].ToString() + ":" + channelValues[3].ToString() + "\r\n");
                }
            }
            catch
            {}
        }

        private void saveFileButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog fd = new SaveFileDialog();
            fd.Filter = "CorvusM3 (.cm3)|*.cm3|All Files (*.*)|*.*";
            fd.FilterIndex = 1;
            fd.ShowDialog();
            string file = fd.FileName;
            if (file != "")
            {
                StreamWriter sw = new StreamWriter(file);
                for (int i = 0; i <= parm.maxParameter; i++)
                {
                    sw.Write("Para[" + i.ToString("000") + "]:" + parm.parameter[i] + "\r\n");
                }
                sw.Close();
            }
        }

        private void loadFileButton_Click(object sender, EventArgs e)
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
                    parm.parameter[count++] = Convert.ToInt32(line.Substring(10));
                }
                propertyGrid.Refresh();
            }
            catch
            {}
        }

        private void loadButton_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                serial.Write("pa\r\n");
                MessageBox.Show("Finish!", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }


        private void flashButton_Click(object sender, EventArgs e)
        {
            parm.flashParameter();
            MessageBox.Show("Finish!", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void flashFirmwareToolStripButton_Click(object sender, EventArgs e)
        {
            if (comPortToolStripComboBox.SelectedItem.ToString() == "")
            {
                MessageBox.Show("Please select COM Port!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                OpenFileDialog flashFWDialog = new OpenFileDialog();
                flashFWDialog.Filter = "binary file (*.bin)|*.bin|All files (*.*)|*.*";
                if (flashFWDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = flashFWDialog.FileName;
                    string applPath = Application.StartupPath;

                    string comPort = comPortToolStripComboBox.SelectedItem.ToString().Substring(3);

                    System.Diagnostics.Process.Start(applPath + @"\uploader\STM32\STMFlashLoader.exe", @" -c --pn " + comPort + " --br 115200 -i STM32F10xxExx -e --all -d --a 8000000 --fn " + path + @" -p --dwp");
                }
            }

        }

        private void buttonMotorMixer_Click(object sender, EventArgs e)
        {
            MotorMixer motorMixer = new MotorMixer();
            motorMixer.Show();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            parm.sendAllPara();
            MessageBox.Show("Finish!", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void motorTimer_Tick(object sender, EventArgs e)
        {
            commandTextBox.Text = "<" + motorValues[0].ToString("000") + ":" + motorValues[1].ToString("000") + ":" + motorValues[2].ToString("000") + ":" + motorValues[3].ToString("000");
            try
            {
                if (serial.IsOpen)
                {   
                    //CorvusM3 does not accept zero values
                    for (int i = 0; i < 5; i++)
                    {
                        if (motorValues[i] == 0)
                        {
                            motorValues[i] = 1;
                        }
                    }
                    serial.Write("<" + motorValues[0].ToString("000") + ":" + motorValues[1].ToString("000") + ":" + motorValues[2].ToString("000") + ":" + motorValues[3].ToString("000") + "\r\n");
                }
            }
            catch
            { }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (button2.BackColor == Color.Transparent)
            {
                button2.BackColor = Color.Red;
                motorTimer.Enabled = true;
            }
            else
            {
                button2.BackColor = Color.Transparent;
                motorTimer.Enabled = false;
                commandTextBox.Text = "";
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            label1.Text = trackBar1.Value.ToString();
            motorValues[0] = trackBar1.Value;
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            label2.Text = trackBar2.Value.ToString();
            motorValues[1] = trackBar2.Value;
        }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            label3.Text = trackBar3.Value.ToString();
            motorValues[2] = trackBar3.Value;
        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            label4.Text = trackBar4.Value.ToString();
            motorValues[3] = trackBar4.Value;
        }

        private void rescanToolStripButton_Click(object sender, EventArgs e)
        {
            comPortToolStripComboBox.Items.Clear();
            
            foreach (string port in Einstellungen.getPorts)
            {           
                comPortToolStripComboBox.Items.Add(port);
            }
            try
            {
                comPortToolStripComboBox.SelectedItem = Einstellungen.comPort;
            }
            catch { }
        }

        private void textBoxUpdateTime_TextChanged(object sender, EventArgs e)
        {
            Einstellungen.dropFrame = textBoxUpdateTime.Text;
        }

        private void toolStripButtonDebugToFile_Click(object sender, EventArgs e)
        {
            if (debugToFile == false)
            {
                debugToFile = true;
                toolStripButtonDebugToFile.Text = "Debug to File ON";
                filename = DateTime.Now.ToShortDateString() + "_" + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString();
                DialogResult dRes = MessageBox.Show("Open Debug-File: " + Path.Combine(Path.GetTempPath(), "CorvusM3_log_" + filename + ".txt"), "Open Debug-File", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                debugWriter = new StreamWriter(Path.Combine(Path.GetTempPath(), "CorvusM3_log_" + filename + ".txt"));
                debugWriter.WriteLine("Debug: " + filename);
                if (dRes == DialogResult.Yes)
                {
                    System.Diagnostics.Process.Start(Path.Combine(Path.GetTempPath(), "CorvusM3_log_" + filename + ".txt"));
                }
            }
            else
            {
                debugToFile = false;
                toolStripButtonDebugToFile.Text = "Debug to File OFF";
                debugWriter.Close();
            }
        }

        private void updateCockpit() {
            string[] data = dataLine.Split(':');

            if (data[0] == "G-A-C")
            {
                cockpitCount++;
                if (cockpitCount < 10)
                {
                    return;
                }
                cockpitCount = 0;


                double pitch = Convert.ToDouble(data[7]) / 100 - 90;
                double roll = Convert.ToDouble(data[8]) / 100 - 90;
                attitudeIndicatorInstrumentControl1.SetAttitudeIndicatorParameters(-roll, pitch);
                headingIndicatorInstrumentControl1.SetHeadingIndicatorParameters(Convert.ToInt32(data[9]) / 100);
            }

        }




    }


    class Einstellungen
    {
        public static string comPort 
        {
            get
            {
                return Properties.Settings.Default.comPort;
            }
            set
            {
                Properties.Settings.Default.comPort = value;
                Properties.Settings.Default.Save();
            }
        }

        public static string[] getPorts
        {
            get 
            {
                return SerialPort.GetPortNames();
            }
        }
        public static string dropFrame
        {
            get
            {
                return Properties.Settings.Default.dropFrame;
            }
            set
            {
                Properties.Settings.Default.dropFrame = value;
                Properties.Settings.Default.Save();
            }
        }

    }


}
