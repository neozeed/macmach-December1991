.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR ScrolledWindow
.hS "Description"
.iX "ScrolledWindow"
A ScrolledWindow should be used to frame other
components and to provide
ScrollBars as necessary to scroll the visible area
of the framed components.  When the area framed by the
ScrolledWindow is completely displayed, the ScrollBars
need not be displayed.
.mS
When the area provided in
the ScrolledWindow is too small to display the entire
component area, the ScrollBars must be displayed and
must then allow scrolling of the visible area.
.mE
.hS "Illustration"
.br
.ne 1.75i
\0
.sp 1.55i
.in +1.1i
.P! graphics/scrolledw.ps
.in -1.1i
.sp
.hS "Navigation"
.mS
A ScrolledWindow must follow the navigation model described in
Chapter 2.
.mE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBKPageDown\fP:"
.br
.mS
Must scroll the window down one page.
.mE
.LI "\fBKPageLeft\fP:"
.br
.mS
Must scroll the window left one page.
.mE
.LI "\fBKPageRight\fP:"
.br
.mS
Must scroll the window right one page.
.mE
.LI "\fBKPageUp\fP:"
.mS
Must scroll the window up one page.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
KPageUp@<PageUp>
KPageDown@<PageDown>
KPageLeft@MCtrl <PageUp>
KPageRight@MCtrl <PageDown>
.TE
.hS "Related Information"
For information about the activation of the ScrollBars,
see the reference page for ScrollBar.
For more information about navigation
within general scrollable components,
see Section 2.3.2.4.
