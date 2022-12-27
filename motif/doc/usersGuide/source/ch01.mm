.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".pn 1
...\".OP
...\"	for the book:
...\"
...\".OF "'''\fH\\\\n(H1\(mi\\\\nP\fR'"
...\".EF "'\fH\\\\n(H1\(mi\\\\nP\fR'''"
...\"
...\".af P 1
.nr H1 0
.H 1 "Introducing Motif"
.P
In this chapter you will work through examples that teach you how to:
.BL
.LI
Point, click, and drag using the mouse
.LI
Select a window to make it active
.LI
Move a window to a new position on the screen
.LI
Change the size of a window
.LI
Minimize a window into an icon
.LI
Restore an icon back into a window
.LI
Use the window menu
.LE
.P
Before you start, you must log into your computer.  Your system
administrator should have created an account for you.
.SK
.iX "Motif Window Manager"
.H 2 "Introducing the Motif Window Manager"
The Motif Window Manager (MWM) is a program that runs with the X Window
System and helps you manage the windows on your screen.  MWM
provides functions for opening, closing, moving, and resizing
windows.  It's MWM that displays the three-dimensional
frame around each window.
.P
Without MWM, your windows would not have borders and it
would be difficult, if not impossible, to perform many simple window
functions.
.H 3 "Starting the Window Manager"
Your system administrator should have configured your account so that 
MWM starts automatically when you start X.  Your account may also be 
configured so that X starts automatically when you log in.
.P
Since different computer systems have different ways of starting X and
Motif, you should consult with your system administrator to learn
how to get started.
.P
Start your Motif environment now.
.SK

.H 3 "The Default Environment"
In combination, X and MWM comprise the Motif environment.  The default
Motif environment includes a single terminal emulator window in the
upper-left corner of the display.  If your screen looks like Figure 1-1,
you have successfully started Motif and are ready to continue with
this guide.
.br
.ne 19P
\0
.FG "The Default Motif Environment Is One Terminal Window"
.sp 17P
.in +.69i
.P! graphics/Screen0.ps 
.in -.69i
.SK

.iX "mouse" "using"
.H 2 "Using the Mouse"
When X starts, an \(mu-shaped pointer appears at the center of the
screen.  As you move the mouse on your desktop, the pointer on the screen
moves correspondingly.
.P
Whenever this guide tells you to ``point to'' something, it simply means move
the mouse until the pointer on your screen is positioned over the
``something.''
.H 3 "Pointer Shapes"
.iX "window" "root"
.iX "mouse" "pointer"
.iX "pointer shapes"
The location of the pointer can cause the shape of the pointer to change.
For example, when the pointer is directly over the root window (the backdrop
behind all windows), the pointer has an ``\(mu'' shape.  When you point to
the inside of a terminal window, the pointer changes to an ``I'' shape.
See Figure 1-2.
.br
.ne 11P
\0
.FG "Pointer Shapes Provide Visual Cues to Activity"
.sp 8P
.in +.94i
.P! graphics/Pointers.ps 
.in -.94i
.H 3 "Clicking, Double-Clicking, and Dragging"
The following terminology is used to refer to actions involving mouse buttons:
.iX "click"
.iX "double-click"
.iX "drag"
.BL
.LI
``Click'' means to press and release a button without moving the pointer
.LI
``Double-click'' means to click a button twice in rapid succession
.LI
``Drag'' means to press and hold a button while moving the pointer
.LE
.SK

.iX "window" "active"
.iX "active window"
.H 2 "Selecting a Window"
Before a window can receive input from you, it must be selected as the
active window.  To select a window, point to any part of the window and
click the select button on the mouse.
.P
Select the terminal window on your screen now.  Point to the window and
then click the select button.  (Remember, by default the select button is
the left mouse button.)
.P
When a window is active, the window frame changes color.  Now, when you
press on the keyboard, the characters appear on the command line in the
active window.
.P
If no window is active, everything you type is lost.
.SK

.iX "windows" "moving"
.iX "moving a window"
.H 2 "Moving a Window"
Using the pointer, you can move a window to a new location on the screen.
To do so, grab the window's title bar with the pointer, drag it to a new
location, and then release the window.
.nr Hu 3
.HU "Step 1: Grabbing the Title Bar With the Pointer"
.iX "pointer"
.iX "terminal emulator"
.iX "title bar"
.iX "grabbing the title bar"
Point to the title bar of the window.  The title
bar is the rectangular area across the top of the window where the words
``Terminal Emulator'' appear.  When properly positioned, the pointer is an
arrowhead as shown in Figure 1-3.
.br
.ne 8P
\0
.FG "Grab the Title Bar With the Pointer"
.sp 5P
.in +1.44i
.P! graphics/TitleBar.ps 
.in -1.44i
.P
When the pointer is positioned, press and hold the select button.
.SK
.nr Hu 3
.HU "Step 2: Dragging the Window to a New Location"
.iX "windows" "moving"
.iX "dragging a window"
.iX "moving a window"
Still holding the select button, slide the mouse to the right across your
desktop.  Now slide the mouse toward you.
.P
As you move the mouse, the pointer on the screen drags an outline of the
terminal window as shown in Figure 1-4.  The outline shows you where the window 
will be moved when you release the mouse button.  The column and row location 
of the window's upper left corner are displayed in a small box in the center of 
the screen.
.br
.ne 20P
\0
.FG "An Outline Shows Where the Window Will Reappear"
.sp 17P
.in +.69i
.P! graphics/Screen1.ps 
.in -.69i
.nr Hu 3
.HU "Step 3: Releasing the Window"
.iX "releasing a window"
.iX "stopping a window move"
Move the outline to the center of the screen.  When the outline is where you want
it for now, release the mouse button.  The window moves to the new
position.
.SK

.H 2 "Changing the Size of a Window"
.iX "windows" "changing sizes"
.iX "resizing windows"
You can change the size of a window by grabbing an area of the
window's frame with the pointer, dragging the frame to the desired
size, and then releasing the frame.
.nr Hu 3
.HU "Step 1: Grabbing the Frame With the Pointer"
Where you grab the window's frame determines how the window will be
resized.  See Table 1-2.
.sp
.TB "Places to Grab on the Window Frame"
.TS
allbox, center, tab(@);
lb lb
l s
c l
c l
l s
c l
c l
l s
c l.
T{
.ce
If you want to stretch
.ce
or shrink the window \&.\|.\|.
T}@T{
.ce
Point to \&.\|.\|.
T}
vertically from the \&.\|.\|.@
top@top of the frame, above the title bar
bottom@bottom of the frame
horizontally from the \&.\|.\|.@
right@right side of the frame
left@left side of the frame
diagonally from the \&.\|.\|.@
bottom left corner@frame's lower left corner
top left@frame's upper left corner
top right@frame's upper right corner
bottom right@frame's lower right corner
.TE
For this exercise, point to the lower right corner of the
window frame.  When positioned correctly, the pointer's shape changes
to an arrowhead pointing into the corner of the frame.  Grab the frame by
pressing and holding down the select button.
.SK
.nr Hu 3
.HU "Step 2: Enlarging or Shrinking the Window"
.iX "enlarging a window"
.iX "shrinking a window"
With the select button still pressed, slide the mouse so that the pointer
moves away from the window.  Now slide the mouse so the pointer moves
toward the window.  As you move the mouse, the pointer on the screen drags
an elastic outline of the window.  The outline shows you the new size of
the window.
.P
The dimensions of the window (width \(mu height) are shown in a small
box in the center of the screen as shown in Figure 1-5.  For a terminal window, 
the measurements are in characters (across) and lines (down); for other types of 
windows, the measurements are in pixels (dots on the screen).
.br
.ne 20P
\0
.FG "A Window and Its Elastic Outline"
.sp 17P
.in +.69i
.P! graphics/Screen2.ps 
.in -.69i
.P
Now, stretch the window until it reaches the lower right corner of the root
window as shown in Figure 1-5.
.nr Hu 3
.HU "Step 3: Releasing the Frame"
When the elastic outline is the correct size, release the mouse button.
The screen is redrawn with the window filling the outline.
.SK

.H 2 "Changing a Window Into an Icon"
.iX "windows" "changing to icons"
.iX "icons"
As you work, your screen can become cluttered with windows.  Changing a few
of those windows into icons allows you to tidy up a cluttered
workspace.  Icons are small graphical images.  Programs executing in a
minimized window continue to execute until they finish or halt because they
require input from you.
.nr Hu 3
.HU "Step 1: Locating the Minimize Button"
Slide the mouse so that you position the pointer on the minimize button\*(EMthe small 
square to the immediate right of the title bar on the window frame as shown in
Figure 1-6.
.br
.ne 8P
\0
.FG "The Minimize Button Is Immediately to the Right of the Title Bar"
.sp 5P
.in +1.44i
.P! graphics/MinBttn.ps 
.in -1.44i
.nr Hu 3
.HU "Step 2: Pressing the Minimize Button"
.iX "icons" "making"
Press the minimize button by clicking the select button.
.P
The window changes into an icon.  MWM displays icons in a row along the
bottom of the root window, starting on the left.  An icon has two parts,
as shown in Figure 1-7.
.br
.ne 11P
\0
.FG "Icons Have Two Parts; An Image and a Label"
.sp 8P
.in +2.3i
.P! graphics/TermIcon.ps 
.in -2.3i
.SK

.H 3 "Moving an Icon"
To move an icon around the screen, follow these steps:
.iX "moving an icon"
.iX "icons" "moving"
.AL 1
.LI
Point to the icon.
.LI
Press and hold the select button.
.LI
Still holding the select button, drag the pointer.  An outline follows the
pointer, showing where the icon will be repositioned.
.LI
To position the icon, release the mouse button.
.LE
.P
Icons are positioned on a grid, so the icon you move may jump a bit if
you set it too close to the margin of the root window or too close to
another icon.
.SK

.H 2 "Restoring a Window From an Icon"
.iX "icons" "normalizing"
.iX "windows" "normalizing from icons"
.iX "normalizing"
To restore an icon (change it back into a window), point to the icon
and double-click the select button.  To double-click a mouse button,
press the button twice in rapid succession.
.nr Hu 3
.HU "Step 1: Pointing to an Icon to Restore"
Point to the icon by positioning the mouse pointer in the icon.
If you can't identify the icon you want to restore because some
icon names are obscured within the icons' title areas, you can
display an entire label by clicking the select button in an icon's
title.
.br
.ne 20P
\0
.FG "An Icon and its Pop-up Label"
.sp 17P
.in +.69i
.P! graphics/Screen3.ps 
.in -.69i
.SK

.nr Hu 3
.HU "Step 2: Changing the Icon Into a Window"
While pointing to the icon, double-click the select button.  That is, press
the left mouse button twice in rapid succession.
.P
The icon changes back into the window it originally came from.  The
window positions itself where it was before being minimized,
as shown in Figure 1-9.
.br
.ne 20P
\0
.FG "The Window is Restored"
.sp 17P
.in +.69i
.P! graphics/Screen4.ps 
.in -.69i
.SK

.H 2 "Displaying a Window Menu and Making Selections"
.iX "window menu"
.iX "menus" "window"
.iX "menu functions" "choosing"
.iX "windows" "operations control"
Each window has a window menu that contains functions for controlling
the window.
.nr Hu 3
.HU "Step 1: Displaying the Window Menu"
The window menu is attached to the window frame and you use the
menu button in the upper-left corner of each window to display it.
.iX "displaying the window menu"
.P
To display the window menu, point to the menu button and then press and
hold the select button.  The menu is displayed as long as you hold the
button down\*(EMdon't release the button yet.  See Figure 1-10.
.br
.ne 17P
\0
.FG "The Window Menu Is an Easy Way to Control a Window"
.sp 14P
.in +1.2i
.P! graphics/WindMenu.ps 
.in -1.2i
.P
.nr Hu 3
.HU "Step 2: Choosing a Function From the Menu"
.iX "selecting from a menu"
.iX "choosing from a menu"
While still holding the select button down, drag the pointer down the menu.
As the pointer moves, it highlights the button for each available selection.
Drag the pointer until you highlight the Maximize function.  Release
the mouse button.
.P
The Maximize function causes the window to expand to fill the entire screen.
.SK
.nr Hu 3
.HU "Step 3: Restoring the Window to its Original Size"
.iX "undoing a menu selection"
.iX "menu functions" "undoing"
Display the window menu again (point to the window menu button in the
upper-left corner of the window and press the select button).  Drag the pointer
down the menu until you highlight the Restore selection.  Release the
mouse button.  The window is restored to its former size and location.  Table 1-3
lists the window menu functions.
.H 3 "Summary of Window Menu Functions"
.TB "Functions in the Window Menu"
.TS
center;
lbw(3i) lb
l l.
T{
To do this \&.\|.\|.
T}	T{
Choose \&.\|.\|.
T}
Restore a window from an icon or after maximizing.	Restore
Change the location of the window.	Move
Change the size of the window.	Size
Shrink the window to its icon representation.	Minimize
Enlarge the window to cover the entire root window.	Maximize
Send a window to the back or bottom of the window
stack, the position closest to the root window.	Lower
Immediately stop the window and make it disappear.	Close
.TE
.nS
When a menu function is meaningless, its name is grayed out and you cannot
select it.  For example, the Restore function in Figure 1-10 is inactive.
.nE
Rather than choosing the Maximize and Restore functions in the previous
example, you could have used the Maximize button in the upper right
corner of the window, immediately to the right of the Minimize button.  Try
it if you want.
.SK

.H 2 "Raising a Window"
To bring a partially concealed window to the front of the root window,
click on the window's frame.
.P
To demonstrate this, create another terminal window by typing this command
in the existing terminal window:
.DS
\fBxterm\ &\ <Return>\fB\fR
.DE
The new window appears in the upper-left corner of the display, partially
covering the first window as shown in Figure 1-11.
.nr Hu 3
.HU "Step 1: Positioning the Pointer"
Position the pointer on any visible portion of the concealed window's frame.
.br
.ne 20P
\0
.FG "A Partially Concealed Window"
.sp 17P
.in +.69i
.P! graphics/Screen7.ps 
.in -.69i
.SK
.nr Hu 3
.HU "Step 2: Pressing the Select Button"
Click the select button on the mouse.
.P
The partially concealed window moves to the front of all other windows as shown
in Figure 1-12.
.br
.ne 20P
\0
.FG "After Clicking on the Frame, the Window Is No Longer Concealed"
.sp 17P
.in +.69i
.P! graphics/Screen8.ps 
.in -.69i
.SK

.H 2 "Quitting Motif"
.iX "commands" "quitting Motif"
.iX "exiting Motif"
.iX "stopping Motif"
.iX "leaving Motif"
.iX "quitting Motif"
.iX "Motif" "quitting"
Before exiting the window environment, follow the recommended
procedure to exit any application programs and stop any commands that may be
running in terminal windows.  When you have a command prompt in each terminal
window, follow your system administrator's instructions for quitting Motif.
.nr Hu 3
.HU "Step 1: Preparing to Exit"
First, exit any application programs running in terminal windows. This
avoids the possible loss of data due to improperly stopping a program.
.P
When you exit a program, the command-line prompt returns to the
terminal window.  (However, if you started the program automatically or
from a menu, exiting will also remove the terminal window.  This is proper
behavior.)
.nS
Choosing Close from the window menu immediately stops any
program running in the window.  Interrupting a program like this may cause
it to lose data.  For example, if you were running a word processor in the
window, you could lose any unsaved changes.  However, you can close the
clock, the load histogram, or an ``idle'' terminal window (one showing a
command prompt) with no ill effect.
.nE
.nr Hu 3
.HU "Step 2: Stopping X and the MWM"
After exiting all programs, follow the procedure given to you by your
system administrator to stop your system.
.P
This removes from the screen all terminal windows (as well as the windows
for clocks, load histograms, and similar windowed programs).  A moment
later the root window disappears.
.P
If a command-line prompt appears, you have exited the X Window System, but
you are still logged into your computer.  If you see \fBlogin:\fR, then you
have completely logged out of your computer.
