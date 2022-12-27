.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 2
.H 1 "Customizing Your Motif Environment"
Your Motif environment\*(EMand the programs you run in it\*(EMcollect
configuration information from a variety of places.  Most of this
information comes from configuration files.
.P
In this chapter you will get some hands-on experience editing your
\&.Xdefaults and .mwmrc files.  More specifically, you will learn
about:
.BL
.LI
Changing colors and fonts manually
.LI
Adding a root menu
.LI
Starting programs automatically
.LE
.P
.iX "configuration files"
Typically, configuration files are read once when a program is first
started.  The files may be read again if the program has a restart
feature.  For example, MWM has a Restart
function that makes it easy to activate changes you have made in a resource
file.
.P
If a client does not have a reset or restart feature that rereads
configuration files, you must close and restart the program yourself for
changes to take effect.
.P
After working through this chapter, you may want to learn more about the
things you can customize.  Refer to the appendixes to this guide, the
\fBmwm\fR man page, and the MWM section of
the \fIOSF/Motif Programmer's Guide\fP.
.SK
.H 2 "Changing Colors and Fonts Manually"
.iX "fonts" "changing"
.iX "customizing" "fonts"
.iX "colors" "changing"
.iX "customizing" "colors"
You can specify the colors and fonts used by a client in several ways:
.BL
.LI
Edit the system-wide defaults for the program listed in a file named
\fB/usr/lib/X11/app-defaults/\fIClientClass\fB\fR (where \fIClientClass\fR is a
file name).  You may need special permissions to edit this
file since it is used by every user on the system.
.LI
Edit your own \fB.Xdefaults\fR file (by hand or using the \fBmre\fR program).
.LI
Specify the colors and fonts on the command line that starts the client.
.LE
.H 3 "Editing Your .Xdefaults File"
Suppose you want all of your terminal windows to have a blue background,
a yellow foreground, and use the fixed font.  Follow these steps:
.AL 1
.LI
Start your text editor and open the \fB.Xdefaults\fR file in your home
directory.
.LI
Insert the following lines into the file:
.DS
\fBXTerm*foreground:       yellow
XTerm*background:       blue
XTerm*font:             fixed\fR
.DE
.LI
Save the file.  (Exit from your editor if you wish.)
.LE
.P
Now start a new terminal window by typing this command in the current
active terminal window:
.DS
\fBxterm\ &\ <Return>\fB\fR
.DE
The new window's background should be blue and the foreground (text) should
be yellow.
.SK
.H 3 "Specifying Colors and Fonts on the Command Line"
Suppose you want to start a new terminal window, but want it to have a
red background.  You can override the settings in your \fB.Xdefaults\fR file
by specifying colors on the command line.  For example, type this command:
.DS
\fBxterm\ -bg\ red\ &\ <Return>\fB\fR
.DE
Since you didn't specify a foreground color (\fB-fg\fR), the yellow value was
read from the \fB.Xdefaults\fR file.  The following table lists common
command-line options for specifying colors.  (To see which options are
actually available for a particular client, refer to the client's
documentation.)
.sp
.TB "Common Command-Line Options for Color"
.TS
allbox, center;
lb lb 
l c.
T{
To color this \&.\|.\|.
T}	T{
Use this option \&.\|.\|.
T}
Background	\f3-bg\fP
Foreground	\f3-fg\fP
Text cursor	\f3-cr\fP
Pointer	\f3-ms\fP
.TE
To specify a font on the command line, use the \fB-fn\fR option.  For
example, this command creates an \fBxterm\fR window using a font named
\fBcr.12x20\fR:
.DS
\fBxterm\ -fn\ cr.12x20\ &\ <Return>\fB\fR
.DE
The \fB12x20\fR in the font's name implies that each character in the font is
12 pixels wide and 20 pixels high, a large font.
.SK
.H 2 "Activating the Root Menu"
.iX "managing" "window manager"
.iX "window manager" "modifying"
.iX "menus" "adding selections"
.iX "adding menu selections"
.iX "deleting menu selections"
.iX "menus" "deleting selections"
.iX "system.mwmrc"
.iX ".mwmrc" "changing"
The \fBsystem.mwmrc\fR file is a text file that controls the menus and other
operations of MWM.  You can copy the file into your
home directory and edit it, just like any other text file.
.P
This section describes how to make a personal copy of \fB.mwmrc\fR and add a
root menu.  A root menu can be displayed anywhere in the root window.
Useful functions in the root menu allow you to start your own programs,
shuffle your windows, refresh your display, and restart the window manager.
.nr Hu 3
.HU "Step 1: Copying system.mwmrc into Your Home Directory"
If you aren't already there, change to your home directory and then type the
following command:
.DS
\fBcp\ \ /usr/lib/X11/system.mwmrc\ \ .mwmrc\ \ <Return>\fB\fR
.DE
This copies \fBsystem.mwmrc\fR into your home directory as \fB.mwmrc\fR.
.nr Hu 3
.HU "Step 2: Editing Your .mwmrc File"
Start your text editor and open the \fB.mwmrc\fR file you just created in
your home directory.  Search for the following line:
.DS
\fBMenu\ DefaultWindowMenu\fR
.DE
.SK

.P
Insert the following text immediately before this line:
.sp
.nf
\fBMenu\ DefaultRootMenu
{
\ \ "Root\ Menu"\ \ \ \ \ f.title
\ \ "New\ Window"\ \ \ \ f.exec\ "xterm\ &"
\ \ "Start\ Clock"\ \ \ f.exec\ "xclock\ -geometry\ 100x90-1+1\ &"
\ \ "Start\ Load"\ \ \ \ f.exec\ "xload\ -geometry\ 80x60-130+1\ &"
\ \ "Shuffle\ Up"\ \ \ \ f.circle_up
\ \ "Shuffle\ Down"\ \ f.circle_down
\ \ "Refresh"\ \ \ \ \ \ \ f.refresh
\ \ no-label\ \ \ \ \ \ \ \ f.separator
\ \ "Restart\ mwm"\ \ \ f.restart
}
.fi
.P \fR
.P
When you are done, double check your typing and then save the file.
.nr Hu 3
.HU "Step 3: Restarting MWM"
The changes you've just made to \fB.mwmrc\fR aren't read by MWM
until it is restarted.  Restart it using the reset key sequence:
.DS
\fB<Alt>\fB\ <CTRL>\fB\ <Shift>\fB\ <!>\fB\fR
.DE
.nS
You must reset the window manager twice.  The first time, MWM
restarts using its built-in defaults.  The second time, it
restarts using your configuration files.  (You can use the reset sequence
to toggle between custom and default behavior at any time.)
.nE
.H 3 "Using the Root Menu"
To display the root menu, point to the root window (the backdrop behind all
other windows), then press and hold the menu button.  Remember, the
menu button is the middle button if you are using a three-button mouse, or
both buttons simultaneously if you are using a two-button mouse.
.P
Just as with pull-down menus, to choose a function in the root menu, drag
the pointer to the desired function and then release.
.SK
.H 2 "Starting Clients Automatically"
.iX "programs" "starting"
.iX "starting clients"
Most implementations of X provide a way to customize the
clients that start automatically when you start it.  As you saw in Chapter
1, the default environment is a single terminal window.
.nr Hu 3
.HU "Step 1: Editing Your Startup File"
Check with your system administrator to find out which file is read 
when X is started on your computer.  Follow these steps to add another terminal
window (\fBxterm\fR), an analog clock (\fBxclock\fR), a load histogram
(\fBxload\fR), and the Motif Resource Editor (\fBmre\fR) to your environment:
.AL 1
.LI
In a terminal window, start your text editor and open the file that is
read when X starts.
.LI
Search for the line that starts the default \fBxterm\fR window (it will
look something like \fBxterm &\fR).  Insert these command lines just after
the existing \fBxterm\fR line:
.sp .5
.nf
\fBxterm\ -geometry\ 80x24+260+260\ &
xclock\ -analog\ -update\ 1\ -geometry\ 100x90-1+1\ &
xload\ -geometry\ 80x60-130+1\ &
mre\ -iconic\ &
.fi
.P \fR
.LI
Save the file and exit from your text editor.
.LE
.P
The first line adds an \fBxterm\fR window, the second line adds an analog
clock, the third line adds a load histogram, and the fourth line adds
\fBmre\fR.  The \fB-iconic\fR option for \fBmre\fR causes it to start as an icon.
.P
Note that all three lines end with an ampersand (\fB&\fR).  The \fB&\fR is
important.  It tells the computer to start the client as a background
process, a process that doesn't require the total attention of the
computer.  Background processing enables you to have more than one program
running at the same time.  It also frees the system for further use.
.SK
.nr Hu 3
.HU "Step 2: Viewing the Result"
Since these changes are intended to affect the programs that start when you
first start X you must exit just as you did at the end of Chapter 1.
.P
Now restart X.  Your startup environment should
now look like Figure 3-1.
.br
.ne 24P
\0
.FG "Your New Default Motif Environment"
.sp 21P
.in +.56i
.P! graphics/Screen9.ps 
.in -.56i
