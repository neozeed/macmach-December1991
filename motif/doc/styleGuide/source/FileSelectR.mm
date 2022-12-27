.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR FileSelectionBox
.hS "Description"
.iX "FileSelectionBox"
A FileSelectionBox
is a special purpose composite component for file selection.
It can be used to traverse through directories, view the
files and subdirectories in them, and then select files.
.mS
The FileSelectionBox must be composed of the following elements:
.BL
.LI
A Text component for displaying and editing a directory mask
used to select the files to be displayed.
The directory mask must be a string specifying the base directory to be
examined and a search pattern.
.LI
A List component for displaying file names.
The file list should display all files and subdirectories in the
base directory that match the search pattern.
.LI
A List component for displaying subdirectories.
The directory list should display the subdirectories of the base
directory, as well as the base directory itself and its parent
directory.
.LI
A Text component for displaying and editing a filename.
.LI
A group of PushButtons, 
labeled \fBOK\fP, \fBFilter\fP, \fBCancel\fP, and \fBHelp\fP 
.LE
.mE
.P
.mS
The user must be able to select a new directory to examine by
scrolling through the list of directories and selecting
the desired directory or by editing the directory mask.
Selecting a new directory from the directory list must not change the
search pattern.
A user must be able to select a new search pattern by editing
the directory mask.
Double clicking
.B BSelect
or pressing 
.B KActivate
on a directory in the
directory list must initiate a search for files
and subdirectories in the new directory,
using the current search pattern.
.mE
.P
.mS
The user must be able to select a file by scrolling through
the list of filenames and selecting the desired file or
by entering the filename directly into the Text component.
.mE
Selecting a file from the list causes that filename to appear in the
file selection Text component.
.P
The user may select a new file as many times as desired.
.mS
The application must not be notified until one of the following
events occur:
.BL
.LI
The \fBOK\fP PushButton is activated.
.LI
\fBKActivate\fP 
is pressed while the Text component has the
keyboard focus.
.LI
.B KActivate
is pressed while the location cursor is on an
item in the file list.
.LI
.B BSelect
is double clicked on an item in the file list.
.LE
.mE
.P
.mS
The FileSelectionBox must initiate a directory and file
search when any of the following occurs:
.BL
.LI
The FileSelectionBox is initialized
.LI
The contents of the directory mask is changed
.LI
The user activates the \fBFilter\fP PushButton
.LI
The user double clicks or presses \fBKActivate\fP on an item in the
directory list
.LI
The user presses \fBKActivate\fP while the directory mask text edit area
has the keyboard focus
.LE
.mE
.br
.ne 5.8i
.hS "Illustration"
.br
.ne 5.5i
\0
.sp 5.3i
.in +1.1i
.P! graphics/fileselbox.ps
.in -1.1i
.hS "Related Information"
For more information,
see the reference page for FileSelection DialogBox.
