.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr Hu 1
.HU "Glossary"
.P
.VL 2i
.LI accelerator
A key or sequence of keys (typically a modifier key and some other key)
that provides a shortcut, immediately accessing a program function.  
.LI "accelerator\ key"
A key on your keyboard that the application has assigned to directly
execute a function in a menu.  Accelerators are listed in the menu next to
the function names.
.LI "active\ window"
The terminal window in which what you type appears.  If there is no active
window, what you type is lost.  Only one terminal window can be active at
a time.
.br
.ne 7
.LI "background\ process"
A process that doesn't require the total attention of the computer for
operation.  Background processing enables the operating system to execute more
than one program or command at a time.  As a general rule, all clients
should run as background processes.
.LI "cascade\ menus"
A submenu or menu-within-a-menu, that appears when you highlight a menu
function that has an arrow next to its name.  Cascade menus
are used to group similar functions together.
.LI "check\ boxes"
A group of toggle buttons that can be set in any combination.  Check boxes
may appear in a menu or in a program's window.  Check boxes are usually
displayed as small square buttons.
.LI click
To press and release a mouse button.  The term comes from the fact that
pressing and releasing the buttons of most mice makes a clicking sound.
.LI client
A program written specifically for X.  Clients make their
own windows and know how to resize and minimize themselves.
.LI "command-line\ prompt"
Shows that the computer is ready to accept your commands.  Each terminal
emulation window has a command-line prompt that acts just like the
command-line prompt you see on the screen immediately after login (before
starting X).  Usually the command-line prompt is either a \fB$\fR (for
Bourne and Korn shells) or a \fB%\fR (for C shells).  You can find the
command-line prompt by pressing <Return>\fR several times.  Every time you
press <Return>\fR, the operating system prints the prompt.
.LI "configuration\ files"
Text files containing set-up and configuration information for X clients.  
A configuration file may be used for one particular client
or for many clients.
.LI "double-click"
To press and release a mouse button twice in rapid succession.
.br
.ne 6
.LI drag
To press and hold down a mouse button while moving the mouse on the
desktop (and the pointer on the screen).  Typically, dragging is used with
menu selecting, and window moving and resizing.
.LI fonts
A font is a style of printed text characters.  Times Roman is the font used
for most newspaper text; Helvetica is the font used for newspaper headlines.
.LI "home\ directory"
The directory in which you are placed after you log in.  Typically, this is
\fB/users/\fR\fIusername\fR, where \fIusername\fR is your login name.  The home
directory is where you keep all your files.  Type \fBcd\fR <Return>\fR to
change to your home directory.  Type \fBecho $HOME\fR <Return>\fR to view the
path and name of your home directory.
.LI icon
A small, graphic representation of an object on the root window (typically
a terminal window).  Objects can be minimized (turned into icons) to
clear a cluttered workspace and restored (returned to their original
appearance) as needed.  Processes executing in an object continue to
execute when the object is minimized.
.LI mnemonic
An underlined character in a menu.  Mnemonics allow you to display menus
and choose functions by typing letters on your keyboard.
.LI "non-client"
A program that is written to run on a terminal and so must be fooled by a
terminal emulation window into running in the window environment.
.br
.ne 9
.LI pointer
Sometimes called the mouse cursor, the pointer shows the location of the
mouse.  The pointer's shape depends on its location.  In the root window,
the pointer is an \(mu.  On a window frame, the pointer is an arrowhead.
Inside the frame, the pointer can be an arrowhead (as when it is inside a
clock or load histogram frame) or an I-beam (as when it is inside a
terminal window).
.LI "pull-down\ menu"
A menu accessed by pointing to the menu bar and pulling down with the
mouse.  Pull-down menus are generally organized by the types of functions
they contain.
.LI "push\ button"
A control that causes an immediate action.  To press a push button on the
screen, point to it then click the select button.
.LI "radio\ buttons"
A group of toggle buttons that allows you to select only one of the
selections at a time.  Radio buttons may appear in a menu or in a program's
window.  Radio buttons are usually displayed as small diamond-shaped buttons.
.LI restore
To change an icon back into its original window.
.LI "root\ menu"
The menu associated with the root window.  The root menu enables you to
control the behavior of your environment.
.LI "root\ window"
The root window is what the screen (the flat viewing surface of the
terminal) becomes when you start X.  To a certain extent, you can think
of the root as the screen.  The root window is the backdrop of your X
environment.  Although you can hide the root window under terminal windows
or other graphic objects, you can never position anything behind the root
window.  All windows and graphic objects appear stacked on the root window.
.br
.ne 6
.LI "scroll\ bars"
A control that allows you to control what portion of a larger object is
displayed within a window.  The slider represents the relative position in
the list.  The size of the slider represents the relative portion of the
list that is currently visible.
.LI "select\ button"
The mouse button used for most operations.  By default the select button is
the left mouse button.
.LI "terminal\ type"
The type of terminal attached to your computer.  The operating system uses
the terminal type to set the \fBTERM\fR environment variable so that it can
communicate with the terminal correctly.  The terminal type is usually set
at login, but can be set afterward.
.LI "terminal\ window"
A terminal window is a window that emulates a complete display terminal.
Terminal windows are typically used to fool non-client programs into
believing they are running in their favorite terminal\*(EMnot a difficult
task in most cases.  When not running programs or executing operating
system commands, terminal windows display the command-line prompt.  Two
terminal windows are supplied with X, \fBhpterm\fR, which emulates HP
terminals, and \fBxterm\fR, which emulates DEC and Tektronix terminals.
.LI "title\ bar"
The title bar is the rectangular area between the top of the window and the
window frame.  The title bar contains the title of the window object, usually
``Terminal Emulator'' for \fBxterm\fR windows, ``xclock'' for clocks, and
``xload'' for load histograms.
.LI "toggle\ buttons"
Small buttons that can be switched on and off.  To switch a toggle
button, point to it and click the select button.
.br
.ne 7
.LI "window\ manager"
The window manager controls the size, placement, and operation of windows
on the root window.  MWM includes the functional window
frames that surround each window object as well as a menu for the root
window.  MWM is a client program of the X Window System.
.LI "window\ menu"
The menu that appears when you press the window menu button on a window
frame.  Every window has a system menu that enables you to control the
size, shape, and position of the window.
.LI xclock
An X client program that displays the time, either analog (hands and
dial) or digital (text readout).
.LI xload
An X client program that displays the work load of the system as a histogram.
.LI xterm
An X client program that displays a terminal window that emulates DEC
and Tektronix terminals.
.LE
