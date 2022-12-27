.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Icons"
.hS "Description"
.iX "icons"
A window
icon is a minimized representation of a window or window
family that can help organize windows and tasks in the display.
.mS
The window manager must iconify all windows of a window family
together.
The application running inside of a window must
continue running even when the application is iconified. 
.mE
.P
An icon is made up of an image, a border, and a label.
The illustration on this reference page
shows a typical Motif icon.
.P
.mS
The image must be
surrounded by a border that indicates when the icon has the
keyboard focus.
.mE
The image area can contain text or a bitmap.
.mS
The label must be located just below the image and its border.
.mE
The label can also indicate when the icon has the keyboard focus.
The label should contain the same text as the title bar of the
corresponding primary window, or an abbreviated form of it.
.mS
When the icon does not have the keyboard focus, the width of the
label must be the same as the width of the image area and its
border.  The text must be centered in the label unless it is too
large to fit in the label, in which case the text should be left
justified in the label and any extra text truncated.
.mE
When the icon has the keyboard focus, the width of the label can
expand to display the entire text.  
.mS
The expanded label must be centered on the icon unless it would push
off the screen,
.mE
in which case the label should be as close to center
as is possible while keeping the entire label visible.
.P
.mS
Clicking
.B BSelect
in an icon must give the icon keyboard focus and
post the icon Menu.
Double-clicking
.B BSelect
anywhere in the icon must restore the
window family, just as the icon Menu item
.BR Restore .
Pressing
.B BSelect
anywhere in the icon and dragging the mouse
pointer must move the icon to track the pointer, just as the icon
Menu item
.BR Move .
.mE
.hS "Illustration"
.br
.ne 1.3i
\0
.sp 1.1i
.in +2.6i
.P! graphics/TermIcon.ps 
.in -2.6i
.hS "Related Information"
See Chapter 5 for more information about icons and the
window manager.
