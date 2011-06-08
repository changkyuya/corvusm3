using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CorvusM3
{
    public class ProgramSetting
    {
        public static string comPort
        {
            get { return Properties.Settings.Default.comPort; }
            set {
                Properties.Settings.Default.comPort = value;
                Properties.Settings.Default.Save();
            }
        }
        public static int windowHeight
        {
            get { return Properties.Settings.Default.windowHeight; }
            set
            {
                Properties.Settings.Default.windowHeight = value;
                Properties.Settings.Default.Save();
            }
        }
        public static int windowWidth
        {
            get { return Properties.Settings.Default.windowWidth; }
            set
            {
                Properties.Settings.Default.windowWidth = value;
                Properties.Settings.Default.Save();
            }
        }
    }
}
