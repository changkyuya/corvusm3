using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CorvusM3
{
    public partial class MotorMixer : Form
    {
        public MotorMixer()
        {
            InitializeComponent();
        }


        private void textBoxM12Y_TextChanged(object sender, EventArgs e)
        {
            // text changed

            textBoxM1A.Text = convert(textBoxM1P, textBoxM1R);
            textBoxM1B.Text = convert(textBoxM1N, textBoxM1Y);

            textBoxM2A.Text = convert(textBoxM2P, textBoxM2R);
            textBoxM2B.Text = convert(textBoxM2N, textBoxM2Y);

            textBoxM3A.Text = convert(textBoxM3P, textBoxM3R);
            textBoxM3B.Text = convert(textBoxM3N, textBoxM3Y);

            textBoxM4A.Text = convert(textBoxM4P, textBoxM4R);
            textBoxM4B.Text = convert(textBoxM4N, textBoxM4Y);

            textBoxM5A.Text = convert(textBoxM5P, textBoxM5R);
            textBoxM5B.Text = convert(textBoxM5N, textBoxM5Y);

            textBoxM6A.Text = convert(textBoxM6P, textBoxM6R);
            textBoxM6B.Text = convert(textBoxM6N, textBoxM6Y);

            textBoxM7A.Text = convert(textBoxM7P, textBoxM7R);
            textBoxM7B.Text = convert(textBoxM7N, textBoxM7Y);

            textBoxM8A.Text = convert(textBoxM8P, textBoxM8R);
            textBoxM8B.Text = convert(textBoxM8N, textBoxM8Y);

            textBoxM9A.Text = convert(textBoxM9P, textBoxM9R);
            textBoxM9B.Text = convert(textBoxM9N, textBoxM9Y);

            textBoxM10A.Text = convert(textBoxM10P, textBoxM10R);
            textBoxM10B.Text = convert(textBoxM10N, textBoxM10Y);

            textBoxM11A.Text = convert(textBoxM11P, textBoxM11R);
            textBoxM11B.Text = convert(textBoxM11N, textBoxM11Y);

            textBoxM12A.Text = convert(textBoxM12P, textBoxM12R);
            textBoxM12B.Text = convert(textBoxM12N, textBoxM12Y);


        }

        private void MotorMixer_Load(object sender, EventArgs e)
        {

        }

        string convert(TextBox box1, TextBox box2)
        {

            int links;
            int rechts;
            int para;

            try
            {
                links = Convert.ToInt16(box1.Text);
                rechts = Convert.ToInt16(box2.Text);
                if (links < 0)
                    links = links * -1 + 128;
                if (rechts < 0)
                    rechts = rechts * -1 + 128;
                para = links * 256 + rechts;
            }
            catch {
                para = 0;
            }

            return para.ToString();
        }

    }
}
