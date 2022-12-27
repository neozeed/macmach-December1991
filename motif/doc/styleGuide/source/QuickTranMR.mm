.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Quick Transfer"
.hS "Description"
.iX "quick selection"
.iX "quick transfer"
.iX "selections" "quick"
Quick transfer is used to make a selection, and then
immediately cut or copy that selection to the
destination cursor.
In quick transfer, the user picks the destination first,
then makes a selection to be pasted there.  This allows
multiple selections to be quickly pasted at the same location.
.P
.mS
The destination cursor must be in the component in which
the user last did a selection, insertion, or editing
operation.
.mE
It can either be at the location cursor of that
component, or determined independently.
.P
.mS
If the destination cursor is determined independently, it
must be at the location where selection, insertion or editing
was last done.  Furthermore, when the current selection is null,
the anchor must be kept at the destination cursor as well.
.mE
The destination cursor should be determined independently in
environments in which strong support of keyboard use is expected.
.P
The following quick-transfer actions should be made available.
.VL 18n
.LI "\fBBQuickPaste Drag\fP"
.iX "BQuickPaste"
.iX "pasting"
.mS
Must select a region for quick transfer, and paste the
region at the destination cursor when the button is released.
.mE
Whether the paste action
cuts or copies the selection to the new location depends
on the type of elements pasted, and the source and destination
of the selection as in primary selection described in Section 3.2.2.
.LI "\fBBQuickCopy Drag\fP"
.iX "BQuickCopy"
.iX "copying"
.mS
Must copy the selection to the position of the pointer.
.mE
.LI "\fBBQuickCut Drag\fP"
.iX "BQuickCutDrag"
.iX "cutting"
.iX "quick transfer"
.iX "quick selection"
.iX "selections" "quick"
.mS
Must cut the selection to the position of the pointer.
.mE
.LI "\fBKQuickPaste\fP"
.iX "KQuickPaste"
.iX "pasting"
.mS
Must set a quick anchor point to start a quick-paste operation.
.mE
Whether the paste action
cuts or copies the selection to the new location depends
on the type of elements pasted, and the source and destination
of the selection as in primary selection described in Section 3.2.2.
.LI "\fBKQuickCopy\fP"
.iX "QuickCopy"
.iX "copying"
.mS
Must set a quick anchor point to start a quick-copy operation.
.mE
.LI "\fBKQuickCut\fP"
.iX "QuickCut"
.iX "cutting"
.mS
Must set a quick anchor point to start a quick-cut operation.
.mE
.LI "\fBKQuickExtend\fP"
.iX "QuickExtend"
.mS
Must perform the quick operation, transferring the elements between the quick
anchor point and the location cursor.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
BQuickPaste@BDrag
BQuickCopy@MCtrl BQuickPaste
BQuickCut@MAlt BQuickPaste
KQuickPaste@<vendor defined>
KQuickCopy@MCtrl KQuickPaste
KQuickCut@MAlt KQuickPaste
KQuickExtend@MShift KQuickPaste
.TE
.hS "Related Information"
See Section 3.2.3 for more information about quick transfer.
