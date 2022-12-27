.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 1
.mc |
...\".nr !_ 0
.mc
.H 1 "Widgets, Gadgets, and Convenience Functions"
.iX "Widget" "definition"
.iX "Convenience functions"
Motif has a variety of widgets and gadgets, each designed to
accomplish a specific set of tasks, either individually or in combination
.mc |
with others.  Convenience functions create certain
.mc
widgets or sets of widgets for a specific purpose.  This chapter explains
widgets, gadgets, and convenience functions.
.H 2 "Widgets"
.iX "Widget"
Widgets are used either individually or in combination to make
the creation of complex applications easier and faster.
Some widgets display information, others are merely containers for
.mc |
other widgets.  Some widgets are restricted to displaying information 
and do not react to keyboard or mouse input.  Others change their 
display in response to input and can invoke functions when instructed 
to do so.  You can customize some aspects of a widget, such as fonts, 
foreground and background colors, border widths and colors, and sizes.
.mc
.P
.iX "Widget" "instance"
.iX "Instance"
.mc |
.ne 4
.mc
An \fBinstance\fP of a widget class is composed of 
a data structure containing values and procedures for that particular widget
instance. 
.iX "Class" "widget structure"
.iX "Widget" "class structure"
There is also
.mc |
a class structure that contains values and procedures applicable to all
.mc
widgets of that class.  
.P
.iX "Widget" "classes"
.iX "Class" "widget"
Widgets are grouped into several classes, depending on the function of
the widget.  Logically, a widget class consists of
the procedures and data associated with all
.mc |
widgets belonging to that class.  These procedures and data can be 
inherited by subclasses.  Physically, a widget class is a pointer to a 
structure.  The contents of this structure are constant for all widgets
of the widget class.  A widget instance is allocated and initialized 
by \fBXmCreate\fIwidgetname\fP, \fBXtCreateWidget\fR, or
\fBXtCreateManagedWidget\fP.  See Chapter 3, "Using Motif Widgets in 
Programs," for specific examples of creating widgets.
.mc
.iX "Widget" "create"
.iX "Create" "widget"
.P
This section provides an overview of the available widgets.
.mc |
The manpages in the \fIOSF/Motif Programmer's Reference\fP
.mc
contain detailed information for each of
the widgets.
.mc |
The following figure shows how widgets might be combined in an application.
.mc
.ig ++
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 22P
.\".P
.\".sC graphics/xmfontsm.tif tiff 300 28P 20P
.\"\&
.\".P
.\".sp +1
.\".FG "Widget Application Screen"
.\".\}
.\".el \{\
.\".br
.\".ne 22P
.\"\&
.\".sp 20P
.\".ce
.\"illustration xmfontsm.ps goes here
.\".sp +1
.\".\}
.++
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "Widget Application Screen"
.mc |
.sp 3.5i
.in +.43i
.P! graphics/xmfontsm.ps 
.in -.43i
.ne 12
.mc
.P
.mc |
Several types of widgets are shown in Figure 2-1.  The large window is a
MainWindow widget containing a menu bar and some push buttons,
.mc
a RowColumn widget with a number of 
.mc |
push button gadgets, and a vertical scroll bar.  The 
.mc
program that produces this window is called
.mc |
\fBxmfonts\fP and is described in Chapter 3.
.mc
.P
The sections in this chapter divide the widgets into five categories as
.mc |
shown in the table below.
.sp
.mc
.iX "Widgets" "categories"
.iX "Categories of widgets"
.iX "Shell" "widgets"
.iX "Display widgets"
.iX "Container widgets"
.iX "Dialog" "widgets"
.iX "Menu widgets"
.mc *
.mc
.TB "Categories of Widgets"
.mc |
.TS H
center, box, tab(;);
lfB  |  lfB
l    |  l   .
Class Name;Widget Class
.mc |
.TH
.mc
_
\fBShell Widgets\fP
XmDialogShell;xmDialogShellWidgetClass
.mc |
XmMenuShell;xmMenuShellWidgetClass
VendorShell;vendorShellWidgetClass
.mc
_
\fBDisplay Widgets\fP
.mc |
Core;widgetClass
.mc
XmPrimitive;xmPrimitiveWidgetClass
XmArrowButton;xmArrowButtonWidgetClass
XmDrawnButton;xmDrawnButtonWidgetClass
XmLabel;xmLabelWidgetClass
XmList;xmListWidgetClass
XmPushButton;xmPushButtonWidgetClass
XmScrollBar;xmScrollBarWidgetClass
XmSeparator;xmSeparatorWidgetClass
XmText;xmTextWidgetClass
.mc |
XmTextField;xmTextFieldWidgetClass
.mc
XmToggleButton;xmToggleButtonWidgetClass
_
\fBContainer Widgets\fP
XmManager;xmManagerWidgetClass
XmDrawingArea;xmDrawingAreaWidgetClass
XmFrame;xmFrameWidgetClass
XmMainWindow;xmMainWindowWidgetClass
.mc |
XmPanedWindow;xmPanedWindowWidgetClass
.mc
XmRowColumn;xmRowColumnWidgetClass
XmScale;xmScaleWidgetClass
XmScrolledWindow;xmScrolledWindowWidgetClass
_
\fBDialog Widgets\fP
XmBulletinBoard;xmBulletinBoardWidgetClass
XmCommand;xmCommandWidgetClass
XmFileSelectionBox;xmFileSelectionBoxWidgetClass
XmForm;xmFormWidgetClass
XmMessageBox;xmMessageBoxWidgetClass
XmSelectionBox;xmSelectionBoxWidgetClass
_
\fBMenu Widgets\fP
XmCascadeButton;xmCascadeButtonWidgetClass
.mc *
.mc
.TE
.mc *
.mc
.H 3 "Shell Widgets"
.iX "Shell widgets"
.iX "Widget" "shell"
Shell widgets are top-level widgets that provide the necessary interface
with the window manager.  Different Shell widget classes are provided for
the various categories of top-level widgets.  The Xt Intrinsics provide
some underlying shells and the Motif toolkit provides the remaining shells.
The Xt Intrinsics provide the following shell classes:
.iX "OverrideShell"
.iX "WMShell"
.iX "VendorShell"
.iX "TransientShell"
.iX "TopLevelShell"
.iX "ApplicationShell"
.BL
.LI
\fBShell\fP - This is the base class for shell widgets.  It is
a subclass of Composite
and provides resources for all other types of shells.
.LI
.mc |
\fBOverrideShell\fP - This class is used for shell windows that completely
.mc
bypass the window manager.  It is a subclass of Shell.
.LI
\fBWMShell\fP - This class contains resources that are necessary for the common
window manager protocol.  It is a subclass of Shell.
.LI
\fBVendorShell\fP - This class contains resources used by vendor-specific
window managers. It is a subclass of WMShell.
.LI
\fBTransientShell\fP - This class is used for shell windows that can be
manipulated by the window manager but cannot be iconified.  It is a 
subclass of VendorShell.
.LI
\fBTopLevelShell\fP - This class is used for normal top-level windows.  It is a
subclass of VendorShell.
.LI
\fBApplicationShell\fP - This class is used for an application's top-level
window.  It is a subclass of TopLevelShell.
.LE
.P
The classes Shell, WMShell, and VendorShell are internal and cannot be
instantiated.
.P
The Motif toolkit provides the following widgets:
.mc |
.ne 15
.VL .5i
.LI "XmDialogShell (xmDialogShellWidgetClass)"
.br
.mc
.iX "XmDialogShell"
.iX "DialogShell"
The DialogShell widget class is a subclass of TransientShell.
Instances of this class are used as the parents of modal and modeless
Dialogs associated with other top-level windows.  DialogShell provides
proper communication with the Motif window manager in accordance with
.mc |
the \fIInter-Client Communications Conventions Manual \fR(ICCCM) for secondary
top-level windows, such as Dialogs.  See Chapter 5, ``Dialog
Widgets,'' for more information about how this widget is used.
.LI "XMenuShell (xmMenuShellWidgetClass)"
.br
.mc
.iX "XmMenuShell"
.iX "MenuShell"
The MenuShell widget class is a subclass of OverrideShell.  Instances
.mc |
of this class are used as the parents of menu panes.  See Chapter 6,
.mc
``Menus,'' for the specifications of menu widgets and menu shells.
.mc |
.LI "VendorShell (vendorShellWidgetClass)"
.br
.mc
.iX "VendorShell"
The VendorShell widget class is a subclass of WMShell. It provides
the common state information and services needed by the window-manager
.mc |
visible shells.  See Chapter 4, ``Shell Widgets,'' for more information.
.LE
.mc
.H 3 "Display Widgets"
.iX "Widget" "display"
.iX "Display widgets"
.mc |
\fBNOTE:\fP
.mc
A complete list of 
resources for each class can be found in the appropriate
.mc |
man page in the \fIOSF/Motif Programmer's Reference\fP.
.mc
Motif provides the following
display widgets:
.mc |
.VL .5i
.LI "Core (widgetClass)"
.br
.mc
.iX "Core"
The Core class is used as a supporting superclass for other widget classes.
It provides common resources that are needed by all widgets, including
.mc |
\fIx\fP and \fIy\fP location, height, width, window border width, and so on.
.LI "XmPrimitive (xmPrimitiveWidgetClass)"
.br
.mc
.iX "XmPrimitive"
.iX "Primitive"
The XmPrimitive class is also used as a supporting superclass for other widget
classes.  It provides resources for border drawing and highlighting,
traversal activation and deactivation, and so on.  
.mc |
.LI "XmArrowButton (xmArrowButtonWidgetClass)"
.br
.mc
.iX "XmArrowButton"
.iX "ArrowButton"
The ArrowButton widget consists of a directional arrow surrounded by a
.mc |
border shadow.  When the arrow button is selected, the shadow moves to give
the appearance that the arrow button has been pressed.  When the
arrow button is unselected, the shadow moves to give the appearance that the
arrow button is released, or out.  The arrow button has the same 
functionality as the push button.  The following figure shows four arrow buttons
.mc
arranged within a RowColumn widget.
.mc |
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 17P
.\".P
.\".sC graphics/arwbtns.tif tiff 300 14P 15P
.\"\&
.\".P
.\".sp +1
.\".FG "ArrowButtons"
.\".sp +1.5
.\".\}
.\".el \{\
.\".br
.\".ne 17P
.\"\&
.\".sp 15.5P
.\".ce
.\"illustration arwbtns.eps goes here
.\".sp +1
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "ArrowButtons"
.mc |
.sp 2.75i
.in +1.7i
.P! graphics/arwbtns.ps 
.in -1.7i
.mc
.sp .5
.P
.\".\}
.iX "XmArrowButton" "arrow direction"
.mc |
.in +.5i
.mc
The direction of the arrow is specified by setting the
\fBXmNarrowDirection\fP resource to the appropriate value.
.iX "RowColumn" "spacing between children"
The spacing  
.mc |
between the ArrowButtons in Figure 2-2 was obtained by setting the 
.mc
RowColumn resources \fBXmNmarginWidth\fP, \fBXmNmarginHeight\fP, and
\fBXmNspacing\fP to 20.
.mc |
.in -.5i
.VL .5i
.LI "XmDrawnButton (xmDrawnButtonWidgetClass)"
.br
.mc
.iX "XmDrawnButton"
.iX "DrawnButton"
The DrawnButton widget consists of an empty widget window surrounded by a 
shadow border.
It provides the application developer with a graphics area
.mc |
that can have the input semantics of push buttons.
.sp
.ne 5
.mc
Callback types are defined for widget exposure and resize to allow the 
application to redraw or reposition its graphics.
If the DrawnButton widget has a highlight and shadow thickness, 
the application should take care not to draw in this area.
This can be done by creating the graphics context to be used for
drawing in the widget with a clipping rectangle.  The
clipping rectangle should take into account the size of the
widget's highlight thickness and shadow.
.mc |
.LI "XmLabel (xmLabelWidgetClass)"
.br
.mc
.iX "XmLabel"
.iX "Label"
.mc |
A Label widget consists of either text or graphics.  It can be instantiated but
it is also used as a superclass for button widgets.  A label's text is a
.mc
compound string and can be multidirectional, multiline, multifont, or
.mc |
any combination of these.  A label
.mc
is considered static because it does not accept any button or key input
.mc |
other than the help button on the widget.  The help callback is
.mc
the only callback defined for Label.
.mc |
.LI "XmList (xmListWidgetClass)"
.br
.mc
.iX "XmList"
.iX "List"
The List widget allows you to make a selection from a list of items.  The
application defines an array of compound strings, each of which becomes
an item in the list.  You can set the number of items in the list that
.mc |
are to be visible.  You can also choose to have the list appear with a
scroll bar so that you can scroll through the list of items.  Items are
.mc
selected by moving the pointer to the desired item and pressing the mouse
.mc |
button or key defined as select.  The selected item is displayed in 
inverse colors. The following figure shows the List Widget.
.ne 10
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 24.5P
.\".C
.\".sC graphics/list1.tif tiff 300 10P 22.5P
.\"\&
.\".P
.\".sp +1
.\".FG "List Widget"
.\".sp +1.5
.\".\}
.\".el \{\
.\".br
.\".ne 24.5P
.\"\&
.\".sp 22.5P
.\".ce
.\"illustration list.1.eps goes here
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "List Widget"
.mc |
.sp 3.75i
.in +1.94i
.P! graphics/list1.ps 
.in -1.94i
.mc
.sp .5
.P
.\".\}
.mc |
.VL .5i
.LI "XmPushButton(xmPushButtonWidgetClass)
.br
.mc
.iX "PushButton"
.iX "XmPushButton"
.mc |
The PushButton widget consists of a text label or pixmap surrounded by a border
.mc
shadow.  You select
the button by moving the mouse cursor to the button and
pressing mouse button 1.
When the mouse button is pressed, the widget and shadow colors will invert,
.mc |
giving the appearance that the push button has been pressed.  When the
.mc
mouse button is released, the colors will revert to the
.mc |
original color scheme, giving the appearance that the push button is
out.  Push buttons are used to invoke actions, such as
.mc
run, cancel, stop, and so on.
.mc |
.ne 4
.LI "XmScrollBar (xmScrollBarWidgetClass)
.br
.mc
.iX "ScrollBar"
.iX "XmScrollBar"
The ScrollBar widget allows you to view data that is too large to be viewed
.mc |
in its entirety.  Scroll bars are combined with a widget that contains the
data to be viewed.  When you interact with the scroll bar, the data scrolls.
.mc
The viewable portion of the data is called the work area.
.mc |
.br
.ne 5
.sp
A scroll bar consists of two arrows pointing in opposite directions at each
.mc
end of a small rectangle.  The rectangle is called the \fBscroll region\fP.
A smaller rectangle called a slider is positioned within the scroll region.
The slider is normally colored to contrast with that of the scroll region.
The ratio of the slider size to the scroll region size corresponds to the 
relationship between the visible data and the total data.  For example,
.mc |
if 10 percent of the data is visible in the work area, the slider takes up 10
.mc
percent of the scroll region.
.mc |
.sp
You may place the scroll bar horizontally, vertically, or both.  Horizontal
scroll bars are placed at the bottom edge of the work area and 
vertical scroll bars are placed on the right edge. 
The ScrollBar widget is shown in the following figure.
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 16.5P
.\".C
.\".sC graphics/scrolbar.tif tiff 300 16P 14.5P
.\"\&
.\".P
.\".sp +1
.\".FG "ScrollBars"
.\".\}
.\".el \{\
.\".br
.\".ne 16.5P
.\"\&
.\".sp 14.5P
.\".ce
.\"illustration scrolbar.eps goes here
.br
.ne 2.75i
\0
.mc |
.FG "ScrollBars"
.mc
.sp 2.5i
.mc |
.in +1.44i
.mc
.P! graphics/scrolbar.ps 
.mc |
.in -1.44i
.mc
.sp .5
.P
.\".\}
.mc |
.VL .5i
.LI "XmSeparator (xmSeparatorWidgetClass)"
.br
.mc
.iX "XmNSeparator"
.iX "Separator"
.iX "Primitive"
.iX "Widget" "primitive"
Separator is a primitive widget to be used as an
item separator placed between items in a display.  Several different
.mc |
line-drawing styles are provided as well as horizontal or vertical
.mc
orientation.
.mc |
.sp
.br
.ne 6
The line drawing done within the separator is automatically
.mc
centered within the height of the widget for a horizontal orientation,
and centered within the width of the widget for a vertical orientation.
.mc |
.sp
.mc
The XmNseparatorType of XmNO_LINE is provided as an escape to the application
programmer who needs a different style of drawing.
A pixmap the height of the widget can be created and used as the 
background pixmap by building an argument list using the XmNbackgroundPixmap
argument type as defined by Core.
.mc |
Whenever the widget is redrawn, its background containing the desired 
separator drawing is displayed.
.LI "XmText (xmTextWidgetClass)"
.br
.mc
.iX "Text"
.iX "XmText"
.mc |
The Text widget provides a single-line or multiline text editor that has a
.mc
user and programmer interface that you can customize.  It can be used for
single-line string entry, forms entry with verification procedures, multipage
.mc |
document viewing, and full-screen editing.  The following figure shows the Text widget.
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 15.5P
.\".P
.\".sC graphics/text1.tif tiff 300 18.5P 13.5P
.\"\&
.\".P
.\".sp +1
.\".FG "Text Widget"
.\".sp +1.5
.\".\}
.\".el \{\
.\".br
.\".ne 15.5P
.\"\&
.\".sp 13.5P
.\".ce
.\"illustration text1.eps goes here
.br
.ne 3i
\0
.mc *
.mc
.FG "Text Widget"
.mc |
.sp 2.5i
.in +1.31i
.P! graphics/text1.ps 
.in -1.31i
.mc
.sp .5
.\".\}
.P
.mc |
.VL .5i
.LI "XmTextField (xmTextFieldWidgetClass)"
.br
.iX "TextField"
.iX "XmTextField"
The TextField widget is similar to the Text widget, but is restricted to
and optimized for single-line text editing.
As with the Text widget, you can customize the user and programmer
interface.
.LI "XmToggleButton (xmToggleButtonWidgetClass)"
.br
.mc
.iX "ToggleButton"
.iX "XmToggleButton"
This widget consists of a text or graphics button face with an indicator
(a square or diamond-shaped box) placed to the left of the text or
graphics.  Select
.mc |
the toggle button by moving the mouse cursor inside the rectangle and
.mc
pressing mouse button 1.  The indicator
is then filled with the selection color, indicating
.mc |
that the toggle button is selected.   Toggle buttons are used for setting
nontransitory data within an application.  The following figure shows Toggle Buttons.
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 15.5P
.\".C
.\".sC graphics/toggles.tif tiff 300 20.5P 17.5P
.\"\&
.\".P
.\".sp +1
.mc |
.\".FG "Toggle Buttons"
.mc
.\".\}
.\".el \{\
.\".br
.\".ne 15.5P
.\"\&
.\".sp 13.5P
.\".ce
.\"illustration toggles.eps goes here
.\".sp +1
.mc |
.\".\}
.mc
.br
.ne 3i
\0
.mc |
.FG "ToggleButtons"
.mc
.sp 2.75i
.mc |
.in +1.56i
.mc
.P! graphics/toggles.ps 
.mc |
.in -1.56i
.mc
.sp .5
.P
.mc *
.mc
.H 3 "Container Widgets"
.iX "Widget" "container"
.iX "Container widgets"
.iX "Core"
.iX "Constraint"
.iX "Composite"
.iX "XmManager"
Container widgets are Composite widgets that provide
applications with general layout functionality.  Since they are Composite
widgets, Container widgets can have children.
All of the container widgets are built from the 
.mc |
Core, Composite, Constraint, and XmManager widget classes.  
.P
Motif
.mc
provides the following container widgets:
.mc |
.VL .5i
.LI "XmManager (xmManagerWidgetClass)"
.br
.mc
.iX "XmManager"
The XmManager class is a Motif widget meta class and is
therefore never instantiated
as a widget.  Its sole purpose is to act as a supporting superclass for other
widget classes.  It supports the
.mc |
visual resources, graphics contexts, and traversal resources necessary for the
.mc
graphics and traversal mechanisms.  XmManager is built from Core,
Composite,
and Constraint.
.mc |
.br
.ne 6
.LI "XmDrawingArea (xmDrawingAreaWidgetClass)"
.br
.mc
.iX "XmDrawingArea"
.iX "DrawingArea"
.mc |
The DrawingArea widget is an empty widget that easily adapts
.mc
to a variety of purposes.
DrawingArea does no drawing and defines no behavior except for invoking callbacks.
Callbacks notify the application when graphics need to be drawn
(exposure events or widget resize), and when the widget receives input from
the keyboard or mouse.
Applications are responsible for defining appearance and behavior as needed
in response to DrawingArea callbacks.
.mc |
.sp
.mc
DrawingArea is a Composite widget and is a subclass of XmManager.  It supports
minimal geometry management for multiple widget or gadget children.
.mc |
.LI "XmFrame (xmFrameWidgetClass)"
.br
.mc
.iX "XmFrame"
.iX "Frame"
.mc |
The XmFrame widget is a manager used to enclose a single child within
.mc
a border drawn by the XmFrame widget.  It is most often used to enclose other
Managers when it is desired to have the same border appearance for the
XmManager and XmPrimitive widgets it manages.
.mc |
.LI "XmMainWindow (xmMainWindowWidgetClass)"
.br
.mc
.iX "XmMainWindow"
.iX "MainWindow"
.iX "MenuBar"
.iX "CommandWindow"
.iX "Work region"
.iX "ScrollBar"
The XmMainWindow widget provides a standard layout for the primary window
.mc |
of an application.  This layout includes a menu bar, a command window, a work
region, and scroll bars.  Any or all of these areas are optional.  The
work region and scroll bars in the main window behave exactly the same as
.mc
their counterparts in the ScrolledWindow widget.
.iX "ScrolledWindow"
You can think of the
.mc |
main window as an extended scrolled window with an optional menu bar and an
optional command window.
.sp
In a fully loaded main window, the menu bar spans the top of the window
horizontally.  The command window spans the main window horizontally and is
placed just below the menu bar.  Any space below the command window is
managed exactly the same as the scrolled window.  To create a fully loaded
main window, you create a menu bar, a command window, two scroll bars (one
.mc
horizontal and one vertical), and a widget to use as the work region.  
.iX "XmMainWindowSetAreas"
.mc |
You then call \fBXmMainWindowSetAreas\fP with those widget IDs.
.ne 5
.LI "XmRowColumn (xmRowColumnWidgetClass)"
.br
.mc
.iX "XmRowColumn"
.iX "RowColumn"
.mc |
The RowColumn widget is a general-purpose RowColumn manager capable
.mc
of containing any widget type as a child.  It requires no special
knowledge about how its children function and provides nothing above
and beyond support for several different layout styles.  
.mc |
.br
.ne 7
.sp
.mc
The type of layout performed is controlled by how the application has set 
the various layout resources.
It can be configured to lay out its children in either a row or a
column fashion.  In addition, the application can specify whether
the children should be packed tightly together (not into organized rows
and columns), or whether each child should be placed in an identically
sized box (thus producing a symmetrical look), or whether specific
.mc |
layout should be done (the current \fIx\fP and \fIy\fP positions of the children
.mc
control their location).
.mc |
.sp
.mc
In addition, the application has control over both the spacing that
occurs between each row and column, and the margin spacing 
between the edges of the RowColumn widget and any children
that are placed against it.
.mc |
.sp
.mc
.iX "Frame"
.mc |
The RowColumn widget has no 3-dimensional visuals associated with it. If
.mc
you want an application
.mc |
to have a 3-dimensional shadow placed around the RowColumn widget, then you
.mc
should create the RowColumn widget as a child of a Frame widget.
.mc |
.LE
.mc
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 8P
.\".C
.\".sC graphics/ptighth.tif tiff 300 18P 6P
.\"\&
.\".P
.\".sp +1
.\".FG "RowColumn Widget"
.\".\}
.\".el \{\
.\".br
.\".ne 8P
.\"\&
.\".sp 6P
.\".ce
.\"illustration ptighth.eps goes here
.\".sp +1
.\".FG "RowColumn Widget"
.\".sp +1.5/
.\".\}
.br
.mc |
.ne 1.65i
.mc
\0
.mc *
.mc
.FG "RowColumn Widget"
.mc |
.sp 1.35i
.in +1.3i
.P! graphics/ptighth.ps 
.in -1.3i
.mc
.P
.mc |
.VL .5i
.LI "XmScale (xmScaleWidgetClass)"
.br
.mc
.iX "XmScale"
.mc |
.iX "Scale"r
.mc
The Scale widget has two basic functions.
.BL
.LI
It is used by an
application to indicate a value from within a range of values.
.LI
It allows the user to input or modify a value from the same range.
.LE
.mc |
.ne 4
.sp
.mc
A Scale widget allows you to select a value from a range of displayed values
by adjusting an arrow to a position along a line.  
.mc |
A scale has an elongated rectangular region similar to that of a
scroll bar.  Inside this region is a slider that is used to
indicate the current value along the scale.  You can 
modify the value of the scale by moving the slider within the
rectangular region of the scale.  A scale can also include a set
of labels and "tick marks" located outside of the scale region.
.mc
These can be used to indicate the relative value at various positions
along the scale.
.mc |
.sp
A scale can be either input and output or output only.  An input/output
scale is one whose value can be set by the application and also 
modified by the user by using the slider.  An output-only scale is one
.mc
that is used strictly as an indicator of the current value of something
and cannot be modified interactively by the user.  The Core resource
.mc |
\fBXmNsensitive\fP is used to specify whether the user can interactively
modify the value of the scale.
.LE
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 25P
.C
.sC graphics/scale.tif tiff 300 11.5P 25P
\&
.P
.sp +1
.FG "Scale Widget"
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration scale.eps goes here
.sp +1
.FG "Scale Widget"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4.5i
.mc
\0
.mc *
.mc
.FG "Scale Widget"
.mc |
.sp 4.25i
.in +1.8i
.P! graphics/scale.ps 
.in -1.8i
.br
.ne 5
.mc
.P
.mc |
.VL .5i
.LI "XmScrolledWindow (xmScrolledWindowWidgetClass)"
.br
.mc
.iX "XmScrolledWindow"
.iX "ScrolledWindow"
.mc |
The ScrolledWindow widget combines one or more scroll bar widgets and a
.mc
viewing area to implement a visible window onto some other (usually larger)
data display.  The visible part of the window can be scrolled through the
.mc |
larger display by the use of scroll bars.
.sp
To use the scrolled window, an application first creates a ScrolledWindow
.mc
widget, the needed ScrollBar widgets, and a widget capable of displaying
.mc |
any desired data as the work area of the scrolled window.  The scrolled window
will position the work area widget and display the scroll bars if so
requested.  When the user performs some action on the scroll bar, the
application will be notified through the normal scroll bar callback
.mc
interface.  
.mc |
.sp
The scrolled window can be configured to operate in an automatic manner, so 
.mc
that it performs all scrolling and display actions with no need for application
program involvement. It can also be configured to provide a minimal support 
framework in which the application is responsible for processing all user input
and making all visual changes to the displayed data in response to that input.
.mc |
.sp
When the scrolled window is performing automatic scrolling, it will create a
.mc
clipping window. Conceptually, this window becomes the viewport through which
the user examines the larger underlying data area. The application simply 
creates the desired data, then makes that data the work area of the
.mc |
scrolled window.
.mc
When the user moves the slider to change the displayed data, the 
workspace is moved under the viewing area so that a new portion of the data
becomes visible.
.mc |
.sp
.mc
There are situations where it is impractical for an application to create a 
large data space and simply display it through a small clipping window. An
example of this is a text editor \*(EM there would be an undesirable amount
of overhead involved with creating a single data area that consisted of a 
.mc |
large file. The application should use the concept of a scrolled window
.mc
(a small viewport onto some larger data), but it should be notified 
when the user scrolls the viewport so it can bring in more data from
.mc |
storage and update the display area.  For this situation,
the scrolled window can be 
.mc
configured so that it provides only visual layout support.  No clipping window
is created and the application must maintain the data displayed in the 
.mc |
work area as well as respond to user input on the scroll bars.  The figure below
shows a
scrolled window with some text in it.  Note that the scroll bars 
indicate that scrolling is possible either vertically or horizontally.
.LE
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 19P
.C
.sC graphics/scrollw1.tif tiff 300 25P 17P
\&
.P
.sp +1
.FG "ScrolledWindow Widget"
.\}
.el \{\
.br
.ne 19P
\&
.sp 17P
.ce
illustration scrollw1.eps goes here
.sp +1
.FG "ScrolledWindow Widget"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.25i
.mc
\0
.mc *
.mc
.FG "ScrolledWindow Widget"
.mc |
.sp 3i
.in +.8i
.P! graphics/scrollw1.ps 
.in -.8i
.mc
.P
.mc |
.in +.5i
Figure 2-10 shows the same window after partially scrolling down.  Compare the
positions of the vertical scroll bar and the text with those of Figure 2-9.
.in -.5i
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 19P
.C
.sC graphics/scrollw2.tif tiff 300 25P 17P
\&
.P
.sp +1
.FG "ScrolledWindow After Scrolling"
.\}
.el \{\
.br
.ne 19P
\&
.sp 17P
.ce
illustration scrollw2.eps goes here
.sp +1
.FG "ScrolledWindow After Scrolling"
.sp +1.5
.\}
.++
.br
.mc |
.ne 3.25i
.mc
\0
.mc |
.FG "ScrolledWindow After Scrolling"
.sp 3i
.in +.8i
.mc
.P! graphics/scrollw2.ps 
.mc |
.in -.8i
.mc
.sp
.mc |
.ne 3
.VL .5i
.LI "XmPanedWindow (xmPanedWindowWidgetClass)"
.br
.mc
.iX "XmPanedWindow"
.iX "PanedWindow"
.iX "Composite" "widget"
.iX "Widget" "composite"
The PanedWindow manager widget is a Composite widget that lays out children in a
.mc |
vertically tiled format.  Children appear from top to bottom, with
.mc
the first child inserted appearing at the top of the PanedWindow manager and the
last child inserted appearing at the bottom.  The
PanedWindow manager will grow to match the width of its widest child, and all
other children are forced to this width. The height of the PanedWindow manager
will be equal to the sum of the heights of all its children, the spacing
between them, and the size of the top and bottom margins.
.mc |
.sp
.mc
.iX "Constraint"
.iX "Widget" "constraint"
The PanedWindow manager widget is also a constraint widget, which means that it
.mc |
creates and manages a set of constraints for each child.  You can
.mc
specify a minimum and maximum size for each pane.  The PanedWindow manager
will not allow a pane to be resized below its minimum size nor beyond its
maximum size.  Also, when the minimum size of a pane is equal to its maximum
size, then no control sash will be presented for that pane or 
for the lowest pane.
.mc |
The following figure shows an example of a PanedWindow widget with three
arrow buttons as its children.
.LE
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 22P
.C
.sC graphics/panedw1.tif tiff 300 12.5P 20P
\&
.P
.sp +1
.FG "PanedWindow Widget"
.sp +1.5
.\}
.el \{\
.br
.ne 22P
\&
.sp 20P
.ce
illustration panedw1.eps goes here
.sp +1
.FG "PanedWindow Widget"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.75i
.mc
\0
.mc |
.FG "PanedWindow Widget"
.sp 3.5i
.in +1.56i
.mc
.P! graphics/panedw1.ps 
.mc |
.in -1.56i
.ne 2
.mc
.sp
.mc |
You can adjust the size of the panes.  To
.mc
facilitate this adjustment, a pane control sash is created for most
children.  The sash appears as a square box positioned on the bottom of
.mc |
the pane that it controls (see Figure 2-11).  You can adjust the size of
.mc
a pane by using the mouse.  Position the pointer inside the sash and a 
crosshair appears.  Press and hold mouse button 1 and the pointer changes
to an arrow pointing up and down.  Continue holding mouse button 1 down
while you move the pointer to achieve the desired size of the pane.
Release mouse button 1 and the panes will be resized.
.mc |
Figure 2-12 shows the PanedWindow after a pane has been resized.
.mc
.ig ++
.br  
.ie \nZ \{\
.br
.ne 22P
.P
.sC graphics/panedw2.tif tiff 300 12.5P 20P
\&
.P
.sp +1
.FG "PanedWindow Widget After Pane Resizing"
.sp +1.5
.\}
.el \{\
.br
.ne 22P
\&
.sp 20P
.ce
illustration panedw2.eps goes here
.sp +1
.FG "PanedWindow Widget After Pane Resizing"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4i
.mc
\0
.mc |
.FG "PanedWindow Widget After Pane Resizing"
.sp 3.75i
.in +1.56i
.mc
.P! graphics/panedw2.ps 
.mc |
.in -1.56i
.mc
.sp
.mc *
.mc
.H 3 "Dialog Widgets"
.iX "Widget" "dialog"
.iX "Dialog widgets"
Dialog widgets are container widgets that provide
.mc |
applications with layout functionality typically used for pop-up dialogs.
.mc
These widgets are used for interaction tasks such as displaying messages,
setting properties, and providing selection from a list of items.
Dialog widgets are thus used primarily as an interface between the
.mc |
user and the application.  
.ne 2
A Dialog widget will normally ask a question
.mc
or present the user with some information that requires a response.
.mc |
In some cases, the application will be suspended until the user provides
.mc
the response.
.mc |
.sp
.mc
.iX "Dialog, definition"
.mc |
A \fBDialog\fP is a collection of widgets, including a dialog shell,
a bulletin board (or subclass of BulletinBoard or some other container widget),
plus various children of the bulletin board, such as the Label, PushButton,
.mc
and Text widgets.
All of the dialog widgets are built from the 
Core, Composite, Constraint, and Manager widget classes.
.mc |
.sp
.mc
The collection of widgets that compose a Dialog can be built from scratch
by building up the necessary argument lists and creating
each individual widget in
the Dialog.
.iX "Convenience functions"
.iX "Convenience dialogs"
For common interaction tasks, \fBconvenience functions\fP are defined that
.mc |
create the collection of widgets that comprise a particular dialog.
.mc
The collections of widgets created by Dialog convenience functions are
referred to as \fBConvenience Dialogs\fP.
.mc |
.sp
.mc
Convenience Dialogs are either modal or modeless.
A modal dialog stops the work session and solicits input from the user.
.mc |
A modeless dialog solicits input from the user, but does not
.mc
interrupt interaction with any application.
.mc |
.sp
.mc
Each dialog has one or more convenience functions that create
any of the subwidgets
.mc |
in that dialog.  For example, a message box has several convenience
.mc
functions:
.iX "XmCreateMessageDialog"
.iX "XmCreateErrorDialog"
.iX "XmCreateInformationDialog"
.iX "XmCreateQuestionDialog"
.iX "XmCreateWarningDialog"
.iX "XmCreateWorkingDialog"
.BL
.LI
.mc |
\fBXmCreateMessageDialog
.mc
.LI
.mc |
XmCreateErrorDialog
.mc
.LI
.mc |
XmCreateInformationDialog
.mc
.LI
.mc |
XmCreateQuestionDialog
.mc
.LI
.mc |
XmCreateWarningDialog
.mc
.LI
.mc |
XmCreateWorkingDialog\fP
.mc
.LE
.P
.mc |
Each of these convenience functions creates a dialog shell and a message box.
Refer to Chapter 5, ``Dialog Widgets and Functions,'' and the individual
manpages for more information.
.mc
.H 3 "Dialog Widget Descriptions"
The following list gives an overview of the Dialog widget set.
See the next section for an overview of the convenience dialogs.
.mc |
.VL .5i
.LI "XmBulletinBoard (xmBulletinBoardWidgetClass)"
.br
.mc
.iX "XmBulletinBoard"
.iX "BulletinBoard"
.iX "Composite"
The BulletinBoard widget is a composite widget that provides simple geometry
management for children widgets.
It does not force positioning on its children, but can be set to reject
geometry requests that would result in overlapping children.
.mc |
The bulletin board is the base widget for most dialog widgets, but is also used
.mc
as a general container widget.  
.mc |
.LI "XmCommand (xmCommandWidgetClass)"
.br
.mc
.iX "XmCommand"
.iX "Command"
The Command widget is a subclass of SelectionBox that includes
a command history region and a command input region.
.mc |
The Command widget also provides a command history mechanism.
.LI "XmFileSelectionBox (xmFileSelectionBoxWidgetClass)"
.br
.mc
.iX "XmFileSelectionBox"
.iX "FileSelectionBox"
The FileSelectionBox widget is a subclass of SelectionBox and BulletinBoard
used to get a selection from a list of alternatives.
.mc |
FileSelectionBox includes an editable text field for the directory mask,
scrolling lists of filenames and directories, and an editable text field
for the selected file.
Four buttons are available: \fBOK\fP, \fBFilter\fP, \fBCancel\fP, and \fBHelp\fP
.mc
by default.
.mc |
.br
.ne 7
.LI "XmForm (xmFormWidgetClass)"
.br
.mc
.iX "XmForm"
.iX "Form"
The Form widget is a constraint-based manager that provides a layout
language used to establish spatial relationships between its children.
.mc |
It maintains these relationships when the form is resized, new children are
added to the form, or its children are resized, unmanaged, remanaged,
.mc
or destroyed.
.mc |
Since it is a subclass of BulletinBoard, a form includes the base level of
.mc
dialog support.
Form can also be used as a general container widget.
.mc |
.br
.ne 6
.LI "XmMessageBox (xmMessageBoxWidgetClass)"
.br
.mc
.iX "XmMessageBox
.iX "MessageBox
The MessageBox widget is a subclass of BulletinBoard used to give information
.mc |
to the user.  A message box includes a symbol and a message.
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.LI "XmSelectionBox (xmSelectionBoxWidgetClass)"
.br
.mc
.iX "XmSelectionBox"
.iX "SelectionBox"
The SelectionBox widget is a subclass of BulletinBoard used to get a selection
from a list of alternatives.
.mc |
A selection box includes a message, an editable text field, and a scrolling
.mc
list of choices.
.mc |
Four buttons are available: \fBOK\fP, \fBCancel\fP, \fBApply\fP, 
and \fBHelp\fP by default.
.LE
.mc
.H 3 "Convenience Dialogs"
.iX "Convenience dialogs"
.mc |
Convenience dialogs are collections of widgets that can be created
.mc
by using convenience functions.  Each convenience dialog instantiates a 
.mc |
dialog widget as a child of a dialog shell.  This section lists the convenience dialogs.
.VL .5i
.LI "BulletinBoardDialog"
.br
.mc
.iX "BulletinBoardDialog"
.mc |
The BulletinBoardDialog convenience function instantiates a bulletin board and
a dialog shell.
.mc
The BulletinBoardDialog is used for interactions not supported by
the standard dialog set.
Necessary dialog components are added as children of the BulletinBoard.
.mc |
.LI "ErrorDialog"
.br
.mc
.iX "ErrorDialog"
.mc |
The ErrorDialog convenience function instantiates a message box
and a dialog shell.
.mc
The ErrorDialog is used to warn the user of an invalid or potentially
dangerous condition.
ErrorDialog includes a symbol and a message.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHel\fPp by default.
.mc
The default ErrorDialog symbol is a hexagon with a hand inside.
.mc |
.ne 7
.LI "FileSelectionDialog"
.br
.mc
.iX "FileSelectionDialog"
.mc |
The FileSelectionDialog convenience function instantiates a file selection box
and a dialog shell.
.mc
The FileSelectionDialog is used to select a file.
.mc |
FileSelectionDialog includes an editable text field for the directory mask,
scrolling lists of filenames and directories, and an editable text field
for the selected file.
Four buttons are available: \fBOK\fP, \fBFilter\fP, \fBCancel\fP, and \fBHelp\fP
.mc
by default.
.mc |
.br
.ne 7
.LI "FormDialog"
.br
.mc
.iX "FormDialog"
.mc |
The FormDialog convenience function instantiates a form and
a dialog shell.
.mc
The FormDialog is used for interactions not supported by
the standard dialog set.
.mc |
Necessary dialog components are added as children of the form.
.LI "InformationDialog"
.br
.mc
.iX "InformationDialog"
.mc |
The InformationDialog convenience function instantiates a message box and
a dialog shell.
.mc
The InformationDialog is used to give information to the user, such as
the status of an action.
InformationDialog includes a symbol and a message.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.mc
The default InformationDialog symbol is a square icon with an ``i''
in the center.
.mc |
.LI "MessageDialog"
.br
.mc
.iX "MessageDialog"
.mc |
The MessageDialog convenience function instantiates a message box and
a dialog shell.
.mc
The MessageDialog is used to give information to the user.
MessageDialog may include a symbol and a message.  There is no symbol
by default.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.LI "PromptDialog"
.br
.mc
.iX "PromptDialog"
.mc |
The PromptDialog convenience function instantiates a selection box and
a dialog shell.
.mc
The PromptDialog is used to prompt the user for text input.
PromptDialog includes a message and a text input region.
.mc |
Four buttons are available: \fBOK\fP, \fBApply\fP,
\fBCancel\fP, and \fBHelp\fP by default.
.LI "QuestionDialog"
.br
.mc
.iX "QuestionDialog"
.mc |
The QuestionDialog convenience function instantiates a message box and
a dialog shell.
.mc
The Question Dialog is used to get the answer to a question from the user.
QuestionDialog includes a symbol and a message.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.mc
A ``?'' is the default QuestionDialog symbol.
.mc |
.LI "SelectionDialog"
.br
.mc
.iX "SelectionDialog"
.mc |
The SelectionDialog convenience function instantiates a selection box and
a dialog shell.
.mc
The SelectionDialog is used to get a selection from a list of alternatives.
SelectionDialog includes a message, an editable text field, and a scrolling
list of choices.
.mc |
Four buttons are available: \fBOK\fP, \fBApply\fP,
\fBCancel\fP, and \fBHelp\fP by default.
.LI "WarningDialog"
.br
.mc
.iX "WarningDialog"
.mc |
The WarningDialog convenience function instantiates a message box and
a dialog shell.
.mc
The WarningDialog is used to warn the user of the consequences of
an action, and give the user a choice of resolutions.
WarningDialog includes a symbol and a message.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.mc
A ``!'' is the default WarningDialog symbol.
.mc |
.LI "WorkingDialog"
.br
.mc
.iX "WorkingDialog"
.mc |
The WorkingDialog convenience function instantiates a message box and
a dialog shell.
The WorkingDialog is used to inform users that there is a time-consuming
operation in progress and allow them to cancel the operation.  
.mc
WorkingDialog includes a symbol and a message.
.mc |
Three buttons are available: \fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default.
.mc
The WorkingDialog symbol is a square icon with an hourglass in the center.
.mc |
.LE
.P
.br
.ne 15
.mc
.H 3 "Menu Widgets"
.iX "Menu widgets"
.iX "RowColumn"
.iX "RadioBox"
.iX "Popup MenuPane"
.iX "Pulldown MenuPane"
.iX "Option menu"
The RowColumn widget is the basis for most of the menu system components.
It has a built-in ability to behave like a RowColumn manager, a
.mc |
radio box, a menu bar, a pull-down menu pane, a pop-up menu pane, and an option
.mc
menu.
Convenience functions 
have been provided to easily create these special versions of
the RowColumn widget.
.P
The Motif menu system is composed of the following widgets and
convenience functions:
.iX "XmMenuShell"
.iX "XmCascadeButton"
.iX "XmSeparator"
.iX "XmToggleButton"
.iX "XmPushButton"
.BL
.LI
XmRowColumn  (Widget)
.LI
MenuBar    (Convenience Function)
.LI
OptionMenu (Convenience Function)
.LI
Pulldown Menupane  (Convenience Function)
.LI
Popup Menupane (Convenience Function)
.LI
XmMenuShell   (Widget)
.LI
.mc |
XmCascadeButton  (Widget and Gadget)
.br
.ne 9
.mc
.LI
XmSeparator    (Widget and Gadget)
.LI
XmLabel    (Widget and Gadget)
.LI
XmToggleButton     (Widget and Gadget)
.LI
XmPushButton (Widget and Gadget)
.LE
.P
Applications are not required to use all of these components to use
the menu system.
.H 2 "Gadgets
.iX "Gadgets"
Gadgets provide
essentially the same functionality as the equivalent primitive widgets.
The primary motivation behind providing a set of gadgets
is to improve performance, both in execution time
and data space.
This applies to both the application and server processes and
minimizes the amount of lost functionality.  The performance difference
between widgets and gadgets is dramatic, so it is highly recommended
that applications use gadgets whenever possible.
.P
.iX "Gadgets"
.mc |
Gadgets can be thought of as windowless widgets.  This means 
.mc
that they do not have windows, translations, actions,
.mc |
or pop-up children.  Also, gadgets do not have any of the visual
.mc
resources found in the XmPrimitive class for primitive widgets.  These
visuals are referenced by a gadget from its parent.
.P
.iX "Object"
.iX "Rect"
.iX "XmGadget"
Examples of display gadgets include buttons, labels and separators.  
All of these gadgets are built from the classes of Object, 
RectObj, and XmGadget.  The table below shows the gadgets and
their class names.
.mc |
.sp
.mc
.br
.ne 3i
.TB "Gadgets"
.TS
center, box, tab(;);
lfB  |  lfB
l    |  l   .
.sp .2
Gadget Name;Gadget Class
.sp .2
_
.mc |
Object;objectClass
RectObj;rectObjClass
.mc
XmGadget;xmGadgetClass
XmArrowButtonGadget;xmArrowButtonGadgetClass
.mc |
XmSeparatorGadget;xmSeparatorGadgetClass
.mc
XmLabelGadget;xmLabelGadgetClass
.mc |
XmCascadeButtonGadget;xmCascadeButtonGadgetClass
.mc
XmPushButtonGadget;xmPushButtonGadgetClass
.mc *
.mc
XmToggleButtonGadget;xmToggleButtonGadgetClass
.sp .2
.TE
.P
The following list provides an overview of the set of display gadgets.
.mc |
.VL .5i
.LI "Object (objectClass)"
.br
.mc
.iX "Object"
The Object class is an Xt Intrinsics meta class and is therefore never
instantiated.  It is used as a supporting superclass to provide common
resources to other classes.
.mc |
.br
.ne 7
.LI "RectObj (rectObjClass)"
.br
.mc
.iX "RectObj"
The RectObj class is an Xt Intrinsics meta class and is therefore never
instantiated.  It is used as a supporting superclass to provide common
resources to other classes.
.mc |
.LI "XmGadget (xmGadgetClass)"
.br
.mc
.iX "XmGadget"
XmGadget is a Motif meta class and is therefore never instantiated.  It
is used as a supporting superclass to provide common
resources to other gadget classes.
.mc |
.LI "XmArrowButtonGadget (xmArrowButtonGadgetClass)"
.br
.mc
.iX "XmArrowButtonGadget"
.iX "ArrowButtonGadget"
.mc |
An arrow button gadget has the same functionality as a push button gadget, but displays a 
.mc
directional arrow within itself.
.mc |
.LI "XmSeparatorGadget (xmSeparatorGadgetClass)"
.br
.iX "XmSeparatorGadget"
.iX "SeparatorGadget"
A separator gadget is used to provide a visual separation between groups
of widgets.  It can draw horizontal and vertical lines in several
different styles.
.LI "XmLabelGadget (xmLabelGadgetClass)"
.br
.mc
.iX "XmLabelGadget"
.iX "LabelGadget"
.mc |
A label gadget consists of either text or graphics.  It can be instantiated, but
it is also used as a superclass for button widgets.  The label gadget's text is a
.mc
compound string and can be multidirectional, multiline, multifont, or
.mc |
any combination of these.  The Label gadget
.mc
is considered static because it does not accept any button or key input
.mc |
other than the Help button on the widget.  The help callback is
the only callback defined for the Label gadget.
.LI "XmCascadeButtonGadget (xmCascadeButtonGadgetClass)"
.br
.iX "XmCascadeButtonGadget"
.iX "CascadeButtonGadget"
Cascade button gadgets appear in MenuBars or in Popup or Pulldown
MenuPanes.
A cascade button gadget usually has an associated submenu.
When the gadget is armed, by moving the pointer over the gadget and
pressing a mouse button, its appearance becomes three dimensional and it
displays its submenu.
When the gadget is activated, by releasing the mouse button while the
pointer is over the gadget or by pressing a mnemonic key, the submenu
remains posted.
A cascade button gadget has a text or graphical label and, when in a
menu, an arrow indicator to the right of the text or graphics that
points to the area where the submenu is displayed.
.LI "XmPushButtonGadget (xmPushButtonGadgetClass)"
.br
.mc
.iX "XmPushButtonGadget"
.iX "PushButtonGadget"
.mc |
Push button gadgets are used to issue commands within an application.
A push button gadget displays a label with a border-shadowing graphic.
When the push button is selected, the shadow moves to give the
appearance that the push button has been pressed.  When the push button
is unselected, the shadow moves to give the appearance that the push button
is out.
.LI "XmToggleButtonGadget (xmToggleButtonGadgetClass)"
.br
.mc
.iX "XmToggleButtonGadget"
.iX "ToggleButtonGadget"
.mc |
A toggle button gadget consists of a text or graphics
.mc
button face with an indicator
(a square or diamond-shaped box) placed to the left of the text or
graphics.  You select
.mc |
the toggle button gadget by placing the mouse cursor inside the rectangle and
.mc
pressing mouse button 1.  The indicator
is then filled with the selection color, indicating
.mc |
that the toggle button gadget is selected.  Toggle button gadgets are used
.mc
for setting
nontransitory data within an application.
.mc |
.LE
.mc
.H 2 "Convenience Functions"
.iX "Convenience functions"
Convenience functions are functions that enable you to create certain
widgets or gadgets, or groups of widgets or gadgets, by making just one
.mc |
function call.  A convenience function creates a predetermined set of
widgets and returns the parent widget's ID.  Convenience
functions are of the form:
.mc
.P
.mc |
\fBXmCreate\fP\fIWidgetName\fP
.P
for widgets and gadgets other than Dialog widgets.  For dialogs,
convenience functions are referred to as \fBconvenience dialogs\fP, and
are of the form:
.P
\fBXmCreate\fP\fIDialogWidgetName\fP\fBDialog\fP
.P
.mc
.iX "Widget" "create"
.iX "Create" "widget"
.iX "XmCreateLabel"
.mc |
.ne 2
.mc
It is very easy to use a convenience function to create a widget.  For
example, you can use the following code segment to create a Label widget:
.mc |
.P
.mc
.nf
.mc |
\fBWidget XmCreateLabel\fP \fI(parent,name,arglist,argcount)\fP
.sp
\fBWidget\fP \fIparent;\fP
\fBString\fP \fIname;\fP
\fBArglist\fP \fIarglist;\fP
\fBCardinal\fP \fIargcount;\fP
.sp
.mc
.fi
.ft R
.mc |
.VL .9i
.mc
.LI\fIparent\fP
Specifies the parent widget for the Label.
.LI\fIname\fP
Specifies the resource name for the Label.  This name is used for
retrieving resources, and therefore it should not be the same as any widget that
.mc |
is a child of the same parent, unless identical resource values
.mc
are to be used for the child widgets.
.LI\fIarglist\fP
Specifies the argument list used to override the default values
for the Label's resources.
.LI\fIargcount\fP
Specifies the number of arguments in the arglist.
.LE
.P
.mc |
.br
.ne 15
.mc
.iX "Widget" "unmanaged"
.iX "Unmanaged widget"
.mc |
The \fBXmCreate\fP\fIWidgetName\fP functions create unmanaged widgets.  Your 
application must manage the set of widgets before they will be displayed.
.mc
You can manage each widget separately or as a group.  Use this code
segment to create and manage each widget separately:
.mc |
.DS
.S -2
.mc
.ft CW
.mc *
.mc
Widget w;

w = XmCreate<widgetname>(parent, name, arglist, argcount);
XtManageChild(w);
.mc |
.S
.mc
.ft R
.mc |
.DE
.P
Use this code segment to create and manage widgets with the same parent
.mc
as a group:
.mc |
.DS
.S -2
.mc
.ft CW
.mc *
.mc
int child_count = 0;
Widget w[10];

.mc |
w[child_count++] = XmCreate<widgetname>(parent, name, arglist, argcount)
w[child_count++] = XmCreate<widgetname>(parent, name, arglist, argcount)
w[child_count++] = XmCreate<widgetname>(parent, name, arglist, argcount)
							.
							.
							.
.mc
XtManageChildren(w, child_count);
.mc *
.mc
.ft R
.mc |
.S
.DE
.mc
