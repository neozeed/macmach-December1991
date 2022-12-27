.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 9
.H 1 "Introduction to the OSF/Motif Window Manager"
.mc
.P
.mc |
The next seven chapters provide information for
application developers about the OSF/Motif Window Manager.
The OSF/Motif Window Manager (MWM) provides window management facilities within
.mc
the framework of the OSF/Motif environment.  MWM provides you with an
industry standard user interface, a high degree of flexibility, and a
pleasing visual interface.
.P
MWM facilitates user-computer communications in the following areas:
.P
.BL
.LI
MWM provides for direct manipulation of graphic objects using an
object-action model.  A user controls the operation of an
application program by selecting a window, menu, icon, or other graphic
object and then indicating an action to be done to that object.
.mc |
.iX "configuration files"
.mc
.LI
.mc |
MWM uses several configuration files, including \fB\&.Xdefaults\fR,
\fB\&.mwmrc\fR, and \fB\&.motifbind\fR.
By editing these files, users can customize the appearance and behavior
of MWM.
.mc
.LI
.mc |
MWM provides keyboard access to window management functionality in
cases where a mouse is not available or the user prefers keyboard access.
.mc
.LI
.mc |
MWM provides consistent appearance and behavior using the OSF/Motif
widgets as specified in the \fIOSF/Motif Style Guide\fR.
.mc
.iX "X Widgets"
.iX "widgets"
.LE
.P
.H 2 "Conventions"
.iX "conventions"
.iX "mouse" "button locations"
.iX "conventions" "mouse buttons"
The OSF/Motif Window Manager
.mc |
can be operated with either a 2-button or a 3-button mouse
by observing the conventions shown in Figure 10-1.  With a 2-button
mouse, you can obtain a third button by pressing buttons 1 and 2
simultaneously.  You can obtain five buttons from a 3-button
mouse by pressing buttons 1 and 2 for a fourth button and buttons 2
 and 3 
for a fifth button.
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 18.5P
.P
.sC graphics/butchds.tif tiff 300 18.5P 17P
\&
.P
.sp +1
.mc |
.FG "Two-Button and Three-Button Mice"
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 18.5P
\&
.sp 16P
.ce
illustration graphics/butchds.eps goes here
.sp +1
.FG "Two- and Three- Button Mice"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.75i
.mc
\0
.mc |
.FG "Two- and Three-Button Mice"
.sp 2.5i
.in +1.7i
.mc
.P! graphics/butchds.ps 
.mc |
.in -1.7i
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 16P
.P
.sC graphics/butnames.tif tiff 300 16P 24.5P
\&
.P
.sp +1
.FG "Mouse Buttons and Their Locations"
.sp +1.5
.\}
.el \{\
.br
.ne 16P
\&
.sp 14.5P
.ce
illustration graphics/butnames.eps goes here
.sp +1
.FG "Mouse Buttons and Their Locations"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2.75i
.mc
\0
.mc *
.mc
.FG "Mouse Buttons and Their Locations"
.mc |
.sp 2.5i
.in +1.06i
.P! graphics/butnames.ps 
.in -1.06i
.mc
.P
When using a keyboard instead of a mouse, follow the instructions in "Using
.mc |
a Keyboard" under the section "Getting User Input" in Chapter 11.
.H 2 "Some Window Manager Precautions"
.mc
.P
.mc |
If you are new to window manager configuring, 
you should be aware of the following conventions:
.mc
.BL
.LI
.mc |
File and resource names used by MWM are case sensitive.
Use uppercase letters where indicated and
only where indicated.  A file named \fB\&.xdefaults\fP is not the same
file as \fB\&.Xdefaults\fP.
.mc
.LI
.mc |
Do not confuse the number "1" (one) with the lowercase  "l" (el).
If you are changing to the \fB/usr/lib/X11\fP directory, you need to type "X one
.mc
one" and not "X el el."
.LI
.mc |
Do not confuse the number "0" (zero) with the uppercase "O" (oh) for the
.mc
same reason.
.LI
When typing a command, use the spacing indicated in the example. Although
.mc |
adding an extra space usually does n't cause problems, omitting a space 
definitely brings your command to a halt.
.mc
.LI
Some keys may have a different label on your keyboard.  The
.mc |
<\fBAlt\fR> key is sometimes labeled <\fBExtend\fR> or
<\fBMeta\fR>.  "Meta" is used when setting button and key bindings.
.mc
Some keys may not appear on your keyboard.  When possible use the
recommended substitute given in the following table:
.P
.mc |
.ne 4i
.TB "Special Purpose Keys and Their Substitutions."
.mc
.TS
center, allbox, tab(;);
cB cBw(1.75i)
c c.
Key;Substitution
_
.mc |
<\fBSelect\fP>;<\fBSpace Bar\fP> or <\fBEnter\fP>
<\fBMenu\fP>;<\fBF4\fP>
<\fBHelp\fP>;<\fBF1\fP>
<\fBAlt\fP>;<\fBMeta\fP> or <\fBExtend\fP>
<\fBPage up\fP>;<\fBF7\fP>
<\fBPage down\fP>;<\fBF8\fP>
<\fBEsc\fP>;<\fBF12\fP>
<\fBNext Field\fP>;T{
<\fBTab\fP> or <\fBCtrl\fP>+<\fBTab\fP>
.mc
T}
.mc |
<\fBPrevious Field\fP>;T{
<\fBShift\fP>+<\fBTab\fP> or
.br
<\fBCtrl\fP>+<\fBShift\fP>+<\fBTab\fP>
.mc
T}
.mc |
<\fBEnter\fP>;<\fBReturn\fR>
.mc
.TE
.mc |

.mc
