.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Edit Menu"
.hS "Description"
.iX "edit menu"
.iX "menus" "edit"
The common Edit Menu contents are described as follows.
Note that you should only include those functions actually
supported by your application.
The illustration on this reference page shows an Edit Menu.
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
.hS "Illustration"
.br
.ne 3.5i
\0
.sp 3.2i
.in +.9i
.P! graphics/editmenu.ps 
.in -.9i
.sp
.hS "Related Information"
See Chapter 4 for more information on the MenuBar system,
and for general information about Menu design.
