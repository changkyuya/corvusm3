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
using System.Threading;

namespace CorvusM3
{
    public partial class CorvusM3 : Form
    {
        int maxGraphValues = 500;
        Serial serialComm = new Serial();
        bool serialIsOpen = false;
        bool isRemoteOn = false;
        bool isGraphOn = false;

        Parameter parameter = new Parameter();
        

        public CorvusM3()
        {
            InitializeComponent();
            toolStripComboBoxSerialPorts.Items.AddRange(serialComm.getPorts());
            serialComm.NewDataEvent += new Serial.NewDataHandler(serialComm_NewDataEvent);
            propertyGridParameter.SelectedObject = parameter;
            toolStripComboBoxSerialPorts.SelectedItem = ProgramSetting.comPort;
            this.Width = ProgramSetting.windowWidth;
            this.Height = ProgramSetting.windowHeight;
            parameter.SerialComm = serialComm;
            parameter.PropGrid = this.propertyGridParameter;
        }

        void serialComm_NewDataEvent(object sender, NewDataEventArgs e)
        {
            textBoxCLIDisplay.Invoke(new delegateUpdateCLI(updateCLI), e);
        }

        public delegate void delegateUpdateCLI(NewDataEventArgs e);

        void updateCLI(NewDataEventArgs e)
        {
            textBoxCLIDisplay.AppendText(e.Dataline + "\r\n");
            switch(e.Dataline.Substring(0,1))
            {
                //remote
                case "r":
                    updateReceiver(e.Dataline);
                    break;
                //remote
                case "s":
                    updateGraph(e.Dataline);
                    break;
                //parameter
                case "p":
                    propertyGridParameter.Refresh();
                    break;
            }
        }

        private void updateGraph(string p)
        {
            try
            {
                string[] values = p.Split(','); 
                chartGyro.Series[0].Points.AddY(values[1]);
                if (chartGyro.Series[0].Points.Count > maxGraphValues)
                    chartGyro.Series[0].Points.RemoveAt(0);
                chartGyro.Series[1].Points.AddY(values[2]);
                chartGyro.Series[2].Points.AddY(values[3]);
                chartGyro.ResetAutoValues();

                chartACC.Series[0].Points.AddY(values[4]);
                if (chartACC.Series[0].Points.Count > maxGraphValues)
                    chartACC.Series[0].Points.RemoveAt(0);
                chartACC.Series[1].Points.AddY(values[5]);
                chartACC.Series[2].Points.AddY(values[6]);
                chartACC.ResetAutoValues();

                chartCopter.Series[0].Points.AddY(values[8]);
                if (chartCopter.Series[0].Points.Count > maxGraphValues)
                    chartCopter.Series[0].Points.RemoveAt(0);
                chartCopter.Series[1].Points.AddY(values[9]);
                chartCopter.Series[2].Points.AddY(values[10]);
                chartCopter.ResetAutoValues();
            }
            catch { }
        }

        private void updateReceiver(string p)
        {
            try
            {
                string[] values = p.Split(',');
                if (values[1] == "0")
                    values[1] = "1500";
                    trackBarRoll.Value = Convert.ToInt16(values[1]);
                if (values[2] == "0")
                    values[2] = "1500";
                    // reverse nick
                    int normalNick = Convert.ToInt16(values[2]);
                    trackBarNick.Value = 2100 - Convert.ToInt16(values[2]) + 900;
                if (values[3] == "0")
                    values[3] = "1500";
                    trackBarYaw.Value = Convert.ToInt16(values[3]);
                if (values[4] == "0")
                    values[4] = "1500";
                    trackBarPitch.Value = Convert.ToInt16(values[4]);
                if (values[5] == "0")
                    values[5] = "1500";
                    trackBarAux1.Value = Convert.ToInt16(values[5]);
                    labelAux1.Text = values[5];
                if (values[6] == "0")
                    values[6] = "1500";
                trackBarAux2.Value = Convert.ToInt16(values[6]);
                labelAux2.Text = values[6];
                if (values[7] == "0")
                    values[7] = "1500";
                trackBarAux3.Value = Convert.ToInt16(values[7]);
                labelAux3.Text = values[7];
                if (values[8] == "0\r")
                    values[8] = "1500";
                trackBarAux4.Value = Convert.ToInt16(values[8]);
                labelAux4.Text = values[8];

                if (Convert.ToInt16(labelPitchMax.Text) < trackBarPitch.Value)
                {
                    labelPitchMax.Text = trackBarPitch.Value.ToString();
                }
                if (Convert.ToInt16(labelPitchMin.Text) > trackBarPitch.Value)
                {
                    labelPitchMin.Text = trackBarPitch.Value.ToString();
                    if (trackBarPitch.Value < 1150)
                    {
                        labelPitchMin.ForeColor = Color.Green;
                    }
                }

                if (Convert.ToInt16(labelNickMin.Text) > normalNick)
                {
                    labelNickMin.Text = normalNick.ToString();
                    if (normalNick < 1150)
                    {
                        labelNickMin.ForeColor = Color.Green;
                    }
                }
                if (Convert.ToInt16(labelNickMax.Text) < normalNick)
                {
                    labelNickMax.Text = normalNick.ToString();
                    if (normalNick > 1850)
                    {
                        labelNickMax.ForeColor = Color.Green;
                    }
                }

                if (Convert.ToInt16(labelRollMax.Text) < trackBarRoll.Value)
                {
                    labelRollMax.Text = trackBarRoll.Value.ToString();
                    if (trackBarRoll.Value > 1850)
                    {
                        labelRollMax.ForeColor = Color.Green;
                    }
                }
                if (Convert.ToInt16(labelRollMin.Text) > trackBarRoll.Value)
                {
                    labelRollMin.Text = trackBarRoll.Value.ToString();
                    if (trackBarRoll.Value < 1150)
                    {
                        labelRollMin.ForeColor = Color.Green;
                    }
                }

                if (Convert.ToInt16(labelYawMax.Text) < trackBarYaw.Value)
                {
                    labelYawMax.Text = trackBarYaw.Value.ToString();
                    if (trackBarYaw.Value > 1850)
                    {
                        labelYawMax.ForeColor = Color.Green;
                    }
                }
                if (Convert.ToInt16(labelYawMin.Text) > trackBarYaw.Value)
                {
                    labelYawMin.Text = trackBarYaw.Value.ToString();
                    if (trackBarYaw.Value < 1150)
                    {
                        labelYawMin.ForeColor = Color.Green;
                    }
                }
            }
            catch { }
        }



        private void toolStripButtonScanPorts_Click(object sender, EventArgs e)
        {
            toolStripComboBoxSerialPorts.Items.Clear();
            toolStripComboBoxSerialPorts.Items.AddRange(serialComm.getPorts());
        }

        private void toolStripButtonSerOpenClose_Click(object sender, EventArgs e)
        {
            if (serialIsOpen == true)
            {
                try
                {
                    serialComm.closePort();
                    serialIsOpen = false;
                    toolStripButtonSerOpenClose.Text = "open";
                    toolStripStatusLabel.Text = "connection closed";
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                try
                {
                    serialComm.openPort(toolStripComboBoxSerialPorts.SelectedItem.ToString());
                    serialIsOpen = true;
                    toolStripButtonSerOpenClose.Text = "close";
                    toolStripStatusLabel.Text = "connection open";
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }



        private void buttonLoadFile_Click(object sender, EventArgs e)
        {
            parameter.loadFile();
            propertyGridParameter.Refresh();
        }

        private void buttonSaveFile_Click(object sender, EventArgs e)
        {
            parameter.saveFile();
            propertyGridParameter.Refresh();
        }

        private void textBoxCLICommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                serialComm.sendData(textBoxCLICommand.Text);
                textBoxCLICommand.Text = "";
            }
        }

        private void toolStripComboBoxSerialPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            ProgramSetting.comPort = toolStripComboBoxSerialPorts.SelectedItem.ToString();
        }

        private void CorvusM3_ResizeEnd(object sender, EventArgs e)
        {
            ProgramSetting.windowHeight = this.Height;
            ProgramSetting.windowWidth = this.Width;
        }

        private void buttonSetMidValues_Click(object sender, EventArgs e)
        {
            labelRollCenter.Text = trackBarRoll.Value.ToString();
            labelNickCenter.Text = trackBarNick.Value.ToString();
            labelYawCenter.Text = trackBarYaw.Value.ToString();

            if (trackBarRoll.Value < 1600 && trackBarRoll.Value > 1400)
            {
                labelRollCenter.ForeColor = Color.Green;
            }
            else
            {
                labelRollCenter.ForeColor = SystemColors.ControlText;
            }
            if (trackBarNick.Value < 1600 && trackBarNick.Value > 1400)
            {
                labelNickCenter.ForeColor = Color.Green;
            }
            else
            {
                labelNickCenter.ForeColor = SystemColors.ControlText;
            }
            if (trackBarYaw.Value < 1600 && trackBarYaw.Value > 1400)
            {
                labelYawCenter.ForeColor = Color.Green;
            }
            else
            {
                labelYawCenter.ForeColor = SystemColors.ControlText;
            }
        }

        private void buttonResetValues_Click(object sender, EventArgs e)
        {
            labelRollCenter.Text = "1500";
            labelRollCenter.ForeColor = SystemColors.ControlText;
            labelRollMin.Text = "1300";
            labelRollMin.ForeColor = SystemColors.ControlText;
            labelRollMax.Text = "1700";
            labelRollMax.ForeColor = SystemColors.ControlText;
            trackBarRoll.Value = 1500;

            labelNickCenter.Text = "1500";
            labelNickCenter.ForeColor = SystemColors.ControlText;
            labelNickMin.Text = "1300";
            labelNickMin.ForeColor = SystemColors.ControlText;
            labelNickMax.Text = "1700";
            labelNickMax.ForeColor = SystemColors.ControlText;
            trackBarNick.Value = 1500;

            labelYawCenter.Text = "1500";
            labelYawCenter.ForeColor = SystemColors.ControlText;
            labelYawMin.Text = "1300";
            labelYawMin.ForeColor = SystemColors.ControlText;
            labelYawMax.Text = "1700";
            labelYawMax.ForeColor = SystemColors.ControlText;
            trackBarYaw.Value = 1500;

            labelPitchMin.Text = "1300";
            labelPitchMin.ForeColor = SystemColors.ControlText;
            labelPitchMax.Text = "1700";
            trackBarPitch.Value = 1500;

            labelAux1.Text = "1500";
            trackBarAux1.Value = 1500;
            labelAux2.Text = "1500";
            trackBarAux2.Value = 1500;
            labelAux3.Text = "1500";
            trackBarAux3.Value = 1500;
            labelAux4.Text = "1500";
            trackBarAux4.Value = 1500;

        }

        private void buttonRemoteOnOff_Click(object sender, EventArgs e)
        {
            if (isRemoteOn == false)
            {
                serialComm.sendData("r");
                isRemoteOn = true;
                buttonRemoteOnOff.Text = "off Remote";
                isGraphOn = false;
                buttonOnOffGraph.Text = "on Graph";
            }
            else 
            {
                serialComm.sendData("x");
                isRemoteOn = false;
                buttonRemoteOnOff.Text = "on Remote";
            }
        }

        private void buttonSaveConfigRemote_Click(object sender, EventArgs e)
        {
            serialComm.sendData("x");
            tabControl1.SelectedIndex = 0;
            serialComm.sendData("T");
        }

        private void buttonOnOffGraph_Click(object sender, EventArgs e)
        {

            if (isGraphOn == false)
            {
                serialComm.sendData("s");
                isGraphOn = true;
                buttonOnOffGraph.Text = "off Graph";
                isRemoteOn = false;
                buttonRemoteOnOff.Text = "on Remote";
            }
            else
            {
                serialComm.sendData("x");
                isGraphOn = false;
                buttonOnOffGraph.Text = "on Graph";
            }
        }

        private void buttonAccCenter_Click(object sender, EventArgs e)
        {
            serialComm.sendData("x");
            tabControl1.SelectedIndex = 0;
            serialComm.sendData("C");
        }

        private void buttonLoadParameter_Click(object sender, EventArgs e)
        {
            serialComm.sendData("a");

        }

        private void buttonSetDefault_Click(object sender, EventArgs e)
        {
            serialComm.sendData("#");
            serialComm.sendData("a");

        }

        private void toolStripButtonFlashFirmware_Click(object sender, EventArgs e)
        {
            try
            {
                if (toolStripComboBoxSerialPorts.SelectedItem.ToString() == "")
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

                        string comPort = toolStripComboBoxSerialPorts.SelectedItem.ToString().Substring(3);

                        serialComm.closePort();
                        System.Diagnostics.Process.Start(applPath + @"\STM32\STMFlashLoader.exe", @" -c --pn " + comPort + " --br 115200 -i STM32F10xxExx -e --all -d --a 8000000 --fn " + path + @" -p --dwp");
                    }
                }
            }
            catch
            {
                MessageBox.Show("Please select COM Port!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

    }
}
