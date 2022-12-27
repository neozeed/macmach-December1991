.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 1
.af H1 A
.H 1 "OSF/Motif Widgets and Components Correspondence"
This appendix shows how actual OSF/Motif widgets correspond to the
components described in this
.IR "Style Guide" .
Gadgets, which are essentially performance-oriented versions of widgets,
are not listed in this appendix, but correspond closely to their
widget counterparts.
.P
.iX "Separator"
.iX "Label"
.iX "PushButton"
.iX "ToggleButton"
.iX "CheckButton"
.iX "RadioButton"
.iX "CascadeButton"
.iX "OptionButton"
.iX "field controls"
.iX "ScrollBar"
.iX "Scale"
.iX "Sash"
.iX "Text"
.iX "Canvas"
.iX "List"
.iX "basic groups"
.iX "Panel"
.iX "Menu"
.iX "MenuBar"
.iX "layout groups"
.iX "Composite"
.iX "PanedWindow"
.iX "framing groups"
.iX "Frame"
.iX "ScrolledWindow"
.iX "MainWindow"
.iX "DialogBox"
.iX "SelectionBox"
.iX "FileSelctionBox"
.iX "PromptBox"
.iX "MessageBox"
.TB "Component to Widget Correspondence"
.TS H
center, expand, box, tab(@);
lb lb
l l.
Components@Motif Widgets
_
.TH
Basic Controls:
.sp
Separator@\fBXmSeparator\fP
Label@\fBXmLabel\fP
PushButton@\fBXmPushButton\fP
@\fBXmArrowButton\fP
@\fBXmDrawnButton\fP
ToggleButton@\fBXmToggleButton\fP
@It can also be simulated by \fBXmPushButton\fP and \fBXmDrawnButton\fP
CheckButton@T{
\fBXmToggleButton\fP with \fBXmNindicatorType\fP set to \fBXmN_OF_MANY\fP
T}
RadioButton@T{
\fBXmToggleButton\fP with \fBXmNindicatorType\fP set to \fBXmONE_OF_MANY\fP
T}
CascadeButton@\fBXmCascadeButton\fP
OptionButton@T{
An \fBXmCascadeButton\fP within an \fBXmRowColumn\fP
with \fBXmNrowColumnType\fP set to \fBXmMENU_OPTION\fP
T}
.sp
_
Field Controls:
.sp
ScrollBar@\fBXmScrollBar\fP
Scale@\fBXmScale\fP
Sash@Private element of \fBXmPanedWindow\fP
Text@\fBXmText\fP and \fBXmTextField\fP
Canvas@T{
\fBXmDrawingArea\fP (without children), \fBXmDrawnButton\fP
T}
List@\fBXmList\fP
.sp
_
Basic Groups:
.sp
Panel@T{
\fBXmRowColumn\fP with \fBXmNrowColumnType\fP set to \fBXmWORK_AREA\fP
and composed of basic controls
T}
Menu@T{
\fBXmRowColumn\fP with \fBXmNrowColumnType\fP set to \fBXmMENU_PULLDOWN\fP
or a \fBXmMENU_POPUP
T}
MenuBar@T{
\fBXmRowColumn\fP with \fBXmNrowColumnType\fP set to \fBXmMENU_BAR\fP
T}
.sp
_
Layout Groups:
.sp
Composite@T{
\fBXmBulletinBoard\fP, \fBXmDrawingArea\fP (with children),
\fBXmForm\fP, \fBXmRowColumn\fP when
\fBXmNrowColumnType\fP is set
to \fBXmWORK_AREA\fP and it is not a basic group
T}
PanedWindow@\fBXmPanedWindow\fP
.sp
_
Framing Groups:
.sp
Frame@\fPXmFrame\fP
ScrolledWindow@\fPXmScrolledWindow\fP
MainWindow@\fPXmMainWindow\fP
.sp
_
DialogBoxes:
.sp
SelectionBox@\fBXmSelectionBox\fP
FileSelection Box@\fBXmFileSelectionBox\fP
PromptBox@\fBXmPromptBox\fP
MessageBox@\fBXmMessageBox\fP
.TE
