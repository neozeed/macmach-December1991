.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".SK
.nr H1 0
.af H1 A
.H 1 "Specifying Colors"
...\".nr !_ 0
.iX "colors" "default"
.iX "colors" "changing"
.iX "customizing" "colors"
.iX "colors" "available"
.iX "available" "colors"
.iX "hexadecimal value (color)"
.P
There are two general ways to specify a color when you are customizing your
environment:
.BL
.LI
Create a color directly using a hexadecimal number
.LI
Use a predefined color name
.LE
.P
To specify a color directly, that is,
without using a color name,
use a three-part hexadecimal number.
The three parts of the number specify the
intensity of red, green, and blue, respectively
as shown in Figure A-1.
.br
.ne 8P
\0
.FG "Specify a Custom Color Using a Hexadecimal Number"
.sp 4P
.in +2.2i
.P! graphics/HexColor.ps 
.in -2.2i
.P
.sp
The number must begin with a # and cannot include any spaces.  Each
of the three parts must use the same number of digits.  Here are some examples:
.sp
.nf
\fB#9047A5\ \ \ \ \ #F0F\ \ \ \ \ #55559999BBBB\ \ \ \ \ #0000FF\ \ \ \ \ #50A070
.fi
.P \fR
.SK
.P
When using a named color, be sure to type the name exactly as
it appears in Table A-1.  This list is contained in the
file \fB/usr/lib/X11/rgb.txt\fR on your computer.
.sp
.in 0
.TB "Available Color Names"
.TS
center;
l l l l.
Aquamarine	Black	Blue	BlueViolet
Brown	CadetBlue	Coral	CornflowerBlue
Cyan	DarkGreen	DarkOliveGreen	DarkOrchid
DarkSlateBlue	DarkSlateGray	DarkSlateGrey	DarkTurquoise
DimGray	DimGrey	Firebrick	ForestGreen
Gold	Goldenrod	Gray	Green
GreenYellow	Grey	IndianRed	Khaki
LightBlue	LightGray	LightGrey	LightSteelBlue
LimeGreen	Magenta	Maroon	MediumAquamarine
MediumBlue	MediumForestGreen	MediumGoldenrod	MediumOrchid
MediumSeaGreen	MediumSlateBlue	MediumTurquoise	MediumVioletRed
MidnightBlue	Navy	NavyBlue	Orange
OrangeRed	Orchid	PaleGreen	Pink
Plum	Red	Salmon	SeaGreen
Sienna	SkyBlue	SlateBlue	SpringGreen
SteelBlue	Tan	Thistle	Transparent
Turquoise	Violet	VioletRed	Wheat
White	Yellow	YellowGreen	
.TE
.in
.SK
.P
Table A-2 lists some of the window elements you can color.  See the documentation 
for the client you want to modify for more information.
.sp
.TB "Some Elements You Can Color"
.TS
allbox, center;
lb lb
l l.
T{
To color this element\&.\|.\|.
T}	T{
Add a color using this resource \&.\|.\|.
T}
\f3xterm\fP window text	\f3XTerm*foreground:\fP
\f3xterm\fP window background	\f3XTerm*background:\fP
\f3xterm\fP window text cursor	\f3XTerm*cursorColor:\fP
\f3xterm\fP window mouse pointer	\f3XTerm*pointerColor:\fP
Window frame text	\f3Mwm*foreground:\fP
Window frame background	\f3Mwm*background:\fP
Top and left window frame bevel	\f3Mwm*topShadowColor:\fP
Bottom and right window frame bevel	\f3Mwm*bottomShadowColor:\fP
Active window frame text	\f3Mwm*activeForeground:\fP
Active window frame background	\f3Mwm*activeBackground:\fP
Top and left active window beveling	\f3Mwm*topShadowColor:\fP
Bottom and right active window beveling	\f3Mwm*bottomShadowColor:\fP
.TE
