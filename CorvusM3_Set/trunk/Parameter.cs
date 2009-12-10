using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace CorvusM3
{
    public class Parameter
    {
        string color;
        string color1;

        [CategoryAttribute("Test"), DescriptionAttribute("das ist die Hilfe")]
        public string Color
        {
            set 
            {
                color = value;
            }
            get 
            { 
                return color; 
            }
        }
        [CategoryAttribute("Test"), DescriptionAttribute("das ist die Hilfe")]
        public string Color1
        {
            set
            {
                color1 = value;
            }
            get
            {
                return color1;
            }
        }
    }
}
