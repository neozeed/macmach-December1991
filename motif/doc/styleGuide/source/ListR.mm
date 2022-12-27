.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR List
.hS "Description"
.iX "List"
A List should be used to present a list of elements
for selection.  The List elements can be selected using
either the single selection model, the browse selection model,
the multiple section model, or the discontiguous selection model.
.mS
This component must be composed of an area for presenting a list
of text or graphics elements.
.mE
It can optionally have vertical
and horizontal ScrollBars, which show different views of the
List elements.
.P
The location cursor in a List should differentiate between normal mode
and add mode.  The location cursor should be a solid box in normal
mode and a dashed box in add mode.
.hS "Illustration"
.br
.ne 2.25i
\0
.sp 1.85i
.in +1.45i
.P! graphics/item.cursor.ps
.in -1.45i
.sp
.hS "Navigation"
The following text describes the navigation actions
of this component.
.VL 10n
.LI "\fBKBeginData\fP:"
.br
.mS
Must move the location cursor to the first item in the List.
.mE
.LI "\fBKEndData\fP:"
.br
.mS
Must move the location cursor to the last item in the List.
.mE
.LI "\fBKUp\fP:"
.mS
Must move the location cursor to the previous item in the List.
.mE
.LI "\fBKDown\fP:"
.mS
Must move the location cursor to the next item in the List.
.mE
.LE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect Click 2\fP:"
.br
.mS
Must cause any default action for the List using the item
that was double-clicked as the selection.
.mE
.LI "\fBKBeginLine\fP:"
.br
.mS
Must move the horizontal scroll region so that the leftmost
edge of the List is visible.
.mE
.LI "\fBKEndLine\fP:"
.br
.mS
Must move the horizontal scroll region so that the rightmost
edge of the List is visible.
.mE
.LI "\fBKLeft\fP:"
.mS
Must scroll the List one character to the left.
.mE
.LI "\fBKPageLeft\fP:"
.br
.mS
Must scroll the List one page to the left.
.mE
.LI "\fBKPageDown\fP:"
.br
.mS
Must scroll the List to the next page.
.mE
.LI "\fBKPageUp\fP:"
.mS
Must scroll the List to the previous page.
.mE
.LI "\fBKRight\fP:"
.mS
Must scroll the List one character to the right.
.mE
.LI "\fBKPageRight\fP:"
.br
.mS
Must scroll the List one page to the right.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the List.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
BSelect@<Btn1>
KUp@<\(ua>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KPageUp@<PageUp>
KPageDown@<PageDown>
KPageLeft@MCtrl <PageUp>
KPageRight@MCtrl <PageDown>
KBeginLine@<Begin>
@<Home>
KEndLine@<End>
KBeginData@MCtrl <Begin>
@MCtrl <Home>
KEndData@MCtrl <End>
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information on List element selection,
see the description of the selection models in Chapter 3.
For more information about the ScrollBars,
see the reference page for ScrollBars.
