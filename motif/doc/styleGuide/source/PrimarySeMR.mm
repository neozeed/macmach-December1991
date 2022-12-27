.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Primary Selection"
.hS "Description"
.iX "primary selection"
.iX "primary transfer"
.iX "selection" "primary"
Each collection maintains its own selection.
.mS
However, there must only be a single primary selection.
.mE
The primary selection is the selection on which selection
actions, such as cut and copy, are performed.
There are two models for how the primary selection moves
among collections:
.BL
.LI
.mS
Focus-based primary selection,
where the primary selection must move to a collection
whenever the collection gets the input focus.
.mE
.LI
.mS
Selection-based primary selection,
where the primary selection must move to an editable
collection whenever a nonnull selection is explicitly started
in that collection.
.mE
.LE
.P
The following primary selection actions, known as the primary
transfer actions, should be made available.
.VL 18n
.LI "\fBBPrimaryPaste Click\fP"
.iX "PrimaryPaste"
.iX "pasting"
.br
.mS
Must paste the primary selection to the position of the pointer
and set the location cursor there.
.mE
Whether the paste action
cuts or copies the primary selection to the new location depends
on the type of elements pasted, and the source and destination
of the primary selection as described in Section 3.2.2.
.LI "\fBBPrimaryCopy Click\fP"
.iX "PrimaryCopy"
.iX "copying"
.iX "selection" "primary"
.iX "primary selections"
.br
.mS
Must copy the primary selection to the position of the pointer
and set the location cursor there.
.mE
.LI "\fBBPrimaryCut Click\fP"
.iX "PrimaryCut"
.iX "cutting"
.mS
Must cut the primary selection to the position of the pointer
and set the location cursor there.
.mE
.LI "\fBKPrimaryPaste\fP"
.iX "PrimaryPaste"
.iX "pasting"
.iX "selection" "primary"
.iX "primary selections"
.mS
Must paste the primary selection to the position of the location cursor.
.mE
Whether the paste action
cuts or copies the primary selection to the new location depends
on the type of elements pasted, and the source and destination
of the primary selection as described in Section 3.2.2.
.LI "\fBKPrimaryCopy\fP"
.iX "KPrimaryCopy"
.iX "copying"
.mS
Must copy the primary selection to the position of the location
cursor.
.mE
.LI "\fBKPrimaryCut\fP"
.iX "KPrimaryCut"
.iX "cutting"
.mS
Must cut the primary selection to the position of the location
cursor.
.mE
.LI "\fBKReselect\fP"
.iX "KReselect"
.iX "selection" "primary"
.iX "primary selections"
Can optionally restore the collection's selection as the primary selection.
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
BPrimaryPaste@BDrag
BPrimaryCopy@MCtrl BPrimaryPaste
BPrimaryCut@MAlt BPrimaryPaste
KPrimaryPaste@<vendor defined>
KPrimaryCopy@MCtrl KPrimaryPaste
@MAlt KCopy
KPrimaryCut@MAlt KPrimaryPaste
@MAlt KCut
KReselect@MCtrl MShift KSelect
.TE
.hS "Related Information"
See Section 3.2.2 for more information about the primary selection.
