.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 5
.mc |
.nr !_ 0
.mc
.H 1 "Menus"
.iX "Menus"
.P
The Motif menu system provides three types of menus\(emPopup, Pulldown, and
Option menus.  This chapter describes how to implement and use these
.mc |
menus.  The chapter includes the following:
.mc
.BL
.LI
.mc |
An overview of the Motif menu system
.mc
.LI
.mc |
Guidelines for creating and interacting with each menu type
.mc
.LI
.mc |
A description of the mouse and keyboard interfaces for each menu type
.mc
.LE
.mc |
.ne 15
.mc
.H 2 "Overview of the Motif Menu System"
.mc *
.mc
.P
Motif provides the ability to create three types of menu systems:
.BL
.LI
.mc |
Popup menu systems
.mc
.LI
.mc |
Pulldown menu systems
.mc
.LI
.mc |
Option menu systems
.mc
.LE
.P
.iX "Menu system"
The term "menu system" refers to a combination of various widgets that
create the visual and interactive behavior of a menu. For example, a
Pulldown menu system characteristically consists of a MenuBar containing a
number of CascadeButtons; the CascadeButtons are used to post various
Pulldown MenuPanes which, in turn, contain various buttons.  
.P
The major widget components of menu systems are RowColumn widgets that are
configured to behave as:
.BL
.LI
.mc |
PopupMenuPanes
.mc
.LI
.mc |
PulldownMenuPanes
.mc
.LI
.mc |
MenuBars
.mc
.LI
.mc |
Option menus
.mc
.LE
.P
For example, a Popup MenuPane is a RowColumn widget
created to behave as a Popup MenuPane; likewise, a MenuBar is a type of
RowColumn widget.
.P
The Motif menu system provides the ability to manually create
the major menu widgets by creating RowColumn widgets of the
appropriate types.  For
example, a Popup MenuPane can be created by creating a MenuShell widget and
a child RowColumn widget of type XmMENU_POPUP. 
However, Motif provides a set of convenience functions
that automatically create  RowColumn widgets of the appropriate type
and, when necessary, a parent MenuShell.  (Popup and Pulldown MenuPanes
require MenuShell parents; MenuBars and Option menus do not have
MenuShell parents.)  For example, the \fBXmCreatePopupMenu\fP convenience
function creates a RowColumn widget configured to act as a Popup MenuPane
and automatically creates its parent MenuShell.
.mc |
.br
.ne 5
.mc
.iX "Convenience functions"
.P
Most of the instructions and examples in this chapter use the convenience
functions.  Creating menus by separately creating MenuShells and RowColumn
widgets is discussed at the end of the chapter.
.H 3 "Convenience Functions and Widgets Used to Create Menus"
.P
A menu is constructed from combinations of widgets created explicitly or by
using convenience functions.
.P
The following convenience functions create RowColumn widgets that act as
.mc |
MenuPanes. MenuPanes are transient features in an application\(emthey are
.mc
not displayed until they are posted by a particular event, and they are
unposted at the conclusion of some other event.
.iX "MenuPanes"
.BL
.LI
\fBXmCreatePopupMenu\fP convenience function.  This function automatically
creates a Popup MenuPane and its required parent MenuShell.
.iX "XmCreatePopupMenu" 
.LI
.mc |
\fBXmCreateSimplePopupMenu\fP and \fBXmVaCreateSimplePopupMenu\fP
convenience functions.
These functions automatically create a Popup MenuPane and its required
parent MenuShell.
They also create the button children of the menu.
.iX "XmCreateSimplePopupMenu"
.iX "XmVaCreateSimplePopupMenu"
.LI
.mc
\fBXmCreatePulldownMenu\fP convenience function.  This function
automatically creates a Pulldown MenuPane and its required parent MenuShell.
.iX "XmCreatePulldownMenu"
.mc |
.LI
\fBXmCreateSimplePulldownMenu\fP and \fBXmVaCreateSimplePulldownMenu\fP
convenience functions.
These functions automatically create a Pulldown MenuPane and its
required parent MenuShell.
They also create the button children of the menu.
.iX "XmCreateSimplePulldownMenu"
.iX "XmVaCreateSimplePulldownMenu"
.mc
.LE
.P
The following convenience functions create RowColumn widgets configured to
act as other components of menu systems. These are nontransient features
of an application.
.BL
.LI
\fBXmCreateMenuBar\fP convenience function.  This function automatically
creates a MenuBar.
Menubars are typically used as the basis for building Pulldown menu
systems.  A MenuBar is the top-level component of a Pulldown menu system.
.iX "XmCreateMenuBar"
.LI
.mc |
\fBXmCreateSimpleMenuBar\fP and \fBXmVaCreateSimpleMenuBar\fP
convenience functions.
These functions automatically create a MenuBar and its button children.
.iX "XmCreateSimpleMenuBar"
.iX "XmVaCreateSimpleMenuBar"
.LI
.mc
\fBXmCreateOptionMenu\fP convenience function.  This function automatically
creates an Option menu. 
.iX "XmCreateOptionMenu"
.mc |
.LI
\fBXmCreateSimpleOptionMenu\fP and \fBXmVaCreateSimpleOptionMenu\fP
convenience functions.
These functions automatically create an Option Menu, its Pulldown
submenu, and the button children of the submenu.
.iX "XmCreateSimpleOptionMenu"
.iX "XmVaCreateSimpleOptionMenu"
.LI
\fBXmCreateRadioBox\fP convenience function.
This function automatically creates a RowColumn widget of type
\fBXmWORK_AREA\fP that behaves as a RadioBox.
In a RadioBox, the ToggleButton or ToggleButtonGadget children behave
like buttons on an automobile radio.
Only one button at a time can be set, and setting one button turns off
all other buttons.
.iX "XmCreateRadioBox"
.LI
\fBXmCreateSimpleRadioBox\fP and \fBXmVaCreateSimpleRadioBox\fP
convenience functions.
These functions automatically create a RowColumn widget of type
\fBXmWORK_AREA\fP that behaves as a RadioBox.
The functions also create the ToggleButtonGadget children of the
RadioBox.
.iX "XmCreateSimpleRadioBox"
.iX "XmVaCreateSimpleRadioBox"
.LI
\fBXmCreateSimpleCheckBox\fP and \fBXmVaCreateSimpleCheckBox\fP
convenience functions.
These functions automatically create a RowColumn widget of type
\fBXmWORK_AREA\fP that behaves as a CheckBox.
The functions also create the ToggleButtonGadget children of the
CheckBox.
A CheckBox is like a RadioBox, except that more than one button can be
set at any time.
.iX "XmCreateSimpleCheckBox"
.iX "XmVaCreateSimpleCheckBox"
.LI
\fBXmCreateWorkArea\fP convenience function.
This function automatically creates a RowColumn widget of type
\fBXmWORK_AREA\fP.
.iX "XmCreateWorkArea"
.mc
.LE
.P
In addition to the RowColumn widgets created by convenience functions,
the following widgets and gadgets are used in menu systems:
.BL
.LI
\fBXmCascadeButton\fP and \fBXmCascadeButtonGadget\fP.  CascadeButtons are used
as the visual means to display Pulldown menus, Option menus, and submenus.  
.LI
\fBXmSeparator\fP and \fBXmSeparatorGadget\fP.  The Separator widget is
used to separate unrelated
buttons or groups of buttons within a MenuPane.
.LI
\fBXmLabel\fP and \fBXmLabelGadget\fP.  The Label widget is used to provide
a title for a MenuPane.
.LI
\fBXmPushButton\fP and \fBXmPushButtonGadget\fP.  PushButtons provide the
means for selecting an item from a menu. 
.LI
\fBXmToggleButton\fP and \fBXmToggleButtonGadget\fP.  ToggleButtons
provide a way to set nontransitory states using menus.  
.LI
\fBXmRowColumn\fP.  The RowColumn widget is a general-purpose RowColumn
manager.  Popup MenuPanes, Pulldown MenuPanes, MenuBars, and Option menus
.mc |
are types of RowColumn widgets.
.mc
When menu RowColumn widgets are created without using convenience functions,
the resource \fBXmNrowColumnType\fP specifies the type of MenuPane created.
.iX "XmRowColumn"
.LI
\fBXmMenuShell\fP.  The MenuShell widget is a shell widget designed
to contain a Popup or Pulldown MenuPane as its child.  The convenience
functions that create Popup and Pulldown menus automatically create their
parent MenuShell.
.iX "XmMenuShell"
.LE
.P
Note that the Motif menu system does not implement Popup MenuPanes,
Pulldown MenuPanes, Option menus, and MenuBars
.mc |
as separate widget classes.  For example, no Popup Menu widget class exists;
.mc
rather, a convenience function exists for creating the appropriately
configured RowColumn widget. 
.H 3 "Introducing the Three Menu Types
.mc |
This section describes Popup, Pulldown,and Option menus.
.mc
.H 4 "Popup Menu System"
.iX "Popup menu system"
A Popup menu system consists of a single Popup MenuPane containing a
combination of Label, PushButton, ToggleButton, and Separator widgets or
gadgets.  In addition, the MenuPane can contain CascadeButtons or
CascadeButtonGadgets that are
used to access Pulldown MenuPanes that function as submenus of the Popup
.mc |
MenuPane.  The following figure shows the top level of a Popup menu system. 
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.C
.sC graphics/popup1.tif tiff 300 24.5P 9.5P
\&
.P
.sp +1
.FG "Top Level of a Popup Menu System"
.sp +1.5
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration popup1.eps goes here
.sp +1
.FG "Top Level of a Popup Menu System"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "Top Level of a Popup Menu System"
.mc |
.sp 1.75i
.in +.8i
.P! graphics/popup1.ps 
.in -.8i
.mc
.P
The CascadeButtons have arrows that indicate the presence of submenus.
.mc |
Moving the pointer to the First Submenu button displays its submenu.
The following figure shows a window with a submenu for a Popup menu system.
.mc
.iX "Submenus"
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.C
.sC graphics/popup2.tif tiff 300 24.5P 9.5P
\&
.P
.sp +1
.FG "Submenu of a Popup Menu System"
.sp +1.5
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration popup2.eps goes here
.sp +1
.FG "Submenu of a Popup Menu System"
.sp +1.5
.\}
.P
.++
.mc |
.sp
.mc
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "Submenu of a Popup Menu System"
.mc |
.sp 1.75i
.in +.8i
.P! graphics/popup2.ps 
.in -.8i
.mc
.P
The submenu contains two PushButtons and one CascadeButton.  Moving the
.mc |
pointer to the Third Submenu button displays its submenu.  The following figure shows
a window with a Popup menu system and two cascading submenus.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.C
.sC graphics/popup3.tif tiff 300 24.5P 9.5P
\&
.P
.sp +1
.mc |
.FG "Popup Menu System with Two Cascading Submenus"
.mc
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration popup3.eps goes here
.sp +1
.mc |
.FG "Popup Menu System with Two Cascading Submenus"
.mc
.sp +1.5
.\}
.++
.br
.mc |
.ne 2i
.mc
\0
.mc |
.FG "Popup Menu System with Two Cascading Submenus"
.sp 1.75i
.mc
.in +.8i
.P! graphics/popup3.ps 
.in -.8i
.mc *
.mc
.P
.H 4 "Pulldown Menu System"
.iX "Pulldown menu system"
A Pulldown menu system typically consists of a MenuBar,
a set of CascadeButtons parented from the MenuBar,
and a Pulldown MenuPane attached to each CascadeButton.  The CascadeButtons 
are displayed within the MenuBar and provide the means for displaying the 
MenuPanes.  In addition, the menu system may include Label, PushButton,
ToggleButton, and Separator widgets or gadgets.
.P
.mc |
The following figure shows a 
MenuBar, which is the top level of a Pulldown menu system.
.mc
.iX "MenuBar"
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.C
.sC graphics/pulld1.tif tiff 300 15.5P 9.5P
\&
.P
.sp +1
.mc |
.FG "MenuBar of a Pulldown Menu System"
.mc
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration pulld1.eps goes here
.sp +1
.mc |
.FG "MenuBar of a Pulldown Menu System"
.mc
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2i
.mc
\0
.mc |
.FG "MenuBar of a Pulldown Menu System"
.sp 1.75i
.in +1.44i
.mc
.P! graphics/pulld1.ps 
.mc |
.in -1.44i
.mc
.P
.mc |
Moving the pointer to Right Menu and pressing
.mc
mouse button 1 displays its Pulldown MenuPane.
.mc |
The following figure shows a window with a Pulldown Menupane
displayed.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.C
.sC graphics/pulld2.tif tiff 300 15.5P 9.5P
\&
.P
.sp +1
.FG "Displaying a Pulldown MenuPane"
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration pulld2.eps goes here
.sp +1
.FG "Displaying a Pulldown MenuPane"
.sp +1.5
.\}
.++
.br
.ne 2.5i
\0
.mc *
.mc
.FG "Displaying a Pulldown MenuPane"
.mc |
.sp 2i
.in +1.44i
.P! graphics/pulld2.ps 
.in -1.44i
.mc
.P
.H 4 "Option Menu System"
.iX "Option menu"
Visually, an Option menu is composed of three areas:
.BL
.LI
A descriptive LabelGadget. Typically, the label describes the types of
options available.
.iX "Label string"
.LI
A Pulldown MenuPane containing PushButtons or PushButtonGadgets.  The buttons
represent the available options.
.LI
A selection area consisting of a CascadeButtonGadget that contains
a label string. The label string reflects the most recent
option chosen from the Pulldown MenuPane.
.iX "Selection area"
.LE
.P
The top level of an Option menu system shows the descriptive label and
.mc |
selection area.  The following figure shows a window with the top level of an Option
menu system.
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 8.5P
.C
.sC graphics/options1.tif tiff 300 23P 6.5P
\&
.P
.sp +1
.FG "Top Level of an Option Menu System"
.sp +1.5
.\}
.el \{\
.br
.ne 8.5P
\&
.sp 6.5P
.ce
illustration options1.eps goes here
.sp +1
.FG "Top Level of an Option Menu System"
.sp +1.5
.\}
.++
.br
.mc |
.ne 1.5i
.mc
\0
.mc *
.mc
.FG "Top Level of an Option Menu System"
.mc |
.sp 1.25i
.in +.8i
.P! graphics/options1.ps 
.in -.8i
.mc
.P
Pressing mouse button 1 while the pointer is in the selection area displays
.mc |
the Pulldown MenuPane containing the options.  The following figure shows a Pulldown
Menupane in an Option Menu system.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 8P
.C
.sC graphics/options2.tif tiff 300 23P 6P
\&
.P
.sp +1
.FG "The Pulldown MenuPane in an Option Menu System"
.sp +1.5
.\}
.el \{\
.br
.ne 8P
\&
.sp 6P
.ce
illustration options2.eps goes here
.sp +1
.FG "The Pulldown MenuPane in an Option Menu System"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 1.5i
.mc
\0
.mc *
.mc
.FG "The Pulldown MenuPane in an Option Menu System"
.mc |
.sp 1.25i
.in +.8i
.P! graphics/options2.ps 
.in -.8i
.mc
.P
.H 2 "Creating Popup Menu Systems"
.iX "Popup menu" "system"
.P
.iX "Popup MenuPane"
The top level of a Popup menu system is a Popup MenuPane.
Popup MenuPanes are implemented as
XmRowColumn widgets configured to
operate as a Popup MenuPanes.  The Popup MenuPane may contain
CascadeButtons or CascadeButtonGadgets,
which are used to access Pulldown MenuPanes that act as
submenus of the Popup MenuPane. (Submenus are discussed later in this chapter.)
.P
.mc |
A Popup MenuPane displays a 3-dimensional shadow around the edge of the
.mc
MenuPane unless the feature has
been disabled by the application.
.P
The Popup MenuPane must be the child of a MenuShell widget.
If the Popup MenuPane is created using the convenience function,
a MenuShell is automatically created as the real
parent of the MenuPane.  If the Popup MenuPane is created without using the
convenience function, the MenuShell widget must be created first.
.iX "MenuShell"
.mc |
.br
.ne 15
.mc
.H 3 "Popup MenuPane Convenience Function"
.P
.iX "Popup MenuPane" "convenience function"
A Popup MenuPane is created using the convenience function:
.sp
.mc |
\fBWidget XmCreatePopupMenu\fP(\fIparent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP)
.mc *
.mc
.sp
.iX "XmCreatePopupMenu"
\fBXmCreatePopupMenu\fP creates a Popup MenuPane and a parent MenuShell,
and returns the widget ID for the MenuPane.  The
Popup MenuPane is created as a RowColumn widget with the
\fBXmNrowColumnType\fP
resource set to XmMENU_POPUP.  This resource cannot be changed by the
application.
.iX "XmNrowColumnType"
.H 3 "Event Handlers for Popup Menu Systems"
.iX "Event handlers"
.P
.mc |
Popup menu systems require an event-handler procedure that is called when a
.mc
specified event (usually, a ButtonPress) occurs in the widget(s) to which
.mc |
the Popup menu system is attached. Usually, this is the \fIparent\fP
.mc
specified by the \fBXmCreatePopupMenu\fP function and the parent's descendents.
.mc |
The event-handler procedure should test that the
proper mouse button has been pressed and then display the Popup MenuPane.
.mc

.mc |
The \fBXtAddEventHandler\fP function registers the event-handler procedure
with the dispatch mechanism.  It has the following syntax:
.mc
.iX "XtAddEventHandler"
.P
.mc |
\fBvoid XtAddEventHandler\fP(\fIw\fP, \fIevent_mask\fP, \fInonmaskable\fP,
.mc
\fIproc\fP, \fIclient_data\fP)
.br
.mc |
      \fBWidget\fP \fIw\fP;
.mc
.br
.mc |
      \fBEventMask\fP \fIevent_mask\fP;
.mc
.br
.mc |
      \fBBoolean\fP \fInonmaskable\fP;
.mc
.br
.mc |
      \fBXtEventHandler\fP \fIproc\fP;
.mc
.br
.mc |
      \fBcaddr_t\fP \fIclient_data\fP;
.mc
.VL 1i
.ne 3
.LI \fIw\fP
.mc |
Specifies the widget to add the callback to
.mc
.ne 3
.LI \fIevent_mask\fP
.mc |
Specifies the event mask for which to call this procedure
.ne 6
.mc
.LI \fInonmaskable\fP
.mc |
Specifies whether this procedure should be called on the nonmaskable events
.mc
.ne 3
.LI \fIproc\fP
.mc |
Specifies the client event-handler procedure
.mc
.ne 3
.LI \fIclient_data\fP
.mc |
Specifies additional data to be passed to the client's event handler
.mc
.sp
.LE

For example, the line
.mc |
.nf
.mc
.sp
.mc |
.ta .5i 1i 1.5i
\f(CWXtAddEventHandler(rc, ButtonPressMask, False, 
	PostIt, popup);\fP
.mc
.sp
.mc |
.ta .5i 3i
.fi
registers the procedure PostIt for the event ButtonPress
within the widget rc and all of rc's descendents.
.H 3 "Procedure for Creating a Popup Menu"
.mc
.P
The following steps create a Popup menu.  Following each step is a code
segment that accomplishes the task.
.AL
.LI
Use the \fBXmCreatePopupMenu\fP convenience function to automatically
create the Popup MenuPane and its required parent MenuShell.  Register the
event handler.  The following
.mc |
lines create a Popup MenuPane as a child of widget \fBform1\fP.
.sp
.mc
.nf
.ta .5i 1i 1.5i
.mc |
\f(CWpopup = XmCreatePopupMenu(form1, "popup", 
	NULL, 0);
XtAddEventHandler(form1, ButtonPressMask, 
	False, PostIt, popup);\fP
.mc
.ta .5i 3i
.fi
.LI
Create the contents of the MenuPane.  The following segment creates a
title (LabelGadget), SeparatorGadget, and three PushButtonGadgets.
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
.mc |
\f(CWXtSetArg(args[0], XmNlabelString, 
	XmStringCreate ("Menu Title", 
	XmSTRING_DEFAULT_CHARSET));
.br
.ne 3
item[0] = XmCreateLabelGadget(popup, "title", 
	args, 1);
item[1] = XmCreateSeparatorGadget(popup, 
	"separator", NULL, 0);
item[2] = XmCreatePushButtonGadget(popup, 
	"button1", NULL, 0);
item[3] = XmCreatePushButtonGadget(popup, 
	"button2", NULL, 0);
item[4] = XmCreatePushButtonGadget(popup, 
	"button3", NULL, 0);
.mc
XtManageChildren(item, 5);\fP
.ta .5i 3i
.fi
.LE
.P
.mc |
The following figure shows the parenting relationships to use when
.mc
creating a Popup menu system using convenience functions.
.mc |
.ne 5
.sp
.FG "Creating a Popup Menu System with Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxwid = 1.00
boxht = .3
down
box "Parent Widget"
line down .25
box "Popup MenuPane"
line down .25
box "Label"
box "Separator"
box "PushButton"
box "PushButton"
box "PushButton"
.PE
.DE
\s10
.mc |
.+n
.br
.ne 15
.H 3 "Interacting with Popup Menus"
.mc
.iX "Popup menu" "interacting with"
.mc |
This section describes mouse and keyboard interactions with popup menus.
.mc
.H 4 "Mouse Input"
.P
Mouse button 3 is the default primary means of interacting with a Popup
menu.
.P
.iX "Popup MenuPane"
A Popup MenuPane is not visible until it is displayed by the user.
The Popup
menu system is normally associated with a particular widget and
all of that widget's descendents.  The
Popup MenuPane is posted (displayed) by moving the pointer into the associated
widget or one of its descendents and then pressing mouse button 3.
If the Popup menu system includes any Pulldown MenuPane submenus, they are not
displayed until the pointer is moved into the associated CascadeButton
widget or gadget.
.P
The application is responsible for posting the Popup MenuPane by using
\fBXtManageChild\fP to display the MenuPane.  This is usually done in the
event handler added to the Popup MenuPane's parent.
All visible MenuPanes (the Popup MenuPane and any displayed submenus) are
automatically unposted when the user has completed interacting with the menu.
.P
Once a MenuPane has been posted,
menu items are armed when the pointer enters them and disarmed when it leaves.
If the pointer is moved into a CascadeButton or CascadeButtonGadget,
the associated submenu is posted.
Releasing mouse button 3 while a menu item is armed
activates the menu item.  If the pointer is not within a menu item when mouse
button 3 is released, then all
visible MenuPanes are unposted.
.P
Ordinarily, a Popup menu is positioned to the right and beneath the
pointer.  However, if this placement causes a portion of the MenuPane to be
inaccessible, the menu may be automatically repositioned to force the
Popup MenuPane on the screen.
.P
The mouse button used to interact with Popup menus can be changed using the
.mc |
RowColumn resource \fBXmNmenuPost\fP.
.iX "XmNmenuPost"
.mc
.H 4 "The Keyboard Interface
.iX "Keyboard interface" "popup menus"
.P
.iX "Traversal" "with popup menus"
Keyboard traversal is activated and deactivated by the user.
When a user is interacting with the menu using the mouse,
traversal is enabled by releasing the mouse button
.mc |
while the pointer is within any CascadeButton or 
.ne 4
CascadeButtonGadget;
.mc
releasing the mouse
button posts the associated submenu and enables traversal for 
.mc |
all the MenuPanes in the Popup menu system.  When traversal is enabled
.mc
.BL
.LI
The directional keys traverse the menu hierarchy.
.LI 
.mc |
\fBKActivate\fP selects the currently armed menu item.
.mc
.LI
.mc |
\fBKCancel\fP unposts the currently posted Pulldown MenuPane.
In the Popup menu, \fBKCancel\fP unposts the menu, returning focus to
the tab group from which the menu was popped up.
.mc
.LI
.mc |
\fBKMenu\fP unposts all the MenuPanes in the Popup menu system,
returning focus to the tab group from which the menu was popped up.
.LI
.mc
Pressing a mnemonic for a menu item in the most recently posted
MenuPane selects that item.
.iX "Mnemonic"
.LI
Pressing an accelerator for a menu item selects that item.
.iX "Accelerator"
.LI
Pressing mouse button 3 disables traversal and reenables interactive
operation.
.LE
.P
.iX "Accelerator"
.mc |
An accelerator, \fBKMenu\fP by default, can be associated with a Popup
menu.
When \fBKMenu\fP is pressed while the pointer is located within
.mc
the associated widget or one of its children, the first MenuPane in the
Popup menu hierarchy is posted and
traversal is enabled.   The user interacts
with the menu as described previously for keyboard
traversal.
.P
Use the resource \fBXmNmenuAccelerator\fP to change the accelerator.
.iX "XmNmenuAccelerator"
.H 3 "Sample Program"
This sample program creates a Popup menu system. The menu can be posted
.mc |
using the mouse or by using the accelerator <\fBCtrl-P\fP>.  Items can be selected
.mc
using the mouse or by using the underlined mnemonics.
.mc |
...\".P
...\"The source code for this program is located on your system
...\"in \fB./demos/xmpopup.c\fP.
.br
.ne 7
.S -2
.in0
.mc
.nf
\f(CW
.ta .5i 1i 1.5i
/* Popup Menu Example */

#include <Xm/Xm.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>


/*********** Callback for the Pushbuttons ***********************/

void ButtonCB (w, client_data, call_data) 
Widget	w;			/*  widget id		*/
caddr_t	client_data;		/*  data from application   */
caddr_t	call_data;		/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Button %s selected.\\n", client_data);
	
}

/************** Event Handler for Popup Menu ************************/

PostIt (w, popup, event)
Widget w;
Widget popup;
XButtonEvent * event;
{
   if (event->button != Button3)
	return;
   XmMenuPosition(popup, event);
   XtManageChild(popup);
}

/***********************Main Logic for Program ********************/


void main (argc, argv)
int argc;
char **argv;
{
.mc |
   XtAppContext app_context;
.mc
   Widget toplevel, popup, rc;
   Widget buttons[2], popupBtn[5];
   Arg args[5];

/* Initialize toolkit */
   
.mc |
   toplevel = XtAppInitialize (&app_context, "PopupMenu", NULL, 0,
                                 &argc, argv, NULL, NULL, 0);
.mc

/* Create RowColumn in toplevel with two PushButtonGadgets */

   XtSetArg(args[0], XmNwidth, 150);
   XtSetArg(args[1], XmNheight, 125);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNadjustLast, False);
   rc = XmCreateRowColumn(toplevel, "rc", args, 5);  
   XtManageChild(rc);

   buttons[0] = XmCreatePushButtonGadget (rc, "buttonA", NULL, 0);
   XtAddCallback(buttons[0], XmNactivateCallback, ButtonCB, "A");

   buttons[1] = XmCreatePushButtonGadget (rc, "buttonB", NULL, 0);
   XtAddCallback(buttons[1], XmNactivateCallback, ButtonCB, "B");
   XtManageChildren (buttons, 2);

/* Create popup menu with accelerator CTRL P */
   
   XtSetArg(args[0], XmNmenuAccelerator, "Ctrl <Key> p");
   popup = XmCreatePopupMenu(rc, "popup", args, 1);
   XtAddEventHandler(rc, ButtonPressMask, False, PostIt, popup);


/* Create title for the popup menu and a separator */
   
   XtSetArg(args[0], XmNlabelString,
      XmStringCreate("Menu Title", XmSTRING_DEFAULT_CHARSET));
   popupBtn[0] = XmCreateLabelGadget(popup, "Title", args, 1);

   popupBtn[1] = XmCreateSeparatorGadget(popup, "separator", NULL, 0);

/* Create three PushButtonGadgets in the popup menu */

   XtSetArg(args[0], XmNmnemonic, '1'); 
   popupBtn[2] = XmCreatePushButtonGadget(popup, "button1", args, 1); 
   XtAddCallback(popupBtn[2], XmNactivateCallback, ButtonCB, "1");

   XtSetArg(args[0], XmNmnemonic, '2'); 
   popupBtn[3] = XmCreatePushButtonGadget(popup, "button2", args, 1);
   XtAddCallback(popupBtn[3], XmNactivateCallback, ButtonCB, "2");

   XtSetArg(args[0], XmNmnemonic, '3');
   popupBtn[4] = XmCreatePushButtonGadget (popup, "button3", args, 1); 
   XtAddCallback (popupBtn[4], XmNactivateCallback, ButtonCB, "3");
   XtManageChildren (popupBtn, 5);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

.mc |
   XtAppMainLoop(app_context);
.mc
}
.ta .5i 3i
\fP
.fi
.mc |
.in
.S
.mc
.H 2 "Creating a Pulldown Menu System
.iX "Pulldown menu system"
.P
The basis of a Pulldown menu system is a MenuBar containing a set of
CascadeButtons. (CascadeButtonGadgets are not allowed as the children of
a MenuBar.)
The CascadeButtons are used to display Pulldown MenuPanes.
One of the CascadeButtons (typically, the one that is used to display help
information) may be treated specially.  This button is always positioned at
the lower right corner of the MenuBar.
.P
Two convenience functions, \fBXmCreateMenuBar\fP and
\fBXmCreatePulldownMenu\fP, create the appropriate RowColumn widgets.
.P
In addition to their use in Pulldown menu systems, Pulldown MenuPanes are
used to create submenus in both Popup and Pulldown menu systems.  Submenus
are discussed in the next section.
.H 3 "MenuBar Create Function
.P
A MenuBar is created using the convenience function:
.mc *
.mc
.sp
.mc |
\fBWidget XmCreateMenuBar\fP\fI(parent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP)
.mc *
.mc
.P
.iX "XmCreateMenuBar"
\fBXmCreateMenuBar\fP creates a MenuBar as a RowColumn widget with
the \fBXmNrowColumnType\fP resource set to XmMENU_BAR.  This resource
cannot be
changed by the application.  No MenuShell is created for MenuBar.
.iX "XmNrowColumnType"
.P
.mc |
The MenuBar displays a 3-dimensional shadow around its edge unless this
.mc
feature has been disabled by the application.
.mc |
.br
.ne 20
.mc
.H 3 "Pulldown MenuPane Create Function"
.iX "Pulldown MenuPane" "convenience function"
.P
A Pulldown MenuPane is created using the convenience function:
.P
.mc |
\fBWidget XmCreatePulldownMenu\fP(\fIparent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP)
.mc *
.mc
.P
To create a Pulldown MenuPane that is displayed using a CascadeButton in a
.mc |
MenuBar, specify the MenuBar as the parent in the
.mc
\fBXmCreatePulldownMenu\fP function.
.P
.iX "XmCreatePulldownMenu"
\fBXmCreatePulldownMenu\fP creates a Pulldown MenuPane and a parent MenuShell,
and returns the widget ID for the MenuPane.  The
Pulldown MenuPane is created as a RowColumn widget with the
\fBXmNrowColumnType\fP
resource set to XmMENU_PULLDOWN.  This resource cannot be changed by the
application.
.iX "XmNrowColumnType"
.P
.H 3 "Creating a Help Button"
.iX "Help button in Menubar"
The MenuBar resource \fBXmNmenuHelpWidget\fP specifies a CascadeButton that
will
be positioned at the lower right corner of the MenuBar.  Typically, this
CascadeButton is used to display help
.mc |
information.  The Pulldown menu sample program creates a Help button.
.mc
.iX "XmNmenuHelpWidget"
.H 3 "Procedure for Creating a Pulldown Menu"
.P
The following steps create a Pulldown menu.  Following each step is a code
segment that accomplishes the task.
.AL
.LI
Use the \fBXmCreateMenuBar\fP convenience function to create the MenuBar.
.mc |
The following lines create the MenuBar as the child of widget \fBform1\fP.
.sp
.mc
\f(CWmenubar = XmCreateMenuBar(form1, "menubar", NULL, 0);
.br
XtManageChild(menubar);\fP
.LI
Create one or more Pulldown MenuPanes as submenus (children) of the MenuBar.
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
.mc |
\f(CWpulldown1 = XmCreatePulldownMenu(menubar, 
	"pulldown1", NULL, 0);
pulldown2 = XmCreatePulldownMenu(menubar, 
	"pulldown2", NULL, 0);\fP
.mc
.ta .5i 3i
.fi
.LI
Create a CascadeButton widget for each Pulldown MenuPane.  The
CascadeButtons and
MenuPanes must have the same parent (in this case, menubar).  Use the
resource \fBXmNsubMenuId\fP to attach each CascadeButton to its MenuPane.
.iX "XmNsubMenuId"
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
\f(CWXtSetArg(args[0], XmNsubMenuId, pulldown1);
.mc |
cascade[0] = XmCreateCascadeButton(menubar, 
	"cascade1", args, 1);
.sp
.mc
XtSetArg(args[0], XmNsubMenuId, pulldown2);
.mc |
cascade[1] = XmCreateCascadeButton(menubar, 
	"cascade2", args, 1);
.mc
XtManageChildren(cascade, 2);\fP
.ta .5i 3i
.fi
.LI
Create one or more buttons in each Pulldown MenuPane.  The following lines
create two PushButtonGadgets in each MenuPane.
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
.mc |
\f(CWpbutton1[0] = XmCreatePushButtonGadget
	(pulldown1, "button1a", NULL, 0);
pbutton1[1] = XmCreatePushButtonGadget
	(pulldown1, "button1b", NULL, 0);
.mc
XtManageChildren(pbutton1, 2); 
.mc |
.sp
pbutton2[0] = XmCreatePushButtonGadget
	(pulldown2, "button2a", NULL, 0);
pbutton2[1] = XmCreatePushButtonGadget
	(pulldown2, "button2b", NULL, 0);
.mc
XtManageChildren(pbutton2, 2);\fP
.ta .5i 3i
.fi
.LE
.P
.mc |
The following figure shows the parenting relationships and
.mc
attachments (dashed lines) to use when
creating a Pulldown menu system using convenience functions. The system
.mc |
includes a CascadeButton MenuPane designated as a Help menu.
.br
.ne 17
.sp
.FG "Creating a Pulldown Menu System With Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxht = .3; boxwid = 1.4;
down
box "Parent Widget"
line down .25
Box1: box "MenuBar"
line down .25
line left 2.00
line down .25
Box2: box "CascadeButton1"
move to Box1.s; move down .25; move left 1.0
line down 1.0
Box3: box "Pulldown MenuPane1"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s; move down .25
line down .25
Box4: box "CascadeButton2"
move to Box1.s; move down .25; move right 1.0
line down 1.0
Box5: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s; move down .25; line right 2.0
line down .25
box "CascadeButton (Help)"
line dashed <-> from Box2.s to Box3.nw
line dashed <-> from Box4.s to Box5.nw
.PE

\s10
.mc *
.mc
.DE
.mc |
.+n
.ne 22
.H 3 "Interacting with Pulldown Menus"
.mc
.iX "Pulldown menus, interacting with"
.mc |
This section describes mouse and keyboard interaction with Pulldown menus.
.mc
.H 4 "Mouse Input"
Mouse button 1 is the default primary means of interacting with a Pulldown
.mc |
menu.  Pressing mouse button 1 while the pointer is positioned in a 
CascadeButton
in the MenuBar arms and highlights the CascadeButton. If the
.mc
CascadeButton has an associated Pulldown MenuPane, the MenuPane is posted.
.mc |
At this point, the pointer can be
.mc
.BL
.LI
Moved down into the Pulldown MenuPane.
.mc |
Menu items are armed when the
pointer enters them and disarmed when it leaves.
.mc
If the pointer is moved into a CascadeButton widget or gadget, the
associated submenu is posted.
.mc |
Releasing mouse button 1 while an item is armed activates
.mc
the menu item.  
.LI
Moved to a different
CascadeButton within the MenuBar.  This unposts the current Pulldown
MenuPane and posts the MenuPane attached to the other CascadeButton.
.LE
.P
Releasing mouse button 1 while the pointer is outside the
menu hierarchy unposts all visible submenus and disarms the
MenuBar.
.P
The mouse button used to interact with Pulldown menus can be changed using the
.mc |
RowColumn resource \fBXmNmenuPost\fP for the MenuBar.
.iX "XmNmenuPost"
.mc
.H 4 "The Keyboard Interface"
.iX "Keyboard interface" "pulldown menus"
Keyboard traversal is activated and deactivated by the user.
When a user is interacting with the menu using the mouse,
traversal is enabled by either of the following:
.iX "Traversal" "with pulldown menus"
.BL
.LI
Releasing the mouse button
while the pointer is within any CascadeButton widget or gadget.  Releasing
the mouse
button posts the associated submenu and enables traversal for all the 
MenuPanes in the Pulldown menu system.
.LI
.mc |
Pressing \fBKMenuBar\fP.  This highlights the first CascadeButton in the
MenuBar and enables traversal.  \fBKLeft\fP and \fBKRight\fP traverse to
other CascadeButtons in the MenuBar.  \fBKDown\fP
or \fBKActivate\fP
.mc
posts the Pulldown MenuPane associated with the highlighted CascadeButton. 
.LE
.P
When traversal is enabled,
.BL
.LI
The directional keys traverse the menu hierarchy.
.LI
.mc |
\fBKActivate\fP selects the currently armed menu item.
.mc
.LI
.mc |
\fBKCancel\fP unposts the currently posted Pulldown MenuPane.
In the MenuBar, \fBKCancel\fP disarms the CascadeButton, returning the
focus to the tab group that had focus when the MenuBar was entered.
.mc
.LI
.mc |
\fBKMenuBar\fP unposts all the submenus and disarms the CascadeButton in
the MenuBar, returning the focus to the tab group that had focus when
the MenuBar was entered.
.LI
.mc
Pressing a mnemonic for a menu item in the most recently posted
MenuPane selects that item.
.iX "Mnemonic"
.mc |
.br
.ne 5
.mc
.LI
Pressing an accelerator for a menu item selects that item.
.iX "Accelerator"
.LI
Pressing mouse button 1 disables
traversal and reenables interactive operation.
.LE
.P
Mnemonics can be used to post the Pulldown MenuPanes.  The mnemonics are
resources of the CascadeButtons in the MenuBar.
To use a mnemonic associated with a MenuBar CascadeButton, preface it with
.mc |
the \fBMeta\fP modifier key.
.mc
.iX "Mnemonic"
.P
Traversal is enabled when a menu is posted using a mnemonic.  The user
interacts with the menu as described previously for
keyboard traversal.
.H 3 "Sample Program"
The following sample program creates a Pulldown menu system consisting of a
MenuBar containing three CascadeButtons.  One CascadeButton is designated
.mc |
as a Help  button; the other two are attached to Pulldown MenuPanes.  
.mc
.P
All the CascadeButtons and PushButtons have mnemonics. 
.mc |
...\".P
...\"The source code for this program is located on your system
...\"in \fB./demos/xmpulldown.c\fP.
.br
.ne 8
.mc
.P
.mc |
.in0
.S -2
.mc
.nf
\f(CW
.ta .5i 1i 1.5i
/* Pulldown Menu Example */

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>

.mc |
/********* Callback for the pushbuttons in the pulldown menu. *****/
.mc

void ButtonCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Button %s selected.\\n", client_data);
	
}

.mc |
/*********************Main Logic***********************************/
.mc

void main (argc, argv)
unsigned int argc;
char **argv;

{
.mc |
   XtAppContext app_context;
.mc
   Widget toplevel, form, menubar;
   Widget menubarBtn[3], pulldowns[2];
   Widget buttons1[3], buttons2[3];
   Arg args [4];

/* Initialize toolkit and create form and menubar */

.mc |
   toplevel =
        XtAppInitialize (&app_context, "PulldownMenu", NULL, 0,
                           &argc, argv, NULL, NULL, 0);
.mc

   XtSetArg(args[0], XmNwidth, 250); 
   XtSetArg(args[1], XmNheight, 125); 
   form = XmCreateForm(toplevel, "form", (ArgList) args, 2);
   XtManageChild(form);

   XtSetArg(args[0], XmNtopAttachment, XmATTACH_FORM); 
   XtSetArg(args[1], XmNrightAttachment, XmATTACH_FORM);
   XtSetArg(args[2], XmNleftAttachment, XmATTACH_FORM); 
   menubar = XmCreateMenuBar(form, "menubar", args, 4); 
   XtManageChild(menubar);

/* Create help button in menubar */

   XtSetArg(args[0], XmNlabelString, XmStringCreate("Help",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'H');
   menubarBtn[0] = XmCreateCascadeButton(menubar, "help", args, 2);
   XtAddCallback(menubarBtn[0], XmNactivateCallback, ButtonCB, "Help");

   XtSetArg(args[0], XmNmenuHelpWidget, (XtArgVal)menubarBtn[0]);
   XtSetValues(menubar, args, 1);


/* Create 2 Pulldown MenuPanes and 2 cascade buttons in menubar */

   pulldowns[0] = XmCreatePulldownMenu(menubar, "pulldown1", NULL, 0);

   XtSetArg(args[0], XmNsubMenuId, pulldowns[0]);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("Left Menu",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[2], XmNmnemonic, 'L');
   menubarBtn[1] = XmCreateCascadeButton(menubar, "button1", args, 3);

   pulldowns[1] = XmCreatePulldownMenu(menubar, "pulldown2", NULL, 0);

   XtSetArg(args[0], XmNsubMenuId, pulldowns[1]);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("Right Menu",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[2], XmNmnemonic, 'R');
   menubarBtn[2] = XmCreateCascadeButton(menubar, "button2", args, 3);
   XtManageChildren(menubarBtn, 3);


   XtSetArg(args[0], XmNlabelString, XmStringCreate("First",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'F');
.mc |
   buttons1[0] =
          XmCreatePushButtonGadget(pulldowns[0], "button1a", args, 2);
   XtAddCallback(buttons1[0], XmNactivateCallback,
                                              ButtonCB, "Left-First");
.mc

   XtSetArg(args[0], XmNlabelString, XmStringCreate("Second",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'S');
.mc |
   buttons1[1] =
          XmCreatePushButtonGadget(pulldowns[0], "button1b", args, 2);
   XtAddCallback(buttons1[1], XmNactivateCallback,
                                             ButtonCB, "Left-Second");
.mc

   XtSetArg(args[0], XmNlabelString, XmStringCreate("Third",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'T');
.mc |
   buttons1[2] =
          XmCreatePushButtonGadget(pulldowns[0], "button1c", args, 2);
   XtAddCallback(buttons1[2], XmNactivateCallback,
                                              ButtonCB, "Left-Third");
.mc
   XtManageChildren(buttons1, 3);


   XtSetArg(args[0], XmNlabelString, XmStringCreate("First",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'F');
.mc |
   buttons2[0] =
          XmCreatePushButtonGadget(pulldowns[1], "button2a", args, 2);
   XtAddCallback(buttons2[0], XmNactivateCallback,
                                             ButtonCB, "Right-First");
.mc

   XtSetArg(args[0], XmNlabelString, XmStringCreate("Second",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'S');
.mc |
   buttons2[1] =
          XmCreatePushButtonGadget(pulldowns[1], "button2b", args, 2);
   XtAddCallback(buttons2[1], XmNactivateCallback,
                                            ButtonCB, "Right-Second");
.mc

   XtSetArg(args[0], XmNlabelString, XmStringCreate("Third",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'T');
.mc |
   buttons2[2] =
          XmCreatePushButtonGadget(pulldowns[1], "button2c", args, 2);
   XtAddCallback(buttons2[2], XmNactivateCallback,
                                             ButtonCB, "Right-Third");
.mc
   XtManageChildren(buttons2, 3);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

.mc |
   XtAppMainLoop(app_context);
.mc
}

.ta .5i 3i
\fP
.fi
.mc |
.in
.S
.mc
.H 2 "Creating Submenus"
.iX "Submenus" "creating"
.iX "Creating" "submenus"
.P
Submenus are implemented using Pulldown MenuPanes attached to CascadeButton
widgets or gadgets.
The submenu and the CascadeButton to which it is attached are children of
the MenuPane (Popup or Pulldown) from which the submenu cascades.
.P
The MenuShell of the submenu is created automatically if the submenu is
created using \fBXmCreatePulldownMenu\fP convenience function:
.iX "XmCreatePulldownMenu"
.P
.mc |
\fBWidget CreatePulldownMenu\fP(\fIparent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP);
.mc *
.mc
.P
The submenu's
.mc |
MenuShell is created as the child of the \fIparent\fP's MenuShell. 
.mc
.H 3 "Procedure for Creating Submenus
.P
The following steps create a submenu of a Popup menu, and then create a
submenu of that submenu.  Following each step
is a code segment that accomplishes the task.
.AL
.LI
Create the Popup MenuPane.  The following line creates the Popup MenuPane as
.mc |
a child of widget \fBform1\fP.
.sp
.nf
.ta .5i 1i 1.5i
\f(CWpopup = XmCreatePopupMenu(form1, "popup", 
	NULL, 0);\fP
.ta .5i 3i
.fi
.mc
.LI
To create a submenu, create a Pulldown MenuPane and CascadeButtonGadget as the
children of the Popup MenuPane.  Use the resource XmNsubMenuId to attach the
CascadeButtonGadget to the MenuPane.
.iX "XmNsubMenuId"
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
.mc |
\f(CWsubmenu1 = XmCreatePulldownMenu(popup, 
	"submenu1", NULL, 0);
.sp
XtSetArg(args[0], XmNsubMenuId, submenu1);
cascade1 = XmCreateCascadeButtonGadget(popup, 
	"cascade1", args, 1);
.mc
XtManageChild(cascade1);\fP
.ta .5i 3i
.fi
.LI
To create a submenu of submenu1, create a Pulldown MenuPane and
CascadeButtonGadget as children of submenu1.
.nf
.ta .5i 1i 1.5i
.mc |
.sp
.mc
\f(CWsubmenu3 = XmCreatePulldownMenu(submenu1,
	"submenu3", NULL, 0);
.mc |
.sp
XtSetArg(args[0], XmNsubMenuId, submenu3);
cascade3 = XmCreateCascadeButtonGadget
	(submenu1, "cascade3", args, 1);
.mc
XtManageChild(cascade3);\fP
.ta .5i 3i
.fi
.LE
.P
.mc |
Figure 6-10 shows the parenting relationships and
.mc
attachments (dashed lines) to use when
creating submenus in a Popup menu system using convenience functions. This
system contains two submenus beneath the top level Popup MenuPane. The
first submenu contains two PushButtons and one CascadeButtonGadget
(CascadeButton3). The
CascadeButtonGadget is used to access the submenu (Pulldown MenuPane3) that
cascades from the first submenu.
.mc |
.br
.ne 20
.sp
.FG "Creating Submenus With Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxht = .3; boxwid = 1.25;
down
box "Parent Widget"
line down .25
Box1: box "Popup MenuPane"
line down .25
line left 1.6
line down .25
Box2: box "Pulldown MenuPane1"
line down .5
line left .8
line down .25
Box3a: box "Pulldown MenuPane3"
line down .25
box "PushButton"
box "PushButton"
move to Box2.s; move down .5; line right .8
line down .5
box "PushButton"
box "PushButton"
Box6: box "CascadeButton3"
move to Box1.s
move down .25;
line down .25
Box3: box "CascadeButton1"
Box4: box "CascadeButton2"
move to Box1.s
move down .25
line right 1.6
line down .25
Box5: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
line dashed <-> from Box2.e to Box3.w
line dashed <-> from Box4.e to Box5.w
line dashed <-> from Box3a.e to Box6.w
.PE
\s10
.mc *
.mc
.DE
.mc |
.+n
.H 3 "Interacting with Submenus"
Interacting with submenus is explained earlier in 
of this chapter in the sections "Interacting with
Popup Menus" and "Interacting with Pulldown Menus."
.mc
.H 3 "Sample Program
.mc |
The following program creates the Popup menu system (with submenus) that is
illustrated in Figure 6-10.
...\"The source code for this program is located on
...\"your system in \fB./demos/xmsubmenus.c\fP.
.mc
.P
.mc |
.in0
.S -2
.mc
.nf
\f(CW
.ta .5i 1i 1.5i
/* Popup Menu with Submenus Example */

#include <Xm/Xm.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>


/*********** Callback for the Pushbuttons ***********************/

void ButtonCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Button %s selected.\\n", client_data);
	
}

/************** Event Handler for Popup Menu ************************/

PostIt (w, popup, event)
Widget w;
Widget popup;
XButtonEvent * event;
{
   if (event->button != Button3)
	return;
   XmMenuPosition(popup, event);
   XtManageChild(popup);
}


/***********************Main Logic for Program ********************/


void main (argc, argv)
int argc;
char **argv;
{
.mc |
   XtAppContext app_context;
.mc
   Widget toplevel, popup, rc;
   Widget submenu1, submenu2, submenu3, buttons[2];
   Widget popupBtn[2], sub1Btn[3], sub2Btn[2], sub3Btn[2];
   Arg args[5];

/* Initialize toolkit */
.mc |
   toplevel = XtAppInitialize (&app_context, "PopupMenu", NULL, 0,
                                 &argc, argv, NULL, NULL, 0);
.mc

/* Create RowColumn in toplevel with two pushbuttons */

   XtSetArg(args[0], XmNwidth, 400);
   XtSetArg(args[1], XmNheight, 125);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNadjustLast, False);
   rc = XmCreateRowColumn(toplevel, "rc", args, 5);  
   XtManageChild(rc);

   buttons[0] = XmCreatePushButtonGadget(rc, "button1", NULL, 0);
   XtAddCallback(buttons[0], XmNactivateCallback, ButtonCB, "1");

   buttons[1] = XmCreatePushButtonGadget(rc, "button2", NULL, 0);
   XtAddCallback(buttons[1], XmNactivateCallback, ButtonCB, "2");
   XtManageChildren(buttons, 2);

/* Create popup menu */

   popup = XmCreatePopupMenu(rc, "popup", NULL, 0);
   XtAddEventHandler(rc, ButtonPressMask, False, PostIt, popup);

/* Create two submenus and CascadeButtons in the popup menu */

.mc |
   submenu1 =
            (Widget)XmCreatePulldownMenu(popup, "submenu1", NULL, 0);

.mc
   XtSetArg(args[0], XmNsubMenuId, submenu1);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("First Submenu",
            XmSTRING_DEFAULT_CHARSET));
.mc |
   popupBtn[0] =
            XmCreateCascadeButtonGadget(popup, "cbutton1", args, 2);
.mc


.mc |
   submenu2 =
            (Widget)XmCreatePulldownMenu(popup, "submenu2", NULL, 0);
.mc

   XtSetArg(args[0], XmNsubMenuId, submenu2);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("Second Submenu",
            XmSTRING_DEFAULT_CHARSET));
.mc |
   popupBtn[1] =
            XmCreateCascadeButtonGadget(popup, "cbutton2", args, 2);
.mc
   XtManageChildren(popupBtn, 2);

/* Create pushbuttons in submenu1 and submenu2. */

.mc |
   sub1Btn[0] =
            XmCreatePushButtonGadget(submenu1, "button1a", NULL, 0);
.mc
   XtAddCallback(sub1Btn[0], XmNactivateCallback, ButtonCB, "1a");

.mc |
   sub1Btn[1] =
            XmCreatePushButtonGadget(submenu1, "button1b", NULL, 0);
.mc
   XtAddCallback(sub1Btn[1], XmNactivateCallback, ButtonCB, "1b");

.mc |
   sub2Btn[0] =
            XmCreatePushButtonGadget(submenu2, "button2a", NULL, 0);
.mc
   XtAddCallback(sub2Btn[0], XmNactivateCallback, ButtonCB, "2a");

.mc |
   sub2Btn[1] =
            XmCreatePushButtonGadget(submenu2, "button2b", NULL, 0);
.mc
   XtAddCallback(sub2Btn[1], XmNactivateCallback, ButtonCB, "2b");
   XtManageChildren(sub2Btn, 2);

/* Create a submenu of submenu 1 */

.mc |
   submenu3 =
          (Widget)XmCreatePulldownMenu(submenu1, "submenu3", NULL, 0);
.mc

   XtSetArg(args[0], XmNsubMenuId, submenu3);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("To Third Submenu",
            XmSTRING_DEFAULT_CHARSET));
.mc |
   sub1Btn[2] =
           XmCreateCascadeButtonGadget(submenu1, "cbutton3", args, 2);
.mc
   XtManageChildren(sub1Btn, 3);

/* Create pushbuttons in submenu 3 */

.mc |
   sub3Btn[0] =
            XmCreatePushButtonGadget(submenu3, "button3a", NULL, 0);
.mc
   XtAddCallback(sub3Btn[0], XmNactivateCallback, ButtonCB, "3a");

.mc |
   sub3Btn[1] =
            XmCreatePushButtonGadget(submenu3, "button3b", NULL, 0);
.mc
   XtAddCallback(sub3Btn[1], XmNactivateCallback, ButtonCB, "3b");
   XtManageChildren(sub3Btn, 2);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

.mc |
   XtAppMainLoop(app_context);
.mc
}


.ta .5i 3i
\fP
.fi
.mc |
.S
.in
.mc
.H 2 "Creating Option Menu Systems"
.iX "Option menu" "system"
The basis of an Option menu system is the following:
.BL
.LI
An Option menu.  An Option menu is created by the convenience function
\fBXmCreateOptionMenu\fP.  It is a specialized RowColumn manager
.mc |
composed of two internal gadgets:
.mc
.iX "Option menu"
.iX "XmCreateOptionMenu"
.mc |
.DL
.mc
.LI
.iX "Selection area, for option menus"
A selection area.  The selection area is a specialized CascadeButtonGadget.
It
provides the means for displaying an associated Pulldown MenuPane, and it 
.mc |
displays the label string of the last item selected from the Pulldown
.mc
MenuPane.  The \fBXmNmenuHistory\fP resource defines the initial item
displayed.
(The default is the first item in the Pulldown MenuPane.)
.iX "XmNmenuHistory"
.LI
A label.  The label is a specialized LabelGadget, and is displayed to the
left of the selection area.
.iX "Label, for option menus"
.LE
.LI
A Pulldown MenuPane attached to the Option menu. The Pulldown MenuPane
contains a PushButton or PushButtonGadget for each available option.
.LE
.P
.mc |
.ne 5
The Option menu typically does not display any 3-dimensional
.mc
visuals around itself or its internal label.  The internal CascadeButtonGadget
.mc |
has a 3-dimensional shadow.  This can be changed by the application
.mc
using the standard visual-related resources.
.H 3 "Option MenuPane Create Function"
.iX "Option menu" "creating"
An Option menu can be created using this convenience function:
.P
.mc |
\fBWidget XmCreateOptionMenu\fP(\fIparent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP)
.mc *
.mc
.P
.iX "XmCreateOptionMenu"
\fBXmCreateOptionMenu\fP automatically
.mc |
creates an Option menu and two internal gadgets\(ema CascadeButtonGadget
.mc
(selection area) and LabelGadget (label area).  The function returns the
widget ID
of the Option menu.  The Option menu is created as a RowColumn
widget with the \fBXmNrowColumnType\fP resource set to XmMENU_OPTION.
.P 
The two internal
gadgets can be accessed separately using the following functions:
.BL
.LI
.mc |
\fBWidget XmOptionLabelGadget\fP(\fIoption_menu\fP)\fR returns the ID of the
.mc
LabelGadget.
.iX "XmOptionLabelWidget"
.LI
.mc |
\fBWidget XmOptionButtonGadget\fP(\fIoption_menu\fP)\fR returns the ID of the
.mc
CascadeButtonGadget.
.iX "XmOptionButtonWidget"
.LE
.P
These functions allow the application to have more control over
the visuals associated with the label and selection areas.
.mc |
.H 3 "Procedure for Creating an Option Menu"
.mc
.P
The following steps create an Option menu.
Following each step is a code segment that accomplishes the task.
.AL
.LI
Create the Pulldown MenuPane that will contain the selection items.
.nf
.ta .5i 1i 1.5i
.mc |
.sp
.mc
\f(CWoptionsubmenu = XmCreatePulldownMenu(form1,
	"optionsubmenu", NULL, 0);\fP
.ta .5i 3i
.fi
.mc |
.br
.ne 10
.mc
.LI
Create the selection items for the Pulldown MenuPane.
.nf
.ta .5i 1i 1.5i
.mc |
.sp
\f(CWoption[0] = XmCreatePushButtonGadget
	(optionsubmenu, "option1", NULL, 0);
option[1] = XmCreatePushButtonGadget
	(optionsubmenu, "option2", NULL, 0);
.mc
XtManageChildren(option, 2);\fP
.ta .5i 3i
.fi
.LI
Use the \fBXmCreateOptionMenu\fP convenience function to create the Option
menu and attach it to the Pulldown MenuPane.  Also specify a string for
the Label area.
.mc |
.sp
.mc
.nf
.ta .5i 1i 1.5i
\f(CWstring = XmStringCreate("Options:",
	XmSTRING_DEFAULT_CHARSET); 
XtSetArg(args[0], XmNlabelString, string);
XtSetArg(args[1], XmNsubMenuId, optionsubmenu);
.mc |
option_menu = XmCreateOptionMenu(form1, 
	"option_menu", args, 2);
.mc
XmStringFree(string);   
XtManageChild(option_menu);\fP
.ta .5i 3i
.fi
.LE
.mc |
.br
.ne 5
.mc
.P
.mc |
The components of the Option menu system must be created in the order
shown. (You cannot use \fBXtSetValues\fP to specify the Option menu's
.mc
submenu.)

.mc |
The following figure shows the parenting relationships and
.mc
attachments (dashed lines) used to
create Option menu systems using convenience functions. Each Pulldown
.mc |
MenuPane contains three options.
.br
.ne 20
.sp
.FG "Creating Option Menu Systems Convenience Functions"
.if n .ig +n
.mc
.DS CB
.mc |
.in +.45i
.mc
.ps 8
.PS
boxht = .3; boxwid = 1.25;
down
Box1: box "Parent Widget"
line down .5
line left 2.00
line down .25
Box2: box "Pulldown MenuPane1"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s
move down .5;
line down .25
Box3: box "OptionMenu1"
Box4: box "OptionMenu2"
move to Box1.s
move down .5
line right 2.00
line down .25
Box5: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
line dashed <-> from Box2.e to Box3.w
line dashed <-> from Box4.e to Box5.w
.PE
\s10
.mc |
.in -.45i
.mc
.DE
.mc |
.+n
.H 3 "Interacting with Option Menus
.mc
.iX "Option menu" "interacting with"
.mc |
The following sections describe mouse and keyboard interactions with option menus.
.mc
.H 4 "Mouse Input"
.P
The Pulldown MenuPane is posted by moving the mouse pointer over the
selection area and pressing mouse button 1.  The Pulldown MenuPane
is positioned so that the last selected item is directly over
.mc |
the selection area; the MenuPane is not repositioned if a portion is inaccessible.
.mc
.P
Menu items are armed when the pointer enters them and
disarmed when it leaves.
Releasing mouse button 1 while a menu item is armed selects the menu item
and changes the label in the selection area.
.P
The mouse button used to interact with Option menus can be changed using the
.mc |
RowColumn resource \fBXmNmenuPost\fP.
.iX "XmNmenuPost"
.mc
.H 4 "The Keyboard Interface"
.iX "Keyboard interface" "option menus"
A mnemonic can be associated with the Option menu.  Typing the mnemonic
posts the Pulldown MenuPane and enables traversal.
.iX "Mnemonic"
.iX "Traversal"
.P
When traversal is enabled,
.BL
.LI
The directional keys traverse the menu hierarchy.
.LI
.mc |
\fBKActivate\fP selects the currently armed menu item.
.mc
.LI
.mc |
\fBKCancel\fP unposts the Pulldown MenuPane.
.mc
.LI
Pressing the mouse button disables traversal and reenables interactive
operation.
.LI
Pressing a mnemonic or accelerator for a menu item selects that item.
.iX "Accelerator"
.LE
.H 3 "Sample Program"
The following sample program contains two option menus with three options
each. The Pulldown MenuPanes can be posted using the mouse or by using the
.mc |
mnemonics \fBF\fP and \fBS\fP. When a MenuPane has been posted using a
.mc
mnemonic, a mnemonic can then be used to select an option.
.P
The source code for this program is located on your system
.mc |
in \fB./demos/xmsamplers/xmoption.c\fP.
.mc
.P
.mc |
.in0
.S -2
.mc
.nf
\f(CW
.ta .5i 1i 1.5i
/* Option Menu Example */

#include <Xm/Xm.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>


/*********** Callback for the Pushbuttons ***********************/

void ButtonCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Option %s selected.\\n", client_data);
	
}

/***********************Main Logic for Program ********************/


void main (argc, argv)
int argc;
char **argv;
{
.mc |
   XtAppContext app_context;
.mc
   Widget toplevel, pulldown1, pulldown2, rc;
   Widget option_menus[2], options1[3], options2[3];
   Arg args[6];

/* Initialize toolkit */
.mc |
   toplevel = XtAppInitialize (&app_context, "OptionMenu", NULL, 0,
                                 &argc, argv, NULL, NULL, 0);
.mc

/* Create RowColumn in toplevel */

   XtSetArg(args[0], XmNwidth, 375);
   XtSetArg(args[1], XmNheight, 75);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNnumColumns, 2);
   XtSetArg(args[5], XmNpacking, XmPACK_COLUMN);
   rc = XmCreateRowColumn(toplevel, "rc", args, 6);
   XtManageChild(rc);

/* Create two pulldown menus in rc */

   pulldown1 = (Widget)XmCreatePulldownMenu(rc, "pulldown1", NULL, 0);

   XtSetArg(args[0], XmNlabelString, XmStringCreate("A-option",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'A'); 
.mc |
   options1[0] =
             XmCreatePushButtonGadget(pulldown1, "option1a", args, 2);
.mc
   XtAddCallback(options1[0], XmNactivateCallback, ButtonCB, "1A");

   XtSetArg(args[0], XmNlabelString, XmStringCreate("B-option",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'B');
.mc |
   options1[1] =
             XmCreatePushButtonGadget(pulldown1, "option1b", args, 2);
.mc
   XtAddCallback(options1[1], XmNactivateCallback, ButtonCB, "1B");

   XtSetArg(args[0], XmNlabelString, XmStringCreate("C-option",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'C');
.mc |
   options1[2] =
             XmCreatePushButtonGadget(pulldown1, "option1c", args, 2);
.mc
   XtAddCallback(options1[2], XmNactivateCallback, ButtonCB, "1C");
   XtManageChildren(options1, 3);


   pulldown2 = (Widget)XmCreatePulldownMenu(rc, "pulldown2", NULL, 0);

   XtSetArg(args[0], XmNlabelString, XmStringCreate("A-option",
            XmSTRING_DEFAULT_CHARSET)); 
   XtSetArg(args[1], XmNmnemonic, 'A'); 
.mc |
   options2[0] =
             XmCreatePushButtonGadget(pulldown2, "option2a", args, 2);
.mc
   XtAddCallback(options2[0], XmNactivateCallback, ButtonCB, "2A");

   XtSetArg(args[0], XmNlabelString, XmStringCreate("B-option",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'B');
.mc |
   options2[1] =
             XmCreatePushButtonGadget(pulldown2, "option2b", args, 2);
.mc
   XtAddCallback(options2[1], XmNactivateCallback, ButtonCB, "2B");

   XtSetArg(args[0], XmNlabelString, XmStringCreate("C-option",
            XmSTRING_DEFAULT_CHARSET));
   XtSetArg(args[1], XmNmnemonic, 'C');
.mc |
   options2[2] =
             XmCreatePushButtonGadget(pulldown2, "option2c", args, 2);
.mc
   XtAddCallback(options2[2], XmNactivateCallback, ButtonCB, "2C");
   XtManageChildren(options2, 3);

/* Create option menus and attach the two pulldown menus */

.mc |
   XtSetArg(args[0], XmNlabelString,
       XmStringCreate("First Option Set:", XmSTRING_DEFAULT_CHARSET));
.mc
   XtSetArg(args[1], XmNmnemonic, 'F');
   XtSetArg(args[2], XmNsubMenuId, pulldown1);
   XtSetArg(args[3], XmNmenuHistory, options1[2]);
   option_menus[0] = XmCreateOptionMenu(rc, "option_menu1", args, 4);

.mc |
   XtSetArg(args[0], XmNlabelString,
      XmStringCreate("Second Option Set:", XmSTRING_DEFAULT_CHARSET));
.mc
   XtSetArg(args[1], XmNmnemonic, 'S');
   XtSetArg(args[2], XmNsubMenuId, pulldown2); 
   XtSetArg(args[3], XmNmenuHistory, options2[0]);
   option_menus[1] = XmCreateOptionMenu(rc, "option_menu2", args, 4);
   XtManageChildren(option_menus, 2);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

.mc |
   XtAppMainLoop(app_context);
.mc
}
.ta .5i 3i
\fP
.fi
.mc |
.S
.in
.H 2 "Selecting a Menu Cursor"
.mc
.iX "Menu cursor"
.iX "Cursor, for menus"
.mc |
OSF/Motif provides a number of menu cursors (listed at the end of this section.) 
.mc
An application can select a specific menu cursor that is used whenever
a menu is
displayed.  This feature provides consistent appearance within menus that
.mc |
belong to the same application. The default menu cursor is \fBarrow\fP.
.mc
.P
.mc |
The menu cursor can be specified at
application startup by the resource \fBXmNmenuCursor\fP.
This resource is not associated with a particular widget, and can 
only be set at application startup.  The resource can be set
.mc
two ways:
.iX "XmNmenuCursor"
.BL
.LI
.mc |
By setting a resource in a defaults file. For example:
.sp
.mc
\f(CW*menuCursor: star\fP
.mc |
.sp
sets \fBstar\fP as the menu cursor.
.mc
.LI
.mc |
By using the \fB-xrm\fP command line argument.  For
.mc
example, the following command line specifies a clock as the menu cursor
.mc |
for the application named \fBmyprog\fP:
.sp
\fBmyprog -xrm "*menuCursor:  clock"\fP
.mc
.LE
.mc |
.br
.ne 7
.mc
.P
The cursor can be specified programmatically using the
\fBXmSetMenuCursor\fP function:
.iX "XmSetMenuCursor"
.P
.mc |
\fBvoid XmSetMenuCursor\fP(\fIdisplay\fP, \fIcursorId\fP)
.mc
.br
.mc |
      \fBDisplay\fP \fI*display\fP;
.mc
.br
.mc |
      \fPCursor\fP \fIcursorId\fP;
.mc
.VL 1i
.ne 3
.LI \fIdisplay\fP
.mc |
Specifies the display for which the cursor is used
.mc
.ne 3
.LI \fIcursorId\fP
.mc |
Specifies the menu cursor
.mc
.sp
.LE
After the function is executed, any menu displayed by the
application on the specified display uses the menu cursor identified in the
.mc |
\fIcursorId\fP variable.  This allows the application
.mc
to use different menu cursors on different displays.
.mc |
.br
.ne 9
.mc
.P
.mc |
The \fBXmGetMenuCursor\fP function returns the cursor ID of the current
.mc
menu cursor for a specified display:
.iX "XmGetMenuCursor"
.P
.mc |
\fBCursor XmGetMenuCursor\fP(\fIdisplay\fP)
.mc
.br
.mc |
      \fBDisplay\fP \fI*display\fP;
.mc
.VL 1i
.ne 3
.LI \fIdisplay\fP
Specifies the display.
.sp
.LE
.P
If the application has not created any menus, no cursor is defined and the
.mc |
function returns the value None.
.mc
.P
The following list shows the valid cursor names. 
.TS
center, tab(;);
l  l  l  l .
.mc
arrow;based_arrow_down;based_arrow_up;boat
bogosity;bottom_left_corner;bottom_right_corner;bottom_side
bottom_tee;box_spiral;centr_ptr;circle
clock;coffee_mug;cross;cross_reverse
crosshair;diamond_cross;dot;dotbox
double_arrow;draft_large;draft_small;exchange
fleur;gobbler;gumby;hand1
hand2;heart;icon;iron_cross
left_ptr;left_side;left_tee;leftbutton
ll_angle;lr_angle;man;middlebutton
mouse;pencil;pirate;plus
question_arrow;right_ptr;right_side;right_tee
rightbutton;rtl_logo;sailboat;sb_down_arrow
sb_h_double_arrow;sb_left_arrow;sb_right_arrow;sb_up_arrow
sb_v_double_arrow;shuttle;sizing;spider
spraycan;star;target;tcross
top_left_arrow;top_left_corner;top_right_corner;top_side
top_tee;trek;ul_angle;umbrella;ur_angle
watch;xterm;;
.TE
.mc |
.ne 20
.H 2 "Creating Menus Without Convenience Functions"
.mc
Applications that use the menu system convenience functions do not need to
explicitly create MenuShell widgets; the
\fBXmCreatePopupMenu\fP and \fBXmCreatePulldownMenu\fP functions create
a Popup or Pulldown MenuPane and the parent MenuShell.
.P
If an application requires access to individual MenuShells in an
application, the MenuShells and MenuPanes can be created by using the
standard X Toolkit create routines or by using the create functions for
MenuShells and RowColumn Widgets. 
.iX "MenuShell" "creating"
.H 3 "Functions for Creating Menus"
.P
Three functions are used in creating menu systems.
.BL
.LI
The MenuShell specific create function:
.mc |
.sp
\fBWidget XmCreateMenuShell\fP (\fIparent\fP, \fIname\fP, \fIarglist\fP,
.mc
\fIargcount\fP)
.mc |
.sp
.mc
creates an instance of a MenuShell widget and
returns the associated widget ID. 
.LI
The X Toolkit function:
.mc |
.sp
\fBWidget XtCreatePopupShell\fP(\fIname\fP, \fIwidget_class\fP, \fIparent\fP,
.mc
\fIargs\fP, \fInum_args\fP)
.mc |
.sp
.mc
can be used to create a MenuShell for a Popup or Pulldown MenuPane.
.mc |
.br
.ne 7
.mc
.LI
The RowColumn specific create function:
.mc |
.sp
\fBWidget XmCreateRowColumn\fP (\fIparent\fP, \fIname\fP, \fIarglist\fP, \fIargcount\fP)
.sp
.mc
creates an instance of a RowColumn widget and returns the associated widget
ID.
.mc |
.br
.ne 16
.mc
.H 3 "Parenting Relationships"
The parenting relationships required to
create a menu system without using convenience functions depend on the
type of menu system being built:
.BL
.LI
If the MenuShell is for a Popup MenuPane, the MenuShell must be the parent
.mc |
of the Popup MenuPane (see Figure 6-12).
.mc
.LI
If the MenuShell is for a MenuPane that is pulled down from 
a MenuBar, the MenuShell must be created as a child of the MenuBar (see
.mc |
Figure 6-13).
.mc
.LI
If the MenuShell is for a submenu MenuPane that is pulled down from a Popup or
another Pulldown MenuPane, the MenuShell must be created as a 
.mc |
child of the Popup or Pulldown MenuPane's parent MenuShell (see Figure 6-14).
.mc
.LI
If the MenuShell is for a Pulldown MenuPane in an Option menu, the
.mc |
MenuShell must have the same parent as the Option menu (see Figure 6-15).
.mc
.LE
.mc |
.ne 20
.sp
.FG "Creating a Popup Menu System Without Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxwid = 1.00
boxht = .3
down
box "Parent Widget"
line down .25
box "MenuShell"
line down .25
box "Popup MenuPane"
line down .25
box "Label"
box "Separator"
box "PushButton"
box "PushButton"
box "PushButton"
.PE
.mc *
.mc
\s10
.mc |
.DE
.+n
.ne 30
.FG "Creating a Pulldown Menu System Without Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxht = .3; boxwid = 1.4;
down
box "Parent Widget"
line down .25
Box1: box "MenuBar"
line down .25
line left 2.00
line down .25
Box2: box "CascadeButton1"
move to Box1.s; move down .25; move left 1.0
line down 1.0
box "MenuShell1"
line down .25
Box3: box "Pulldown MenuPane1"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s; move down .25
line down .25
Box4: box "CascadeButton2"
move to Box1.s; move down .25; move right 1.0
line down 1.0
box "MenuShell2"
line down .25
Box5: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s; move down .25; line right 2.0
line down .25
box "CascadeButton (Help)"
line dashed <-> from Box2.s to Box3.w
line dashed <-> from Box4.s to Box5.w
.PE
.mc *
.mc
\s10
.mc *
.mc
.DE
.mc |
.+n
.ne 20
.FG "Creating Submenus Without Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxht = .3; boxwid = 1.15;
down
box "Parent Widget"
line down .25
Box1: box "MenuShell"
line down .25
line left 2.10
line down .25
Box2: box "MenuShell1"
line down .75
line left .75
line down .25
box "MenuShell3"
line down .25
Box4: box "Pulldown MenuPane3"
line down .25
box "PushButton"
box "PushButton"
move to Box2.s
move down .75
line right .75
line down .25
Box3: box "Pulldown MenuPane1"
line down .25
box "PushButton"
box "PushButton"
Box8: box "CascadeButton3"
move to Box1.s; move down .25
line down .25
box "Popup MenuPane"
line down .25
Box5: box "CascadeButton1"
Box6: box "CascadeButton2"
move to Box1.s; move down .25
line right 1.4
line down .25
box "MenuShell2"
line down .25
Box7: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
line dashed <-> from Box4.e to Box8.w
line dashed <-> from Box3.e to Box5.w
line dashed <-> from Box6.e to Box7.w
.PE
\s10
.mc *
.mc
.DE
.mc |
.+n
.ne 20
.FG "Creating an Option Menu System Without Convenience Functions"
.if n .ig +n
.mc
.DS CB
.ps 8
.PS
boxht = .3; boxwid = 1.25;
down
Box1: box "Parent Widget"
line down .25
line left 2.00
line down .25
box "MenuShell1"
line down .25
Box2: box "Pulldown MenuPane1"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
move to Box1.s
move down .25;
line down .25
Box3: box "OptionMenu1"
Box4: box "OptionMenu2"
move to Box1.s
move down .25
line right 2.00
line down .25
box "MenuShell2"
line down .25
Box5: box "Pulldown MenuPane2"
line down .25
box "PushButton"
box "PushButton"
box "PushButton"
line dashed <-> from Box2.e to Box3.w
line dashed <-> from Box4.e to Box5.w
.PE
\s10
.mc *
.mc
.DE
.mc |
.+n
.mc
.H 3 "Sample Program"
The following program creates a Popup menu system without using the
convenience functions. 
.mc |
Figure 6-14 illustrates the menu system
.mc
created by this program.
.P
The source code for this program is located on your system
.mc |
in \fB./demos/xmsamplers/xmmenushel.c\fP.
.mc
.P
.mc |
.in0
.S -2
.mc
\f(CW
.nf
.ta .5i 1i 1.5i
/* Popup Menu and Submenus created with MenuShells */

#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>


/*********** Callback for the Pushbuttons ***********************/

void ButtonCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
   /*  print message and terminate program  */
   printf ("Button %s selected.\\n", client_data);
	
}

/************** Event Handler for Popup Menu ************************/

PostIt (w, popup, event)
Widget w;
Widget popup;
XButtonEvent * event;
{
   if (event->button != Button3)
	return;
   XmMenuPosition(popup, event);
   XtManageChild(popup);
}


/***********************Main Logic for Program ********************/


void main (argc, argv)
int argc;
char **argv;
{
.mc |
   XtAppContext app_context;
.mc
   Widget toplevel, rc, buttons[2];
   Widget popupshell, mshell1, mshell2, mshell3;
   Widget popup, submenu1, submenu2, submenu3;
   Widget popupBtn[2], sub1Btn[3], sub2Btn[2], sub3Btn[2];
   Arg args[5];

/* Initialize toolkit */
   
.mc |
   toplevel = XtAppInitialize (&app_context, "PopupMenu", NULL, 0,
                                 &argc, argv, NULL, NULL, 0);
.mc

/* Create RowColumn in toplevel with two pushbuttons */

   XtSetArg(args[0], XmNwidth, 150);
   XtSetArg(args[1], XmNheight, 50);
   XtSetArg(args[2], XmNresizeWidth, False);
   XtSetArg(args[3], XmNresizeHeight, False);
   XtSetArg(args[4], XmNadjustLast, False);
   rc = XmCreateRowColumn(toplevel, "rc", args, 5);  
   XtManageChild(rc);

   buttons[0] = XmCreatePushButtonGadget(rc, "button1", NULL, 0);
   XtAddCallback(buttons[0], XmNactivateCallback, ButtonCB, "1");

   buttons[1] = XmCreatePushButtonGadget(rc, "button2", NULL, 0);
   XtAddCallback(buttons[1], XmNactivateCallback, ButtonCB, "2");
   XtManageChildren(buttons, 2);

/* Create MenuShell for a Popup MenuPane */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100); 
   popupshell = XmCreateMenuShell(rc, "popupshell", args, 2);

/* Create RowColumn Widget configured as Popup MenuPane */

   XtSetArg(args[0], XmNrowColumnType, XmMENU_POPUP);
   popup = XmCreateRowColumn (popupshell, "popup", args, 1);
   XtAddEventHandler(rc, ButtonPressMask, False, PostIt, popup);

/* Create MenuShells and Pulldown MenuPanes for two submenus */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100);
   mshell1 = XmCreateMenuShell (popupshell, "mshell1", args, 2);

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu1 = XmCreateRowColumn (mshell1, "submenu1", args, 1);

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100); 
   mshell2 = XmCreateMenuShell (popupshell, "mshell2", args, 2);

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu2 = XmCreateRowColumn (mshell2, "submenu2", args, 1);

/* Create two Cascade Buttons in the Popup MenuPane */

   XtSetArg(args[0], XmNsubMenuId, submenu1);
   XtSetArg(args[1], XmNlabelString, XmStringCreate("First Submenu",
            XmSTRING_DEFAULT_CHARSET));
.mc |
   popupBtn[0] =
             XmCreateCascadeButtonGadget(popup, "cbutton1", args, 2);
.mc

   XtSetArg(args[0], XmNsubMenuId, submenu2); 
   XtSetArg(args[1], XmNlabelString, XmStringCreate("Second Submenu",
            XmSTRING_DEFAULT_CHARSET));
.mc |
   popupBtn[1] =
             XmCreateCascadeButtonGadget(popup, "cbutton2", args, 2);
.mc
   XtManageChildren (popupBtn, 2);

/* Create pushbuttons in MenuPanes submenu1 and submenu2 */

.mc |
   sub1Btn[0] =
            XmCreatePushButtonGadget(submenu1, "button1a", NULL, 0);
.mc
   XtAddCallback(sub1Btn[0], XmNactivateCallback, ButtonCB, "1a");

.mc |
   sub1Btn[1] =
            XmCreatePushButtonGadget(submenu1, "button1b", NULL, 0);
.mc
   XtAddCallback(sub1Btn[1], XmNactivateCallback, ButtonCB, "1b");

.mc |
   sub2Btn[0] =
            XmCreatePushButtonGadget(submenu2, "button2a", NULL, 0);
.mc
   XtAddCallback(sub2Btn[0], XmNactivateCallback, ButtonCB, "2a");

.mc |
   sub2Btn[1] =
            XmCreatePushButtonGadget(submenu2, "button2b", NULL, 0);
.mc
   XtAddCallback(sub2Btn[1], XmNactivateCallback, ButtonCB, "2b");
   XtManageChildren (sub2Btn, 2);


/* Create a MenuShell for the submenu of submenu1 */

   XtSetArg(args[0], XmNheight, 100);
   XtSetArg(args[1], XmNwidth, 100);
   mshell3 = XmCreateMenuShell (mshell1, "mshell3", args, 2);

/* Create the MenuPane for the submenu of submenu1 */

   XtSetArg(args[0], XmNrowColumnType, XmMENU_PULLDOWN);
   submenu3 = XmCreateRowColumn (mshell3, "submenu3", args, 1);

/* Create the Cascade Button in submenu1 for accessing submenu3 */

   XtSetArg(args[0], XmNsubMenuId, submenu3);
.mc |
   XtSetArg(args[1], XmNlabelString,
      XmStringCreate("To Third Submenu", XmSTRING_DEFAULT_CHARSET));
   sub1Btn[2] =
         XmCreateCascadeButtonGadget(submenu1, "cbutton3", args, 2);
.mc
   XtManageChildren(sub1Btn, 3);

/* Create pushbuttons in submenu */

.mc |
   sub3Btn[0] =
            XmCreatePushButtonGadget(submenu3, "button3a", NULL, 0);
.mc
   XtAddCallback(sub3Btn[0], XmNactivateCallback, ButtonCB, "3a");

.mc |
   sub3Btn[1] =
            XmCreatePushButtonGadget(submenu3, "button3b", NULL, 0);
.mc
   XtAddCallback(sub3Btn[1], XmNactivateCallback, ButtonCB, "3b");
   XtManageChildren (sub3Btn, 2);

/* Get and dispatch events */

   XtRealizeWidget(toplevel);

.mc |
   XtAppMainLoop(app_context);
.mc
}
.ta .5i 3i
\fP
.fi
.mc |
.S
.in

.mc
