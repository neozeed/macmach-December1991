.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".pn 7
...\".nr P 7
...\".nr !- 7
...\".PH ""
...\".OH "#Revision 1.0##User's Guide#"
...\".EH "#User's Guide##Revision 1.0#"
...\".OP
...\"PF "''- \\\\nP -''"
...\".af P i
...\"
...\" book stuff:
...\".OF "'''\fH\\\\nP\fR'"
...\".EF "'\fH\\\\nP\fR'''"
...\".ds !! Preface
...\".ds !@ OSF/Motif User's Guide
...\"
.nr `P 7
.ds bT "OSF/Motif User's Guide
.nr Hu 1
.HU "Preface"
.iX "OSF/Motif"
.iX "Motif"
...\" book stuff:
...\".nr P 7
...\".nr !_ 0
...\".nr Hu 2
.P
Welcome to OSF/Motif\|\s-4\uTM\d\s+4, a user environment based on the
X Window System
designed to make your interaction with your computer easier and 
more productive.
.P
This guide is intended to teach you the fundamentals of working with 
Motif\|\s-4\uTM\d\s+4.  More specifically, you will learn how to:
.BL
.LI
Create and manipulate windows on your screen
.LI
Use application programs written for the Motif environment
.LI
Personalize your Motif environment to suit your own needs 
and tastes
.LE
.SK
.nr Hu 2
.HU "Are You Ready?"
Before you begin working through the examples, take note that this guide
assumes the following:
.BL
.iX "login"
.iX "password"
.LI
You have a valid login on your computer and know the password.
.iX "mre"
.LI
The required software has been properly installed.  This includes the X
Window System, OSF/Motif, and the Motif Resource Editor program 
(\fBmre\fR).
.iX "Motif Window Manager"
.iX "X Window System"
.LI
Your system administrator has explained how to log into your  computer 
and start the X Window System and the Motif Window Manager.
.iX "text editor"
.LI
You are familiar enough with your computer to execute simple commands 
and use a text editor.
.iX "behavior" "default"
.LI
You are using the default configurations for Motif.  If you (or your
system administrator) have customized your Motif environment, the
appearance and behavior of your environment may not match the examples 
that follow.
.LI
You have a system that uses a mouse or some other pointing device.
.LE
.SK
.iX "how to use this guide"
.nr Hu 2
.HU "Tips for Using This Guide"
To get the most out of this guide, you should work your way sequentially
through the numbered chapters:
.BL
.LI
Chapter 1, \fIIntroducing Motif\fP, introduces you to the mouse and 
teaches you simple techniques for manipulating windows on your display.
.LI
Chapter 2, \fIUsing a Motif Application\fP, guides you through a simple
application and teaches you how to use common program controls such as
menus, push buttons, and scroll bars.
.LI
Chapter 3, \fICustomizing Your Motif Environment\fP, leads you through 
some simple customizations to your environment.
.LE
.nr Hu 3
.HU "Work the Examples"
You should read each chapter in a single sitting\*(EMyou'll probably 
spend between 25 and 45 minutes per chapter.  Most importantly, as you 
read the text and look at the figures, work the examples on your own 
computer.
.nr Hu 3
.HU "After the Examples"
After completing Chapter 3, you will be prepared to start using Motif in
your everyday work.  It's also likely that you will want to further
customize your own environment.  To help you get started, this guide
includes the following appendixes:
.BL
.LI
Appendix A, Specifying Colors, provides a list of color names and 
briefly describes how to specify custom colors.
.LI
Appendix B, A List of Font Names, lists the font names that are 
available for you to choose from.
.LI
Appendix C, Clients That Help You Manage Windows describes two utility
programs for the X Window System.
.LE
.P
Additional material about Motif is listed at the end of this preface.  
You do not need to read and understand this additional information to 
use Motif effectively.
.SK
.nr Hu 3
.HU "Pressing the Correct Mouse Button"
You can use Motif with either a two-button or a three-button mouse 
by observing the following conventions:
.sp
.iX "mouse button locations:
.iX "buttons" "mouse"
.iX "select button"
.iX "menu button"
.iX "custom button"
.TB "Mouse Buttons and Their Locations"
.TS
center, allbox, tab(@);
lb lb lb
l l l.
T{
Button Name
T}@T{
On a 3-button
.br
mouse press\&.\|.\|.
T}@T{
On a 2-button
.br
mouse press\&.\|.\|.
T}
Select@The left button@The left button
Menu@The middle button@Both buttons simultaneously
Custom@The right button@The right button
.TE
Chapter 1 contains more information about using the mouse.
.nr Hu 3
.HU "Typographical Conventions"
This guide uses the following typographical conventions:
.iX "conventions"
.iX "typographical conventions"
.BL
.LI
\fBBoldfaced\fR strings represent literals; type them exactly as they 
appear.
.LI
\fIItalicized\fR strings represent variable information that you 
provide.
.LI
Ellipses (\&.\|.\|.) indicate that additional arguments are optional.
.LI
<Key>\fR represents a key on the keyboard.
.LE
.SK
.nr Hu 2
.HU "Resetting Motif Behavior"
.iX "resetting"
.iX "behavior" "switching"
.iX "behavior" "default"
If the mouse buttons or other functions do not seem to be operating as
described in this guide, you can reset the entire environment to use the
default behavior.  To do this, press these four keys 
simultaneously:
.DS
\fB<Alt>\fB\ <CTRL>\fB\ <Shift>\fB\ <!>\fB\fR
.DE
A message box may appear at the center of the screen to confirm that you
want to reset the behavior.  Point to the \fBOK\fP button and click the 
left mouse button\*(EMthe select button.
.P
To return to the previous behavior, press the reset keys again.
.SK
.nr Hu 2
.HU "Other Motif Documentation"
.iX "Motif documentation"
.iX "documentation" "Motif"
As you begin to use Motif you will probably want to learn more about 
it.  The following books provide further information.
.BL
.LI
\fIOSF/Motif Programmer's Guide\fP
.LI
\fIOSF/Motif Programmer's Reference\fP
.LI
\fIOSF/Motif Style Guide\fP
.LI
\fIApplication Environment Specification\*(EMUser Environment Volume\fP
.LE
