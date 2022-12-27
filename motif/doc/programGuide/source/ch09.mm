.mc |

.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 8
.H 1 "Keyboard Interface"
.iX "Keyboard interface"
.iX "Traversal"  
The keyboard interface allows the user to interact with
an application using the keyboard in place of, or as a supplement to, the
mouse.  This capability is necessary in a variety of situations, such as
.mc |
mouseless systems or applications that do not want to force the user to
.mc
switch back and forth between the keyboard and mouse.
.P
The keyboard interface involves two major components:
.BL
.LI
.mc |
Keyboard focus and traversal from widget to widget
.mc
.LI
.mc |
Keyboard input processing to an individual widget
.mc
.LE
.H 2 "Keyboard Focus Models"
.iX "Focus"
.mc |
Traversal provides the means of moving the keyboard focus within an
.mc
application.  The keyboard focus indicates which widget is currently
active.  When a particular widget has keyboard focus, all keyboard
input directed at the application goes to that widget, regardless of the
location of the pointer.
.mc |
.br
.ne 8
.mc
.P
Motif supports two focus models:
.BL
.LI
.mc |
.iX "Focus" "pointer-driven"
.iX "Pointer-driven focus"
The pointer-driven focus model. In the pointer-driven
.mc
model, a widget receives keyboard input only when the cursor is
positioned within the widget's bounding rectangle; moving the cursor out of
the widget causes it to lose focus. 
.LI
.iX "Focus" "click-to-type"
.iX "Click-to-type focus"
.mc |
The click-to-type focus model.  In the click-to-type model, when
.mc
the window manager passes the focus to the topmost shell widget, the
topmost shell widget redirects the focus to one of its descendents.
The user can move the focus to another descendent of
the topmost shell widget either
by pressing the arrow or tab keys, or by clicking mouse button 1 in a
widget.  Clicking mouse button 1 in a widget may cause that widget to ask
for and receive the input focus. When a descendent has focus, it continues
.mc |
to receive all keyboard input until either of the following occurs:
.DL
.mc
.LI
The user requests that the focus be moved
to another descendent of the topmost shell widget.
.LI
The window manager takes the focus away from the topmost shell widget.
.LE
.LE
.P
An application sets the desired model
.mc |
by means of the \fBXmNkeyboardFocusPolicy\fP resource, which is exported by the
.mc
VendorShell
widget class.  The specified focus model is active for the complete widget
hierarchy built from the topmost shell widget.  
.P
The functionality described in the rest of this chapter applies only to the
.mc |
click-to-type focus model. The Motif menu system provides its own
type of keyboard traversal.  This is explained in Chapter 6, ``Menus.''
.mc
.P
.iX "Focus" "accepting"
.iX "Accepting focus"
.P
.mc |
Only Primitive widgets and gadgets can have the keyboard focus, since they
are not merely containers; the user interacts with them.
In this discussion, gadgets 
.mc
are considered comparable to Primitive widgets. 
.P
.mc |
Each Primitive widget has a Boolean resource,
.mc
XmNtraversalOn, that specifies whether or not the widget will
accept the focus.  The default is False, which denies
focus.  The resource must be set to True in order for the widget to accept
the focus.
.P
When a widget has accepted the keyboard focus, a highlight is drawn
around the widget.
.H 2 "Grouping Widgets Into Tab Groups"
.iX "Tab groups"
.P
.mc |
Motif uses the concept of tab groups to group Primitive widgets.
Any manager or Primitive widget can be defined as a tab group. If a manager
.mc
widget is in a tab group, its
Primitive children are part of the tab group.
.P
Two functions manage the addition and deletion of tab
groups for an application.
.P
.iX "XmAddTabGroup"
.mc |
Tab groups are ordinarily specified by the \fBXmNnavigationType\fP
resource.
\fBXmAddTabGroup\fP is called to control the order of traversal of tab
groups.
.mc
\fBXmAddTabGroup\fP adds the specified tab group to the list of tab groups
associated with a particular widget hierarchy.
.P
.iX "XmRemoveTabGroup"
.mc |
\fBXmAddTabGroup\fP(\fItab_group\fP)
.mc
.br
.mc |
      \fBWidget\fP \fItab_group\fP;
.mc
.VL 1i
.ne 3
.LI \fItab_group\fP
.mc |
Specifies the manager or Primitive widget that defines a tab group.
.mc
.sp
.LE
.P
\fBXmRemoveTabGroup\fP removes the tab group from the list of tab
groups associated with a particular widget hierarchy.
.P
.mc |
\fBXmRemoveTabGroup\fP(\fItab_group\fP)
.mc
.br
.mc |
      \fBWidget\fP \fItab_group\fP;
.mc
.VL 1i
.ne 3
.LI \fItab_group\fP
.mc |
Specifies the manager or Primitive widget that defines a tab group.
.mc
.sp
.LE
.H 2 "Traversal Within and Between Tab Groups"
.iX "Traversal" "between tab groups"
.iX "Traversal" "within tab groups"
.iX "Tab groups, traversal"
Traversal involves two types of focus changes\(emchanging the focus to a
different widget within a particular tab group and changing the focus to
another tab group.
.mc |
.br
.ne 9
.mc
.P
Movement among the Primitive widgets within a tab group is controlled
by the order in which the widgets were created.  
The following keys change
the focus to another widget in the same tab group:
.BL
.LI
The down arrow key moves the focus to the next widget for which the
XmNtraversalOn resource has been set to True.
When the focus
reaches the end of the tab group, it wraps to the beginning.
The right arrow key
has the same effect unless its behavior is defined by the particular widget.
.mc |
For example, a Text widget configureOBd for single-line edit defines the
.mc
behavior of the right arrow key; therefore, that key does not change the
focus. 
.LI
The up arrow key moves the focus to the previous widget.  When the focus
reaches the beginning of the tab group, it wraps to the end.
The left arrow key
has the same effect unless its behavior is defined by the particular widget.
.LI
The home key moves the focus to the first Primitive widget in the tab group.
.LE
.P
Movement between tab groups is controlled by the
order in which the application has registered the tab groups.
.BL
.LI
.mc |
The 
\fB<Tab>\fP 
moves the focus to the first widget in
.mc
the next tab group.  When the focus reaches the end of the tab group list,
the focus wraps to the beginning of the list. 
.LI
.mc |
\fB<Shift>\fP \fB<Tab>\fP  moves the focus to the first widget
.mc
in the previous tab group.
When the focus reaches the beginning of the tab group list, it
wraps to the end of the list.
.LE
.P
Clicking mouse button 1 within certain widgets (typically, text widgets) 
moves the focus to the indicated widget.  The focus remains there
.mc |
until either the widget hierarchy loses the focus, or the user moves the
.mc
focus to another widget.  A widget must have its
XmNtraversalOn resource set to True in order to get focus this way.
.P
Certain widgets must be placed within their own tab group; that is,
the widget cannot be included in a tab group containing other widgets.
.BL
.mc |
.br
.ne 12
.mc
.LI
Each List widget and the ScrollBar widget
must be registered as
its own tab group, since they define special behavior for the
arrow keys.
.LI
Each
multiline text widget must be registered as its own tab group, since
it defines special behavior for both the arrow keys and the
.mc |
Tab keys.  Single-line text widgets do not
.mc
have this requirement.
.LI
The Option menu
widget must be registered as its own tab group because it consists of two
internal Primitive widgets.
.LE
.H 2 "Keyboard Input Processing to a Widget"
Keyboard input into a widget that has focus is handled by definitions of the
widget's default translations for keyboard input. Refer to the \fIOSF/Motif
.mc |
Programmer's Reference\fP for the default translations.
.mc
