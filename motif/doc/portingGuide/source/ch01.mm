.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.PH ""
.OF "'''\\\\nP'"
.EF "'\\\\nP'''"
.OH "###Porting Guide#"
.EH "#Porting Guide###"
.H 1 "Introduction"
This document provides information for porting
Applications to OSF/Motif.  It is of particular
interest to developers porting applications from
the XUI toolkit.
.P
In certain areas the OSF/Motif toolkit is 
different from the XUI toolkit.  In some instances, the
functionality present in XUI has been changed; in other
instances the functionality found in XUI is not present in OSF/Motif.
The purpose of this chapter (and of Appendix A) is to help
developers with significant applications
on XUI to move those applications, as painlessly as possible, to the
OSF/Motif toolkit.  
.SP
Developers should be aware that the changes outlined in this document
are merely a starting point and are by no means complete.
This is particularly true of the section entitled
"Widget Hierarchy Differences by Class" later in this chapter.
A complete version of this document will be available with Motif
Release 1.0.
.SP
.ce
.B "NOTE"
.SP
The OSF/Motif toolkit provides some functionality that goes beyond
what is present in XUI.  However, since this functionality does 
not conflict with the XUI API, it is not documented here.  
.SP 1.5
.P
This remainder of this document describes, in general, the changes that were 
made to XUI in order to produce the OSF/Motif toolkit.
The bulk of these changes were made to incorporate Presentation
Manager (PM) behavior
and the 3D beveled appearance.  Other changes were made to
provide consistent naming or to prepare for enhanced functionality.
.SP 1.5
.H 1 "Hierarchy"
.P
XUI defined a DwtCommon class which was a superclass not only for
primitive widgets such as DwtLabel and DwtScrollBar, but also for
composite widgets such as DwtMenu and DwtMainWindow.  It defined
a number of generally useful resources.  These resources were
also repeated in the DwtDialog widget, and some of them in the
DwtSText widget, neither of which were subclasses of DwtCommon.
Figure 1 on the following page outlines the XUI widget hierarchy.
.SP
Motif defines XmManager as the superclass for all Motif composite
widgets, including both XmMainWindow (which was in XUI's DwtCommon)
and XmBulletinBoard (equivalent to DwtDialog, which was not).
XmPrimitive is the superclass all for primitive widgets, including
both XmScrollBar (which was in XUI's DwtCommon), and XmText
(equivalent to DwtSText, which was not).
The reorganization of the class hierarchy based on XmManager
and XmPrimitive (as well as XmGadget, the superclass for gadgets)
was primarily done to simplify coding of PM-style keyboard
traversal.  Figure 2 outlines the OSF/Motif widget hierarchy.
.SP
.ne 8i
.if n .ig +n
.PS
.ps -2
.vs -2
down
box wid .65 "Core";
line down .25;
Composite: [
	box wid .65 "Composite";
	]
line down .25;
line left 2; down
line down .25;
box wid .65"Text";
line down .25; box wid .65 "SText";
#
move down .25 from bottom of Composite
move left .75; line down .25;
box wid .65 "Constraint";
line down .25;
Dialog: [
	box wid .65"Dialog"
	]
line down .25 from bottom of Dialog
define level1 %
	 line right .1; move right .05; "$1" ljust 
	%
level1(Attached)
move down .25 from bottom of Dialog;
line down .25; 
level1(Command)
move down .5 from bottom of Dialog;
line down .25
level1(Message)
move down .75 from bottom of Dialog;
line down .25
level1(Selection)
move right .15; move down .1; line down .15;
level1(FileSelection)
#
move down .25 from bottom of Composite
line right .75; line down .25;
Common: [
	box wid .65 "Common"
	]
line down .25; 
level1(Help)
move down .25 from bottom of Common;
line down .25; 
level1(MainWindow)
move down .5 from bottom of Common;
line down .25; 
level1(Menu)
move down .75 from bottom of Common;
line down .25; 
level1(Scale)
move down 1 from bottom of Common;
line down .25; 
level1(Scroll)
move down 1.25 from bottom of Common;
line down .25; 
level1(Window)
move down 1.5 from bottom of Common;
line down .25; 
level1(ScrollWindow)
move right .15; move down .1; line down .15;
level1(ListBox)
move down 1.75 from bottom of Common;
line down .5
level1(Label)
move right .15; move down .1; line down .15;
level1(PullDown)
move left .15; line down .25; 
level1(PushButton)
move left .15; line down .25; 
level1(Separator)
move left .15; line down .25; 
level1(Toggle)
#
move down .25 from bottom of Composite
line right 2; line down .25; 
Shell: [
	box wid .65 "Shell"
	]
line down .25 from bottom of Shell
level1(OverrideShell)
move right .15; move down .1; line down .15; 
level1(MenuPopup)
move down .25 from bottom of Shell
line down .5; 
level1(WMShell)
move right .15; move down .1; line down .15; 
level1(VendorShell)
move right .15; move down .1; line down .15
level1(TransientShell)
move left .15; line down .25; 
level1(TopLevelShell)
move right .15; move down .1; line down .15;
level1(HelpShell)
move left .15; line down .25
level1(ApplicationShell)
move left .15; line down .25
level1(HiddenShell)
#
#line right .1; move right .05; "$1" ljust 
.ps
.vs
.PE
.+n
.FG "XUI Widget Hierarchy"
.SP 1.5
.br
.ne 8i
.if n .ig +n
.PS
.ps -2
.vs -2
down
box dashed ht 2.25i wid 1.2i;
move up 1i;
move left .6i;
line dashed left .35i;
move left .1
"Core" rjust;
move up 1.25i
move right 1i;
move down .1;
box wid .65 "Object";
line down .25;
box wid .65"RectObj";
line down .15; line right 1.85; line down .25;
Gadget: [
	box wid .65"XmGadget"
	]
#box wid .65"XmGadget";
move up .25 from top of Gadget; move left 1.85;
line down .15;
box wid .65 "WindowObj";
line down .25;
line right 1;
line down .25;
Primitive: [
	box wid .65 "XmPrimitive"
	]
#box wid .65 "XmPrimitive";
move up .75; 
move left 1;
line left 1;
line down .25
box wid .65 "Composite";
line down .25;
line left 1;
line down .25;
Shell: [
	box wid .65 "Shell"
	]
#box wid .65 "Shell";
move up .75;
move right 1;
line right 1;
line down .25;
box wid .65 "Constraint";
line down .25;
Manager: [
	box wid .65 "XmManager"
	]
#box wid .65 "XmManager"
#
line down .25 from bottom of Gadget
define level1 %
	 line right .1; move right .05; "$1" ljust 
	%
level1(XmArrowButtonGadget)
move down .25 from bottom of Gadget;
line down .25; 
level1(XmSeparatorGadget)
move down .5 from bottom of Gadget;
line down .25;
level1(XmLabelGadget)
move right .15; move down .1; line down .15; 
level1(XmCascadeButtonGadget)
move left .15; line down .25; 
level1(XmPushbuttonGadget)
move left .15; line down .25; 
level1(XmToggleButtonGadget)
#
line down .25 from bottom of Primitive
level1(XmArrowButton)
move down .25 from bottom of Primitive;
line down .25; 
level1(XmList)
move down .5 from bottom of Primitive;
line down .25; 
level1(XmScrollBar)
move down .75 from bottom of Primitive;
line down .25; 
level1(XmSeparator)
move down 1 from bottom of Primitive;
line down .25; 
level1(XmText)
move down 1.25 from bottom of Primitive;
line down .25; 
level1(XmLabel)
move right .15; move down .1; line down .15;
level1(XmCascadeButton)
move left .15; line down .25; 
level1(XmDrawnButton)
move left .15; line down .25; 
level1(XmPushButton)
move left .15; line down .25; 
level1(XmToggleButton)
move left .15
#
line down .25 from bottom of Shell
level1(OverrideShell)
move right .15; move down .1; line down .15;
level1(XmMenuShell)
move down .25 from bottom of Shell;
line down .5; 
level1(WMShell)
move right .15; move down .1; line down .15;
level1(VendorShell)
move right .15; move down .1; line down .15;
level1(ApplicationShell)
move left .45; move up .25; line down .5; 
level1(TransientShell)
move right .15; move down .1; line down .15;
level1(XmDialogShell)
#
line down .25 from bottom of Manager
level1(XmDrawingArea)
move down .25 from bottom of Manager;
line down .25; 
level1(XmFrame)
move down .5 from bottom of Manager;
line down .25; 
level1(XmPanedWindow)
move down .75 from bottom of Manager;
line down .25; 
level1(XmRowColumn)
move down 1 from bottom of Manager;
line down .25; 
level1(XmScale)
move down 1.25 from bottom of Manager;
line down .25; 
level1(XmScrolledWindow)
move right .15; move down .1; line down .15; 
level1(XmMainWindow)
move down 1.5 from bottom of Manager;
line down .5; 
level1(XmBulletinBoard)
move right .15; move down .1; line down .15;
level1(XmForm)
move left .15; line down .25; 
level1(XmMessageBox)
move left .15; line down .25; 
level1(XmSelectionBox)
move right .15; move down .1; line down .15;
level1(XmCommand)
move left .15; line down .25; 
level1(XmFileSelectionBox)
.ps 
.vs
.PE
.+n
.FG "OSF/Motif Widget Hierarchy"
.H 1 "3D Visuals"
.P
Motif uses consistent visuals for all widgets that
support a "3D bevelled" appearance.
The resources which control these are found in XmManager,
XmPrimitive, and XmGadget, and include:
.SP
.DS
- XmNbottomShadowColor
- XmNbottomShadowPixmap
- XmNshadowThickness
- XmNtopShadowColor
- XmNtopShadowPixmap
.DE
.SP
These replace some resources used in XUI which had specific
visual effects, in particular:
.SP
.DS
- DwtNhalfBorder in DwtSText.
- DwtNbordHighlight, DwtNfillHighlight, and DwtNshadown in DwtPushButton
.DE
.SP 1.5
.H 1 "Resolution Independence"
.P
XUI provided a resolution independence mechanism which allowed the
x,y position of children in dialog boxes (DwtDialog or its subclasses)
to be specified in either pixel units or font units, based upon a
DwtNunits resource in the dialog box.
.SP 
Motif provides a resolution independence mechanism (not in the AES,
however) for all widgets.  Each widget (or gadget) has an XmNunitType
resource, the value of which can be XmPIXELS, Xm100TH_MILLIMETERS,
Xm1000TH_INCHES, Xm100TH_POINTS, or Xm100TH_FONT_UNITS.  The unitType
of a widget determines the unit used for evaluating its own width
and height, and its own (not its children's) x,y position.
.SP 1.5
.H 1 "Input Model"
.P
XUI uses a "Globally Active" input model.  When a user
clicks on a widget, that widget determines whether or not the
window should take the keyboard focus.  In particular, scrollbars
in XUI do not take focus.
.SP
Motif uses a "Passive" input model.  When a user clicks anywhere
within a window, the window manager determines whether or not
that window should have the focus.  In particular, the Motif
Window Manager, when operating using an explicit
keyboardFocusPolicy, sets focus to the window depending upon
which button has been pressed.
.SP
Consequently, Motif widget developers should not explicitly
call XSetInputFocus.
.SP 1.5
.H 1 "Scrolling Model"
.P
In XUI, both DwtSText and DwtListBox had scrolling built-in.
DwtListBox always provided vertical scrollbars if necessary,
and optionally provided horizontal scrollbars based upon
DwtNHorizontal.  DwtSText optionally provided a vertical
scrollbar based on DwtNscrollVertical, with DwtNscrollLeftSide
determining if the scrollbar should be placed to the left or right
of the text.
.SP
In Motif, scrollbars are provided by placing either an
XmText or XmList widget inside of an XmScrolledWindow
widget, which actually provides the scrollbars.
Scrollbars can be placed vertically on either the left or
right, and horizontally on either the top or bottom.
.SP
The convenience function XmCreateScrolledList creates
an XmList inside an XmScrolledWindow, and attaches
XmList-specific callbacks to the scrollbars.
The argument list provided with XmCreateScrolledList
is passed to the XmList widget, which interprets
some of its arguments to control the scrollbars, including
XmNlistSizePolicy, XmNscrollBarDisplayPolicy, and
XmNscrollBarPlacement.  Depending upon the policies chosen,
both horizontal and vertical scrollbars will either always be
displayed, or just displayed when needed.
.SP
The convenience function XmCreateScrolledText creates
an XmText inside an XmScrolledWindow, and attaches
XmText-specific callbacks to the scrollbars.
The argument list provided with XmCreateScrolledText
is passed to the XmText widget, which interprets
some of its arguments to control the scrollbars.
XmNscrollHorizontal and XmNscrollVertical specify
whether or not to provide horizontal or vertical scrollbars,
and XmNscrollLeftSide and XmNscrollTopSide specify
their placement.
.SP 1.5
.H 1 "Compound String Functionality" 
.P
The compound string functionality supported in XUI has been
altered in Motif to present a more vendor neutral naming scheme,
as well as expanded application functionality.   Most of the
changes are simply name changes.   In the case of certain
functions, argument order may have changed, or arguments may be
missing.  
Appendix A lists the name changes, as well as
suggested replacements for particular functions. 
.SP 1.5
.H 1 "Gadget Functionality"
.P
In both XUI and Motif, gadgets (windowless widgets) increase performance
by minimizing time for startup, exposure, and resize processing,
and by eliminating server space for a window.   Both Motif and XUI
subclass gadgets from RectObj, not from Core, so that gadgets
are smaller than their corresponding widgets.  The missing
Core resources deal with translations and accelerators,
and with color and other aspects of the display, i.e.
XtNbackground, XtNbackgroundPixmap, XtNborderColor, XtNborderPixmap,
XtNcolormap, XtNdepth, and XtNscreen.  These color-specific resources
are taken from the gadget's parents.  That is, all of the gadget
children of a manager widget share the same color-related resources.
.SP
In both XUI and Motif, widgets also contain resources which control
highlighting--XUI defines DwtNhighlight and DwtNhighlightPixmap
in DwtCommon and DwtDialog, Motif defines XmNhighlightColor and
XmNhighlightPixmap in XmPrimitive and XmManager.  Gadgets do
not contain these resources.  In both toolkits, the highlighting
of a gadget is determined by the resources of its manager parent.
.SP
Motif expands on this model of resource sharing with the
other color-specific resources it defines--XmNbottomShadowColor,
XmNbottomShadowPixmap, XmNtopShadowColor and XmNtopShadowPixmap.
These are defined in XmPrimitive and XmManager, but not in XmGadget.
Gadgets in Motif do not have these resources, and they are taken
instead from the manager widget which is the gadget's parent.
.SP
XUI takes a more aggressive approach to reducing gadget size.
In XUI, gadgets have many fewer resources than the corresponding
widgets.  In gadgets, the default value of the missing resources
are assumed, and cannot be changed.  Widgets must be used in those
cases where a different value is required.
.SP 1.5
.H 1 "Help" 
.P
The OSF UEC Selection Team did not choose to adopt the XUI DwtHelp
widget as part of Motif.  DwtHelp uses file-based technology to provide
a text-only help network.  The UEC Selection Team chose to defer
this component until the industry could provide a hypermedia-based
help system that could support graphics as well as text.

Help callbacks are now provided as part of every widget and gadget in
Motif, and applications can build their own help dialogs using
XmCreateBuletinBoardDialog or XmCreateFormDialog.
.SP 1.5
.H 1 "Shells"
.P
Both XUI and Motif place top-level widgets in Shell widgets.
Users generally do not instantiate these shell widgets directly.
Rather, they are instantiated as part of the functions
which create their children.  For example, XmCreatePopupMenu
creates an XmRowColumn widget inside an XmMenuShell,
XmCreateFormDialog creates an XmForm widget inside an
XmDialogShell, and XmCreateMainWindow creates an XmMainWindow
widget inside an ApplicationShell.

To speed up menu processing, all pulldown menu children of a menu bar
share the same XmMenuShell widget.
.SP 1.5
.H 1 "Name Changes"
.P
This section lists general rules for changing XUI widget, function, 
resource and other names to comply with OSF/Motif.   
Exceptions to each rule appear in Appendix A.
.SP 1.5
Many of these changes are simply
intended to create a vendor neutral naming scheme.  
For example, OSF/Motif uses the "Xm" (X Motif) prefix instead of
the "Dwt" prefix used in XUI and the "Xaw" prefix used in the
Athena Widget Set.
.SP 1.5
Certain changes to enumeration literals are intended to 
reflect more closely the naming conventions set out in the Xt
Intrinsics manual.  For example, enumeration literals are all
upper case in the Intrinsics, and (except for the Xm prefix),
the same convention has been followed in Motif.
For example, the literal DwtAttachNone used for constraint resources
in DwtAttachedDB widgets has been changed to XmATTACH_NONE.
Callback reasons have been similarly changed, for example, from
DwtCRArm to XmCR_ARM.
.SP 1.5
The general rules for name changes are as follows:
.SP 1.5
.AL 1
.LI
Change the prefix Dwt to Xm.
.SP .5
For resource manager function names, change the prefix DwtDrm to Mrm.
.SP .5
For resource names, change the prefix DwtN to XmN and the
prefix DwtNadb to XmN.
.SP .5
.LI
Make the changes to widget class names specified in "Widget
Hierarchy Differences by Class" later in this document.
.LI
Change all create function names from Dwt*Create to XmCreate*,
while also keeping in mind the changes to widget class names.
.LI
The XUI create functions that do not use arglists (e.g.,
DwtLabel) have no counterpart in OSF/Motif.
.LI
Change the enumeration literals of the form DwtAaaaAaaa to the
form XmAAAA_AAAA. 
.LI
Change callback reasons from DwtCRAaaaAaaa to XmCR_AAAA_AAAA.
.LI
All String functions begin with XmString. 
.SP .5
All FontList functions begin with XmFontList.
.SP .5
All Clipboard functions begin with XmClipboard.
.SP .5
All resource manager (Mrm) functions begin with Mrm.
.LE
.SP 1.5
.H 1 "Widget Hierarchy Differences by Class"
.P
This section outlines the changes to the XUI widget hierarchy to
comply with OSF/Motif.  The XUI widget classes are listed alphabetically
with their Motif equivalents.  Beneath each widget class appear
functionality changes and name changes within the class.  
Be aware that the listed changes are merely a starting point.  They
are not complete at this time. 
.SP .5
.ne 4i
.TS H
tab(@);
lb cb cb
l l l
l l s
l l l.
Item@XUI@Motif
.SP .3
_
.SP .3
.TH
Class:@DwtAttachedDB@XmForm
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNhorizontal@XmNhorizontalSpacing
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtCommandWindow@XmCommand
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNprompt@XmNpromptString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtCommon@
.SP .5
\0\0Functionality changes:@T{
The widget class DwtCommon has no equivalent in Motif.  Its
resources are found in XmManager, XmPrimitive, and to some
extent, XmGadget.
T}@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtDialogBox@XmBulletinBoard
.SP .5
\0\0Functionality changes:@T{
DwtNunits becomes XmNunitType, which has been moved to
XmPrimitive, XmManager, and XmGadget.
.SP .5
DwtModal has been split into XmDIALOG_APPLICATION_MODAL and
XmDIALOG_SYSTEM_MODAL.
T}@
.SP .5
\0\0Resource name changes:@DwtNstyle@XmNdialogStyle
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtFileSelection@XmFileSelectionBox 
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNfilterLabel@XmNfilterLabelString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtLabel@XmLabel
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtListBox@XmList
.SP .5
\0\0Functionality changes:@T{
There are now three selection callbacks
(single_selection, multiple_selection, and extend_selection) for XmList.
.SP .5
If XmNselection_policy is XmSINGLE_SELECTION then the first selection
goes to the single_selection callback, otherwise it goes to the
multiple_selection callback.  Unless applications
specifically set the selection policy to XmSINGLE_SELECTION
(the default is XmMULTIPLE_SELECTION), they will not get
callbacks on selections in the list. 
T}@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtMainWindow@XmMainWindow
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtMenu@XmRowColumn
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNmenuNumColumns@XmNnumColumns
@DwtNmenuPacking@XmNpacking
@DwtNmenuIsHomogeneous@XmNisHomogeneous
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.TE
.br
.ne 4i
.TS H
tab(@);
lb cb cb
l l l
l l s
l l l.
Item@XUI@Motif
.SP .3
_
.SP .3
.TH
Class:@DwtMessageBox@XmMessageBox
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNcancelLabel@XmNcancelLabelString
@DwtNdefaultPushButton@XmNdefaultButtonType
@DwtNhelpLabel@XmNhelpLabelString
@DwtNokLabel@XmNokLabelString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtMessageBox@XmMessageBox
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNcancelLabel@XmNcancelLabelString
@DwtNdefaultPushButton@XmNdefaultButtonType
@DwtNhelpLabel@XmNhelpLabelString
@DwtNokLabel@XmNokLabelString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtPullDownMenuEntry@XmCascadeButton
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtPushButton@XmPushButton
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtScale@XmScale
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNtitleLabel@XmNtitleString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtScrollBar@XmScrollBar
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNinc@XmNincrement
@DwtNpageDecCallback@XmNpageDecrementCallback
@DwtNpageInc@XmNpageIncrement
@DwtNpageIncCallback@XmNpageIncrementCallback
@DwtNunitDecCallback@XmNdecrementCallback
@DwtNunitIncCallback@XmNincrementCallback
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtScrollWindow@XmScrolledWindow
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtSelection@XmSelectionBox
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNcancelLabel@XmNcancelLabelString
@DwtNhelpLabel@XmNhelpLabelString
@DwtNokLabel@XmNokLabelString
@DwtNselectionLabel@XmNselectionLabelString
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtSeparator@XmSeparator
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtSText@XmText
.SP .5
\0\0Functionality changes:@T{
XmNlosingFocusCallback functionally
replaces DwtNlostFocusCallback.
.SP .5
XmCR_LOSING_FOCUS functionally replaces DwtCRLostFocus.
The Xm callback is different than the Dwt callback in 
that it is issued before focus is lost.
T}@
.SP .5
\0\0Resource name changes:@DwtNautoShowInsertPoint@XmNautoShowCursorPosition
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtToggleButton@XmToggleButton
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@DwtNindicator@XmNindicatorOn
@DwtNshape@XmNindicatorType
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP
.T&
l l l
l l s
l l l.
Class:@DwtWindow@XmDrawingArea
.SP .5
\0\0Functionality changes:@@
.SP .5
\0\0Resource name changes:@@
.SP .5
\0\0Enumeration literal name changes:@@
.SP .5
\0\0Callback reason name changes:@@
.SP .5
.TE
