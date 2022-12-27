.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr Hu 1
.HU "Glossary"
.iX "Glossary"
.VL 2i
.LI "\fBaccelerator\fR"
.iX "Accelerator"
A keyboard key or keys used to cause some action to
occur.  For example, the 
<\fBShift\fP><\fBMenu\fP>
keys could be used to post a menu instead of a mouse button action.
.LI "\fBactive\ window\fR"
The terminal window where what you type appears.  If there is no active
window, what you type is lost.  Only one terminal window can be active at
a time.
.LI "\fBampersand\ (&)\fR"
Placed at the end of a command, an ampersand specifies that the client
started by the command should be started as a background process.
.LI "\fBapplication\ program\fR"
A computer program that performs some useful function, such as word
processing, computer-aided design, or data base management.
.br
.ne 3
.LI "\fBapplication\ server\fR"
A computer used solely to provide processing power for application
programs.
.LI "\fBatom\fR"
.iX "Atom"
A 32-bit number that represents a string value.
.LI "\fBbackground\ process\fR"
A process that does not require the total attention of the computer for 
operation.  Background processing enables the operating system to execute
more than one program or command at a time.
.LI "\fBbitmap\fR"
An array of data bits used for graphic images.  Strictly speaking, a pixmap
of depth one (capable of two-color images).
.LI "\fBbitmap\ device\fR"
An output device, such as a Cathode Ray Tube (CRT), that displays bitmaps.
.LI "\fBbutton\ (mouse)\fR"
A button on a mouse pointing device; mouse buttons can be mapped to the
keyboard.
.LI "\fBbutton\ (window\ frame)\fR"
A graphical control that simulates a real-life pushbutton.  The pointer
and the mouse are used to ``push'' the button and perform some action.
.LI "\fBbutton\ binding\fR"
Association of a mouse button operation with a window manager or
application function.
.LI "\fBcallback\fR"
.iX "Callback"
A procedure that is called if and when certain specified
conditions are met.  This is accomplished by specifying the procedure in a
callback list.  Individual widgets can define callback lists as required.
.LI "\fBcancel\fR"
A label given to a push button in some dialog boxes that performs the
action of closing the dialog box without implementing any changes.
.LI "\fBcascading\ menu\fR"
A submenu that provides selections that amplify the parent selection
on a pull-down or pop-up menu.
.br
.ne 7
.LI "\fBchild\ widget\fR"
.iX "Widget" "child"
.iX "Child widget"
A child widget is a subwidget of a composite widget.  The
composite
widget is referred to as the \fIparent\fP of the child widget.  The parent
controls where the child is placed and when it is mapped.  If the parent is
destroyed, the child is automatically destroyed.
.LI "\fBchild\ window\fR"
A window that is not a primary window, but rather is an offspring of a 
primary or secondary window.
.LI "\fBclass\fR"
.iX "Class"
The general group that a widget belongs to.
.LI "\fBclick\fR"
To press \fIand release\fR a mouse button.  The term comes from the fact that
pressing and releasing the buttons of most mice makes a clicking sound.
.LI "\fBclient\fR"
An application program written specifically for the X Window System.  Some
clients make their own windows.  Other clients are utility programs.
.LI "\fBclient\ area\fR"
The area within the borders of a primary window's frame that is controlled
by an application.
.LI "\fBclose\fR"
A label given to a push button in some dialog boxes that performs the
action of closing the dialog box.  Close is also used as a selection in
menus to close the window associated with the menu.
.LI "\fBcolormap\fR"
A display resource that controls the set of colors appearing on the
display.
.LI "\fBcomposite\ manager\fR"
.iX "Widget" "composite manager"
.iX "Composite manager widget"
A composite manager is a manager widget
with special knowledge
about the handling of one or more particular widgets.  For example, a TitleBar
and ScrollBar can be registered with a Panel widget, and the Panel widget will
position the TitleBar and ScrollBar widgets correctly.  Normally, a manager
widget has no knowledge about its children.
.br
.ne 4
.LI "\fBComposite\fR"
.iX "Widget" "composite"
.iX "Composite widget class"
This class provides the resources and
functionality that allows subclass widgets to manage the layout and children.
.LI "\fBconstraint\fR"
.iX "Constraint"
Resources that certain manager widgets can impose on 
their children are called \fIConstraint\fP resources.  For example, if a
PanedWindow widget wants its children to be a certain size, it can 
specify the size by using the resources \&\fBXtNmin\fP and \&\fBXtNmax\fP.
The manpages will specify those manager widgets that have Constraint resources.
.LI "\fBcontrol\ key\fR"
The keyboard key usually labeled <\fBCtrl\fR> and used as a modifier key.
.LI "\fBconvenience\ dialog\fR"
A widget or collection of widgets created by a Dialog convenience
function.
.LI "\fBconvenience\ function\fR"
.iX "Convenience function"
A convenience function is a function that creates certain combinations
of widgets, including the necessary Shell widget.  
.LI "\fBCore\fR"
.iX "Core"
Core is the basic class from which all widgets are built.  It
acts as a superclass for other widget classes and provides resources that
are required by all widgets.
.LI "\fBcursor\fR"
A graphical image, usually an I-beam or rectangle, that shows the location
where text will appear on the screen when keys on the keyboard are typed or
where a selection will be made using the Select mouse button or
the <\fBSelect\fR> key.
.LI "\fBdefault\ (selection)\fR"
An object or action that is specified for selection if no other selection
is specified.
.LI "\fBdialog\fR"
.iX "dialog"
A collection of widgets, including a DialogShell, a BulletinBoard (or a
subclass of BulletinBoard or some other container widget), plus various
children of BulletinBoard such as Label, PushButton, and Text widgets.
Dialogs are used as an interface between the application and its user.
.br
.ne 3
.LI "\fBdialog\ box\fR"
A secondary window that the user can display and that
contains application controls.
.LI "\fBdisplay\fR"
Strictly speaking, the combination of a keyboard, mouse, and one or more
screens that provide input and output services to a system.  While
``display'' is sometimes used to mean just the CRT screen, a display, as
defined here, can actually include more than one physical screen.
.LI "\fBdouble-click\fR"
To press \fIand release\fR a mouse button twice in rapid succession.
.LI "\fBdrag\fR"
To press \fIand hold down\fR a mouse button while moving the mouse.
.LI "\fBgrab\fR"
.iX "Grab"
A procedure by which a window will act upon a key or button event
that occurs for it or any of its descendents.  This precludes the necessity of
setting up translations for all windows.
.LI "\fBgraphical\ user\ interface\fR"
A form of communication between people and computers that uses
graphics-oriented software such as windows, menus, and icons to ease the
burden of the interaction.
.LI "\fBgrayed\ selection\fR"
A menu selection that is not currently available and so has been dimmed.
.LI "\fBhighlight\fR"
A graphic technique used to provide a visual cue to the current selection
or to the current location of the input focus.  Highlighting is frequently
accomplished by reversing the video image of the selection.
.br
.ne 5
.LI "\fBicon\fR"
A small graphic image used to represent a window.  Windows can be turned
into icons (minimized) to save room or unclutter the workspace.
.LI "\fBinactive\fR"
A window that does not have the input focus.
.LI "\fBinsertion\ cursor\fR"
The graphical symbol that provides the visual cue to the location of the
insertion point.
.LI "\fBinstantiate\fR"
.iX "Instantiate"
To represent an abstraction by a concrete instance.  To
instantiate a widget means that a widget class creates an instance of that
class.
.LI "\fBintern\fR"
.iX "Intern"
The procedure used to define an atom.
.LI "\fBkeyboard\fR"
One of many input devices; the traditional method of entering text into an
application.
.LI "\fBlabel\fR"
The text part of an icon.
.LI "\fBlocation\ cursor\fR"
A graphic symbol that marks the current location of the keyboard
input focus for selection.  Typically, this symbol is a box that surrounds
the current object.  The location cursor is sometimes known as the
selection cursor.
.LI "\fBlower\fR"
To move a window to the bottom of the window stack on the workspace.
.LI "\fBmanager\ class\fR"
.iX "Widget" "manager"
.iX "Manager"
A class that provides the resources and
functionality to implement certain features, such as keyboard interface and
traversal mechanism.  It is built from core, composite, and constraint
classes.
.LI "\fBmatte\fR"
An optional decorative border between the client area and the window frame,
similar to a matte used in framing real-life pictures.
.LI "\fBmaximize\fR"
To enlarge a window to its maximum size.
.LI "\fBmaximize\ button\fR"
A control button placed on the MWM window frame and used to initiate the
maximize function.
.br
.ne 3
.LI "\fBmenu\fR"
A list of available selections from which a user chooses.
.LI "\fBmenu\ bar"
A rectangular area at the top of the client area of a window that contains
the titles of the standard pull-down menus for that application.
.LI "\fBmessage\ box\fR"
The generic name for any dialog box that provides information, gives the
current state of a work in progress, asks a question, issues a warning, or
draws attention to an error.
.LI "\fBmeta\ class\fR"
.iX "Class" "meta"
.iX "Meta class"
A meta class is a set of structures and functionality that 
a widget uses to export that functionality to subclass widgets.  Each instance
of a widget subclass will have the features common to that widget class and
will export these features to child widgets of that class.
Included in this class are Core, Composite, Constraint, Primitive, Button,
Manager, MenuMgr, and MenuPane.
A meta class widget is never instantiated.
.LI "\fBminimize\fR"
To turn a window into an icon.  The term "iconify" is sometimes used
instead of minimize.
.LI "\fBminimize\ button\fR"
A control button placed on the MWM window frame and used to initiate the
minimize function.
.LI "\fBmnemonic\fR"
A single character (frequently the initial character) of a menu selection,
which when the menu is displayed and the character is typed on the
keyboard, initiates the selection.
.LI "\fBmodal\ dialog\fR"
A Dialog that interrupts the work session to solicit input from the user.
.LI "\fBmodeless\ dialog\fR"
A Dialog that solicits input from the user but does \fInot\fP interrupt the
work session.
.LI "\fBmodifier\ key\fR"
A key that, when pressed with another key, changes the meaning of the other
key.  <\fBCtrl\fR>, <\fBAlt\fR>, and <\fBShift\fR> are modifier keys.
.br
.ne 4
.LI "\fBmouse\fR"
A pointing device commonly used in conjunction with a keyboard in
point-and-click, object-oriented user interfaces.
.LI "\fBmouse\ button\fR"
One of the buttons on a mouse pointing device.  Mouse buttons can be
pressed, released, dragged, clicked, and double-clicked.
.LI "\fBnormalize\fR"
To change an icon back into its normal (original) client window
appearance.  The opposite of iconify.
.LI "\fBopen\fR"
To start an action or begin working with a text, data, or graphics file.
.LI "\fBpersistence\fR"
.iX "Persistence"
Persistence means that a specified character set is used for all subsequent
text segments in a compound string until a new character set is
encountered.
.LI "\fBpixel\fR"
Short for picture element.  The individual dots, or components, of a
display screen.  They are arranged in rows and columns and form the images that are
displayed on the screen.
.LI "\fBpixmap\fR"
An array of data bits used for graphics images.  Each pixel (picture
element) in the map can be several bits deep, resulting in multicolor
graphics images.
.LI "\fBpoint\fR"
To position the pointer or location cursor.
.LI "\fBpointer\fR"
The graphical image that appears on the workspace and represents the
current location of a mouse or other pointing device. 
.LI "\fBpointing\ device\fR"
A device such as a mouse, trackball, or graphics tablet that allows users 
to move a pointer about on the workspace and point to graphical objects.
.LI "\fBpop-up\fR"
.iX "Pop-up"
A type of widget that appears as the result of some user
action (usually
clicking a mouse button) and then disappears when the action is
completed.
.br
.ne 8
.LI "\fBpop-up\ menu\fR"
A menu that provides no visual cue to its presence, but simply pops up
when users perform a particular action.  Pop-up menus area associated with
a particular area of the workspace, such as the client area of an
application, and users must memorize where these areas are.
.LI "\fBpost\fR"
.iX "Post"
The action required to make a pop-up or pull-down menu appear.
This action is normally a click or button press on one of the mouse buttons.
.br
.ne 4
.LI "\fBpress\fR"
To hold down a mouse button or a key.  Note that to hold down a mouse
button \fIand move\fR the mouse is called "dragging." 
.LI "\fBprimary\ window\fR"
A top-level window of an application.  Primary windows can be minimized.
.LI "\fBPrimitive\fR"
.iX "Widget" "primitive"
.iX "Primitive"
The primitive class provides the resources
and functionality for the low-level widgets that are managed by the manager
class.  Primitive class widgets cannot have
normal child widgets, but they can have pop-up child widgets.
.LI "\fBproperty\fR"
.iX "Property"
Public information (that is, information that is available to any client) 
associated with a window.
.LI "\fBprotocol\fR"
.iX "Protocol"
A mutually agreed upon mechanism for communicating between clients to
accomplish certain actions.
.LI "\fBpull-down\ menu\fR"
A menu that is pulled down from a client application's title bar.
.LI "\fBpush\ button\fR"
A graphic control that simulates a real-life push button.  The
pointer and mouse are used to push the button and start some action.
.LI "\fBrelease\fR"
To let up on a mouse button or key that has been pressed.  Sometimes it is
the press that initiates the action; sometimes it is the release.
.br
.ne 2
.LI "\fBresize\fR"
To change the height or width of a window.
.LI "\fBresize\ border\fR"
See \fBresize frame handles\fR.
.LI "\fBresize\ frame\ handles\fR"
The MWM frame part that surrounds the client area of an application and
that is used to change the height or width of the window.
.LI "\fBresource\fR"
A program parameter that controls an element of appearance or behavior.
Resources are usually named for the elements they control.
.LI "\fBrestart\fR"
To start again; generally referring to starting the window manager again.
.br
.ne 3
.LI "\fBrestore\fR"
To return an icon or maximized window to its normal size.
.LI "\fBroot\ menu\fR"
See \fBworkspace menu\fR.
.LI "\fBroot\ window\fR"
See \fBworkspace\fR.
.LI "\fBsave\fR"
To write changes in a data file to a storage device for safekeeping.
.LI "\fBscreen\fR"
The physical CRT that displays information from the 
computer.  In the OSF/Motif environment, in most cases "screen" and
"workspace" are synonymous.
.LI "\fBscroll\ bar\fR"
A graphic device used to change a user's view of a list or data
file.  A scroll bar consists of a slider, scroll area, and scroll arrows.
The user changes the view by sliding the slider up or down in the scroll
area or by pressing one of the scroll arrows.  This causes the view to
scroll up or down in the window adjacent to the scroll bar.
.br
.ne 3
.LI "\fBscroll\ region\fR"
The rectangular portion of a ScrollBar that contains the two arrows and the
slider. 
.LI "\fBselect\fR"
To choose an object to be acted upon or an action to be performed.
.LI "\fBselection\fR"
The object or action that is selected.  Menus are composed of selection
items.  Dialog boxes contain controls, each of which represents a selection.
.LI "\fBselection\ cursor\fR"
See \fBlocation cursor\fR.
.LI "\fBselect\ button\fR"
The mouse button used to make a selection.
.LI "<\fBSelect\fR>\ key"
The special-purpose keyboard key used to make a selection.  Keyboards
without a Select key use a substitute to provide the select functionality.
.LI "\fBset\ or\ setting\fR"
Usually refers to specifying a value for a resource or a property.
.LI "<\fBShift\fR>\ key"
One of the modifier keys on the keyboard.
.br
.ne 3
.LI "\fBsize\fR"
Used as a verb to describe changing the size of a window on the workspace.
.LI "\fBslider\fR"
One of graphical components of a scroll bar or scale.  The slider is the
object that is dragged along the scroll area to cause a change.
.LI "\fBstate\fR"
A generic term used to describe the condition or mode of an object or action.
.LI "\fBsubclass\fR"
.iX "Subclass"
A class of widgets that inherits resources from a higher class.
.LI "\fBsubmenu\fR"
A cascading menu.
.LI "\fBtext\ cursor\fR"
See \fBinsertion cursor\fR.
.LI "\fBtitle\ area\fR"
An area at the top of the window frame immediately beneath the resize
border.  The title bar has two functions: it contains a title or name that
identifies the window and it can be grabbed and dragged to relocate the
window. 
.LI "\fBtitle\ bar\fR"
The bar at the top of the window frame immediately beneath the resize frame
handle.  The title bar may contain the title area and window buttons.
.br
.ne 5
.LI "\fBtransient\ window\fR"
A window of short duration such as a dialog box.  The window is
displayed for only a short time, usually just long enough to convey some
information or get some operational directions. 
.LI "\fBtranslations\fR"
.iX "Translation"
Action procedures that are invoked for an event
or sequence of
events.  
.LI "\fBtype\fR"
As a verb, to press and release a keyboard key.
.LI "\fBunderlined\ letter\fR"
A letter used as a mnemonic.  The underline provides the visual cue to the
mnemonic function.
.br
.ne 7
.LI "\fBwidget\fR"
.iX "Widget"
A widget is a graphic device capable of receiving input from the
keyboard
and the mouse and communicating with an application or another widget by means
of a callback.  Every widget is a member of only one class and always has a
window associated with it.
.LI "\fBwidget\ instance\fR"
.iX "Widget" "instance"
.iX "Instance"
The creation of a widget so that it is seen on the
display.  Note that some widgets (meta class, for example) cannot be
instantiated.
.LI "\fBwidget\ tree\fR"
.iX "Widget" "tree"
A widget tree is a hierarchy of widgets within a specific
program.   Examples of widget trees can be found in Chapter 3.
The shell widget 
is the root of the widget tree.
Widgets with no children of any kind are leaves of the tree. 
.LI "\fBwindow\fR"
A data structure that represents all or part of the CRT display screen.
Visually, a window is represented as a rectangular subset of the display
screen. 
.LI "\fBwindow\ decoration\fR"
The frame and window control buttons that surround windows managed by the
a window manager such as the OSF/Motif Window Manager.
.LI "\fBwindow\ frame\fR"
The area surrounding a window.  A window frame can consist of resize
frame handles, a window menu button, a title bar, and window control buttons.
.br
.ne 6
.LI "\fBwindow\ manager\fR"
A program that controls the size, placement, and operation of windows
on the workspace.  The window manager includes the functional window
frames that surround each window object and may include a separate menu for
the workspace.  
.LI "\fBwindow\ menu\fR"
The menu that appears when the window menu button is pressed.  The window
menu typically contains selections for restoring, moving, sizing,
minimizing, maximizing, and closing the window.
.br
.ne 3
.LI "\fBwindow\ menu\ button\fR"
The graphic control button that appears at the left side of the title bar
in the window frame.
.br
.ne 5
.LI "\fBworkspace\fR"
The CRT screen.  The area on which the windows of a user's environment
display.  The workspace is sometimes called the "desk," "desktop," or "root
window."  
.LI "\fBworkspace\ menu\fR"
An optional pop-up menu associated with the workspace.
.LE
