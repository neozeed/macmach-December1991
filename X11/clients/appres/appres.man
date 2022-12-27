.TH APPRES 1 "Release 4" "X Version 11"
.SH NAME
appres - list application resource database
.SH SYNOPSIS
.B "appres"
[[classname [instancename]] [-xrm resource ...] 
.SH DESCRIPTION
.PP
The \fIappres\fP program prints the resources seen by an application of the
specified \fIclass\fP and \fIinstance\fP name.  It is used to determine which
resources a particular program would load.  For example,
.sp 1
.in +.5i
.nf
%  appres XTerm
.fi
.in -.5i
.sp 1
would list the resources that any \fIxterm\fP program would load.  To also
match particular instance names, 
.sp 1
.in +.5i
.nf
%  appres myxterm XTerm
.fi
.in -.5i
.sp 1
If no application
class is specified, the class \fI-NoSuchClass-\fP (which should have no
defaults) is used.
.SH "SEE ALSO"
.PP
X(1), xrdb(1), listres(1)
.SH COPYRIGHT
Copyright 1989, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Jim Fulton, MIT X Consortium
