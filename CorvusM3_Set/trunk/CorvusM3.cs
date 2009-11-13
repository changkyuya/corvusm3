﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using ZedGraph;
using System.Globalization;
using System.Configuration;
using System.Xml;


namespace CorvusM3
{
    public partial class CorvusM3 : Form
    {
        SerialPort serial;
        string dataLine = "";
        double counter = 0;
        bool on = false;
        
        

        public CorvusM3()
        {
            InitializeComponent();
            CorvusM3.CheckForIllegalCrossThreadCalls = false;
            setupGraph();
            setupGraphReceiver();
            Parameter parm = new Parameter();
            propertyGrid.SelectedObject = parm;
            
            foreach ( string port in Einstellungen.getPorts) {
                comPortToolStripComboBox.Items.Add(port); 
            }
            try
            {
                comPortToolStripComboBox.SelectedItem = Einstellungen.comPort;
            }
            catch { }
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            serial = new SerialPort(comPortToolStripComboBox.SelectedItem.ToString(), 115200);
            serial.DtrEnable = true;
            //serial.RtsEnable = true;
            serial.DataReceived += new SerialDataReceivedEventHandler(serial_DataReceived);
            serial.ReadTimeout = 100;
            serial.Open();
            infoToolStripStatusLabel.Text = "connection open";
        }

        void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (serial.BytesToRead > 0)
            {
                dataLine = serial.ReadLine();
                dataTextBox.AppendText(dataLine + "\r\n");
                addGraph();
            }
        }

        private void closeToolStripButton_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                serial.Close();
                serial.Dispose();
                infoToolStripStatusLabel.Text = "connection closed";
            }
        }




        private void commandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (!serial.IsOpen) {
                serial.Open();
            }
            if (e.KeyCode == Keys.Enter)
            {
                serial.WriteLine(commandTextBox.Text);
                dataTextBox.AppendText(">" + commandTextBox.Text + "\r\n");
                commandTextBox.Text = "";
            }
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
                }
                else
                {
                    infoToolStripStatusLabel.Text = "Steering OFF";
                    //radioButton1.Location = new Point(168, 168);
                    nickRollRadioButton.Location = new Point(168, 168);
                    nickRollRadioButton.Checked = false;
                    yawTrackBar.Value = 1500;
                    pitchTrackBar.Value = 1000;
                }
            } 
            if (on == true)
            {
                if (e.Button == MouseButtons.Left)
                {
                    yawTrackBar.Value -= 100;
                    if (yawTrackBar.Value < 1400) yawTrackBar.Value = 1400;
                }
                if (e.Button == MouseButtons.Right)
                {
                    yawTrackBar.Value += 100;
                    if (yawTrackBar.Value > 1600) yawTrackBar.Value = 1600;
                }
                
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
                commandTextBox.Text = e.X.ToString() + " - " + e.Y.ToString();
            }
        }

        private void nickRollPanel_MouseWheel(object sender, MouseEventArgs e) {

            if (on == true)
            {
                int newPitch = pitchTrackBar.Value + e.Delta/3;
                if (newPitch < 1000) newPitch = 1000;
                if (newPitch > 2000) newPitch = 2000;
                pitchTrackBar.Value = newPitch;

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

    }

    public class Parameter { 
        public string Color {
            set { }
            get { return " "; }
        }
    }
}