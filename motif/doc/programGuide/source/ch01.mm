.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 0
.H 1 "Introduction to the OSF/Motif Toolkit"
.H 2 "The OSF/Motif Toolkit and the X Window System"
The OSF/Motif Widget set is based on 
.mc
the Xt Intrinsics, a set of functions and procedures that provide
.mc |
quick and easy access to the lower levels of the X Window
system. 
You can see from Figure 1-1 that the Motif Widget system is layered
on top of the Xt Intrinsics, which in turn are layered on top of
the X Window System, thus extending the basic
abstractions provided by X.
.sp
.mc
.iX "Xt Intrinsics"
.mc |
.ne 15
.FG "User Interface Development Model"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
Box1: box height 2.25i width 3.0i
move to Box1.n
move down .125
"Application" below
move to Box1.s
up
Box2: box height 0.375i width 3.0i "Hardware Platform"
up
Box3: box height 0.375i width 3.0i "OS and Networking"
move to Box3.n
move right .125
up
Box4: box height 0.375i width 2.75i "X Window System"
move to Box4.n
move right .125
up
Box5: box height 0.375i width 2.5i  "Xt Intrinsics"
move to Box5.n
move right .125
up
Box6: box height 0.375i width 2.25i "Motif Widgets"
.PE
.ps
.DE
.mc |
.+n
.mc
.iX "Widget"
.P
.mc |
The Motif Widget 
.mc
system supports independent development of new or extended widgets.
The Motif Widget system consists of a number of different widgets, each of
which can be used independently or in combination to aid in creating
.mc |
complex applications.  You can write applications faster and with fewer
lines of code using the Motif Widgets; however, Motif Widgets require more
memory than similar applications written without them.
.mc
.P
.mc |
This guide explains the individual widgets and shows you how to 
create and use these widgets in your applications.
.mc
.H 2 "Widget Classes and Hierarchy"
.iX "Widget" "hierarchy"
.iX "Hierarchy, widget"
.iX "Widget" "classes"
.iX "Class" "widget"
Every widget is dynamically allocated and contains state information.  
Every widget belongs to one class, and each class has a structure that
is statically allocated and initialized and contains operations for that
.mc |
class.  Figure 12 shows the basic widget classes.  
.br
.ne 20 
.sp
.FG "Basic Widget Class Hierarchy"
.if n .ig +n
.mc
.DS
.ps 8
.in +1i
.PS 4.0 3.5 
boxwid = 0.75
down
box dashed ht 2.5i wid 1.5i;
move up 1.25i;
move left .75i;
line dashed left .5i;
move left .1
"\s10Core\s8" rjust;
move up 1.125i;
move right 1.35i;
down
box wid 1 "Object";
line down .25;
box wid 1"RectObj";
line down .5; move up .25; line right 1.5; line down .25;
box wid 1"XmGadget";
move up .75; move left 1.5;
line down .25;
.mc |
box wid 1 "[unnamed]";
.mc
line down .25;
line right 1.5;
line down .25;
box wid 1 "XmPrimitive";
move up .75; 
move left 1.5;
line left 1.5;
line down .25
box wid 1 "Composite";
line down .25;
line left 1.5;
line down .25;
box wid 1 "Shell";
move up .75;
move right 1.5;
line right 1.5;
line down .25;
box wid 1 "Constraint";
line down .25;
box wid 1 "XmManager"
.PE
\s10
.mc *
.mc
.DE
.mc |
.+n
.mc
.P
.iX "Core"
.iX "Composite"
.iX "Primitive"
.iX "Shell"
.iX "Constraint"
The basic class is the Core class.  It contains resources that are inherited
by all other classes.  Two classes are layered beneath the Core class,
the Composite class and the Primitive class.  The Primitive class has no
.mc |
other classes beneath it, but the Composite class has two: the Constraint 
.mc
class and the Shell class.  Each lower class can inherit some or all of 
.mc |
the resources belonging to a higher class.  For example, a Manager class widget can 
.mc
inherit some or all of the resources belonging to the Constraint class,
the Composite class, and the Core class.  You can find exactly what
.mc |
resources a given widget has by examining its manpage in the
\fIOSF/Motif Programmer's Reference\fP.
.mc
.P
.iX "Object"
.iX "RectObj"
.mc *
.mc
This section has a number of hierarchy
diagrams to help you understand how the widgets relate to each other.
.mc |
Figure 1-2 shows the highest level of widget classes.  You can see that
the Core class is composed of Object, RectObj, and an unnamed class.
Core is the base class for all other widget classes.
.mc
.P
.iX "Primitive"
.mc |
Figure 1-3 shows the subclasses of the Primitive class.
.ne 20 
.sp
.FG "Primitive Class Widgets"
.if n .ig +n
.mc
.DS
.ps 8
.in +1i
.PS 4.0 3.0
boxwid = 0.75
down
box wid 1.15 "XmPrimitive"
define list %
             line down .75
             { line right .15; box same "$1" }
%
list(XmArrowButton)
list(XmList)
list(XmScrollBar)
list(XmSeparator)
.mc |
list(XmText)
.mc
line down .75
.mc |
line right .15; box same "XmTextField"
move up 4.25
.mc
move left 1.30
line left 2
line down .25
box same "XmLabel"
line down .5
line right .15; box same "XmCascadeButton"
move left 1.30
list(XmDrawnButton)
list(XmPushButton)
line down .75
line right .15; box same "XmToggleButton"
.PE
\s10
.in -1i
.mc *
.mc
.DE
.mc |
.+n
.mc
.SK
.iX "Shell"
.mc |
Figure 1-4 shows the Subclasses of the Shell class.
.ne 10
.sp
.FG "Shell Widgets"
.if n .ig +n
.mc
.DS
.ps 8
.in +1i
.PS 4.0 3.5
boxwid = 0.75
down
box wid 1.25 "Shell"
line down
move up .25
line left 2
line down .25
box same "OverrideShell"
line down 3.5
box same "XmMenuShell"
move up 4.5
move right 2
down
box same "WMShell"
line down
box same "VendorShell"
line down .25
line right 2
line down .25
box same "TransientShell"
line down 1.5
box same "XmDialogShell"
move up 3
move left 2
line down .5
box same "TopLevelShell"
line down
box same "ApplicationShell"
.PE
\s10
.in -1i
.mc *
.mc
.DE
.mc |
.+n
.mc
.SK
.iX "Manager"
.mc |
Figure 1-5 shows the Manager class widgets.  Note from Figure 1-2 that
.mc
Manager is a subclass of Composite and Constraint.
.mc |
.ne 10
.sp
.FG "Manager Widgets"
.if n .ig +n
.mc
.DS
.ps 8
.in +.25i
.PS 5.0 3.5
boxwid = 1.00
down
box wid 1.25 "XmManager"
line down .5
line left 2.25
line down .25
box same "XmBulletinBoard"
move up .75 
move right 1.5
line down .25
box same "XmRowColumn"
define list %
     line down .25
     { line right .15; move right .1; "$1" ljust }
     %
move left .375
list(XmCreateMenuBar) 
list(XmCreateOptionMenu)
list(XmCreatePopupMenu)
list(XmCreatePulldownMenu)
list(XmCreateRadioBox)
.mc |
list(XmCreateWorkArea)
.mc
move up 2
move right .75
line right 2.625
line down .25
box same "XmScrolledWindow"
list(XmCreateList)
list(XmCreateText)
list(XmCreateScrolledList)
list(XmCreateScrolledText)
line down .25
box same "XmMainWindow" 
move up 2.5
move left 1.5
line down 3.0
line right 2.25
line down .25
box same "XmScale"
move up .75
move left 1.5
line down .25
box same "XmFrame"
move up .75
line left 1.5
line down .25
box same "XmPanedWindow"
move up .75
line left 1.5
line down .25
box same "XmDrawingArea"
.PE

\s10
.in -.25i
.mc *
.mc
.DE
.mc |
.+n
.mc
.SK
.mc |
Figure 1-6 shows the Dialog widgets that are a subclass of Manager.  Note
.mc
that all of the Dialog widgets are subclasses of BulletinBoard.  Also,
note the convenience functions that are present.  These are explained
.mc |
in detail in Chapter 5, ``Dialog Widgets and Functions.''
.ne 10
.sp
.mc
.iX "Dialog widgets"
.mc |
.FG "Dialog Widgets"
.if n .ig +n
.mc
.DS
.ps 8
.in +.5i
.PS 4.75 3.25
boxwid = 1.00
down
box wid 1.25 "XmManager"
line down .25
box same "XmBulletinBoard"
define list %
     line down .25
     { line right .1; move right .05; "$1" ljust }
     %
list(XmCreateBulletinBoardDialog)
line down .25
move left 1.75
line down .25
box same "XmSelectionBox"
move left .5
list(XmCreatePromptDialog)
list(XmCreateSelectionDialog)
line down .25
line left .75
line down .25
box same "XmFileSelectionBox"
move left .5
list(XmCreateFileSelectionDialog)
move up 1
move right 1.25
line right .75
line down .25
box same "XmCommandBox"
move up 2.25
move left .25
line right 1.75
line down .25
box same "XmMessageBox"
move left .5
list(XmCreateErrorDialog)
list(XmCreateInformationDialog)
list(XmCreateMessageDialog)
list(XmCreateQuestionDialog)
list(XmCreateWarningDialog)
list(XmCreateWorkingDialog)
move up 2.25
line right 2.25
line down .25
box same "XmForm"
move left .5
list(XmCreateFormDialog)
.PE
\s10
.in -.5i
.mc *
.mc
.DE
.mc |
.+n
.mc
.SK
.mc |
Figure 1-7 shows the gadgets that are an integral part of the Motif
.mc
toolkit.  
.mc |
.ne 5
.sp
.FG "Gadgets"
.if n .ig +n
.mc
.DS
.ps 8
.in +1i
.PS 4.0 3.0
boxwid = 1.0
down
box wid 1.65 "XmGadget"
define list %
             line down .75
             { line right .15; box same "$1" }
%
list(XmArrowButtonGadget)
line down .75
line right .15; box same "XmSeparatorGadget"
move up 1.25
move left 1.80
line left 2
line down .25
box same "XmLabelGadget"
line down .5
line right .15; box same "XmCascadeButtonGadget"
move left 1.80
.mc |
list(XmPushButtonGadget)
.mc
line down .75
line right .15; box same "XmToggleButtonGadget"
.PE
\s10 
.in -1i
.mc *
.mc
.DE
.mc |
.+n
.ne 6i
.mc
.H 2 "Compiling Sample Programs"
.iX "Compiling" "sample programs"
.mc |
There are a number of sample programs discussed throughout this guide.
.mc
The source code for most of these programs can be found in the 
.mc |
\fB ./demos\fP directory.  There is also a Makefile in this directory that
.mc
you can use to compile and link the programs.  Follow this procedure
to compile and link a program.
.AL
.LI
Copy the program source code file and the Makefile found
in \fB ./demos\fP 
to your work directory.
.mc |
Do not attempt to compile the program in the \fB ./demos\fP
.mc
directory.
.LI
Compile the program by executing the following command:
.mc |
.sp .5
\fBmake\fP \fIprogramname\fP
.sp .5
.mc
.LI
If there is a defaults file commented into the beginning of the source
.mc |
code, move that defaults file to the 
\fB/usr/lib/X11/app-defaults\fP directory before you run the program.  

.mc
