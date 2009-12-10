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
