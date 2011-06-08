using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ZedGraph;
using System.IO.Ports;

namespace DynamicData
{
	public partial class Form1 : Form
	{
		// Starting time in milliseconds
		int tickStart = 0;
        SerialPort port = new SerialPort();
        bool run = false;
        bool comOpen = false;

		public Form1()
		{
            InitializeComponent();

            
            
		}

		private void Form1_Load( object sender, EventArgs e )
		{
			GraphPane myPane = zedGraphControl1.GraphPane;
			myPane.Title.Text = "TESTDATA X, Y, Z";
			myPane.XAxis.Title.Text = "Time";
			myPane.YAxis.Title.Text = "Values";

			// Save 1200 points.  At 50 ms sample rate, this is one minute
			// The RollingPointPairList is an efficient storage class that always
			// keeps a rolling set of point data without needing to shift any data values
            RollingPointPairList list = new RollingPointPairList(500);
            RollingPointPairList list1 = new RollingPointPairList(500);
            RollingPointPairList list2 = new RollingPointPairList(500);

			// Initially, a curve is added with no data points (list is empty)
			// Color is blue, and there will be no symbols
			LineItem curve = myPane.AddCurve( "X", list, Color.Blue, SymbolType.None );
            curve.Line.IsOptimizedDraw = true;
            
            if (checkBoxFast.Checked == false)
            {
                curve.Line.IsSmooth = true;
            }
            else {
                curve.Line.IsSmooth = false;
            }
            
            // Y
            LineItem curve1 = myPane.AddCurve("Y", list1, Color.Red, SymbolType.None);
            curve1.Line.IsOptimizedDraw = true;

            if (checkBoxFast.Checked == false)
            {
                curve1.Line.IsSmooth = true;
            }
            else
            {
                curve1.Line.IsSmooth = false;
            }
            
            // Z
            LineItem curve2 = myPane.AddCurve("Z", list2, Color.Green, SymbolType.None);
            curve2.Line.IsOptimizedDraw = true;

            if (checkBoxFast.Checked == false)
            {
                curve2.Line.IsSmooth = true;
            }
            else
            {
                curve2.Line.IsSmooth = false;
            }
            

			// Sample at 50ms intervals
			timer1.Interval = 1;
			timer1.Enabled = true;

			// Just manually control the X axis range so it scrolls continuously
			// instead of discrete step-sized jumps
			myPane.XAxis.Scale.Min = 0;
			myPane.XAxis.Scale.Max = 60;
			myPane.XAxis.Scale.MinorStep = 1;
			myPane.XAxis.Scale.MajorStep = 5;

			// Scale the axes
			zedGraphControl1.AxisChange();

			// Save the beginning time for reference
            tickStart = Environment.TickCount;
		}

		private void timer1_Tick( object sender, EventArgs e )
		{
			// Make sure that the curvelist has at least one curve
			//if ( zedGraphControl1.GraphPane.CurveList.Count <= 0 )
			//	return;

			// Get the first CurveItem in the graph
            LineItem curve = zedGraphControl1.GraphPane.CurveList[0] as LineItem;
            LineItem curve1 = zedGraphControl1.GraphPane.CurveList[1] as LineItem;
            LineItem curve2 = zedGraphControl1.GraphPane.CurveList[2] as LineItem;
			//if ( curve == null )
			//	return;

			// Get the PointPairList
            IPointListEdit list = curve.Points as IPointListEdit;
            IPointListEdit list1 = curve1.Points as IPointListEdit;
            IPointListEdit list2 = curve2.Points as IPointListEdit;
			// If this is null, it means the reference at curve.Points does not
			// support IPointListEdit, so we won't be able to modify it
			//if ( list == null )
			//	return;

			// Time is measured in seconds
			//double time = ( Environment.TickCount - tickStart ) / 100.0;

            if (run == true)
            {
                
                String s = port.ReadLine();
                s = s.Replace("\r", "\r\n");
                if (checkBoxFast.Checked == false)
                {
                    textBox.AppendText(s);
                }
                else
                {
                    
                }
                try
                {
                    string[] ss = s.Split(':');
                    double sx = Convert.ToDouble(ss[1]) / 10;
                    double sy = Convert.ToDouble(ss[2]);
                    double sy1 = Convert.ToDouble(ss[3]);
                    double sy2 = Convert.ToDouble(ss[4]);
                    //list.Add(time, ss);
                    list.Add(sx, sy);
                    list1.Add(sx, sy1);
                    list2.Add(sx, sy2);

                    // Keep the X scale at a rolling 30 second interval, with one
                    // major step between the max X value and the end of the axis
                    //Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
                    //if (time > xScale.Max - xScale.MajorStep)
                    //{
                    //    xScale.Max = time + xScale.MajorStep;
                    //    xScale.Min = xScale.Max - 30.0;
                    //}
                    Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
                    if (sx > xScale.Max - xScale.MajorStep)
                    {
                        xScale.Max = sx + xScale.MajorStep;
                        xScale.Min = xScale.Max - 60.0;
                    }
                }
                catch(Exception ex){}
            }

			// 3 seconds per cycle
			//list.Add( time, Math.Sin( 2.0 * Math.PI * time / 3.0 ) );




			// Make sure the Y axis is rescaled to accommodate actual data
			zedGraphControl1.AxisChange();
			// Force a redraw
			zedGraphControl1.Invalidate();


		}

		private void Form1_Resize( object sender, EventArgs e )
		{
			SetSize();
		}

		// Set the size and location of the ZedGraphControl
		private void SetSize()
		{
			// Control is always 10 pixels inset from the client rectangle of the form
			Rectangle formRect = this.ClientRectangle;
			formRect.Inflate( -10, -10 );

			if ( zedGraphControl1.Size != formRect.Size )
			{
				zedGraphControl1.Location = formRect.Location;
				zedGraphControl1.Size = formRect.Size;
			}
		}

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (port.IsOpen == false)
            {
                port = new SerialPort(textBoxCOM.Text, 115200);
                port.Open();
            }
            port.WriteLine("1");
            run = true;
            timer1.Start();
           
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                port.WriteLine("0");
                run = false;
                string over = port.ReadExisting();
                string[] overItems = over.Split(';');
                int overCount = overItems.Length;
                textBox.AppendText("\r\noverflow:" + overCount.ToString());
            }
            timer1.Stop();
            
        }

        private void checkBoxFast_CheckedChanged(object sender, EventArgs e)
        {
 
            LineItem curve = zedGraphControl1.GraphPane.CurveList[0] as LineItem;
            if (checkBoxFast.Checked == false)
                {
                    curve.Line.IsSmooth = true;
                }
                else
                {
                    curve.Line.IsSmooth = false;
                }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            port = new SerialPort(textBoxCOM.Text, 115200);
            port.Open();
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            port.Close();
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {

            //TODO: clear einbauen

        }

        private void toolStripStatusLabel1_Click(object sender, EventArgs e)
        {

        }
	}
}