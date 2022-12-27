.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Help Menu"
.hS "Description"
.iX "Help Menu"
.iX "menus" "help"
The common Help Menu contents are described as follows.
Note that you should only include those functions actually
supported by your application.
The illustration on this reference page shows a Help Menu.
.VL 1i
.LI "\fBOn\ \o'C_'ontext\fP"
.iX "In Context"
.mS
Must initiate context-sensitive help
by changing the shape of the pointer to the question pointer
described in Section 2.2.2.  When the user moves the pointer
to the component help is wanted on and presses
.BR BSelect ,
any available context-sensitive help for the component
must be presented, and the pointer reverts from the
question pointer.
This action must have the mnemonic
.BR C .
.mE
.LI "\fBOn\ \o'H_'elp\fP"
.iX "On Help"
.mS
Must provide information on how to use
the application's help facility.
.mE
.LI "\fBOn\ \o'W_'indow\fP"
.iX "On Window"
.mS
Must provide general information about the
window from which help was requested.
.mE
.LI "\fBOn\ \o'K_'eys\fP"
.iX "On Keys"
.mS
Must provide information about the application's
use of function keys, mnemonics, and keyboard accelerators.
.mE
.LI "\fB\o'I_'ndex\fP"
.iX "Index"
.mS
Must provide an index for all help information in
the application.
.mE
The index can provide search capabilities.
.LI "\fB\o'T_'utorial\fP"
"Tutorial"
.mS
Must provide access to the application's tutorial.
.mE
.LI "\fBOn\ \o'V_'ersion\fP"
.iX "On Version"
.mS
Must provide the name and version of the application.
.mE
It can provide other information as well.
.LE
.hS "Illustration"
.br
.ne 3.5i
\0
.sp 3.3i
.in +.9i
.P! graphics/helpmenu.ps 
.in -.9i
.sp
.hS "Related Information"
See Chapter 4 for more information on the MenuBar system,
and for general information about Menu design.
