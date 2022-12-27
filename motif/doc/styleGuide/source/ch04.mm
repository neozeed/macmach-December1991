.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 3
.H 1 "Application Design Principles"
.iX "design" "applications"
.iX "application design"
This chapter is directed at application designers,
rather than the people who write the algorithms of an
application, although they are often one in the same.
Application designers are the people who
design the interface between the
application algorithms and the user.
In designing user interfaces, an application
designer chooses the proper controls or groups of
controls, or components, to pass
data between the base application and the user, lay out
those components naturally, and ensure
that user interaction with the components and layout
is easy to use.
.P
This chapter describes the three elements of application
user interface design, as well as the principles for designing
new user interface components.  The four major sections
of this chapter are as follows:
.BL
.LI
Choosing components
.LI
Layout
.LI
Interaction
.LI
Component design
.LE
.H 2 "Choosing Components"
.iX "components" "choosing"
.iX "choosing components"
.iX "basic controls"
.iX "field controls"
.iX "basic groups"
.iX "layout groups"
.iX "framing groups"
.iX "DialogBox" "groups"
The user interface of an application is made up of components that
perform two simple functions:
presenting the application information to the user, and
allowing the user to enter data for the application.
Components can be divided into six types:
basic controls,
field controls,
basic groups,
layout groups,
framing groups,
and
DialogBox groups.
The groups are containers for controls and groups of controls.
Groups can be nested.
.P
The component types that should be available in a user interface
toolkit are described in the following lists.
The details of
these component are described in the reference section in
Chapter 7 of this
guide.  The correspondence of the these components to widgets
available in the OSF/Motif toolkit is described in
Appendix B.
.BL
.LI
.iX "basic controls"
\fBBasic Controls\fP
.VL 17n
.LI "Separator"
.iX "Separator"
.iX "basic controls" "Separator"
Draws a separating line within windows and between Menu items.
A Separator does not allow application interaction.
.LI "Label"
.iX "Label"
.iX "basic controls" "Label"
Displays static text and images.
A Label presents application information to the user.
.LI "PushButton"
.iX "PushButton"
.iX "basic controls" "PushButton"
A button used to activate an operation.
A PushButton contains a label that indicates the operation of
the button.  The label can contain text or an image.
.LI "CascadeButton"
.iX "CascadeButton"
.iX "basic controls" "Cascade Button"
A button used to display a PullDown Menu.
A CascadeButton contains a label that indicates the Menu to be displayed.
CascadeButtons can also contain an arrow graphic
after the label to distinguish it from PushButtons and to
indicate the direction of the cascading Menu.
.LI "\fBOptionButton\fP"
.iX "OptionButton"
.iX "basic controls" "OptionButton"
A button used to display an Option Menu.  An Option Menu
allows for a one of many selection.  An OptionButton contains
a label that indicates the current state of the Option Menu,
and a bar graphic to distinguish it from a PushButton.
.LI "ToggleButton"
.iX "ToggleButton"
.iX "basic controls" "ToggleButton"
A button with two states: on and off.
A ToggleButton contains a label that indicates the active state.
Normally, preceding the label is a graphic indicator of the state
of the ToggleButton.
.LI "CheckButton"
.iX "CheckButton"
.iX "basic controls" "CheckButton"
A ToggleButton in a group of ToggleButtons where any
number of the ToggleButtons can be on at a time.
The graphic indicator for a CheckButton is usually a filled
square to indicate the on state or an empty square to indicate
the off state.
On color systems, the filled color can be
distinct from general application colors to visually distinguish
the on state.
.LI "RadioButton"
.iX "RadioButton"
.iX "basic controls" "RadioButton"
A ToggleButton in a group of ToggleButtons where only
one of the ToggleButtons can be on at a time.
The graphic indicator for a RadioButton is usually a filled
diamond or circle to indicate the on state or an empty diamond or
circle to indicate
the off state.
On color systems, the filled color can be
distinct from general application colors to visually distinguish
the on state.
.LE
.LI
\fBField Controls\fP
.iX "field controls"
.VL 17n
.LI "Sash"
.iX "Sash"
.iX "Panes"
.iX "PanedWindow"
Used to set the boundary between two components.  A Sash is usually
a small square on the boundary between two components.  The separated
components are called Panes, and a group of Panes, Separators,
and Sashes is called a PanedWindow.
.LI "Scale"
.iX "Scale"
.iX "field controls"
Used to set or display a value in a range.
A Scale is usually composed of a slider, moving within
an element that indicates the size of the range, and a label
that indicates the current value.
The position of the slider indicates the
value relative to the range.  The slider is moved 
directly by
using the mouse pointer or by
using the arrow keys.
A Scale can also have buttons with arrow graphics
for moving the slider with the mouse.
.LI "ScrollBar"
.iX "ScrollBar"
.iX "field controls" 
Used to scroll the visible area of a component.
A ScrollBar is usually composed of a slider, moving within
an element that indicates the full size of the component, and
buttons with arrow graphics for moving the slider with the mouse.
The slider indicates the relative position and size of the visible
area of the component.  The slider is moved 
directly by
using the mouse pointer or by
using the arrow keys.
.LI "List"
.iX "List"
.iX "field controls" 
Used for selecting elements from a list of elements.  A List
can allow multiple items to be selected or can be constrained
to only allow one item to be selected
at a time.  A List is usually composed of a vertical
list of items.  A List can also have both horizontal
and vertical ScrollBars for scrolling the visible portion
of the list of items.
.LI "Text"
.iX "Text"
.iX "field controls"
Used for displaying, entering, and modifying text.  There are
single line and multiple line variants.  Multiline Text can
have both horizontal and vertical ScrollBars for
scrolling the visible portion of the text area.  
.LI "Canvas"
.iX "Canvas"
.iX "field controls" 
Used for displaying, entering, and modifying graphics.
A Canvas can have both horizontal and vertical ScrollBars
for scrolling the visible portion of the drawing area.
.LE
.LI
\fBBasic Groups\fP
.iX "basic groups"
.VL 17n
.LI "Panel"
.iX "Panel"
.iX "basic groups" "Panel"
Organizes a collection of components in a horizontal,
vertical, or 2-dimensional layout.
A Panel is usually composed of just one type of button.
.LI "Menu"
.iX "Menu"
.iX "basic groups" "Menu"
Organizes a collection of buttons, Labels, and Separators
in a horizontal, vertical, or 2-dimensional layout within
a separate Menu window.  There are three types of Menus:
PullDown, Popup, and Option.  A Menu is only available
while it is pulled down or popped up.
.LI "MenuBar"
.iX "MenuBar"
.iX "basic groups" "MenuBar"
Organizes a collections of CascadeButtons in a horizontal
layout at the top of a MainWindow.
.LE
.LI
\fBLayout Groups\fP
.VL 17n
.LI "Composition"
.iX "Composition"
.iX "layout groups" 
Organizes a collection of components in an arbitrary layout.
.LI "PanedWindow"
.iX "PanedWindow"
.iX "layout groups" 
A linear grouping of components, Separators, and Sashes.
Sashes are used to set the boundary between two components.
The separated components are called Panes and can contain
any components.
.LE
.LI
\fBFraming Groups\fP
.iX "framing groups"
.VL 17n
.LI "Frame"
.iX "Frame"
.iX "framing groups" "Frame"
Draws framing decorations around a component.
.LI "ScrolledWindow"
.iX "ScrolledWindow"
.iX "framing groups" "ScrolledWindow"
Frames a component and adds ScrollBars
for scrolling the visible area of the component.
.LI "MainWindow"
.iX "MainWindow"
.iX "framing groups" "MainWindow"
Organizes the contents of a primary window.  A MainWindow
frames the client area and can optionally include ScrollBars,
a MenuBar, a command area, and a message area.
.LE
.LI
\fBDialogBox Groups\fP
DialogBox groups"
.sp
A DialogBox group can either be nested within another group or
organizes the contents of a secondary window.
Although a DialogBox may contain any component,
a simple DialogBox is composed of a Label and a Panel
of PushButtons for supplying a response to the DialogBox.
The OSF/Motif toolkit provides a number of ready-designed
DialogBoxes for common uses: CommandDialog, FileSelectionDialog,
MessageDialog, PromptDialog, and
SelectionDialog.
There are also a number of different types of MessageDialogs:
ErrorDialog, InformationDialog, QuestionDialog, WorkingDialog,
and WarningDialog.
.LE
.H 3 "Guidelines for Choosing a Main Component Group"
.iX "main component groups" 
.iX "Menu"
.iX "MainWindow"
At the highest level, components are organized into
MainWindows, Menus, and DialogBoxes.  Correctly deciding
which component group to use for which parts of an
application is one of the most important tasks of
application designer.
.P
.mS
Every application must contain at least one MainWindow. 
.mE
The MainWindow can contain a MenuBar, ScrollBars, a command
area, a message area, and the client area.
The client area of the MainWindow contains the framework
of an application.  The client area should contain all
the components needed to perform the primary actions of
the applications.  For example, in a text editor application,
the MainWindow usually contains a text editing area,
and in a graphics editor application,
the MainWindow usually contains a graphics editing area and the
components for changing drawing functions.
Components that are used constantly throughout the application
should be contained in the MainWindows client area.
Components that are used intermittently or infrequently should
be placed in Menus or DialogBoxes.
.P
.iX "Menu"
The advantages of Menus are that they are
readily available, quickly accessed and dismissed, and easy
to browse through.
The most commonly used Menus should be placed in the
MenuBar in the MainWindow to increase these advantages.
Because Menus are readily available, and quickly accessed and
dismissed, they should be used for components that are
used frequently.
The time delays of bringing up a DialogBox for frequently
used components can greatly reduce user productivity. 
Because Menus are easy to browse through,
they should also be used for components that are
commonly accessed by most users.
.P
.iX "Menu"
The advantages of Menus also cause some disadvantages.
A Menu is a very short-lived component group.  It is displayed
only while the user makes a selection.  Once the user makes
a selection, the Menu disappears.  So a Menu is not well-suited
for making several selections at once.
.P
.iX "Tear-Off Menus"
.iX "DialogBox"
Note that some
systems contain a component group called a Tear-Off Menu.
Once a Tear-Off Menu is pulled down, it can be made to remain
posted until specifically dismissed.
A Tear-Off Menu allows the user to make multiple selections
because it can stay posted after a user selection.  A Tear-Off
Menu behaves more like a DialogBox than a Menu.
.P
Menus are also modal.  That is, while a Menu is posted,
the user cannot interact with other elements of the application.
Because of this,
Menus should not be used for components that the user may want to
have available while interacting with other elements of
the application.
.P
.iX "Menu"
The other disadvantage of Menus is that
they can only contain buttons and Labels.
Menus should be used for performing simple actions
and setting values, but they are not suitable for more
complicated functions like text entry.
.P
.iX "DialogBox"
.iX "MainWindow"
DialogBoxes are used for two general purposes: to
present information to the user, and to take user input.
Applications should use DialogBoxes to present transient
information to the user, like warnings, cautions, or
conformations to actions.
DialogBoxes that present information to the user are called
Message DialogBoxes.
Message DialogBoxes are often placed on the screen 
by applications without a request from the user.
Because Message DialogBoxes can disrupt a user's work,
applications should present constantly updated information
in the client area of the MainWindow.
Also, applications can present minor cautions,
incidental notes, and simple help in the message area of the MainWindow.
.P
.iX "DialogBox"
The advantages of using DialogBoxes to take user input
are that they are long-lived, and can contain any components.
Because they are long-lived, they can be used to perform
more than one action at a time, and can remain available
while the user interacts with other parts of the application.
Applications should use user-input DialogBoxes like
small applications.  
DialogBoxes should be used to perform tasks ancillary to
the application, where the user can take more than one
action before dismissing the DialogBox.
Applications should also use DialogBoxes where the task
requires more complicated interaction than is available
in a Menu, like file selection.
.P
The following table summarizes the suggested uses for
MainWindows, Menus, and DialogBoxes.
.P
.TB "Suggested Window Types"
.TS
center, tab(@);
lb cb s s
lb lb lb lb
l c c c.
@Use This Main Component Group
For These Cases@MainWindow@Menu@DialogBox
_
Primary application actions@X@\(em@\(em
Ancillary application actions@\(em@\(em@X
Components used intermittently@\(em@X@\(em
Components used frequently@X@X@\(em
Components used seldomly@\(em@\(em@X
Components accessed by most users@\(em@X@\(em
Simple actions@X@X@\(em
Complex actions@X@\(em@X
Presenting transient information@\(em@\(em@X
Presenting constantly updated information@X@\(em@\(em
Presenting minor information@X@\(em@\(em
.TE
.H 3 "Guidelines for Choosing Interactive Methods"
.iX "interactive methods"
.iX "components" "interactive"
Choosing the correct components for a task is
usually simple.  In most cases, a task seems to
naturally belong to a component; for instance, 
scrolling a region with a ScrollBar,
or choosing a value from a range with a Scale.
Each component's general purpose is described in
Section 4.1,
as well as in the reference section in Chapter 7.
In some cases though, it is difficult to decide between
two controls or groups of controls that could perform the same task.
The following sections give some guidelines for choosing
among some common similar components.
.BL
.LI
.iX "single-choice components"
.iX "components" "single-choice"
Choosing a single-choice component
.LI
.iX "multiple-choice components"
.iX "components" "multiple-choice"
Choosing a multiple-choice component
.LI
.iX "Menu"
.iX "PushButton"
Choosing between a Menu and PushButton
.LI
.iX "Text"
.iX "Canvas"
.iX "Label"
Choosing between Text or Canvas, and a Label
.LE
.H 4 "Choosing a Single-Choice Component"
.iX "single-choice components"
.iX "components" "single-choice"
A single-choice component
allows the user to select a single item from a group of
items.  Applications should use single-choice component
to limit the user to one choice; for example, choosing among
available pen widths in a drawing program.
The available single-choice components are as follows:
.BL
.LI
.iX "RadioBox"
RadioBox
.LI
.iX "OptionButton"
.iX "basic controls"
OptionButton
.LI
.iX "List"
List
.LE
.P
.iX "single-choice components"
.iX "components" "single-choice"
You should choose a single-choice component based on the
number of items to choose among, the space available 
for the selection area, and the permanence of the selection.
For a small number of items, the
best component is a RadioBox or an OptionButton.
RadioBoxes should usually contain no more than 5 or 6 items.
OptionMenus, which are connected to OptionButtons, should
usually contain no more 10 to 12 items.
If there is very little space available in your application,
you should use an OptionButton over a RadioBox, since
the OptionButton takes up very little space.
.P
.iX "RadioBox"
.iX "OptionButton"
.iX "List"
When the number of items to choose from gets larger than
a RadioBox or OptionButton can easily handle, you should
use a List.  You should also use a List when the items
to choose from can change.  For application consistency,
the choices in a RadioBox or OptionButton should not
change, but if they do, you should use an OptionButton
over a RadioBox.  If a RadioBox or OptionButton choice
becomes unavailable, it should be disabled rather than removed.
Section 4.3.1.6
describes disabling components in detail.
.H 4 "Choosing a Multiple-Choice Component"
.iX "multiple-choice components"
.iX "components" "multiple-choice"
.iX "CheckBox"
.iX "List"
A multiple-choice control or group of controls allows the user to
simultaneously choose multiple items from a group of items.
Applications should use multiple-choice components, rather than
a series of single-choice actions, where the user may want
to select more than one item from a group.
The available multiple-choice components are as follows:
.BL
.LI
CheckBox
.LI
List
.LE
.P
As with single-choice components, the main factor when choosing
which to use is the number of items in the group.
For small groups of fewer than seven static elements,
applications should use a CheckBox.
Applications should otherwise use a List.
Note that a List component can be used as a single- or
multiple-choice component.
.H 4 "Choosing Between a Popup Menu and a Panel or PullDown Menu"
.iX "Popup Menus"
.iX "PullDown Menus"
Popup Menus
should only be considered as shortcuts to application
features because they are hidden in the application.
Features hidden in Popup Menus are difficult to find
for beginning users of an application because they
provide no cue to their existence.
Applications should use Popup Menus to allow users
to remain focused on their work areas, and when
there is not enough space in the client area for
a Panel of buttons.
.P
Popup Menus contain a set of buttons that can be used in
the same way as a Panel of buttons or a PullDown Menu.
The primary
difference is availability.  A Popup Menu is only visible
when requested by the user.  At all other times it is hidden,
providing no cue to its existence.  When the user requests
a Popup Menu, the application displays it at the location
of the pointer.
Buttons and PullDown Menus on the other hand are always visible, but
users need to move the focus away from their main work
to activate them.
Another difference is that buttons take up screen space
while Popup Menus do not.
PullDown Menus provide a good compromise
in space and availability versus Popup Menus and Panels.
.P
.iX "Panel"
Applications should use a Panel of buttons when
the user makes frequent or multiple selections, and
when space in the client area allows.  Whenever
the choice between Popup Menus and Panels
is difficult, applications should favor Panels.
.H 4 "Choosing Between Text or Canvas, and a Label"
.iX "Text"
.iX "Canvas"
.iX "Label"
A Label is a simple mechanism for displaying text
or graphics.  A Label does not have any mechanisms
for the user to edit its contents, or the overhead
required for editing.  Applications should use
a Label for displaying text or graphics that do not need
user editing.  Applications should use
a Text or Canvas when the text or graphics needs
to be edited.  On some systems, the contents of
a Label are not selectable.  If the user needs to
select the contents of some noneditable text
or graphics, applications should use a Text or
Canvas and disable editing rather than using a Label.
.H 2 "Layout"
.iX "layout"
The previous section described the components that
are available for building an application.  This
section describes how to combine those components
into a coherent application that encourages a user's
sense of control.  The guidelines presented in this
section will help you create applications that are
both consistent with themselves and with other
OSF/Motif compliant applications.
.P
This chapter discusses the following client area design
topics:
.BL
.LI
Client areas
.LI
Grouping components
.LI
Menu design
.LI
DialogBox design
.LE
.P
Design the layout of your application windows according to the natural
use order and the natural
scanning order of the people who will be using your application.
.P
First design for the the natural use
order.  Consider the tasks that the user will perform with your
application.  The components should be positioned so that moving
among the components is simple and quick
while performing the most common tasks\(em the less pointer
movement, the better.  This is also true for keyboard traversal\(em
the fewer keystrokes required to perform a task, the better.
.P
The natural scanning order is most important when arranging small groups
of components to help the user find the correct component for the task.
You should put the most important and most used commands first.
In most
cases, this order is from left to right and from top to bottom.
For users in right-to-left direction language environments
the natural scanning direction
is from right to left and from top to bottom.
.H 3 "Common Client Areas"
.iX "client areas"
.iX "MainWindow"
.iX "DialogBox"
.iX "Menu"
.iX "command area"
.iX "message area"
.iX "ScrollBars"
.iX "MenuBar"
You application is presented in windows.  The windows
can be either MainWindows, DialogBoxes or Menus.  The
contents of these windows are the application client areas.
Some of these client areas have common features that are
described in this section.
This section describes in detail the contents of the common
MainWindow areas.
.BL
.LI
Command area
.LI
Message area
.LI
ScrollBars
.LI
MenuBar
.LE
.P
Following this is a description of some common DialogBoxes.
These common client areas provide a familiar base for users
new to your application to begin working.
.br
.ne 4.2i
.P
Figure 4-1 shows a MainWindow with its client areas.
.br
.ne 3.9i
\0
.FG "A Typical MainWindow and Its Common Client Areas"
.sp 3.7i
.in +.85i
.P! graphics/mainw.ps
.in -.85i
.sp
.P
.iX "MainWindow"
MainWindows contain the framework for your application.
In a text editing application, a MainWindow usually contains the text.
In a spreadsheet application, a MainWindow usually contains the spreadsheet.
In a mail browsing application, a MainWindow usually contains a list of mail
messages and an area for viewing a message.
.mS
Every application must have at least one MainWindow.
.mE
.P
.iX "multiple MainWindows"
.iX "MainWindow"
Some applications may want more than one MainWindow; for example,
applications with more than one primary function,
like the query and update features of a
database, and applications with more than one instance
of the same primary function, like a graphics editor
working on multiple figures.
When an application has multiple MainWindows that
serve the same primary function, each window should be treated
as a separate application.
.mS
Each window must close and iconify separately.
When an application has multiple MainWindows that
serve different primary functions, each window must
be iconifiable separately, but it is up the application
to decide whether each closes separately, or whether
closing one window closes the entire application.
.mE
You should
take special care to identify, using the window manager
title bar or a prominent internal label, which DialogBoxes
belong to each MainWindow. 
.H 4 "Command Area"
.iX "command area"
Even though user interface applications are oriented toward
direct manipulation of graphical elements,
your application can provide an area for a user to enter
a typed command.  This can be particularly useful when
updating an existing command-based application to a graphical
user-interface-based application.
.P
If your application includes a command area, it should run
from border to border across the bottom of the MainWindow,
just above the message area, or just below the
MenuBar.  We recommend that a command area run just above the
message area.
.H 4 "Message Area"
.iX "message area"
Your application can provide a message area in the MainWindow
for presenting application messages.
Your application should not use a message area for warnings
or messages requiring immediate action.  These should
be displayed in a DialogBox.
.P
If your application includes a message area, it should
run from border to border across the bottom of the MainWindow,
below any ScrollBars or command area.
Your application can display and remove the message area
as needed.
.H 4 "ScrollBars"
.iX "ScrollBars"
.iX "scroll bar"
An application can use ScrollBars in its MainWindow to
provide a means for viewing an area larger than the MainWindow.
Your application defaults should place ScrollBars on
the bottom and right sides
of the application. (ScrollBar placement can be
a user preference.)  You should place ScrollBars below the
MenuBar and above any command or message area.  ScrollBars
should not scroll the MenuBar, command area, or message area.
The ScrollBars can be displayed and removed as needed.
.H 4 "MenuBar"
.iX "MenuBar"
.iX "pull-down menu"
.iX "windows " menu bar"
The MenuBar is a basic group that organizes the
most common features of an application.
.mS
The MenuBar
must be a horizontal bar at the top edge of the
application just below the title area of the window
frame.
.mE
The MenuBar contains a list of Menu topics
in CascadeButtons with PullDown Menus connected to
them.
.mS
Commands must not be included as topics
in the MenuBar because they inhibit Menu browsing.
.mE
Each Menu topic should have a single-letter
mnemonic indicated by underlining.
.P
While we recommended that you include the common Menus
in the MenuBar of your application,
your choice of Menu titles and items depends on the
nature of your application.
If your application requires it, you should
design more relevant titles and selections,
but do not change the meanings of words
used in the common Menus.
.P
The following PullDown Menus provide general
functions common to most applications.
Remember that these Menu guidelines,
like all the guidelines in this
.IR "Style Guide" ,
only apply in a left-to-right language direction
environment in an English locale.
You need to make the appropriate changes for other locales.
.VL 10n
.LI "\fB\o'F_'ile\fP"
.iX "file menu"
.iX "menus" "file"
The
.B File
Menu should contain components for performing actions
on the files, such as opening, saving, closing, and printing.
It should also contain components for performing actions
on the application as a whole, such as quitting.
.mS
If your application includes a
.B File
Menu, it must be the first entry in the MenuBar, and must
be placed
to the far left.
.mE
The
.B File
Menu should have a mnemonic of
.BR F .
.LI "\fB\o'E_'dit\fP"
.iX "edit menu"
.iX "menus" "edit"
The
.B Edit
Menu should contain components for performing actions
on the current data of the application, such as an
undo action or making global substitutions in
a block of text.
It should also include components for interacting
with the system clipboard, such as cut, copy, and paste.
.mS
If your application includes an
.B Edit
Menu, it must be placed to the right of the
.B File
Menu.
.mE
The
.B Edit
Menu should have a mnemonic of
.BR E .
.LI "\fB\o'V_'iew\fP"
.iX "view menu"
.iX "menus" "view"
The
.B View
Menu should contain components for changing the
user's view on the data.  Components in the
.B View
Menu should not actually change the data.
The exact contents of the
.B View
Menu are application specific, but it
may include components that change the appearance
of the data, the amount of data that is displayed, or
the order in which the data is displayed.
.mS
If your application includes a
.B View
Menu, it must be placed to the right of the
.B Edit
Menu.
.mE
The
.B View
Menu should have a mnemonic of
.BR V .
.LI "\fB\o'O_'ptions\fP"
.iX "Options menu"
.iX "menus" "options"
The
.B Options
Menu should contain components for a user to customize
the application.  The exact contents of the
.B Options
Menu are application specific.
.mS
If your application includes an
.B Options
Menu, it must be placed to the right of the
.B View
Menu.
.mE
The
.B Options
Menu should have a mnemonic of
.BR O .
.LI "\fB\o'H_'elp\fP"
.iX "help"  "menu"
.iX "menus" "help"
The
.B Help
Menu should contain components that provide user help
facilities.
The components in the
.B Help
Menu usually bring up a DialogBox with help information.
Every application should have a
.B Help
Menu.
.mS
If your application includes a
.B Help
Menu, it must be the last Menu item and placed to the
far right on the MenuBar.
.mE
The
.B Help
Menu should have a mnemonic of
.BR H .
.LE
.P
The recommended contents of the
.BR File ,
.BR Edit ,
and
.BR Help
Menus follow.  The contents of the
.BR View
and
.BR Options
Menus are application specific and are not specified here.
.H 5 "File Menu Contents"
.iX "file menu"
.iX "menus" "file"
Figure 4-2 shows a sample
.B File
Menu.  The common Menu contents are described following the figure.
Note that you should only include those functions actually
supported by your application.
.br
.ne 3.8i
\0
.FG "The File Menu and Its Selections"
.sp 3.6i
.in +.85i
.P! graphics/filemenu.ps 
.in -.85i
.sp
.VL 1i
.LI "\fB\o'N_'ew\fP"
.iX "New"
.mS
Must create a new file.  The
.B New
operation must clear existing data from the
client area.
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
This action must have the mnemonic
.BR N .
.mE
.LI "\fB\o'O_'pen\ .\ .\ .\fP"
.iX "Open"
.mS
Must open an existing file.  The
.B Open
operation must prompt the user for the
name of the file with a DialogBox.
.mE
The title bar should be updated with the name of the newly opened file.
.mS
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
.mE
This DialogBox should be displayed after the user selects a new file.
.mS
This action must have the mnemonic
.BR O .
.mE
.LI \fB\o'S_'ave\fP
.iX "Save"
.mS
Must save the currently opened file without removing the
existing contents of the client area.
If the currently opened file has no name,
.B Save
must prompt for a filename with a DialogBox.
This action must have the mnemonic
.BR S .
.mE
.LI "\fBSave \o'A_'s\ .\ .\ .\fP"
.iX "Save As"
.mS
Must save the currently opened file under a new name
without removing the existing contents of the client area.
The
.B "Save As"
operation must prompt for the name of the file with a DialogBox.
If the user tries to save the new file under an existing name,
.B "Save As"
must alert the user with a DialogBox of a possible loss of data.
This action must have the mnemonic
.BR A .
.mE
.LI \fB\o'P_'rint\fP
.iX "Print"
.mS
Must schedule a file for printing.
If your application requires
specific printing information before printing, the operation must
first request that information with a DialogBox, and the
entry title must be followed by an ellipses.
.mE
Printing information can also be specified for the application
in the
.B Options
Menu.
.mS
This action must have the mnemonic
.BR P .
.mE
.LI "\fB\o'C_'lose\fP"
.iX "Close"
Can be supplied in applications that have multiple independent
primary windows.
.mS
This action must not be supplied in applications with a single
primary window, or multiple dependent primary windows.
This action must only close the current primary
window and its associated secondary windows; that is, the window family.
This action must have the mnemonic
.BR C .
.mE
You can include this action even though it is similar to the
.B Close
action in the window Menu.
This ensures that users have a way to close the primary window even
if they are not running a compliant window manager.
Applications should prompt the user to save any unsaved changes
if the action would cause loss of data.
.LI "\fB\o'E_'xit\fP"
.iX "Exit"
.mS
Must end the current application and all windows associated with it.
.mE
This action is equivalent to closing all primary windows of the application. 
.mS
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
This action must have the mnemonic
.BR E .
.mE
You should include this action even though it is similar to the
.B Close
action in the window Menu.
This ensures that users have a way to end the application even
if they are not running a compliant window manager.
Applications should prompt the user to save any unsaved changes
if the action would cause loss of data.
If your application does not have a
.B File
Menu, put
.B Exit
at the end of the first PullDown Menu.
.LE
.P
You should include Menu items in the order described.  If you
add new Menu items, you should insert them near similar elements.
.H 5 "Edit Menu Contents"
.iX "edit menu"
.iX "menus" "edit"
Figure 4-3 shows a sample
.B Edit
Menu.  The common Menu contents are described following the figure.
Note that you should only include those functions actually
supported by your application.
.br
.ne 3.8i
\0
.FG "The Edit Menu and Its Selections"
.sp 3.6i
.in +.85i
.P! graphics/editmenu.ps 
.in -.85i
.sp
.VL 1.75i
.LI "\fB\o'U_'ndo\ \ \ Alt+Backspace\fP"
.iX "Undo"
.mS
Must reverse the most recently executed action.
.mE
To provide a visual cue to the user, the
.B Undo
selection title should be dynamically modified to indicate
what is being undone.
For example, if the most recently executed action were a paste,
the action name would be
.BR "Undo paste" .
Your application should be able to undo all of the
actions in the 
.B Edit
Menu.  
.mS
This action must have the mnemonic
.BR U .
If this action has an accelerator, it must override
the standard undo action for a specific component, and
must be
.BR "MAlt KBackSpace" .
.mE
.LI "\fBCu\o't_'\ \ \ Shift+Del\fP"
.iX "Cut"
.mS
Must remove the selected portion of data from the client
area to the clipboard buffer.
.mE
Your application can determine whether the area that was occupied
by the removed data is left blank or whether the remaining data is 
compressed to fill in the space.
Usually, graphics applications leave the space blank while text applications
compress the remaining text to fill in the space.
.mS
This action must have the mnemonic
.BR T .
If this action has an accelerator, it must override
the standard cut action for a specific component,
.mE
and both
.BR "MShift KDelete"
and
.BR "KPrimaryCut"
should be supported.
.LI "\fB\o'C_'opy\ \ \ Ctrl+Ins\fP"
.iX "Copy"
.mS
Must copy the selected portion of data to the clipboard
buffer without removing the original data from the client area.
This action must have the mnemonic
.BR C .
If this action has an accelerator, it must override
the standard copy action for a specific component,
.mE
and both
.BR "MCtrl KInsert"
and
.B "KPrimaryCopy"
should be supported.
.LI "\fB\o'P_'aste\ \ \ Shift+Ins\fP"
.iX "Paste"
.mS
Must paste the contents of the clipboard
buffer into a client area. 
.mE
When there is one main editing area, the application
can force the paste to occur in that area.
When an accelerator is used and the focus is in an editable
component the paste should go to that editable component.
When the focus policy is explicit,
and when there are multiple possible destinations for the
paste, the paste should go to the last editable component with the
input focus.
When the focus policy is implicit,
and when there are multiple possible destinations for the
paste, the paste should go to the component with the
destination cursor, as described in Section 3.2.3.
.P
Your application can determine whether the pasted data is reformatted to 
fit in the client area and whether existing data moves to create room
for the pasted data.
Text applications usually reformat the pasted text to fit into
the margins of the text field, and they will move the existing text to 
make room for the new text.
Graphics applications usually do neither.
.mS
This action must have the mnemonic
.BR P .
If this action has an accelerator, it must override
the standard paste action for a specific component,
.mE
and both
.BR "MShift KInsert"
and
.BR "KPrimaryPaste"
should be supported.
.LI "\fBCl\o'e_'ar\fP"
.iX "Clear"
.mS
Must remove a selected portion of data from the client area without 
copying it to a clipboard buffer.
.mE
The remaining data is not compressed to fill the space that was
occupied by the cleared data.
.mS
This action must have the mnemonic
.BR E .
.mE
.LI "\fB\o'D_'elete\fP"
.iX "Delete"
.mS
Must remove a selected portion of data from the client area without 
copying it to a clipboard buffer.  
.mE
The remaining data is compressed to fill the space that was
occupied by the deleted data.
.mS
This action must have the mnemonic
.BR D .
.mE
.LE
.P
You should include Menu items in the order described.  If you
add new Menu items, you should insert them near similar elements.
.H 5 "Help Menu Contents"
.iX "help" "menu"
.iX "menus" "help"
Figure 4-4 shows a sample
.B Help
Menu.  The common Menu contents are described following the figure.
Note that you should only include those functions actually
supported by your application.
.br
.ne 3.75i
\0
.FG "The Help Menu and Its Selections"
.sp 3.55i
.in +.85i
.P! graphics/helpmenu.ps 
.in -.85i
.sp
.VL 1i
.LI "\fBOn\ \o'C_'ontext\fP"
.iX "On Context"
.iX "help" "context-sensitive"
.mS
Must initiate context-sensitive help
by changing the shape of the pointer to the question pointer
described in Section 2.2.2.
When the user moves the pointer
to the component help is wanted on and presses
.BR BSelect ,
any available context-sensitive help for the component
must be presented, and the pointer reverts from the
question pointer.
This action must have the mnemonic
.BR C .
.mE
.LI "\fBOn\ \o'H_'elp\ .\ .\ .\fP"
.iX "On Help"
.mS
Must provide information on how to use
the application's help facility.
.mE
.LI "\fBOn\ \o'W_'indow\ .\ .\ .\fP"
.iX "On Window"
.mS
Must provide general information about the
window from which help was requested.
.mE
.LI "\fBOn\ \o'K_'eys\ .\ .\ .\fP"
.iX "On Keys"
.mS
Must provide information about the application's
use of function keys, mnemonics, and keyboard accelerators.
.mE
.LI "\fB\o'I_'ndex\ .\ .\ .\fP"
.iX "Index"
.iX "help" "index"
.mS
Must provide an index for all help information in
the application.
.mE
The index can provide search capabilities.
.LI "\fB\o'T_'utorial\ .\ .\ .\fP"
.iX "Tutorial"
.iX "help" "tutorial"
.mS
Must provide access to the application's tutorial.
.mE
.LI "\fBOn\ \o'V_'ersion\ .\ .\ .\fP"
.iX "On Version"
.iX "help" "version"
.mS
Must provide the name and version of the application.
.mE
It can provide other information as well.
.LE
.P
Help is usually provided in DialogBoxes, but you should not
use ellipses in the
.B help
Menu for help actions provided in a message area.
You should include Menu items in the order described.  If you
add new Menu items, you should insert them near similar elements.
.H 4 "DialogBoxes"
.iX "DialogBox"
Applications use DialogBoxes to interact with the user
about application details not directly related to
the primary purpose of the application.
Applications display DialogBoxes only when needed to
convey a message to the user, or when the user requests
it to provide input to the application.
They follow the same general layout guidelines as a MainWindow. 
Additional guidelines for DialogBox design are given in
Section 4.2.4,
as well as in the reference section in Chapter 7.
.P
The following DialogBoxes provide general
functions common to many applications.
.H 5 "CommandDialog"
.iX "Command DialogBox"
.iX "DialogBox" "Command"
A
.B CommandDialog
should be used to enter keyboard commands.
It should not interrupt the user's interaction with the application,
that is, it should not be modal.
It should include a CommandBox as shown in Figure 4-5.
.br
.ne 2.5i
\0
.FG "A CommandDialog"
.sp 2.3i
.in +1.9i
.P! graphics/commandbox.ps
.in -1.9i
.H 5 "FileSelectionDialog"
.iX "FileSelection DialogBox"
.iX "DialogBox" "FileSelection"
A
.B FileSelectionDialog
should be used to enter the name of file for processing.
It should not interrupt the user's interaction with the application,
that is, it should not be modal.
It should include a FileSelectionBox as shown in Figure 4-6.
.br
.ne 5.7i
\0
.FG "A FileSelectionDialog"
.sp 5.5i
.in +1.1i
.P! graphics/fileselbox.ps
.in -1.1i
.H 5 "PromptDialog"
.iX "Prompt DialogBox"
.iX "DialogBox" "Prompt"
A
.B PromptDialog
should be used to prompt the user for input.
It can interrupt the user's interaction with the application,
that is, it can be application modal.
It should include a message, a text input area, and one
of the following button arrangements as shown in Figure 4-7.
.DS
\fBOK Cancel\fP
\fBOK Cancel Help\fP
\fBOK Apply Cancel\fP
\fBOK Apply Cancel Help\fP
\fBOK Apply Reset Cancel\fP
\fBOK Apply Reset Cancel Help\fP
.DE
.br
.ne 2.5i
\0
.FG "A PromptDialog"
.sp 2.3i
.in +1.25i
.P! graphics/promptbox.ps
.in -1.25i
.H 5 "SelectionDialog"
.iX "Selection DialogBox"
.iX "DialogBox" "Selection"
A
.B SelectionDialog
should be used to allow a user to make a selection from
a list of choices.
It can interrupt the user's interaction with the application,
that is, it can be application modal.
It should contain a SelectionBox as shown in Figure 4-8.
.br
.ne 4.4i
\0
.FG "A SelectionDialog"
.sp 4.15i
.in +1.45i
.P! graphics/selbox.ps 
.in -1.45i
.H 5 "MessageDialog"
.iX "Message DialogBox"
.iX "DialogBox" "Message"
A
.B MessageDialog
should be used to convey a message to the user.
.mS
It must not interrupt the user's interaction with the application,
that is, it must not be modal.
.mE
It should include a message, and one of the following button
arrangements.
.DS
\fBOK\fP
\fBOK Help\fP
\fBOK Cancel\fP
\fBOK Cancel Help\fP
\fBYes No\fP
\fBYes No Help\fP
\fBYes No Cancel\fP
\fBYes No Cancel Help\fP
\fBCancel\fP
\fBCancel Help\fP
\fBRetry Cancel\fP
\fBRetry Cancel Help\fP
.DE
There are a number of different types of MessageDialogs:
ErrorDialog, InformationDialog, QuestionDialog, WorkingDialog,
and WarningDialog.
.P
.iX "Error DialogBox"
.iX "DialogBox" "Error"
An
ErrorDialog
should be used to convey a message about a
user error.
It should stop user interaction with the application
until it is dismissed,
that is, it should be application modal.
It should include an error symbol,
a message, and one of the following button arrangements
as shown in Figure 4-9.
.DS
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.br
.ne 2.15i
\0
.FG "An ErrorDialog"
.sp 1.9i
.in +1.25i
.P! graphics/errorbox.ps
.in -1.25i
.P
.iX "Information DialogBox"
.iX "DialogBox" "Information"
An
InformationDialog
should be used to convey information to the user.
.mS
It must not interrupt the user's interaction with the application,
that is, it must not be modal.
.mE
It should include an information symbol, a message, and one 
of the following button arrangements
as shown in Figure 4-10.
.DS
\fBOK\fP
\fBOK Help\fP
.DE
.br
.ne 2.3i
\0
.FG "An InformationDialog"
.sp 2.1i
.in +1.25i
.P! graphics/infobox.ps
.in -1.25i
.P
.iX "Question DialogBox"
.iX "DialogBox" "Question"
A
QuestionDialog
should be used to get a user response to a question.
It should interrupt the user's interaction with the application,
that is, it should be application modal.
It should include a question symbol, a message, and one
of the following button arrangements
as as shown in Figure 4-11.
.DS
\fBYes No\fP
\fBYes No Help\fP
\fBYes No Cancel\fP
\fBYes No Cancel Help\fP
.DE
.br
.ne 2.25i
\0
.FG "A QuestionDialog"
.sp 2.05i
.in +1.25i
.P! graphics/questbox.ps
.in -1.25i
.P
.iX "Warning DialogBox"
.iX "DialogBox" "Warning"
A
WarningDialog
should be used to alert the user to a possible danger.
It should interrupt the user's interaction with the application,
that is, it should be application modal.
It should contain a warning symbol, a message, and one of the
following button arrangements
as shown in Figure 4-12.
.DS
\fBYes No\fP
\fBYes No Help\fP
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.br
.ne 1.7i
\0
.FG "A WarningDialog"
.sp 1.5i
.in +.9i
.P! graphics/warnbox.ps 
.in -.9i
.P
.iX "Working DialogBox"
.iX "DialogBox" "Working"
A
WorkingDialog
should be used to show work in progress and give the user
an opportunity to cancel the operation.
It should not interrupt the user's interaction with the application,
that is, it should not be modal.
It should contain a working symbol, a message, and one of the
following button arrangements
as shown in Figure 4-13.
.DS
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.br
.ne 1.8i
\0
.FG "A WorkingDialog"
.sp 1.6i
.in +1.3i
.P! graphics/progbox.ps 
.in -1.3i
.H 3 "Grouping Components"
.iX "grouping" "components"
.iX "components" "grouping"
.iX "components" "arranging"
This section describes how you should group
components using the group components described in
Section 4.1.
.BL
.LI
Grouping similar components
.LI
Arranging components for fixed layout
.LI
Arranging components for resizing layout
.LI
Arranging components for aligned layout
.LI
Arranging components in PanedWindows
.LE
.P
Separators are a good tool for visually separating
groups of components.  You should use Separators
in your application anyplace where the border
between two groups of components is not obvious
by some other means, such as for a significant change
in component types, framing, or a division by space. 
.H 4 "Grouping Similar Components"
.iX "grouping" "components"
.iX "control panel"
.iX "panels" "control"
Components similar in appearance and function group
naturally together.  You should organize similar
components together.  Similar PushButtons, as in a Menu,
and a Panel of CheckButtons, as in a list of options,
are good examples of where grouping is important.  
Without grouping, related actions are difficult to find.
One of the most important cases is a Panel of RadioButtons.
Without grouping, when you turned on one RadioButton,
you might not see the previous button turn off.
You can also associate a title with a group of components
to better associate the group and its purpose.
.H 4 "Arranging Components for Fixed Layout"
.iX "fixed layout"
.iX "layout" "fixed" 
Components that are grouped using a Composition
group component can be positioned as they
best fit, both physically and visually.
This is often the case in the MainWindow client area,
and is also useful when
the layout of the components is important
to the application.
.P
When you layout components in this manner,
remember that your application can be resized by the user.
This can be compensated for in a number of ways.
If the area has a fixed size and is not appropriate to scroll,
like a control panel, you can either clip the area or turn off resizing.
If the area has a fixed size and is appropriate to scroll,
you should include ScrollBars for scrolling the viewable area.
.H 4 "Arranging Components for Resizing Layout"
.iX "resizing layout"
.iX "layout" "resizing"
Composition component groups can also be positioned so that 
the position and size of each component is relative to
the Composition component group or other components in the
group.  This style of layout permits the components
to change size proportionally to any change in the size of
the Composition component.
.P
Sometimes a window is resized too small to be useful.  Your
application can either simply ignore this, clip the region
once all the components are at their minimum size, remove
less useful components to make room for more shrinking,
or replace all the components with a message that indicates
that the minimum size is reached, stating that the user needs to
enlarge the window to continue working in it.
.H 4 "Arranging Components for Aligned Layout"
.iX "columns" "aligning"
.iX "vertical alignment"
.iX "alignment" "vertical"
.iX "horizontal alignment"
.iX "alignment" "horizontal"
Panels can arrange components aligned horizontally, vertically,
or in two dimensions.  Buttons are usually aligned in Panels
horizontally along the bottom of of the client area, either
in a MainWindow or a DialogBox.  ToggleButtons should be
arranged in vertical Panels so the graphics align and look
neat.  A common use of Panels is in building Menus,
or RadioBoxes, that is,
a set of RadioButtons.
Two-dimensional Panels are commonly used in graphics
programs to present
drawing styles, and in spreadsheets to contain cells.
.P
You application should use Panels to provide the best
organization for large groups of similar components.
Panels should be placed in a Composition component group
to allow for proper user resizing either by using
ScrollBars or by resizing the elements of the Panel.
.H 4 "Arranging Components in PanedWindows"
.iX "windows" "panes"
.iX "PanedWindow"
.iX "PanedWindow"
.iX "vertical panes"
.iX "Panes"
PanedWindows provide a way for the user to simply adjust the
size of components relative to one another.
You should use PanedWindows to separate user tasks in an application
with limited space.  This allows the user to ignore elements
of the application that are unused.
.P
PanedWindows can also be used to present two simultaneous views of the
same data.  For example, a text editor may use multiple Panes,
with a separate Text component in each Pane
to allow the user to write in one section of a document, while
looking at another.  The user can then resize the Panes to show
more or less of either block of text.
.P
PanedWindows can be composed of either vertical (one on top of the other)
or horizontal (side by side) sets of Panes, Separators, and Sashes.
Users can resize Panes by dragging the boundary between them.
Making one Pane bigger makes the other Pane smaller, while the
overall size of the window remains the same.
.H 3 "Menu Design"
.iX "PullDown Menus"
.iX "Popup Menus"
.iX "Options Menu"
.iX "menus" "designing"
Menus are the primary means of organizing most of an application's
features.  Because of screen size limitation and visual simplicity,
Menus organize both components used frequently by users and components used in
most application sessions.
.P
There are three types of Menus.
.BL
.LI
PullDown Menus
.LI
Popup Menus
.LI
Option Menus
.LE
.P
PullDown Menus are pulled down from a CascadeButton.  CascadeButtons
should always be available in the context that they are needed.
Menus can also contain CascadeButtons, so that Menus can be nested.
The MenuBar is a horizontal collection of CascadeButtons.
.P
Popup Menus are context sensitive, but give no cue to their existence.
They are popped up when the user presses
.B BMenu
over a component with an associated Popup Menu.  Popup Menus should
only be used to provide shortcuts, since new users of an application
may not realize or remember that they exist.
Even within a single control, such as a Canvas, the contents of a
Popup Menu can depend upon position within the control, or
the state of the elements at that position, such as whether
the Menu is popped up from within a selected range of elements.
.P
Option Menus provide a means of selecting from a set of choices
while taking up very little space.  An Option Menu is popped up
from an OptionButton, which is distinguished by a bar graphic
on the right side of the button.
.P
Menus are composed of titles, elements, mnemonics, and accelerators.
A Menu's title should be unique to avoid confusion.
The title should clearly indicate the purpose of the Menu.
.BL
.LI
A PullDown Menu's title is taken from the label in the CascadeButton.
.LI
A Popup Menu's title should be placed at the top of the Popup
Menu and separated from the Menu elements by a Separator.
.LI
An Option Menu's title is usually a label to the left of
the OptionButton, but can be at the top of the Option Menu itself.
.LE
.P
Most basic controls can be Menu elements including
Labels, Separators, PushButtons, ToggleButtons, and CascadeButtons.
The elements can be identified by either a text label or a graphic.
.mS
A Menu must be wide enough to accommodate its widest element.
.mE
.P
.iX "mnemonics"
A mnemonic provides a quick way to access Menu elements from the
keyboard.  While a Menu is posted, pressing the mnemonic letter
of an element activates that element.  The MenuBar's mnemonics
can be used by pressing
.B MAlt
with the mnemonic letter.
An element's mnemonic should be the first character of the
element's label.  If that character conflicts with another
mnemonic in the Menu, another character in the label should be used.
The mnemonic of an element should be underlined in the element's
label.
When the appropriate mnemonic letter does not appear in the
element's label, it should appear in parentheses after the
label.
.P
.iX "accelerators"
An accelerator provides a way to access Menu elements from the
keyboard without posting the Menu.  Accelerators are useful
to the experienced user for saving time when using frequently used
components.  You should provide accelerators primarily as a matter
of utility, not design conformity.
.P
If a keyboard accelerator exists for a Menu entry, it should appear
following the Menu's label, justified on the same line.
The accelerator and the selection should be separated by enough space
to make them visually distinct.
.P
You should use the following guidelines when designing Menus and
Menu systems:
.BL
.LI
Keep Menu structures simple.
.LI
Group like Menu elements together.
.LI
List Menu selections by frequency of use.
.LI
List Menu selections by order of use.
.LI
Separate destructive actions.
.LI
Provide mnemonics and accelerators.
.LE
.H 4 "Keeping Menu Structures Simple"
.iX "menus" "structures"
.iX "structures" "menu"
.iX "multiple cascading submenus"
.iX "submenus" "multiple cascading"
.iX "dialog box"
Applications should keep Menu structures simple.
One the primary benefits of Menus is the ease of access
to the elements of the Menu.  While cascading submenus
help the application and the user organize Menu elements,
each level of a submenu reduces the ease of access to the
Menu elements.  Multiple levels of cascading submenus can
also quickly create visual clutter.  Whenever you consider
using a cascading submenu, you should consider using a
DialogBox or more PullDown Menus instead.
.H 4 "Grouping Like Menu Elements Together"
.iX "grouping" "menu selections"
.iX "menus" "selections" "ordering" 
.iX "related items"
Applications should group Menu elements into logical groups.
This helps the user locate specific Menu elements.
You should first try to place a new Menu element into
the common Menu groups described in Section 4.2.1.5.
If that is not appropriate, you should group new Menu
elements according to function, with the more
frequently used element appearing first.  You should
also use Separators between logical groups of elements. 
.H 4 "Listing Menu Selections by Frequency of Use"
.iX "ordering" "menu selections"
As in other client areas,
applications should order Menu elements according to
the frequency of usage, positioning the
most frequently used elements near the top of the Menu.
.H 4 "Listing Menu Selections by Order of Use"
.iX "ordering menu selections"
.iX "menus" "selections" "ordering" 
More important to Menu design than the frequency of use
is the order of use.  Applications should order
Menu elements according to the order of usage.
For example, the
.B Copy
element should be placed before
.BR Paste.
This helps the user's interactions flow smoothly.
.H 4 "Separating Destructive Actions"
.iX "destructive actions"
.iX "selection"
Applications should separate destructive actions
from frequently chosen selections.  This is
to avoid accidental selection of the destructive
element.  Destructive elements, like
.B Delete
or
.BR Clear ,
should be placed at the end of a PullDown Menu
and separated from other elements by a Separator.
.H 4 "Providing Mnemonics and Accelerators"
.iX "accelerators"
.iX "mnemonics"
Applications should provide mnemonics and accelerators
to Menu elements.
Try to choose mnemonics and accelerators that are
easy to remember by using letters from the element's
title.  Note that mnemonics and accelerators only
add to the utility of your applications.  They never
detract from the basic ability of a new user.
.P
Applications should provide accelerators for frequently used menu items.
In general, accelerators should not be assigned for every menu item in an
application.  It is preferable to assign accelerators that have some
mnemonic value, although accelerators that use function keys are
acceptable.
.P
Applications should not use accelerators that are a combination of the
modifier
.B MAlt
and letter keys to avoid conflicts with mnemonics.
For example,
.B "MAlt <E>"
as an accelerator for 
.B Exit
conflicts with the use of
.B "MAlt <E>"
to pulldown the Edit menu
since it is the mnemonic for the 
.B Edit
CascadeButton in the MenuBar.
.P
Similarly, applications that involve
text entry should not use
accelerators that are combinations of the 
modifier
.B MShift
and letter keys to avoid conflict with the text entry commands.
Applications that expect field controls to have bindings that
include combinations of the modifier
.B MCtrl
and letter keys, such as text editors,
should also avoid these combinations.
.P
Accelerator bindings that use only one modifier
are preferable to bindings that use two or more modifier keys.
.H 3 "DialogBox Design"
.iX "DialogBox" "designing"
.iX "design" "DialogBox"
When designing a DialogBox, you should follow all the
same layout principles as for other applications areas.
DialogBoxes are usually transitory.
The user usually wants to simply respond and get back
to the primary tasks of the application. 
To help the user respond quickly, there are a number of
common DialogBox actions.  By using and ordering
these actions consistently, you provide the user
with cues to quickly respond to each DialogBox.
.P
When a DialogBox is displayed, all components within the DialogBox
should reflect the current state of the application.  For example,
if the DialogBox is used for changing the current font in
a text editor, the DialogBox should be initially displayed with
the current font.
If the DialogBox is modeless, then any changes to the application
should be updated in the DialogBox.
.P
DialogBoxes may limit how a user can interact with other windows
in order to force the order of interaction.
These limitations, which are called modes, are described in
the following text.
.VL 22n
.LI "\fBModeless\fP"
.iX "modeless
Allows interaction with the secondary window and all other
windows.
.LI "\fBPrimary modal\fP"
.iX "primary modal"
Does not allow interaction with any ancestor of the window.
.LI "\fBApplication modal\fP"
.iX "application modal"
Does not allow interaction with any window created by
the same application even if the application has multiple
primary windows.
.LI "\fBSystem modal\fP"
.iX "system modal"
Does not allow interaction with any window on the screen.
This includes windows from all other applications and any
icon box.  To indicate a system modal secondary window,
the pointer should change shape to a caution pointer
whenever it leaves the system modal secondary window. 
.LE
.P
Remember that the guidelines presented in this section,
like all the guidelines in this
.IR "Style Guide" ,
only apply in a left-to-right language direction
environment in an English locale.
You need to make the appropriate changes for other locales.
.H 4 "Common Dialog Box Actions"
.iX "common DialogBox actions"
.iX "DialogBox" "actions"
.iX "actions" "DialogBox"
.P
While your application may sometimes require special 
DialogBox actions, most share common actions.
The common actions provide a consistent
means for the user to quickly respond to DialogBoxes
and get back to the primary application tasks.
The common actions should be presented in a horizontal
collection of PushButtons at the bottom of the DialogBox,
separated from the rest of the DialogBox by a Separator.
.P
No DialogBox will contain all of the common actions in the following
list.
You should use the ones appropriate to your application
or determine new actions so they do not conflict with the
common actions listed.
If you create a new action, you should give it an
active-voice label that indicates its purpose.
A active-voice label describes the action that pressing the
button causes.
The actions are listed in the approximate sequence in which
they should appear in DialogBoxes.
.VL .75i
.LI \fBYes\fP
.iX "Yes"
.mS
Must indicate an affirmative response to a
question posed in the DialogBox, and then close the window.
.mE
While
.B Yes
is not an active-voice label, it implies a positive response
to a question in a
.BR QuestionDialog
or a
.BR WarningDialog .
Only use
.B Yes
if it is a clear answer to the question.
.LI \fBNo\fP
.iX "No"
.mS
Must indicate a negative response to a question posed in the DialogBox,
and then close the window.
.mE
While
.B No
is not an active-voice label, it implies a negative response
to a question in a
.BR QuestionDialog .
Only use
.B No
if it is a clear answer to the question.
.LI \fBOK\fP
.iX "OK"
.mS
Must cause the application to apply any changes made
in the DialogBox and dismiss the DialogBox.
.mE
While
.B OK
is not an active-voice label, its usage is too common to change.
.LI \fBApply\fP
.iX "Apply"
.mS
Must apply any changes made to components in the DialogBox.
.mE
.LI \fBRetry\fP
.iX "Retry"
.mS
Must cause the task in progress to be attempted again.
.mE
This action is commonly found in message boxes that report an
error.
.LI \fBStop\fP
.iX "Stop"
.mS
Must end the task in progress at the next possible breaking point.
.mE
This action is commonly found in a
.BR WorkingDialogBox .
.LI \fBReset\fP
.iX "Reset"
.mS
Must cancel any user changes that have not been applied to
the application.
It must also reset the status of the
DialogBox to the state since the last time the DialogBox
action was applied or to the initial state of the DialogBox.
.mE
.LI \fBCancel\fP
.iX "Cancel"
.mS
Must close the DialogBox without performing any DialogBox
actions not yet applied to the application.
Pressing
.B KCancel
anywhere in the DialogBox, except during a cancelable drag
operation, must perform the action of this button.
.mE
.LI \fBHelp\ .\ .\ .\fP
.iX "Help"
.mS
Must provide any help for the DialogBox.
.mE
The ellipses should not be used if the help action is not
provided in a new DialogBox.
.LE
.P
If a DialogBox action causes an error, the DialogBox should
not be dismissed before the error is displayed.  Instead,
the DialogBox should remain available after the error is
dismissed to give the user a chance to correct the error
and reuse the DialogBox.
.H 4 "Arranging Common Actions"
You should arrange PushButton actions in
DialogBoxes in the same way you arrange other PushButtons,
according to order and frequency of use.
The common action PushButtons should be ordered
as presented in the previous section.
Positive responses to the DialogBox should be presented
first, followed by negative responses, and canceling
responses.
.B Help
should always be the last action on the right.
.P
The following rules should be used when determining what
default buttons to place in a DialogBox.
.BL
.LI
Modal DialogBoxes should use one of the
following button arrangements unless superseded
by another rule:
.DS
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.LI
Modeless DialogBoxes should use one of the following
button arrangements unless superseded by another
rule:
.DS
\fBOK Apply Cancel\fP
\fBOK Apply Cancel Help\fP
\fBOK Apply Reset Cancel\fP
\fBOK Apply Reset Cancel Help\fP
.DE
.LI
Information DialogBoxes should use one of the following
button arrangements:
.DS
\fBOK\fP
\fBOK Help\fP
.DE
.LI
Question DialogBoxes should use one of the following
button arrangements:
.DS
\fBYes No\fP
\fBYes No Help\fP
.DE
.P
It is possible that both the
.B Yes
and
.B No
actions of a Question DialogBox will perform an action.  If
this is the case the Question DialogBox should use one of the
following button arrangements: 
.DS
\fBYes No Cancel\fP
\fBYes No Cancel Help\fP
.DE
.LI
Warning DialogBoxes should use one of the following
button arrangements:
.DS
\fBYes No\fP
\fBYes No Help\fP
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.LI
Working DialogBoxes should use one of the following
button arrangements:
.DS
\fBCancel\fP
\fBCancel Help\fP
\fBRetry Cancel\fP
\fBRetry Cancel Help\fP
.DE
.LE
.H 4 "Determining DialogBox Location and Size"
.iX "DialogBox" "location"
.iX "DialogBox" "size"
Your application determines the size and location
of its DialogBoxes.
You should size and place DialogBoxes so that they
do not obscure important information in other windows
of your application.
The initial size of a DialogBox should be large enough to contain the
dialog components without crowding or visual confusion, but otherwise
should be as small as possible.  DialogBoxes should follow the
same rules for resizing as a MainWindow as described in Section
4.2.2.3.
.P
You should place DialogBoxes on the screen so they are completely
visible.
.P
In general you should place
DialogBoxes close to either the component that caused it to be
displayed, the
current action, or the information needed to respond to it.
When a DialogBox relates to an item in an underlying window,
you should position the DialogBox to the right of the item.
If there is not enough room to the right of the item, try
to position the DialogBox to the left, below or above the item,
in that order, depending on screen space available.  You should
only obscure related information as a last resort.
.P
If a DialogBox does not relate to items in the underlying windows,
the DialogBox should be placed centered in the application's work
area.
.P
If two DialogBoxes need to overlay,
you should offset the top DialogBox
to the right and
below the title of the lower DialogBox.
Use your best judgement, knowing that the screen
area for DialogBoxes is limited.  
.P
While the previous suggestions seem simple enough, they
cannot always be followed completely.  Therefore, DialogBoxes, once
displayed, should be movable so that the user can relocate them as needed to
see information in underlying windows.
.H 2 "Interaction"
.iX "interactions"
This section gives guidelines for creating applications
with consistent interactions.  When an application behaves
as expected and the user is not surprised by the 
results of the actions, the user can complete tasks quicker.
This section presents the following guidelines for
good application interaction:
.BL
.LI
Supply indications of actions.
.LI
.iX "feedback"
Give feedback.
.LI
Allow user flexibility.
.LE
.H 3 "Supplying Indications of Actions"
The first step to consistent interaction is to provide
cues to the result of every action.  This means
that actions of components should be indicated
by the component's shape, label, and graphics.
It also means that the actions and interactions
of components should remain consistent, so the
user always knows what to expect.  
.P
Lastly, it means that interactions should be simple.
As interactions become complicated, it also becomes
difficult to visually represent the interaction.
Complicated interactions
and components create the possibility for more errors.
Even the most complicated concepts can be clarified
by careful organization, so if your application's
interactions seem complicated, consider reorganizing
them for simplicity.
.H 4 "Using Common Components"
Users expect components to behave consistently
across all applications.
PushButtons always perform an action.  
OptionButtons always provide selections.
Because of this, when users want to perform an
action, they look for a PushButton, usually in
a Menu.  They do not look for an OptionButton.
You should use the components that are provided when
appropriate, rather than create new ones.
You should not alter the look of a component
so that it is unrecognizable as the type of
component that it is.
.H 4 "Using Intuitive Labels"
One of the best indicators of the action
of a component is its label.  The label can
be either text or a graphic.  You should
choose your labels carefully to indicate the
action of each component.
.P
Components that perform actions should be labeled
with active verbs.
Components that present options should be
labeled with nouns.
You should also label component groups, including
Panels, with nouns to indicate the
the contents of the group.
.P
Consider the use of graphics as labels with two
cautions.  Graphic labels cannot be nouns or
active verbs, so choose a graphic whose meaning
is clear in the context of the component.  Also
remember that while graphic symbols are very language
independent, they can be highly culture specific.
In some cultures, a mailbox graphic can indicate a
mailer action, but since mailboxes are not common in
all cultures, an envelope graphic may be better, or you
can provide a mechanism for changing the graphic based on
the locale.
.H 4 "Using Graphics to Show Action" 
.iX "graphics"
Many components also include a small graphic symbol following
the label to indicate the action of the component.
CascadeButtons should use an arrow graphic that points
in the direction the cascading Menu will appear.
OptionButtons should use a rectangle graphic
to distinguish them from PushButtons.
Any component that causes a DialogBox to be displayed
should include an ellipses following the label.
.P
All previously mentioned graphics should follow the text
or graphic of the label.  In left-to-right direction
language environments, the graphic should be on the right.
In right-to-left direction language environments, the graphic
should be on the left.
Note that the graphic indicating the state of a ToggleButtons is
precedes the label in a left-to-right direction language environment.
.H 4 "Showing Default Actions"
.iX "default push button"
.iX "defaults"
Your application should use default values for common
settings or obvious selections.  Default values should
be shown in the on state.  For example, the default value for
a Text area should be in the Text area in the selected
state whenever text entry is requested; the default
selection in a List should be set in the selected state
whenever a list selection is requested; or the default
RadioButton should be filled in a Panel at application
start-up time.  In any case, once the state is changed,
the new state should take the place of the default until
the state is reset.  Your application can decide whether to
save its state after being closed. 
.P
Groups of controls, such as a DialogBox, can also have a default action.
The default action is usually activated by pressing
.B KActivate.
.B KActivate
is usually bound to
.BR <Return>
or
.BR MCtrl <Return> .
The default action of a component group should be
distinguished from the other selections by an
extra border as shown in Figure 4-14.
.br
.ne 2.1i
\0
.FG "A Default PushButton"
.sp 1.9i
.in +.9i
.P! graphics/dfltbut.ps 
.in -.9i
.P
The
.B OK
PushButton should be the default PushButton in
modal DialogBoxes and modeless DialogBoxes that are transient.
The
.B Apply
PushButton should be the default PushButton in a
modeless DialogBox that is likely to be displayed for
multiple actions.
.P
Using the keyboard to navigate through the PushButtons, the
button with the location cursor should become the default PushButton.
This ensures that pressing
.B KActivate
over a PushButton invokes the correct PushButton.
When the location cursor leaves the PushButtons, the
original default button should once again become the default.
.H 4 "Avoiding Component Modes"
.iX "components" "simple"
.iX "components" "modal"
.iX "modes"
A component has modes when its action changes based on some
previous action or the state of the application.
This is very confusing to the user, who was expecting
the original action of the component.
Components in your application should not have modes.
Your application should use multiple components, rather
than modal components.
.H 4 "Showing Unavailable Components"
.iX "disabling components"
As the state of your application changes, certain
components become inappropriate.
For example, the
.B Minimize
selection in a window Menu is inappropriate when the window is
already minimized.  In such cases, you should make the inappropriate
components unavailable.
This is also called disabling the components.
Disabled components should be visually deemphasized, usually
by graying the label of the component.
.P
You should not remove unavailable components from
the application client areas.  The components should
remain visible to remind the user of their existence,
and to ensure application consistency.
Figure 4-15 shows a disabled Menu element.
.br
.ne 3.0i
\0
.FG "A Disabled Menu Element"
.sp 2.8i
.in +.6i
.P! graphics/disable.ps 
.in -.6i
.sp
.P
You should disable Menu items and components to help avoid errors.
In general,
you should disable the lowest level component that results in an
irreversible error state.  For example, consider a graphics editor that has
a DialogBox that is used for aligning selected graphics objects.
If the user may want to see the choices in the DialogBox even if
the choices are not available,
this DialogBox should be displayable at all times; its Menu item should
not be disabled.  If there are no graphics objects to align,
the DialogBox should still be displayed, but
its
.B OK
and
.B Apply
buttons should be disabled.
Once graphics objects are selected, the
.B OK
and
.B Apply
buttons should become enabled.
.P
Menu items that are inappropriate and will result in error messages
should be disabled.  Some examples are the 
.B Edit
Menu's
.B Cut
and
.B Copy
actions when nothing is selected.
The
.B Edit
Menu's
.B Undo
entry should be disabled if the last operation cannot be undone.
.P
Menu items that perform no action, need not be disabled.
For example, the
.B New
entry in the
.B File
Menu need not be disabled immediately after it is invoked.
It can be selectable repeatedly, even though it
performs no new action, because it does not result in an error state.
.P
A Menu item that displays a modal DialogBox should be disabled if it would
cause an error either on display of the box, or on clicking the
.B OK
or
.B Apply
buttons in the DialogBox.
Since the DialogBox is modal, nothing can be changed in the
application to correct the error until the box is dismissed,
and it should not be displayable
until the application is in a state that makes the action of
the DialogBox possible.
.P
Menu items that display modeless DialogBoxes should never be disabled.
If a box is modeless, at any time the user can change
the state of the application to make the DialogBox useful.
If the action cannot be completed because some necessary information is not
yet available, then the
.B OK
and
.B Apply
buttons should be disabled.
.P
You should avoid frequently disabling and enabling components in
situations where the state change would cause a distracting flashing.
For example, editing a Text component in a DialogBox may cause some
buttons to be inappropriate at each invalid text value.
In this case, you should display an error message if
the
.B OK
or
.B Apply
button is clicked for an inappropriate text value.
Of course, the error message should explain the valid text values.
.H 3 "Providing Feedback"
.iX "feedback"
Another important element to user interaction is
providing feedback about the current state of the
application.  This is done,
as described in the previous section, using labels
and graphics, and by keeping the interface consistent.
Your application should also dynamically indicate
the state of the application's actions.  For example,
the mouse pointer shape changes to indicate when
and where special actions can occur.  Chapter 2
describes mouse pointer shapes in detail.
Other ways to provide the user feedback are
described in the following sections.
.H 4 "Showing Progress"
.iX "progress"
.iX "delay"
If an action takes a long time to complete,
the user may mistake the delay to mean that
the system or the application stopped
working.  For actions that take a long time
to complete, your application should indicate
that there will be a delay with a
.BR WorkingDialog .
If your application can track the progress of long
actions, it should try to update the
.B WorkingDialog
with the progress of the action.
.H 4 "Providing Warnings"
.iX "warnings"
Certain actions can cause destructive results,
such as closing an application before saving changes
in the current file.
Applications should not disallow such destructive
actions, instead they should warn the user of
the consequences with a
.BR WarningDialog.
.mS
The
.B WarningDialog
must allow the user to cancel the destructive action.
.mE
Note that too many
.B WarningDialogs 
can be disruptive to the user's main task.
.B WarningDialogs
should be reserved for truly destructive actions.
For destructive actions that can easily be recovered
from,
applications should provide undo actions to reverse them;
for example, the
.B Undo
element of the
.B Edit
PullDown Menu.
.H 4 "Providing Help"
.iX "Help" 
.iX "help" "contex-sensitive"
Even in the most intuitive application, the purpose
of a component or the way to do a task can be
hard to figure out for a new user.  Your application
should provide a help mechanism for all of its aspects.
Section 4.2.1.5 describes the most common base for
a help mechanism, the
.B Help
PullDown Menu on the MenuBar.
Context-sensitive help should also be available
by using the
.BR KHelp
key.
.B KHelp
is usually bound to
.BR <F1> ,
or 
.BR <Help> .
.H 3 "Allowing User Flexibility"
.iX "flexibility"
Good user-application interaction should also allow user
flexibility.  No matter
how well your application is designed, some users will
not like parts of it.  They will want to change
some elements of it; for example, from simple elements
like the colors and fonts to complicated elements
like the default values.  You should allow users
to adjust elements of your applications because it
increases their sense of control over the applications.
You should consider the following attributes of your
applications for user customization:
.BL
.LI
Application parameters
.LI
Colors
.LI
Fonts
.LI
Default values
.LI
Key bindings
.LI
Labels
.LI
Messages
.LI
Help information
.LE
.P
The exact list of attributes you should allow the user to
customize depends on your application.
.H 2 "Component Design"
.iX "components" "design"
.iX "design" "components"
For consistency with other applications,
you should always try to use existing components for
your application tasks, but there are cases where
new components are needed.
In designing new components, you should follow the
same rules as application designers follow.
You should think of components as small applications.
They perform a task, present information to the user,
and take information from the user.
.mS
New components must follow the guidelines for designing
applications.
.mE
.P
The first step to designing a new component is to compare
its features with those of the other components.  If the
new component has a feature that is the same as another
component, the mechanisms for using the feature,
layout, key bindings, graphics, and so on,
should be the same in the new component.
You should also try to match the appearance style
of components on your system.  Most OSF/Motif
compliant systems use a 3-dimensional beveled
presentation style.  Appearance is not specified
as a matter of component style in this
.IR "Style Guide" ;
however, any new components will assimilate better with existing
components if they try to conform to the implied appearance
style for the system on which they will be used.
