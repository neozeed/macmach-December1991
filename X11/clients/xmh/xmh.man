.\" $XConsortium: xmh.man,v 1.19 89/12/22 22:08:40 converse Exp $
.TH XMH 1 "Release 4" "X Version 11"
.SH NAME
\fIxmh\fR \- X interface to the MH message handling system
.SH SYNOPSIS
.B xmh
[-path \fImailpath\fR] [-initial \fIfoldername\fR] [-flag] [-\fItoolkitoption\fR ...]
.SH DESCRIPTION
The
.I xmh
program provides a window-oriented user interface to the Rand \fIMH\fR Message
Handling System.  To actually do things with your mail, it makes calls to the
\fIMH\fR package.  Electronic mail messages may be composed, sent, 
received, replied to, forwarded, sorted, and stored in folders. 

To specify an alternate collection of mail folders in which to process
mail, use \fB-path\fR followed by the pathname of the alternate mail directory.
The default mail path is the value of the Path component in 
$HOME/.mh_profile, or $HOME/Mail if the \fIMH\fR Path is not given.  
To specify an alternate folder which may receive new mail and is initially 
opened by \fIxmh\fR, use the \fB-initial\fR flag.  
The default initial folder is `inbox'.  
The option \fB-flag\fR will cause \fIxmh\fR to attempt to
change the appearance of its icon when new mail has arrived.  
These three options have corresponding application-specific resources, 
named \fBMailPath\fR, \fBInitialFolder\fR, and \fBMailWaitingFlag\fR, which
can be used in a resource file.  
The standard toolkit command line options are given in \fIX(1)\fP.

Please don't be misled by the size of this document.  It introduces 
many aspects of the Athena Widget Set, and provides extensive mechanism
for customization of the user interface.  \fIxmh\fR really is easy to use.

.SH INSTALLATION
The current version of \fIxmh\fR requires that the user is already set up
to use \fIMH\fR, version 6.  To do so, see if there is a file
called .mh_profile in your home directory.  If it exists, check to see if it
contains a line that starts with ``Current-Folder''.  If it does, 
you've been using version 4 or earlier of \fIMH\fR; to convert to version
6, you must remove that line.  (Failure to do so causes spurious output to
stderr, which can hang \fIxmh\fR depending on your setup.)

If you do not already have a .mh_profile, you can create one (and
everything else you need) by typing ``inc'' to the shell.  You should
do this before using \fIxmh\fR to incorporate new mail.

For more information, refer to the \fImh(1)\fR documentation.

.SH BASIC SCREEN LAYOUT
\fIxmh\fR starts out with a single window, divided into four main areas:

.TP 4
.B \-
Six buttons with pull-down command menus.
.PP
.TP 4
.B \-
A collection of buttons, one for each top level folder. 
New users of mh will have two folders, ``drafts'' and ``inbox''.
.PP
.TP 4
.B \-
A listing, or Table of Contents, of the messages in the open folder.
Initially, this will show the messages in ``inbox''.
.PP
.TP 4
.B \-
A view of one of your messages.  Initially this is blank.

.SH XMH AND THE ATHENA WIDGET SET
\fIxmh\fR uses the X Toolkit Intrinsics and the Athena Widget Set.
Many of the features described below (scrollbars, buttonboxes, etc.) are
actually part of the Athena Widget Set, and are described here only for
completeness.  For more information, see the Athena Widget Set documentation.

.SS SCROLLBARS
Some parts of the main window will have a vertical area on the left containing
a grey bar.  This area is a \fIscrollbar\fR.  They are used whenever the
data in a window takes up more space than can be displayed.
The grey bar indicates what portion of your data is visible. Thus, if the
entire length of the area is grey, then you are looking at all your data.
If only the first half is grey, then you are looking at the top half of
your data.  
The message viewing area will have a horizontal scrollbar if the text
of the message is wider than the viewing area.

You can use the pointer in the scrollbar to change what part of the data is
visible.  If you click with the middle button, then the top of the grey
area will move to where the pointer is, and the corresponding
portion of data will be displayed.  If you hold down the middle button,
you can drag around the grey area.  This makes it easy to get to the top
of the data: just press with the middle, drag off the top of the
scrollbar, and release.

If you click with button 1, then the data to the right of the
pointer will scroll to the top of the window.  If you click with pointer
button 3, then the data at the top of the window will scroll down to where
the pointer is.

.SS BUTTONBOXES, BUTTONS, AND MENUS
Any area containing many words or short phrases, each enclosed in a
rectangle or rounded boundary, is called a \fIbuttonbox\fR.  
Each rectangle or rounded area is actually a button that you
can press by moving the pointer onto it and pressing pointer button 1.
If a given buttonbox has more buttons in it than can fit, it will
be displayed with a scrollbar, so you can always scroll to the button you
want.

Some buttons have pull-down menus. 
Pressing the pointer button while the pointer is over one of these 
buttons will pull down a menu.  Holding the button down while moving the 
pointer over the menu, called dragging the pointer, will highlight each
selectable item 
on the menu as the pointer passes over it.  To select an item in the menu,
release the pointer button while the item is highlighted. 

.SS ADJUSTING THE RELATIVE SIZES OF AREAS
If you're not satisfied with the sizes of the various areas of the main window,
they can easily be changed.  Near the right edge of the border between
each region is a black box, called a \fIgrip\fR.  Simply point to that
grip with the pointer, press a pointer button, drag up or down, and
release.  Exactly what happens depends on which pointer button you press.

If you drag with the middle button, then only that border will move.  This
mode is simplest to understand, but is the least useful.

If you drag with pointer button 1, then you are adjusting the size of
the window above.  \fIxmh\fR will attempt to compensate by adjusting some
window below it.

If you drag with pointer button 3, then you are adjusting the size
of the window below.  \fIxmh\fR will attempt to compensate by adjusting
some window above it.

All windows have a minimum and maximum size; you will never be allowed to
move a border past the point where it would make a window have an invalid
size.

.SH PROCESSING YOUR MAIL
This section will define the concepts of the selected folder, current folder,
selected message(s), current message, selected sequence, and current 
sequence.  Each \fIxmh\fR command is introduced.  

For use in customization,
action procedures corresponding to each command are given; these action
procedures can be used to customize the user interface, particularly the
keyboard accelerators and the functionality of the buttons in the optional
button box created by the application resource \fBCommandButtonCount\fR.

.SS SELECTED FOLDER
A folder contains a collection of mail messages, or is empty.

The selected folder is whichever foldername appears in the bar above the 
folder buttons.  Note that this is not necessarily the same folder that is
being viewed.  
To change the selected folder, just press on the desired folder button;
if that folder has subfolders, select a folder from the pull down menu.

The Table of Contents, or toc, lists the messages in the viewed folder.
The title bar above the Table of Contents displays the name of the 
viewed folder.

The toc title bar also displays the name of the viewed sequence of messages 
within the viewed folder.
Every folder has an ``all'' sequence, which contains all the messages
in the folder, and initially the toc title bar will show ``inbox:all''.

.SS FOLDER COMMANDS
The \fIfolder\fR command menu contains commands of a global nature:

.TP 8
.B Open Folder
Display the data in the selected folder.  Thus, the selected folder also
becomes the viewed folder.  
The action procedure corresponding
to this command is \fBXmhOpenFolder(\fR[\fIfoldername\fR]\fB)\fR.
It takes an optional argument as the name of a folder to select and open; if no
folder is specified, the selected folder is opened.  It may be specified 
as part of an event translation from a folder menu button or from a 
folder menu, or as a binding of a keyboard accelerator to any widget other
than the folder menu buttons or the folder menus.
.TP 8
.B Open Folder in New Window
Displays the selected folder in an additional main window.
Note, however, that you may not reliably display the same folder in more 
than one window at a time, although \fIxmh\fR will not prevent you from trying.
The corresponding action is \fBXmhOpenFolderInNewWindow()\fR.
.TP 8
.B Create Folder
Create a new folder.
You will be prompted for a name for the new folder;
to enter the name, move the pointer to the blank box provided and type.
Subfolders are created by specifying the parent folder, a slash, and the
subfolder name.  For example, 
to create a folder named ``xmh'' which is a subfolder of an existing folder
named ``clients'', type ``clients/xmh''.
Click on the Okay button when finished, or just type Return;
click on Cancel to cancel this operation. 
The action corresponding to Create Folder is \fBXmhCreateFolder()\fR.
.PP
.TP 8
.B Delete Folder
Destroy the selected folder.  You will be asked to confirm this action (see
CONFIRMATION WINDOWS).  Destroying a folder will also destroy any subfolders
of that folder.  The corresponding action is \fBXmhDeleteFolder()\fP.
.PP
.TP 8
.B Close Window
Exits \fIxmh\fR, after first confirming that you won't lose any changes;
or, if selected from any additional \fIxmh\fP window, simply closes that
window.  The corresponding action is \fBXmhClose()\fP.

.SS HIGHLIGHTED MESSAGES, SELECTED MESSAGES
.SS AND THE CURRENT MESSAGE
It is possible to highlight a set of adjacent messages in the area of the
Table of Contents.
To highlight a message, click on it with pointer button 1.
To highlight a range of messages, click on the first one with
pointer button 1 and on the last one with pointer button 3; or
press pointer button 1, drag, and release.
To extend a range of selected messages, use pointer button 3.  
To highlight all messages in the table of contents, 
click rapidly three times with pointer button 1.  
To cancel any selection in the table of contents, click rapidly twice.

The selected messages are the same as the highlighted messages, if any.  If no
messages are highlighted, then the selected messages are considered the same
as the current message.

The current message is indicated by a '+' next to the message number.  It
usually corresponds to the message currently being viewed. 
When a message is viewed, the title bar above the view will identify the message.

.SS TABLE OF CONTENTS COMMANDS
The \fITable of Contents\fP command menu
contains commands which operate on the open, or viewed folder.

.TP 18
.B Incorporate New Mail
Add any new mail received to your inbox folder, and set the current
message to be the first new message.  (This command is selectable only if
``inbox'' is the folder being viewed.)  The corresponding action is 
\fBXmhIncorporateNewMail()\fP.
.TP 18
.B Commit Changes
Execute all deletions, moves, and copies that have been marked in this
folder.  The corresponding action is \fBXmhCommitChanges()\fP.
.TP 18
.B Pack Folder
Renumber the messages in this folder so they start with 1 and increment by
1.  The corresponding action is \fBXmhPackFolder()\fP.
.TP 18
.B Sort Folder
Sort the messages in this folder in chronological order.  As a side
effect, this also packs the folder.  The corresponding action is
\fBXmhSortFolder()\fP.
.TP 18
.B Rescan Folder
Rebuild the list of messages.  This can be used whenever you suspect 
that \fIxmh\fR's
idea of what messages you have is wrong.  (In particular, this is necessary
if you change things using straight \fIMH\fR commands without using 
\fIxmh\fR.)  The corresponding action is \fBXmhForceRescan()\fP.

.SS MESSAGE COMMANDS
The \fIMessage\fR command menu contains commands which operate on the selected
message(s), or if there are no selected messages, the current message.

.TP 18
.B Compose Message
Composes a new message.  A new window will be brought up for composition;
a description of it is given in the COMPOSITION WINDOWS section below.
This command does not affect the current message.
The corresponding action is \fBXmhComposeMessage()\fP.
.PP
.TP 18
.B View Next Message
View the first selected message.  If no messages are highlighted, view the
current message.  If current message is already being viewed, view the
first unmarked message after the current message.
The corresponding action is \fBXmhViewNextMessage()\fP.
.PP
.TP 18
.B View Previous
View the last selected message.  If no messages are highlighted, view the
current message.  If current message is already being viewed, view the
first unmarked message before the current message.
The corresponding action is \fBXmhViewPrevious()\fP.
.PP
.TP 18
.B Mark Deleted
Mark the selected messages for deletion.  If no messages are highlighted, then
this mark the current message for deletion and automatically display the 
next unmarked message.
The corresponding action is \fBXmhMarkDeleted()\fP.
.PP
.TP 18
.B Mark Move
Mark the selected messages to be moved into the current (selected) folder.
(If the current folder is the same as the viewed folder, 
this command will just beep.)  If no messages are highlighted,
this will mark the current message to be moved and display the next 
unmarked message.
The corresponding action is \fBXmhMarkMove()\fP.
.PP
.TP 18
.B Mark Copy
Mark the selected messages to be copied into the current folder.  (If the
current folder is the same as the viewed folder, this command will just
beep.)  If no messages are highlighted, mark the current message to be
copied.
The corresponding action is \fBXmhMarkCopy()\fP.
.PP
.TP 18
.B Unmark
Remove any of the above three marks from the selected messages, or the
current message, if none are highlighted.
The corresponding action is \fBXmhUnmark()\fP.
.PP
.TP 18
.B View in New Window
Create a new window containing only a view of the first selected message,
or the current message, if none are highlighted.
The corresponding action is \fBXmhViewInNewWindow()\fP.
.PP
.TP 18
.B Reply
Create a composition window in reply to the first selected message, or the
current message, if none are highlighted.
The corresponding action is \fBXmhReply()\fP.
.PP
.TP 18
.B Forward
Create a composition window whose body is initialized to be the contents
of the selected messages, or the current message if none are highlighted.
The corresponding action is \fBXmhForward()\fP.
.PP
.TP 18
.B Use as Composition
Create a composition window whose body is initialized to be the contents
of the first selected message, or the current message if none are selected.
Any changes you make in the composition will be saved in a new 
message in the ``drafts'' folder, and will not change the original message.
However, this command was designed to be used within the ``drafts'' folder 
to compose message drafts, and there is an exception to this rule.
If the message to be used as composition was selected from 
the ``drafts'' folder, the changes will be reflected in the original message
(see COMPOSITION WINDOWS).  The action procedure corresponding to this
command is \fBXmhUseAsComposition()\fR.
.PP
.TP 18
.B Print
Print the selected messages, or the current message if none are selected.
\fIxmh\fR normally prints by invoking
the \fIenscript\fR(1) command, but this can be customized with the 
application-specific resource \fBPrintCommand\fR.
The action procedure corresponding to this command is \fBXmhPrint()\fR.

.SS SEQUENCE COMMANDS
The \fISequence\fR command menu contains commands pertaining to
message sequences (See MESSAGE-SEQUENCES),
and a list of the message-sequences defined for the currently viewed folder.
The selected message-sequence is indicated by a check mark in its entry
in the margin of the menu.  To change the selected message-sequence,
select a new message-sequence from the sequence menu.  

.TP 18
.B Pick Messages
Define a new message-sequence.  
The corresponding action is \fBXmhPickMessages()\fP.
.PP
The following menu entries will be sensitive only if the current folder
has any message-sequences other than the ``all'' message-sequence.  
.TP 18
.B Open Sequence
Change the viewed sequence to be the same as the selected sequence.
The corresponding action is \fBXmhOpenSequence()\fP.
.PP
.TP 18
.B Add to Sequence
Add the selected messages to the selected sequence.
The corresponding action is \fBXmhAddToSequence()\fP.
.PP
.TP 18
.B Remove from Sequence
Remove the selected messages from the selected sequence.
The corresponding action is \fBXmhRemoveFromSequence()\fP.
.PP
.TP 18
.B Delete Sequence
Remove the selected sequence entirely.  The messages themselves are
not affected; they simply are no longer grouped together to define a
message-sequence.  The corresponding action is \fBXmhDeleteSequence()\fP.

.SS VIEW COMMANDS
Commands in the View menu and in the buttonboxes of 
view windows (which result from the Message command ``View In New'')
correspond in functionality to commands of the same
name in the Message menu, but they operate on the viewed message 
rather than the selected messages or current message.

.TP 18
.B Close Window
When the viewed message is in a separate view window, this command will
close the view, after confirming the status of any unsaved edits.
The corresponding action procedure is \fBXmhCloseView()\fR.
.TP 18
.B Reply
Create a composition window in reply to the viewed message.
The related action procedure is \fBXmhViewReply()\fR.
.TP 18
.B Forward
Create a composition window whose body is initialized to be the contents of
the viewed message.  The corresponding action is \fBXmhViewForward()\fR.
.TP 18
.B Use As Composition
Create a composition window whose body is initialized to be the contents of
the viewed message.  Any changes made in the composition window will be
saved in a new message in the ``drafts'' folder, and will not change the
original message.  An exception: if the viewed message was selected from
the ``drafts'' folder, the original message is edited.  The action 
procedure corresponding to this command is \fBXmhViewUseAsComposition()\fR.
.TP 18
.B Edit Message
This command enables the direct editing of the viewed message.
The action procedure is \fBXmhEditView()\fR.
.TP 18
.B Save Message
This command is insensitive until the message has been edited; when
activated, edits will be saved to the original message in the view.
The corresponding action is \fBXmhSaveView()\fR.
.TP 18
.B Print
Print the viewed message.  \fIxmh\fR prints by invoking
the \fIenscript\fR(1) command, but this can be customized with the 
application-specific resource \fBPrintCommand\fR.
The corresponding action procedure is \fBXmhPrintView()\fR.

.SH OPTIONS
The \fIOptions\fR menu contains one entry.

.TP
.B Read in Reverse
When selected, a check mark appears in the margin of this menu entry.
Read in Reverse will switch the meaning of the next and previous 
messages, and will increment in the opposite direction.  This is useful
if you want to read your messages in the order of most recent first.
The option acts as a toggle; select it from the menu a second time to
undo the effect.  The check mark appears when the option is selected.

.SH COMPOSITION WINDOWS
Aside from the normal text editing functions, there are six command
buttons associated with composition windows:
.TP 18
.B Close Window
Close this composition window.  If changes have been made since the
most recent Save or Send, you will be asked to confirm losing them.
The corresponding action is \fBXmhCloseView()\fP.
.PP
.TP 18
.B Send
Send this composition.  The corresponding action is \fBXmhSend()\fP.
.PP
.TP 18
.B New Headers
Replace the current composition with an empty message.  If changes have
been made since the most recent Send or Save, you will be
asked to confirm losing them. 
The corresponding action is \fBXmhResetCompose()\fP.
.PP
.TP 18
.B Compose Message
Bring up another new composition window.  The corresponding action 
is \fBXmhComposeMessage()\fP.
.PP
.TP 18
.B Save Message
Save this composition in your drafts folder.  Then you can safely close the
composition.  At some future date, you can continue working on the
composition by opening the drafts folder, selecting the message, and
using the ``Use as Composition'' command.  
The corresponding action is \fBXmhSave()\fP.
.PP
.TP 18
.B Insert
Insert a related message into the composition.  If the composition window
was created with a ``Reply'' command, the related message is the message
being replied to, otherwise no related message is defined and this button
is insensitive.  The message may be filtered before being inserted;
see \fBReplyInsertFilter\fP under APPLICATION RESOURCES for more information.
The corresponding action is \fBXmhInsert()\fP.

.SH ACCELERATORS
Accelerators are shortcuts.  They allow you to invoke commands
without using the menus, either from the keyboard or by using the pointer.
.PP
\fIxmh\fP defines pointer accelerators for common actions:
To select and view a message with a single click, use pointer button
2 on the message's entry in the table of contents.  To select and open
a folder or a sequence in a single action, make the folder or sequence
selection with pointer button 2.

To mark the highlighted messages to be moved in a single action,
or current message if none have been highlighted,
use pointer button 3 to select the target folder.
Similarly, selecting a sequence with pointer button 3 will add 
the highlighted or current message(s) to that sequence.
In both of these operations, the selected folder or sequence 
and the viewed folder or sequence are not changed.

\fIxmh\fP defines the following keyboard accelerators over the surface of
the main window, except in the view area while editing a message:
.nf
	Meta-I		Incorporate New Mail
	Meta-C		Commit Changes
	Meta-R		Rescan Folder
	Meta-P		Pack Folder
	Meta-S		Sort Folder

	Meta-space	View Next Message
	Meta-c		Mark Copy
	Meta-d		Mark Deleted
	Meta-f		Forward the selected or current message
	Meta-m		Mark Move
	Meta-n		View Next Message
	Meta-p		View Previous Message
	Meta-r		Reply to the selected or current message
	Meta-u		Unmark

	Ctrl-V		Scroll the table of contents forward
	Meta-V		Scroll the table of contents backward
	Ctrl-v		Scroll the view forward
	Meta-v		Scroll the view backward
.fi

.SH TEXT EDITING COMMANDS
All of the text editing commands are actually defined by the Text widget
in the Athena Widget Set.
The commands may be bound to different keys than the defaults
described below through the X Toolkit Intrinsics key re-binding mechanisms.
See the X Toolkit Intrinsics and the Athena Widget Set documentation for 
more details.

Whenever you are asked to enter any text, you will be using a standard
text editing interface.  Various control and meta keystroke combinations
are bound to a somewhat Emacs-like set of commands.  In addition, the
pointer buttons may be used to select a portion of text or to move the
insertion point in the text. Pressing pointer button 1 causes the
insertion point to move to the pointer.  Double-clicking
button 1 selects a word, triple-clicking selects a line, quadruple-clicking
selects a paragraph, and clicking rapidly five times selects
everything.  Any selection may be extended in
either direction by using pointer button 3.

In the following, a \fIline\fR refers to one displayed row of characters
in the window.  A \fIparagraph\fR refers to the text between carriage
returns.  Text within a paragraph is broken into lines for display based on the
current width of the window.
When a message is sent, text is broken into lines based upon the values
of the \fBSendBreakWidth\fP and \fBSendWidth\fP application-specific 
resources.

The following keystroke combinations are defined:
.sp
.nf
.ta 1.0i 3.0i 4.5i
Ctrl-a	Beginning Of Line	Meta-b	Backward Word
Ctrl-b	Backward Character	Meta-f	Forward Word
Ctrl-d	Delete Next Character	Meta-i	Insert File
Ctrl-e	End Of Line	Meta-k	Kill To End Of Paragraph
Ctrl-f	Forward Character	Meta-q	Form Paragraph
Ctrl-g	Multiply Reset	Meta-v	Previous Page
Ctrl-h	Delete Previous Character	Meta-y	Insert Current Selection
Ctrl-j	Newline And Indent	Meta-z	Scroll One Line Down
Ctrl-k	Kill To End Of Line	Meta-d	Delete Next Word
Ctrl-l	Redraw Display	Meta-D	Kill Word
Ctrl-m	Newline	Meta-h	Delete Previous Word
Ctrl-n	Next Line	Meta-H	Backward Kill Word
Ctrl-o	Newline And Backup	Meta-<	Beginning Of File
Ctrl-p	Previous Line	Meta->	End Of File
Ctrl-r	Search/Replace Backward	Meta-]	Forward Paragraph
Ctrl-s	Search/Replace Forward	Meta-[	Backward Paragraph
Ctrl-t	Transpose Characters
Ctrl-u	Multiply by 4	Meta-Delete	Delete Previous Word
Ctrl-v	Next Page	Meta-Shift Delete	Kill Previous Word
Ctrl-w	Kill Selection	Meta-Backspace	Delete Previous Word
Ctrl-y	Unkill	Meta-Shift Backspace	Kill Previous Word
Ctrl-z	Scroll One Line Up
.sp
In addition, the pointer may be used to cut and paste text:
.ta .5i 2.0i
	Button 1 Down	Start Selection
	Button 1 Motion	Adjust Selection
	Button 1 Up	End Selection (cut)

	Button 2 Down	Insert Current Selection (paste)

	Button 3 Down	Extend Current Selection
	Button 3 Motion	Adjust Selection
	Button 3 Up	End Selection (cut)
.fi
.sp
.SH CONFIRMATION DIALOG BOXES
Whenever you press a button that may cause you to lose some work or is
otherwise dangerous, a popup dialog box will appear asking you to confirm the
action.  This window will contain an ``Abort'' or ``No'' button and a
``Confirm'' or ``Yes''
button.  Pressing the ``No'' button cancels the operation, and pressing
the ``Yes'' will proceed with the operation. 

Some dialog boxes contain messages from \fIMH\fR.  Clicking on the
message field will cause the dialog box to resize so that you can read
the entire message.

.SH MESSAGE-SEQUENCES
An \fIMH\fP message sequence is just a set of messages associated with some name.
They are local to a particular folder; two different folders can have
sequences with the same name.  In all folders, the sequence ``all'' is
predefined; it consists of the set of all messages in that folder.  As
many as nine sequences may be defined for each folder, including 
the predefined ``all'' sequence.  (The
sequence ``cur'' is also usually defined for every folder; it consists of
only the current message.  \fIxmh\fR hides ``cur'' from the user, instead
placing a ``+'' by the current message.  Also, \fIxmh\fR does not support
the ``unseen'' sequence, so that one is also hidden from the user.)

The message sequences for a folder (including one for ``all'') are
displayed in the ``Sequence'' menu, below the sequence commands.
The table of contents (also known as the ``toc'') is at any one time
displaying one message sequence.  This is called the ``viewed sequence'',
and its name will be displayed in the toc title bar just after the
folder name.  Also, at any time one of the sequences in the menu will
have a check mark next to it.  This is called the ``selected sequence''.
Note that the viewed sequence and the selected sequence are not necessarily
the same.  (This all pretty much corresponds to the way the folders work.)

The \fBOpen Sequence\fR, \fBAdd to Sequence\fR, \fBRemove from Sequence\fR,
and \fBDelete Sequence\fR commands are active only if the viewed folder
contains message-sequences.
.PP
Note that none of the above actually affect whether a message is in the
folder.  Remember that a sequence is a set of messages within the folder;
the above operations just affect what messages are in that set.

To create a new sequence, select the ``Pick'' menu entry.  A new window will
appear, with lots of places to enter text. Basically, you can describe the
sequence's initial set of messages based on characteristics of the
message.  Thus, you can define a sequence to be all the messages that were
from a particular person, or with a particular subject, and so on.  You
can also connect things up with boolean operators, so you can select all
things from ``weissman'' with the subject ``xmh''.

Hopefully, the layout is fairly obvious.  The simplest cases are the
easiest: just point to the proper field and type.  If you enter in more
than one field, it will only select messages which match all non-empty
fields.

The more complicated cases arise when you want things that match one field
or another one, but not necessarily both.  That's what all the ``or''
buttons are for.  If you want all things with the subject ``xmh'' or
``xterm'', just press the ``or'' button next to the ``Subject:'' field.
Another box will appear where you can enter another subject.

If you want all things either from ``weissman'' or with subject ``xmh'', but
not necessarily both, select the ``-Or-'' button.  This will essentially
double the size of the form.  You can then enter ``weissman'' in a from: box
on the top half, and ``xmh'' in a subject: box on the lower part.

If you select the ``Skip'' button, then only those messages that
\fIdon't\fR match the fields on that row are included.

Finally, in the bottom part of the window will appear several more boxes.
One is the name of the sequence you're defining.  (It defaults to the name
of the selected sequence when ``Pick'' was pressed, or to ``temp'' if
``all'' was the selected sequence.)  Another box defines which sequence to
look through for potential members of this sequence; it defaults to the
viewed sequence when ``Pick'' was pressed.

Two more boxes define a date range; only messages within that date range
will be considered.  These dates must be entered in 822-style format: each
date is of the form ``dd mmm yy hh:mm:ss zzz'', where dd is a one or two
digit day of the month, mmm is the three-letter abbreviation for a month,
and yy is a year.  The remaining fields are optional: hh, mm, and ss
specify a time of day, and zzz selects a time zone.  Note that if the time
is left out, it defaults to midnight; thus if you select a range of ``7
nov 86'' - ``8 nov 86'', you will only get messages from the 7th, as all
messages on the 8th will have arrived after midnight.

``Date field'' specifies which date field in the header to look at for
this date range; it probably won't be useful to anyone.  If the sequence
you're defining already exists, you can optionally merge the old set with
the new; that's what the ``Yes'' and ``No'' buttons are all about.
Finally, you can ``OK'' the whole thing, or ``Cancel'' it.

In general, most people will rarely use these features.  However, it's
nice to occasionally use ``Pick'' to find some messages, look through
them, and then hit ``Delete Sequence'' to put things back in their original
state.

.SH WIDGET HIERARCHY
In order to specify resources, it is useful to know the hierarchy of 
widgets which compose \fIxmh\fR.  In the notation below, indentation
indicates hierarchical structure.  The widget class name is given first,
followed by the widget instance name.
The application class name is Xmh.
.PP
The hierarchy of the main toc and view window is identical for additional
toc and view windows, except that a topLevelShell widget is inserted
in the hierarchy between the application shell and the Paned widget.
.sp
.nf
.ta .5i 1.0i 1.5i 2.0i 2.5i 3.0i 3.5i 4.0i 4.5i 5.0i 5.5i 6.0i 6.5i 7.0i
Xmh xmh
	Paned xmh
		SimpleMenu  folderMenu
			SmeBSB  open
			SmeBSB  openInNew
			SmeBSB  create
			SmeBSB  delete
			SmeLine  line
			SmeBSB  close
		SimpleMenu  tocMenu
			SmeBSB  inc
			SmeBSB  commit
			SmeBSB  pack
			SmeBSB  sort
			SmeBSB  rescan
		SimpleMenu  messageMenu
			SmeBSB  compose
			SmeBSB  next
			SmeBSB  prev
			SmeBSB  delete
			SmeBSB  move
			SmeBSB  copy
			SmeBSB  unmark
			SmeBSB  viewNew
			SmeBSB  reply
			SmeBSB  forward
			SmeBSB  useAsComp
			SmeBSB  print
		SimpleMenu  sequenceMenu
			SmeBSB  pick
			SmeBSB  openSeq
			SmeBSB  addToSeq
			SmeBSB  removeFromSeq
			SmeBSB  deleteSeq
			SmeLine  line
			SmeBSB  all
		SimpleMenu  viewMenu
			SmeBSB  reply
			SmeBSB  forward
			SmeBSB  useAsComp
			SmeBSB  edit
			SmeBSB  save
			SmeBSB  print
		SimpleMenu  optionMenu
			SmeBSB  reverse
		Viewport.Core  menuBox.clip
			Box  menuBox
				MenuButton  folderButton
				MenuButton  tocButton
				MenuButton  messageButton
				MenuButton  sequenceButton
				MenuButton  viewButton
				MenuButton  optionButton
		Grip  grip
		Label folderTitlebar
		Grip  grip
		Viewport.Core  folders.clip
			Box  folders
				MenuButton  inbox
				MenuButton  drafts
					SimpleMenu  menu
						SmeBSB <folder_name>
							.
							.
							.

		Grip  grip
		Label  tocTitlebar
		Grip  grip
		Text toc
			Scrollbar  vScrollbar
		Grip  grip
		Label  viewTitlebar
		Grip  grip
		Text  view
			Scrollbar  vScrollbar
			Scrollbar  hScrollbar
.sp
\fIThe hierarchy of the Create Folder popup dialog box:\fR
.sp
	transientShell  prompt
		Dialog  dialog
			Label  label
			Text  value
			Command  okay
			Command  cancel
.sp
\fIThe hierarchy of the Notice dialog box, which reports messages from MH:\fR
.sp
	transientShell  notice
		Dialog  dialog
			Label  label
			Text  value
			Command  confirm
.sp
\fIThe hierarchy of the Confirmation dialog box:\fR
.sp
	transientShell  confirm
		Dialog  dialog
			Label  label
			Command  yes
			Command  no
.sp
\fIThe hierarchy of the dialog box which reports errors:\fR
.sp
	transientShell  error
		Dialog  dialog
			Label  label
			Command  OK
.sp
\fIThe hierarchy of the composition window:\fR
.sp
	topLevelShell  xmh
		Paned  xmh
			Label  composeTitlebar
			Text  comp
			Viewport.Core  compButtons.clip
				Box  compButtons
					Command  close
					Command  send
					Command  reset
					Command  compose
					Command  save
					Command  insert
.sp
\fIThe hierarchy of the view window:\fR
.sp
	topLevelShell  xmh
		Paned  xmh
			Label  viewTitlebar
			Text  view
			Viewport.Core  viewButtons.clip
				Box  viewButtons
					Command  close
					Command  reply
					Command  forward
					Command  useAsComp
					Command  edit
					Command  save
					Command  print
.sp
\fIThe hierarchy of the pick window:\fR
\fI(Unnamed widgets have no name.)\fR
.sp
	topLevelShell  xmh
		Paned  xmh
			Label  pickTitlebar
			Viewport.core  pick.clip
				Form  form
					Form
\fIThe first 6 rows of the pick window have identical structure:\fR
						Form
							Toggle
							Toggle
							Label
							Text
							Command

						Form
							Toggle
							Toggle
							Text
							Text
							Command
						Form
							Command
			Viewport.core  pick.clip
				Form  form
					From
						Form
							Label
							Text
							Label
							Text
						Form
							Label
							Text
							Label
							Text
							Label
							Text
						Form
							Label
							Toggle
							Toggle
						Form
							Command
							Command
						
.fi				
.SH APPLICATION-SPECIFIC RESOURCES

Resource instance names begin with a lower case letter but are otherwise
identical to the class name.
.PP
If TocGeometry, ViewGeometry, CompGeometry, or PickGeometry are not
specified, then the value of Geometry is used instead.  If the resulting
height is not specified (e.g., "", "=500", "+0-0"), then the default
height of windows is calculated from fonts and line counts. If
the width is not specified (e.g., "", "=x300", "-0+0), then half of the
display width is used.  If unspecified, the height of a pick window
defaults to half the height of the display.

Any of these options may also be specified on the command line by
using the X Toolkit Intrinsics resource specification mechanism.
Thus, to run \fIxmh\fR showing all message headers,

% xmh -xrm '*HideBoringHeaders:off'

The following resources are defined:
.TP 8
.B Banner
A short string that is the default label of the folder, Table of Contents,
and view.  The default is "xmh    MIT X Consortium    R4".
.PP
.TP 8
.B BlockEventsOnBusy
Whether to disallow user input and show a busy cursor while \fIxmh\fP is
busy processing a command.  Default is true.
.PP
.TP 8
.B BusyCursor
The name of the symbol used to represent the position of the pointer,
displayed if \fBBlockEventsOnBusy\fR is true, when \fIxmh\fR is
processing a time-consuming command.
The default is "watch".
.PP
.TP 8
.B BusyPointerColor
The foreground color of the busy cursor.  Default is XtDefaultForeground.
.PP
.TP 8
.B CheckFrequency
How often to check for new mail, make checkpoints, and rescan the Table
of Contents, in minutes.  If \fBCheckNewMail\fR is true, \fIxmh\fR checks
to see if you have new mail each interval.  If \fBMakeCheckpoints\fR is
true, checkpoints are made every fifth interval.  Also every fifth 
interval, the Table of Contents is checked for inconsistencies with the
file system, and rescanned.  To prevent all of these checks from occurring,
set \fBCheckFrequency\fR to 0.  The default is 1.
.PP
.TP 8
.B CheckNewMail
If true, \fIxmh\fP will check at regular intervals to see if new mail
has arrived for any of the folders.  A visual indication will be given
if new mail is waiting to be retrieved.  Default is True. (See BUGS).
The interval can be adjusted with the \fBCheckFrequency\fR.
.PP
.TP 8
.B CommandButtonCount
The number of command buttons to create in a button box in between the toc
and the view areas of the main window.  \fIxmh\fP will create these buttons
with the names \fIbutton1, button2\fP and so on, in a box with the name
\fIcommandBox\fR.  The user can specify labels and actions for the buttons
in a private resource file; see the section on Actions.  The default is 0.
.PP
.TP 8
.B CompGeometry
Initial geometry for windows containing compositions.
.PP
.TP 8
.B Cursor
The name of the symbol used to represent the pointer.  Default is ``left_ptr''.
.PP
.TP 8
.B DraftsFolder
The folder used for message drafts.  Default is ``drafts''.
.PP
.TP 8
.B Geometry
Default geometry to use.  Default is none.
.PP
.TP 8
.B HideBoringHeaders
If ``on'', then \fIxmh\fR will attempt to skip uninteresting header lines
within messages by scrolling them off.  Default is ``on''.
.PP
.TP 8
.B InitialFolder
Which folder to display on startup.  May also be set with the command-line
option \fB-initial\fR.  Default is ``inbox''.  
.PP
.TP 8
.B InitialIncFile
The file name of your incoming mail drop.  \fIxmh\fR tries to construct
a filename for the ``inc -file'' command, but in some installations
(e.g. those using the Post Office Protocol) no file is appropriate.
In this case, \fBInitialIncFile\fR should be specified as the empty string,
and \fIinc\fR will be invoked without a -file argument.  The default
is to use the value of the environment variable \fBMAIL\fR, or if that
is not set, to append the value of the environment variable \fBUSER\fR
to \fI/usr/spool/mail/\fR.
.PP
.TP 8
.B MailPath
The full path prefix for locating your mail folders.  May also be set
with the command-line option, \fB-path\fR.  The default is the
Path component in $HOME/.mh_profile, or ``$HOME/Mail'' if none.
.PP
.TP 8
.B MailWaitingFlag
If true, \fIxmh\fP will attempt to set an indication in its icon when
new mail is waiting to be retrieved.  If this option is true, then
CheckNewMail is assumed to be true as well.  The \fB-flag\fP command line
option is a quick way to turn MailWaitingFlag on.
.PP
.TP 8
.B MakeCheckpoints
If true, \fIxmh\fP will attempt to save checkpoints of volatile information.
The frequency of checkpointing is controlled by the resource 
\fBCheckFrequency\fR.
.PP
.TP 8
.B MhPath
What directory in which to find the \fIMH\fR commands.  If a command isn't found
here, then the directories in the user's path are searched.  Default is
``/usr/local/mh6''.
.PP
.TP 8
.B PickGeometry
Initial geometry for pick windows.
.PP
.TP 8
.B PointerColor
The foreground color of the pointer.  Default is XtDefaultForeground.
.PP
.TP 8
.B PrefixWmAndIconName
Whether to prefix the window and icon name with "xmh: ".  Default is true.
.PP
.TP 8
.B PrintCommand
What sh command to execute to print a message.  Note that stdout and stderr
must be specifically redirected!  If a message or range of messages is
selected for printing, the full file paths of each message file is
appended to the specified print command.  The default is ``enscript >/dev/null
2>/dev/null''.
.PP
.TP 8
.B ReplyInsertFilter
A shell command to be executed when the \fIInsert\fP button is activated
in a composition window.  The full path and filename of the source
message is added to the end of the command before being passed to \fIsh\fP(1).
The default filter is \fIcat\fP; i.e. it inserts the entire message
into the composition.  Interesting filters are:
\fIawk -e '{print "    " $0}'\fP or
\fI<mh directory>/lib/mhl -form mhl.body\fP.
.PP
.TP 8
.B ReverseReadOrder
When true, the next message will be the message prior to the current message
in the table of contents, and the previous message will be the message
after the current message in the table of contents.  The default is false.
.PP
.TP 8
.B SendBreakWidth
When a message is sent from \fIxmh\fP, lines longer than this value will be
split into multiple lines, each of which is no longer than \fBSendWidth\fP.
This value may be overridden for a single message by inserting an additional
line in the message header of the form \fISendBreakWidth: value\fP.  This
line will be removed from the header before the message is sent.
The default is 85.
.PP
.TP 8
.B SendWidth
When a message is sent from \fIxmh\fP, lines longer than \fBSendBreakWidth\fP
characters will be split into multiple lines, each of which is no longer than
this value.
This value may be overridden for a single message by inserting an additional
line in the message header of the form \fISendWidth: value\fP.  This
line will be removed from the header before the message is sent.
The default is 72.
.PP
.TP 8
.B SkipCopied
Whether to skip over messages marked for copying when using ``View Next
Message'' and ``View Previous Message''.  Default is true.
.PP
.TP 8
.B SkipDeleted
Whether to skip over messages marked for deletion when using ``View Next
Message'' and ``View Previous Message''.  Default is true.
.PP
.TP 8
.B SkipMoved
Whether to skip over messages marked for moving to other folders when
using ``View Next Message'' and ``View Previous Message''.  Default is true.
.PP
.TP 8
.B StickyMenu
If true, when popup command menus are used, the most recently selected
entry will be under the cursor when the menu pops up.  Default is false.
See the file \fIclients/xmh/Xmh.sample\fR for an example of how to 
specify resources for pop up command menus.
.PP
.TP 8
.B TempDir
Directory for \fIxmh\fR to store temporary directories.  For privacy, a user
might want to change this to a private directory.  Default is ``/tmp''.
.PP
.TP 8
.B TocGeometry
Initial geometry for master \fIxmh\fR windows.
.PP
.TP 8 
.B TocPercentage
The percentage of the main window that is used to display the Table of 
Contents.  Default is 33.
.PP
.TP 8
.B TocWidth
How many characters to generate for each message in a folder's table of
contents.  Default is 100.  Use 80 if you plan to use \fImhl\fR a lot,
because it will be faster, and the extra 20 characters may not be useful.
.PP
.TP 8
.B ViewGeometry
Initial geometry for windows showing only a view of a message.

.SH ACTIONS

Because \fIxmh\fR provides action procedures which correspond to command
functionality and installs accelerators, users can customize accelerators
in a private resource file.  \fIxmh\fR provides action procedures
which correspond to entries in the command menus; these are given in the
sections describing menu commmands.
For examples of specifying customized resources, see the file
\fIclients/xmh/Xmh.sample\fR.  Unpredictable results can occur if 
actions are bound to events or widgets for which they were not designed.
.PP
In addition to the actions corresponding to commands,
these action routines are defined:
.TP 18
.B XmhPushFolder(\fR[\fIfoldername, ...\fR]\fB)\fR
This action pushes each of its argument(s) onto a stack of foldernames.
If no arguments are given, the selected folder is pushed onto the stack.
.TP 18
.B XmhPopFolder()
This action pops one foldername from the stack and sets the selected folder.
.TP 18
.B XmhPopupFolderMenu()
This action should always be taken when the user selects a folder button.
A folder button represents a folder and zero or more subfolders.  The menu
of subfolders is built upon the first reference, by this routine.  If there
are no subfolders, this routine will mark the folder as having no subfolders,
and no menu will be built.  In that case the menu button emulates a toggle
button.  When subfolders exist, the menu will popup, using the menu button
action PopupMenu().
.TP 18
.B XmhSetCurrentFolder()
This action allows menu buttons to emulate toggle buttons in the function
of selecting a folder.  This action is for menu button widgets only,
and sets the selected folder.
.TP 18
.B XmhLeaveFolderButton()
This action insures that the menu button behaves properly when the user
moves the pointer out of the menu button window.
.TP 18
.B XmhPushSequence(\fR[\fIsequencename, ...\fR]\fB)\fR
This action pushes each of its arguments onto the stack of sequence names.
If no arguments are given, the selected sequence is pushed onto the stack.
.TP 18
.B XmhPopSequence()
This action pops one sequence name from the stack of sequence names,
which then becomes the selected sequence.
.TP 18
.B XmhPromptOkayAction()
This action is equivalent to pressing the okay button in the Create Folder popup.
.TP 18
.B XmhCancelPick()
This action is equivalent to pressing the cancel button in the pick window.

.SH CUSTOMIZATION USING \fIMH\fR
The initial text displayed in a composition window is generated by
executing the corresponding \fIMH\fP command; i.e. \fIcomp\fP, \fIrepl\fP,
or \fIforw\fP, and therefore message components may be customized as
specified for those commands.  \fIComp\fP is executed only once per
invocation of \fIxmh\fP and the message template is re-used for each
successive new composition.
.SH FILES
~/Mail
.br
~/.mh_profile - \fIMH\fR profile
.br
/usr/local/mh6 - \fIMH\fR commands
.br
~/Mail/<folder>/.xmhcache - scan folder
.br
~/Mail/<folder>/.mh_sequences - sequence definitions
.br
/tmp - temporary files
.SH SEE ALSO
X(1), xrdb(1), X Toolkit Intrinsics, Athena Widget Set, mh(1), enscript(1)
.SH BUGS
- Printing support is minimal.
.br
- Should handle the ``unseen'' message-sequence.
.br
- Should determine by itself if the user hasn't used \fIMH\fR before, and
offer to create the .mh_profile, instead of hanging on inc.
.br
- Still a few commands missing (rename folder, remail message).
.br
- A bug in \fIMH\fR limits the the number of characters in .mh_sequences to 
BUFSIZ.
When the limit is reached, the .mh_sequences file often becomes corrupted,
and sequence definitions may be lost.
.br
- Except for the icon, there isn't an indication that you have new mail.
.br
- There should be a resource, ShowOnInc, which when true, would show 
the current message in the view after incorporating new mail.
.br
- The CheckFrequency resource should be split into two separate resources.
.br
- WM_SAVE_YOURSELF protocol is ignored.
.br
- WM_DELETE_WINDOW protocol doesn't work right when requesting deletion
of the first toc and view, while trying to keep other \fIxmh\fP windows around.
.br
- Doesn't support annotations when replying to messages.
.SH COPYRIGHT
Copyright 1988, 1989, Digital Equipment Corporation.
.br
Copyright 1989, Massachusetts Institute of Technology
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Terry Weissman, Digital Western Research Laboratory
.br
modified by Donna Converse, MIT X Consortium
