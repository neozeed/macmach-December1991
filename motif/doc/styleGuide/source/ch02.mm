.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 1
.H 1 "Input and Navigation Models"
.iX "models" "input"
.iX "models" "navigation"
.iX "navigation" "model"
.iX "keyboard" "focus" 
.iX "input devices"
.iX "focus" "keyboard"
Consistent models increase the user's sense of control of a system.
By implementing consistent models across systems and applications,
you encourage that sense of control.
This chapter describes the models OSF/Motif uses to move among windows
and components and to interact with the components.
.BL
.LI
The keyboard focus model, which determines which component
on the screen receives keyboard events.
.LI
The input device model, which describes how different input
devices, such as the keyboard and the mouse, interact with
applications.
.LI
The navigation model, which determines how the keyboard focus
is moved among components.
.LE
.P
The activation and selection models, also important for system
and application consistency, are described in Chapter 3.
.H 2 "The Keyboard Focus Model"
.iX "input focus"
.iX "keyboard" "focus"
.iX "active window"
.iX "windows" "active"
.iX "location cursor"
.iX "cursor" "location"
.iX "motif window manager"
.iX "visual cue"
.iX "mouse"
.iX "keyboard" "events"
.iX "keyboard" "input"
.iX "cursor" "text insertion"
.iX "text" "insertion cursor"
.iX "focus" "implicit"
.iX "focus" "explicit"
.iX "focus policies"
A typical workspace can contain many windows.  Each window
expects to receive input from the keyboard, the mouse, or both.
The window that receives keyboard events has the input focus.
Indeed, when keyboard input is directed to a window it is actually
received by some control within the window.
The
.B "keyboard focus"
model determines which window in the workspace
and which component within that window gets each keyboard input.
The keyboard focus may also be referred to as the input focus.
.P
.mS
The window with the keyboard focus must be highlighted in some way,
usually by a change in shade or color to the window border.
The component with the keyboard focus must be highlighted by a
location cursor.
.mE
The location cursor is usually a box drawn
around the component with the keyboard focus, but can be indicated
in a number of ways, like
showing or blinking a text insertion cursor,
outlining an element of a list, or changing the background
shading of a drawing area.  Location cursors are described in
detail in Section 2.3.2.1.
.P
.mS
In order to avoid conflicts, the window manager must allow only one
window to have the keyboard focus at a time.
Each application must allow only one component at a time
to have the keyboard
focus within the window that has the keyboard focus.
.mE
.P
The keyboard focus model is defined by a focus policy.  A focus policy
is a specific mechanism for moving the focus among windows and components.
This section only attempts to describe the focus policies; however,
their
impact on window managers, applications, and components is described
in detail where it applies.
.mS
It is sufficient at this point to note that
window managers, applications, and new components must support both
explicit and implicit focus policies.
.mE
Chapter 5
describes in detail how to move the focus among windows
using explicit and implicit focus policies.
.H 3 "Implicit Focus"
.iX "implicit focus"
.iX "focus" "implicit"
.iX "focus policies"
.iX "keyboard" "focus"
.iX "focus" "keyboard"
.iX "mouse pointer"
.iX "pointers" "mouse"
.iX "pointers" "track"
.iX "track pointer"
.iX "track listener"
.iX "pointers"
.iX "real-estate driven"
In the
.B "implicit focus"
policy, the keyboard focus moves to the window into which
a user moves the mouse pointer.
No explicit action is performed to set the keyboard focus in
the implicit focus model.
Keyboard events are sent to the window that the mouse pointer is
in, more specifically to the component that the mouse pointer is in.
In implicit mode, the keyboard focus tracks the mouse pointer.
Because of this, there is no
way to move the keyboard focus from the keyboard using implicit mode.
Implicit focus is sometimes referred to as pointer,
track pointer, or track listener policy, or as being real-estate driven. 
In this focus policy, the location cursor for keyboard events
does not need to be shown except in components like Text, in which
keyboard input is common; however, the application can show it.
.P
.iX "focus policies"
.iX "focus" "implicit"
.iX "implicit focus"
The mouse pointer always behaves as if the focus policy is implicit.
That is, mouse events always go to the window and component that the
mouse pointer is in.
.H 3 "Explicit Focus"
.iX "explicit focus"
.iX "focus" "explicit"
.iX "focus policies"
.iX "focus" "keyboard"
.iX "mouse pointer"
.iX "pointers" "mouse"
.iX "location cursor"
.iX "cursor" "location"
.iX "click-to-type"
The
.B "explicit focus"
policy requires the user to explicitly select
which window receives the keyboard focus.
Mouse events are sent to the component that the pointer is over,
.mS
so the mouse pointer must always behave as if the focus policy
is implicit, even when the focus policy is explicit.
.mE
In explicit focus mode, a
user moves the keyboard focus to a window by pressing
.B BSelect
while the mouse pointer is over the window.  Simply moving the mouse
pointer over a window does not give the window the keyboard focus.
Because of this, explicit mode is often called click-to-type.
In explicit mode, a user moves the keyboard focus to a specific
component within a window by pressing
.B BSelect
over the component.
.mS
Pressing
.B BSelect
must not move focus to a component that is not
traversable or does not accept input.
.mE
Pressing
.B BSelect
in a component that is only used to change the visible portion
of another component, such as a ScrollBar or Sash, can
either give that component focus or not.
.mS
In this focus policy, the location of keyboard focus must be shown
by a location cursor.
.mE
.P
In explicit mode, focus can also be moved among windows by
the keyboard using
.BR KNextFamilyWindow ,
.BR KNextWindow ,
.BR KPrevWindow ,
and
.BR KPrevFamilyWindow .
These keys are usually bound to
.BR "MAlt <F6>" ,
.BR "MAlt KTab" ,
.BR "MAlt KBackTab" ,
and
.BR "MAlt MShift <F6>" ,
respectively as shown in the table at the end of this section.
.P
.iX "internal window navigation"
.iX "navigation" "window"
.iX "windows" "navigation"
In explicit mode, keyboard focus moves explicitly among the components in a window
as well as among the windows.
Moving the keyboard focus among components in a window using the
keyboard is called
.BR "component navigation" .
Keyboard events go to the component
in the window with the keyboard focus.
Keyboard focus is moved among components using
.BR KNextField ,
.BR KPrevField ,
.BR KMenuBar ,
.BR KMenu ,
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp .
These keys are usually bound to
.BR KTab ,
.BR KBackTab ,
.BR <F10> ,
.BR <F4> ,
or
.BR <Menu> ,
and the arrow keys as shown in the table at the end of this section.
Internal window navigation is described in detail later in this chapter.
.P
.iX "keyboard-based operations"
.iX "operation" "keyboard-based"
.iX "focus policies"
.iX "focus" "explicit"
.iX "focus policies"
.iX "focus" "implicit"
If the focus policy is implicit,
an implementation can still enable keyboard-based operations
that explicitly move the location cursor.
.mS
It must at least implement the special cases for Menu traversal
as described later in this chapter.
.mE
.B KMenuBar
moves the cursor to the MenuBar.
.B KMenu
pops up a Popup Menu.
.P
.mS
When an implicit focus policy is in use within a window
and a keyboard operation moves the cursor to another component,
the application must temporarily switch to an explicit focus policy.
It must revert to an implicit focus policy when:
.BL
.LI
A mouse button is pressed.
.LI
A keyboard operation moves the cursor out of a Menu system.
.LI
A button, other than a CascadeButton, is activated.
.LI
.B KCancel
is typed, except when
.B KCancel
leaves the location cursor in a
Menu system.
.B KCancel
is usually bound to
.BR <Cancel>
or
.BR <Escape> .
.LI
A modal operation, like keyboard-driven dragging, is completed.
.LE
.mE
.br
.ne 12
.P
.TB "Keyboard Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KTab@<Tab>
KBackTab@MShift <Tab>
KNextFamilyWindow@MAlt KTab
KPrevFamilyWindow@MAlt KBackTab
KNextWindow@MAlt <F6>
KPrevWindow@MAlt MShift <F6>
KNextField@KTab
@MCtrl KTab
KPrevField@KBackTab
@MCtrl KBackTab
KMenuBar@<F10>
KMenu@<Menu> or <F4>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
KCancel@<Cancel> or <Escape>
.TE
.H 2 "The Input Device Model"
.iX "input devices"
.iX "operation" "keyboard only"
.iX "operation" "mouse only"
.iX "traversal" "keyboard"
.iX "keyboard" "input"
.iX "mouse input"
.iX "pointing devices"
User interface applications can take input from both pointing devices
and keyboards.  For whatever reason a user may prefer to interact with
an application by using either a keyboard or a pointing device.  Some users
may even be restricted to using only a keyboard.  Because of these preferences
and possible restrictions,
.mS
all features of an application must be available
from the keyboard.
.mE
.P
Design your application so that the user can control it
using a pointing device, the keyboard, or both.  Although you
may decide to make the pointing device the primary means of control,
.mS
you must provide the user with keyboard methods to
control the application.
.mE
Designing applications for dual accessibility enables users
to choose the input device they find best suited to
their particular work situation and personal preferences.
.H 3 "Pointing Devices"
.iX "pointing devices"
.iX "pointers"
.iX "mouse"
.iX "graphics tablet"
.iX "track ball"
.iX "joystick"
.iX "BSelect"
.iX "BDrag"
.iX "BCustom"
.iX "BMenu"
.iX "direct manipulation"
A pointing device is a tool that lets the user move a pointer
around on the screen.  A pointing device also has some means of
activating the object under the pointer.
A pointing device allows the user to directly manipulate screen objects.
Rather than entering a keyboard command for each action, a user can
point to an object and directly manipulate it using the pointing device.
.P
The most typical pointing device is a mouse, although a
graphics tablet, track ball, joystick, and other tools
also work as pointing devices.
Throughout this guide we use the term mouse to refer to
all pointing devices.  You can use any pointing device
in place of a mouse.
.P
With a mouse, objects on the screen can be manipulated by combining
the mouse pointer with the mouse buttons.  This guide assumes
that a mouse, or any pointing device, has three buttons:
.VL 10n
.LI "\fBBSelect\fP"
Used for selection, activation, and setting the location cursor.
.mS
This button must be the leftmost button, except for left-handed
users where it can be the rightmost button.
.mE
.LI "\fBBDrag\fP"
Used for moving and copying elements.
.mS
This button must be the middle mouse button.
.mE
.LI "\fBBCustom\fP"
Used for additional user interactions, including popping up
Menus and special dialogs, and for application-specific
interactions.
.mS
This button must be the rightmost button, except for left-handed
users where it can be the leftmost button.
This guide also mentions the
.BR BMenu
button,
which must be
bound to
.BR BCustom 
on a three-button mouse.
.mE
.B BMenu
is used for popping up Menus.
.LE
.P
.mS
If your pointing device only has two buttons,
.B BSelect
still must be bound to the leftmost button.
.mE
There are a number of alternatives to properly
binding
.BR BDrag ,
.BR BCustom ,
and
.BR BMenu .
.BL
.LI
.B BDrag
can be bound to the rightmost button, and
.B BCustom
and
.B BMenu
are then bound to chording both buttons.  Chording
a button means to press, click, or release two or more
buttons simultaneously.
.LI
.iX "BDrag"
.iX "BCustom"
.iX "BMenu"
Alternatively,
.B BDrag
can be bound to the rightmost button,
.B BCustom
is undefined, and
.B BMenu
is then bound to
.BR "MCtrl MShift BSelect" .
.LI
Finally,
.B BDrag
can be left undefined, and
.B BCustom
and
.B BMenu
are
then bound to the rightmost button.
.LE
.P
As with a 3-button mouse, the actions of the
rightmost and leftmost mouse buttons can be switched 
for left-handed users.
.P
.iX "BSelect"
.mS
If your mouse has only one button,
.B BSelect
must be bound to it,
.B BDrag
and
.B BCustom
must be left undefined, and
.B BMenu
must be bound to
.BR "MCtrl MShift BSelect" .
.mE
.P
.iX "BSelect"
If your mouse has more than three buttons, the first three
correspond to those on a 3-button mouse.
.P
.iX "Press"
.iX "Release"
.iX "Click"
.iX "Drag"
.iX "MultiClick"
.iX "MultiPress"
.iX "MultiDrag"
Users can combine mouse button press and release actions
along with mouse movements to activate elements of applications
and components.  Various mouse button actions are described
as follows, along with this \fIStyle Guide's\fP usage conventions.
.VL 15n
.LI "\fBPress\fP"
Indicates pressing a mouse button without releasing it.
This mouse action is often used to select an object for action.
This guide uses
.B "BSelect Press"
to indicate a press on the first mouse button.
.LI "\fBRelease\fP"
Indicates releasing a mouse button after pressing it.
This mouse action is often used to perform the action initiated
by a press or a drag.
This guide uses
.B "BSelect Release"
to indicate a release on the first mouse button.
.LI "\fBClick\fP"
Indicates pressing and releasing a mouse button without moving the pointer.
This mouse action is used to select an object or perform
an action.
The activation action actually occurs on the release of the
mouse button.
This guide uses
.B "BSelect Click"
to indicate a click on the first mouse button.
.LI "\fBDrag\fP"
Indicates pressing a mouse button without releasing it and then
moving the position of the pointer.
The action of a drag operation tracks the position of
the mouse pointer.
The drag action ends with a release action.
This mouse action is commonly used to select a range of objects,
or move a selected object.
This guide uses
.B "BSelect Drag"
to indicate a drag using the first mouse button.
.LI "\fBMultiClick\fP"
Indicates a number of clicks in quick succession.
MultiClicks are often referred to by the actual number of
clicks, as in double-click or triple-click.
A double-click is often used to perform the default
action of an object.
This guide uses
.B "BSelect Click 2"
to indicate a double-click on the first mouse button,
.B "BSelect Click 3"
to indicate a triple-click, and so on.
This guide uses
.B "BSelect Click 2+"
to indicate two or more clicks on the first mouse button.
.LI "\fBMultiPress\fP"
Indicates a number of clicks in quick succession without a
final release.
As in MultiClick, MultiPresses are often referred to by the actual
number of presses.
This guide uses
.B "BSelect Press 2"
and the variations mentioned in
MultiClick to indicate MultiPress actions on the first mouse button.
.LI "\fBMultiDrag\fP"
Indicates a MultiPress action followed by moving the position of
the pointer.
The action of a MulitDrag operation tracks the position of
the mouse pointer.
The MultiDrag action ends with a release action.
As in MultiClick, MultiDrags are often referred to by the actual
number of presses.
This guided uses
.B "BSelect Drag 2"
and the variations mentioned in
MultiClick to indicate MultiDrag actions using the first mouse button.
.LE
.H 3 "Pointer Shapes"
.iX "pointer shapes"
.iX "shapes" "pointer"
.iX "mouse pointer shapes"
.iX "design" "pointer shapes"
The shape of the mouse pointer provides the user with an important visual cue,
indicating the functionality of the area in which the mouse pointer is
currently located.
You should not create new mouse pointer shapes for functions
that already have mouse pointer shapes associated with
them, except for localization, but you can create new mouse pointer shapes for
functions not already associated with a pointer shape.
.mS
You must not use a predefined shape to symbolize a function
that it was not designed to represent.
.mE
.P
.iX "hotspot"
.iX "pointer shapes"
.iX "shapes" "pointer"
A list of the defined pointer shapes follows, along with
their usage and hotspots.  A pointer's hotspot is the
actual position on the pointer that tracks the movements of
the mouse.
.mS
As the pointer changes from one shape to another,
the location of the hotspot must not move on the screen.
.mE
The hotspot is the precise location on the pointer where
mouse actions occur.
.VL 15n
.if n \{
.LI "Arrow"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/arrowP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "arrow pointer"
.iX "pointers" "arrow"
The upper left pointing arrow pointer is a general-purpose pointer.
It is used
in most window areas for single object selection and activation.
The hotspot for the arrow pointer should be in the point of 
the arrow.
.if n \{
.LI "I-beam"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/ibeamP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "I-beam pointer"
.iX "pointers" "I-beam"
You can optionally use an I-beam pointer in any Text component.
It is used to change the location of the text insertion
cursor and to perform actions on text.
If the I-beam is used, it can be hidden during the time
between any keyboard action and a mouse movement.
This helps the user distinguish the I
beam pointer from the text insertion cursor, which can also be
an I-beam.
The hotspot for the I-beam pointer should be on the vertical
bar of the I-beam about one-third up from the bottom.
.if n \{
.LI "X"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/xP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "X pointer"
.iX "pointers" "X"
The X pointer can indicate when the pointer is outside of any
application area.
The hotspot for the X pointer should be where the lines
intersect.
.if n \{
.LI "Resize"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.35i
\0
.in -15n
.P! graphics/8sizeP.ps
.in +15n
.sp -.65i
.fl
'br\}
.iX "resize pointer"
.iX "pointers" "resize"
The resize pointers indicate positions for area resize, and
they remain during a resize operation.  The direction of the
arrow in the pointer indicates the direction of increasing
size.  The horizontal and vertical pointers indicate resize
in either the horizontal or vertical direction.
The diagonal pointers indicate resize
in both the horizontal and vertical directions simultaneously.
The hotspot for the resizing pointers should be on the elbow or the
line at the position pointed to by the arrow.
.if n \{
.LI "Hourglass"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.1i
\0
.in -15n
.P! graphics/hourglassP.ps
.in +15n
.sp -.4i
.fl
'br\}
.iX "hourglass pointer"
.iX "pointers" "hourglass"
The hourglass pointer, a working pointer, indicates that 
an action is in progress in the area, and that the pointer
has no effect in the area.  While the hourglass pointer
is active, all mouse button and keyboard events are ignored
in the area.
The hotspot for the
hourglass pointer should be located at the center of the hourglass,
though it should not be used for activation.
The hourglass pointer can be used interchangeably with the watch pointer.
.if n \{
.LI "Watch"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/watchP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "watch pointer"
.iX "pointers" "watch"
The watch pointer, a working pointer, indicates that 
an action is in progress in the area, and that the pointer
has no effect in the area.  While the watch pointer
is active, all mouse button and keyboard events are ignored
in the area.
The hotspot for the
watch pointer should be located at the top of the watch,
though it should not be used for activation.
The watch pointer can be used interchangeably with the hourglass pointer.
.if n \{
.LI "Moving"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/movingP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "four-directional arrow pointer"
.iX "arrow pointer" "four-directional"
.iX "pointers" four-directional arrow"
The 4-directional arrow pointer indicates a move 
operation in progress, or a resize operation before the resize direction
has been determined.
During a move operation, the object, or an outline of the object
should move to track the location of the pointer.
During a resize operation, the pointer is used to indicate a
direction for resizing.  The 4-directional arrow pointer
should change to the appropriate resize arrow when the resize
direction is determined, either by crossing an object boundary
with the pointer or by pressing a keyboard direction key.
The hotspot for the 4-directional arrow pointer should be
at the spot where the arrows intersect.
.if n \{
.LI "Sighting"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/sightP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "sighting pointer"
.iX "pointers" "sighting"
The sighting pointer is used to make fine position selections.
For example, in a drawing program it may be used to
indicate a pixel to fill or the connecting points of lines.
The hotspot for the sighting pointer should be
at the spot where the lines intersect.
.if n \{
.LI "Caution"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.1i
\0
.in -15n
.P! graphics/cautionP.ps
.in +15n
.sp -.4i
.fl
'br\}
.iX "caution pointer"
.iX "pointers" "caution"
The caution pointer is used to indicate that action is
expected in another area before input can be given to
the current area, and that the pointer has no effect in the
area.  While the caution pointer is active, all mouse button
and keyboard events are ignored in the area.
The hotspot for the
caution pointer should be located at the center of the caution symbol,
though it should not be used for activation.
.if n \{
.LI "Question"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/questP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "question pointer"
.iX "pointers" "question"
The question pointer is used to request an input position or component
from the user.
This is often used to input an object for interactive
help.
The user requests interactive help, then the question pointer
is displayed to allow the user to indicate what position or
component help is requested for.
The hotspot for the question pointer should be at the bottom of the
question mark.
.if n \{
.LI "Other\ Arrow"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/menuarrowP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "arrow pointer"
You can optionally use an arrow pointing to the upper right corner
to indicate a pending Menu action.  This shape indicates that
a Menu is popped up or pulled down and waiting for a
Menu item to be activated or the Menu to be removed.
The hotspot for this arrow pointer should be in the point of 
the arrow.
.LE
.P
.iX "pointers" "designing"
.iX "hotspot"
If you need to design a new mouse pointer shape, keep the
following in mind:
.BL
.LI
The shape of the pointer should give a hint to its purpose.  
.LI
The hotspot should be easy to locate, and obvious.
.LI
The shape should be easy to see.
.LI
The shape should not create visual clutter.
.LE
.P
.iX "gain"
.iX "acceleration"
The details of how the pointer tracks the motion of the mouse
are not handled at the level of the application, but they are
worth noting here.  There are two concepts that define how
the pointer tracks the motion of the mouse: gain and acceleration.
.P
.iX "gain"
.iX "zoom feature"
Gain refers to the ratio of the distance the pointer moves to the
distance the mouse moves.  If the gain is increased, the mouse
pointer moves farther for a given mouse movement.
The gain should not change across the environment.  Instead,
if your application requires finer motion, it should include
.iX "zoom feature"
a zoom feature.  A zoom feature changes the relative size of an
area of your application.  Zooming in on an area allows the user
to make finer adjustments than normal.  Zooming out from an
area allows the user to move through the application more
quickly.
.P
.iX "acceleration"
Acceleration is a temporary change in the gain.  It is
commonly used to change the gain, based on the speed of the
mouse movements.  The acceleration could be set so that if the
mouse moves slowly, the gain is reduced to allow for finer
adjustments of pointer position, or if the mouse moves
quickly, the gain is increased to allow quicker screen movement.
This can be very confusing to new users.  Gain
and acceleration are handled on a global scale in the system,
outside of the application.
Applications should not change the gain and acceleration
characteristics of mouse movement.
.H 3 "Warp Pointer Only If Explicitly Enabled"
.iX "pointers" "warping"
.iX "warping"
The pointer position is intended only as input to applications.
It is not intended as an output mechanism for applications.
Changing the location of the
mouse pointer is known as warping the pointer.
.mS
Your application must not warp the pointer, unless you provide
the user with a means of disabling this behavior.
.mE
.P
Warping the pointer is confusing to users, and reduces 
their sense of control.
Also, warping the pointer can
cause problems for users of absolute location pointing
devices (like graphics tablets).  Graphics tablets map
pointer device locations to absolute screen locations,
so if the pointer is warped, the pointer loses synchronization
with the pointing device, making some screen locations
impossible to reach.  Note that for these types of devices,
accelerating the pointer has the same effect as warping
the pointer, but this can be handled by the pointer
driver software.
.H 2 "The Navigation Model"
.iX "navigation" "model"
.iX "keyboard" "navigation"
.iX "mouse"
.iX "keyboard"
.iX "location cursor"
.iX "cursor" "location"
.iX "menus" "navigation"
.iX "scrollable component navigation"
.iX "components" "Scrollable"
Regardless of whether they use a mouse, a keyboard, or both,
users needs to move the mouse pointer or the location cursor to new
positions.  That is, they will need to navigate around the workspace.
The model is simple for mouse users, and more complicated for
keyboard users.
This section describes the general navigation model for mouse and
keyboard users, describes the more complicated Menu navigation
model, and then describes navigation within scrollable components.
.H 3 "Mouse-Based Navigation"
.iX "navigation" "mouse-based"
.iX "mouse-based navigation"
In mouse-based navigation,
the pointer tracks the
motions of the pointing devices.  
This is described earlier in this chapter, and is known as
the implicit focus policy.
.mS
The mouse must always behave as if
the focus policy is implicit.
.mE
Components that are used to
adjust the size and location of other elements need not move the
location cursor when they receive the keyboard focus.  Instead,
after the mouse has acted on the component, the focus should
remain on the component that previously had it.
.P
.iX "pointer navigation"
.iX "PullDown Menus"
.iX "Popup Menus"
.iX "Options Menu"
.iX "MenuBar"
.iX "menus" "navigation"
.iX "CascadeButton"
.iX "spring-loaded"
The only exception to the simple model of pointer navigation
is a Menu system because Menus are not available on the screen
until activated.
Activating a Menu causes it to be shown on the screen.
There are three types of Menus:  PullDown Menus, Popup Menus,
and Option Menus.  The MenuBar is also a special kind of Menu.
A
.B "Menu system"
consists of all the Menus cascading from a single
CascadeButton, OptionButton, or Popup Menu.  The
.B "MenuBar system"
consists of the MenuBar and all of its associated Cascade Menus.
.P
A PullDown Menu is generally activated by pressing
.B BSelect
on a CascadeButton, which displays the PullDown Menu.
A Popup Menu is generally activated by pressing
.B BMenu
in an area with a Popup Menu, which displays the Popup Menu.
An Option Menu is generally activated by pressing
.B BSelect
on an OptionButton, which displays the Option Menu.
A PullDown Menu pulled down from a CascadeButton within a
PullDown, Popup, or Option Menu is called a CascadeMenu.
A MenuBar is generally activated by moving the input focus to
the MenuBar.  Since a MenuBar is always visible, activating
the MenuBar does not change its appearance.
Navigating to a Menu is equivalent to activating a Menu.  
.mS
.B "BSelect Press"
with the pointer on a Menu must activate the Menu
in a spring-loaded manner.
.mE
Spring loaded means that the Menu
is removed when the button is released, except on a CascadeButton.
.mS
.B "BMenu Press"
with the pointer in an area with a Popup Menu must activate the Menu
in a spring-loaded manner.
Once a Popup Menu is posted,
.B BMenu
must behave just as
.B BSelect
is described for any Menu system.
.B BSelect
must also be available from within a posted Popup Menu system,
and must behave just as in any Menu system.
.B "BSelect Release"
within a spring-loaded Menu system must activate the button under
the pointer at the time of the release.  Note that if the release is
on a CascadeButton, the associated Cascade Menu must be posted.
.mE
.P
.mS
Whenever any Menu is popped up or pulled down,
the location cursor must be placed on the
Menu's default entry, or on the first entry in the Menu if there is no
default entry.
.mE
Support for default entries in Menus is optional.
.P
.mS
A spring-loaded Menu must be removed when the mouse button that activated
it is released.
While a spring-loaded Menu system is popped up or pulled down,
moving the pointer within the Menu system must move the location
cursor to track the pointer.  If the pointer rests on a CascadeButton,
the Menu must be pulled down and must also become traversable.
The Menu must be removed once the pointer moves to a
Menu item outside of the Menu or its CascadeButton.
If the Menu system is part of a MenuBar, moving the pointer to
any other element on the MenuBar must unpost the current Menu
system and post the PullDown Menu associated with the new element.
Releasing the button must activate any component in the Menu system,
including a CascadeButton.
.mE
Note that releasing the mouse button
in a CascadeButton, even a CascadeButton in the MenuBar,
causes the associated Menu to remain posted.
.P
.iX "posted menu"
.mS
.B "BSelect Release"
with the pointer on a CascadeButton or OptionButton must activate
the associated Menu in a posted manner.
.B "BMenu Release"
with the pointer in an area with a Popup Menu must activate the Menu
in a posted manner.
.mE
A posted Menu remains visible until explicitly unposted.  The keyboard
focus model is forced to explicit, and the Menu system
can be traversed using the keyboard as described later in this section.
.mS
A posted Menu must not be unposted until one of
the following events occurs:
.BL
.LI
A mouse button is pressed
.LI
A keyboard operation moves the cursor out of a Menu system
.LI
A keyboard operation moves the cursor to a parent of the
posted Menu.
.LI
A Menu item, other than a CascadeButton, is activated
.LI
.B KCancel
is typed from the top of the Menu system
.LE
.mE
.P
If a mouse button is pressed outside of the Menu to unpost
the Menu, the button press and it's release can also effect
the underlying component,
otherwise both the press and the release must have no additional effect.
.mS
Either both the press and release must effect the underlying component,
or neither the press nor the release must effect the underlying component.
.mE
.P
.mS
Once a PullDown or Option Menu is posted,
.B "BSelect Press"
in the Menu system must cause the Menu to behave as a
spring-loaded Menu.
Once a Popup Menu is posted,
.B "BMenu Press"
in the Menu system must cause the Menu to behave as a
spring-loaded Menu.
.mE
Because of this, the pointer driven activation behavior
of spring-loaded Menus fully describes pointer driven activation
behavior in Menus.
.H 3 "Keyboard-Based Navigation"
.P
.iX "location cursor"
.iX "cursor" "location"
The navigation model for keyboard users is more complicated
than the pointer navigation model.
As described earlier, keyboard navigation is only required
when the focus policy is explicit.
.mS
Since window managers,
applications, and components must support the explicit focus
policy, they must also support keyboard navigation.
.mE
The keyboard navigation model is composed of the following:
.BL
.LI
A focus identifier (the location cursor)
.LI
Navigating among windows
.LI
Navigating within windows
.LE
.H 4 "Location Cursor"
.iX "location cursor"
.iX "cursor" "location"
.iX "cursor"
The window with the focus is identified by highlighting the
window border as described in
Chapter 5.
Within the window with the focus, the component with the keyboard focus
is identified by the location cursor.  The term location
cursor refers to any visual element that indicates the
location where keyboard events are sent.  The location
cursor is not strictly a cursor.  The name cursor simply
refers to its use as indicating the location of input.
.P
.iX "location cursor"
.iX "cursor" "location"
The location cursor is shown in a number of ways, depending
on the type of component with the keyboard focus.
Possible location cursor types and their uses are
described in the following text.  When designing new components, you should
try to use one of the existing styles of location cursor,
but you can create your own if it is appropriate.
.VL 20n
.LI "\fBBox\fP"
.mS
The box cursor must be the default location cursor.
.mE
It is shown as a box drawn around the object.
Figure 2-1 shows the first ToggleButton in a group
with a box style location cursor.
.br
.ne 1.6i
\0
.FG "A Box Style Location Cursor"
.sp 1.4i
.in -20n
.in +2.4i
.P! graphics/box.cursor.ps
.in -2.4i
.in +20n
.LI "\fBOutline Highlight\fP"
.iX "outline highlight"
This cursor style is similar to the box style cursor.
The outline is shown of a component whose outline is
not normally shown.
This form of location cursor is commonly used within
Menu systems to show the Menu item with the location cursor.
Figure 2-2 shows a Menu with an outline highlight style
location cursor.
.br
.ne 1.8i
\0
.FG "An Outline Highlight Style Location Cursor"
.sp 1.6i
.in -20n
.in +2.2i
.P! graphics/out.cursor.ps
.in -2.2i
.in +20n
.LI "\fBText Cursor\fP"
.iX "text" "cursor"
.iX "cursor" "text insertion"
In Text components, the text cursor
acts as the location cursor to indicate that the
Text component has the keyboard focus.
Figure 2-3 shows possible text cursor shapes.
.br
.ne 1.4i
\0
.FG "Text Cursor Shapes"
.sp 1.2i
.in -20n
.in +1.6i
.P! graphics/cursors.ps
.in -1.6i
.in +20n
.P
.mS
A text cursor must be shown differently when the the
Text has the keyboard focus.
.mE
This can be done by graying and emboldening, making it
static and blinking, or shown and hidden.
.mS
If the text cursor is hidden when the Text component
does not have the focus, then
when it is traversed to, the text cursor
must appear in the same location that it was in the
last time it was traversed out of.  The initial position
of the text cursor must be at the beginning of the text.
A caret is also used to show the destination cursor in Text
components.
.mE
A Text component can optionally include an outline highlight
style location cursor to reinforce the location of
the keyboard focus.
Figure 2-4 shows a Text component with the keyboard focus.
.br
.ne 1.65i
\0
.FG "A Text Cursor Style Location Cursor"
.sp 1.45i
.in -20n
.in +1.2i
.P! graphics/text.ps
.in -1.2i
.in +20n
.LI "\fBItem Cursor\fP"
.iX "item cursor"
.iX "cursor" "item"
A component that manages groups of noncomponent
objects, like a list box, should indicate that it
has the keyboard
focus with an item cursor style location cursor.
An item cursor highlights a single object, or group
of objects, that the component acts on.  In the case
of a List, the item cursor should be a
box around an item.  In the case of a Drawing area,
it could be a box with resize handles around a drawn
object.
Components that use an item cursor to indicate keyboard
focus can optionally include an outline highlight
style location cursor to reinforce the location of
the keyboard focus.
Figure 2-5 shows a List component with the keyboard focus.
.br
.ne 2.3i
\0
.FG "An Item Cursor Style Location Cursor"
.sp 2.1i
.in -20n
.in +1.45i
.P! graphics/item.cursor.ps
.in -1.45i
.in +20n
.LI "\fBFill\fP"
.iX "fill"
Some very small components, like a Sash for resizing Panes,
should indicate the keyboard focus by filling.
.mS
Where this is
the case, there must be no other meaning associated with
the filled state.
.mE
Filling avoids the problem caused by other
styles of location cursor that make the small component too
large.
Figure 2-6 shows the first of two Sashes with the keyboard
focus.
.br
.ne 2.15i
\0
.FG "A Fill Style Location Cursor"
.sp 1.95i
.in -20n
.in +1.2i
.P! graphics/panedw1.ps
.in -1.2i
.in +20n
.LE
.P
.mS
Components must be designed and positioned within applications
so that adding and removing the component's location cursor does not
change the amount of space that the component takes up on the screen.
.mE
That is, there is always room for the location cursor.
.H 4 "Window Navigation"
.iX "window navigation"
.iX "main windows"
.iX "primary windows"
.iX "windows" "main"
.iX "windows" "primary"
.iX "secondary windows"
.iX "transient windows"
.iX "windows" "secondary"
.iX "windows" "transient"
.iX "DialogBox"
A typical environment has several applications in operation simultaneously.
Each application typically has a main or primary window that displays
data and in which the user carries on
primary interaction with the application.
Applications can have additional windows
to communicate context-specific interactions with the user of the application.
These additional windows are called secondary windows, or transient windows.
DialogBoxes are often used to create secondary windows.
.P
.iX "window navigation"
.iX "primary windows"
.iX "secondary windows"
.iX "windows" "primary"
.iX "windows" "secondary"
The
.B "window navigation"
model can be divided into two levels:
.BL
.LI
Moving among window families (among primary windows)
.LI
Moving within a window family (among secondary windows)
.LE
.P
.iX "window family"
.iX "KNextFamilyWindow"
.iX "KPrevFamilyWindow"
.iX "KNextWindow"
.iX "KPrevWindow"
A window family consists of a single primary window and
all of its associated secondary windows.
.B KNextFamilyWindow
and
.B KPrevFamilyWindow
move the focus among windows in a window family.
.B KNextWindow
and
.B KPrevWindow
move the focus among window families.
Window navigation is described in more detail in
Chapter 5.  The following table summarizes the
common window navigation bindings.
.P
.TB "Window Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KTab@<Tab>
KBackTab@MShift <Tab>
KNextFamilyWindow@MAlt <F6>
KPrevFamilyWindow@MShift MAlt <F6>
KNextWindow@MAlt KTab
KPrevWindow@MAlt KBackTab
.TE
.H 4 "Component Navigation"
.iX "component navigation"
.iX "navigation" "components"
.iX "location cursor"
.iX "cursor" "location"
.iX "keyboard" "focus"
.iX "fields"
.iX "tab groups"
Component navigation moves
the location cursor, and therefore the keyboard focus,
among components within a window.
Some components, like Text, Menus, and List,
have additional keyboard methods of traversal, which
are described in the behavior descriptions of those components. 
The model for component navigation organizes components into
traversable groups called fields or tab groups.
.mS
Components that have their own
.BR "internal navigation" ,
such as a List,
must be a Field.  Groups must be fields, unless the group has no
controls in it.  When a component is
used as a Pane of a PanedWindow it must be a field.
Top level components in a window must be fields.
.mE
The following components have their own internal navigation,
and should be fields within a window.
.BL
.LI
Canvases
.LI
Text
.LI
Lists
.LI
Sashes
.LI
ScrollBars
.LI
Scales
.LE
.P
Single line Text components can be part of another field, rather
than a field of their own.
.P
Menu systems, including the MenuBar, follow a different
traversal model that is described later in this section.
.P
.iX "KNextField"
.iX "KPrevField"
.mS
Moving the location cursor among field components and fields
in a window must be done with
.B KNextField
and
.BR KPrevField .
.mE
Fields are often called tab groups because 
.B KNextField
and
.B KPrevField
are usually bound to
.B <Tab>
and
.B <BackTab>
as shown in the following table.
Some controls, such as Text, use
.B <Tab>
and
.B <BackTab>
internally.
.mS
These controls must use the
.B <Ctrl>
modified bindings for
.B KNextField
and
.B KPrevField .
.mE
.br
.ne 12
.P
.TB "Field Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KTab@<Tab>
KBackTab@MShift <Tab>
KNextField@KTab
@MCtrl KTab
KPrevField@KBackTab
@MCtrl KBackTab
.TE
.br
.ne 10
.P
.iX "KDown"
.iX "KLeft"
.iX "KRight"
.iX "KUp"
.iX "location cursor"
.mS
.B KNextField
must move the location cursor forward through fields according to
the following rules.
.BL
.LI
If the next field is a field control,
.B KNextField
must move the location cursor to it.
.LI
If the next field is a field group,
.B KNextField
must move the location cursor to the first traversable
component within the field group, unless the field
contains a default component.
.LI
If the field contains a traversable default component,
.B KNextField
must move the location cursor to the default component instead
of the first component,
unless the application arranges for the cursor to be placed
on some other component in the field.
.LE
.mE
.P
Application developers should generally make a default component the
first component in a field.
Section 3.3.6 describes the use of default components in detail.
.mS
If the next field contains no traversable components,
the components are fields, or the components are inside nested fields,
.B KNextField
must skip the field.
.mE
.P
.mS
.B KPrevField
must move the location cursor backward through fields
using the same rules as
.BR KNextField .
.mE
.P
Developers should follow these general rules for field
navigation:
.BL
.LI
Fields should be traversed from the upper-left
to the lower-right in a left-to-right direction language
environment.  In a right-to-left direction language
environment, fields should be traversed from upper-right to
the lower-left.
.LI
.mS
Field navigation must wrap between the first and last field.
.mE
.LI
If one field is the child of another field,
.B KNextField
should traverse the parent field before the child.
.LI
.mS
In the case of a PanedWindow, each Sash and Pane must be
a field, and must be traversed in top-to-bottom order.
Any fields within a Pane must be traversed before the next sash.
.mE
.LI
If the field's default component is in a nested field, these rules
can be relaxed to make sure that the filed and all of its
descendents are traversed by
.B KNextField
before traversing to another field.
.LE
.P
When using an explicit keyboard focus policy,
and a window gets the focus, the location cursor
should be placed in the last component in that window to
have the location cursor.
If that component is no longer
traversable, or if the window has never had the location cursor before,
the cursor should be placed in the first traversable component
in the window.
If the window is a DialogBox, the location cursor should be placed
on the default component in the DialogBox,
though an application can place the cursor on any
specific component in the DialogBox.
If the default component is not traversable, or if there is no default component
the location cursor should be placed as for any other window.
.P
When using an explicit keyboard focus policy,
and a field gets the focus,
the location cursor should be placed in the first
traversable component in the field.  If the field is a DialogBox
nested in a window (that is, a field with a default control),
the location cursor should be placed
on the default component in the DialogBox,
though an application can place the cursor on any
specific component in the DialogBox.
If the default component is not traversable, or if there is no default component
the location cursor should be placed as for any other field.
.P
.mS
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp
must move the location cursor among components in a field group,
or among elements of a field control.
.mE
In a graphics, these keys can move the location cursor by an element
or by a pixel.
These keys modified with
.B MCtrl
can move the cursor in a large increment.
Whan an increment and a large increment is depends on the type
of the component.
For example, in a scale an increment could be a unit of the scale,
and a large increment could be a number of units, or the distance between
major tick marks on the scale.
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp
usually are bound to the arrow keys as shown in the following table.
.br
.ne 12
.P
.TB "Component Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
.TE
.P
.iX "field navigation"
.iX "navigation" "field"
.iX "KDown"
.iX "location cursor"
.iX "cursor" "location"
.iX "KUp"
.iX "KRight"
.iX "KLeft"
.mS
Navigation within a field must behave as described in the following text.
.mE
Note that
.B KDown
and
.B KRight
do not need to traverse components or elements in the same order.
.VL 15n
.LI "\fBKDown\fP"
.mS
in a left-to-right direction language environment
must move the location cursor through all traversable components
or elements
in the field, starting at the upper-left
and ending at the lower-right then wrapping back
up to the upper-left.
.mE
In a right-to-left direction language environment
the location cursor can move, starting at the upper-right
and moving to the lower-left.
.LI "\fBKUp\fP"
.mS
must move the location cursor through all traversable components
or elements
in the opposite direction of
.BR KDown .
.mE
.LI "\fBKRight\fP"
.mS
...\"in a left-to-right direction language environment
must move the location cursor through all traversable components
or elements
in the field, starting at the upper-left
and ending at the lower-right then wrapping back
up to the upper-left.
.mE
...\"In a right-to-left direction language environment
...\"the location cursor can move, starting at the upper-right
...\"and moving to the lower-left.
.LI "\fBKLeft\fP"
.mS
must move the location cursor through all traversable components
or elements
in the opposite direction of
.BR KRight .
.mE
.LE
.P
Separators and Labels should not be traversable.
Labels can be traversable if the system supports quick transfer
of the Label text or graphic.
The ScrollBars of ScrolledWindow need not be traversable.
.P
.iX "field controls"
.iX "field groups"
.iX "KBeginLine"
.iX "KEnd Line"
.iX "KBegin Data"
.iX "KEnd Data"
Field controls should also support the following behavior.
Component navigation in
field groups can also support this behavior.
.VL 15n
.LI "\fBKBeginLine\fP"
In a left-to-right direction language environment, this action
should move the location cursor to the leftmost edge of the data.
.LI "\fBKEndLine\fP"
In a left-to-right direction language environment, this action
should move the location cursor to the rightmost edge of the data.
.LI "\fBKBeginData\fP"
Should move the location cursor to the first element
or the topmost or topleftmost edge of the data.
.LI "\fBKEndData\fP"
Should move the location cursor to the last element
or the bottommost or bottomrightmost edge of the data.
.LE
.br
.ne 15
.P
These keys are usually bound as shown in the following table.
.br
.ne 12
.P
.TB "Component Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KBeginData@MCtrl KBeginLine
KEndData@MCtrl KEndLine
KBeginLine@<Begin> or <Home>
KEndLine@<End>
.TE
.H 3 "Menu Traversal"
.iX "Menu traversal"
The Menu traversal model is different from the field traversal
model.
This is so that Menus are traversable even
when the focus policy is implicit.
.mS
If a Menu is traversed to
while the focus policy in the application is implicit, the focus 
policy must temporarily change to explicit.
The focus policy must revert to implicit whenever any
one of the following occurs:
.BL
.LI
A mouse button is pressed
.LI
A keyboard operation moves the cursor out of a Menu system
.LI
A Menu item, other than a CascadeButton, is activated
.LI
.B KCancel
is typed from the top of the Menu system
.LE
.mE
.P
.iX "Menu system" "activation"
.iX "activation" "Menu system"
.iX "MenuBar"
.iX "KMenuBar"
Traversing to a Menu system is the same as activating
the Menu system.
.mS
If the MenuBar is inactive,
.B KMenuBar
must traverse to, or activate, the MenuBar system.
The location cursor must be placed on the first traversable
Menu in the MenuBar.  If there are no traversable Menus
in the MenuBar,
.B KMenuBar
must do nothing.
If the MenuBar system is active,
.B KMenuBar
must unpost all Menus in the MenuBar system and return the
location cursor to the component that previously had it.
.mE
.P
.iX "KMenu"
.mS
If the keyboard focus is in an area with an inactive Popup Menu,
.B KMenu
must post (activate) the Popup Menu.
The location cursor must be placed on the first traversable
item in the Popup Menu.
.mE
If there are no traversable items
in the Popup Menu, it is up to the system and the application
whether to post the Menu or not.
.mS
If there is an active Popup Menu,
.B KMenu
must unpost the Popup Menu system and return the location cursor
to the component that previously had it.
.mE
.P
.iX "KActivate"
.iX "KSelect".mS
If the keyboard focus is in an OptionButton,
.BR KActivate
or
.B KSelect
must post, or activate, the Option Menu.
The location cursor must be placed on the previously selected item 
in the Option Menu.  If the Option Menu is pulled down for the first
time, the location cursor must be placed on the default item
in the Menu.
.mE
If there are no traversable items
in the Option Menu, the application should decide
whether to post the Menu or not.
.mS
If there is an active Option Menu,
.B KActivate
or
.B KSelect
must select the current item in the Option Menu,
unpost the active Option Menu system, and return the location cursor
to the OptionButton.
.mE
.P
.B KMenuBar
and 
.B KMenu
are usually bound to
.BR <F10> ,
and
.B <F4>
or
.BR <Menu> ,
respectively.
.B KActivate
is usually bound to
.BR <Return>
or
.BR <Enter> .
.B KSelect
is usually bound to
.BR <Select> ,
.BR <Space> ,
and
.BR "MCtrl <Space>" ,
The following table shows the common Menu traversal bindings.
.P
.ne 18
.TB "Menu Traversal Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KMenuBar@<F10>
KMenu@<F4>
KActivate@<Return>
@<Enter>
@MCtrl <Return>
@MCtrl <Enter>
KSelect@<Select>
@<Space>
@MCtrl <Space>
.TE
.P
Once a Menu system is posted the Menu items can be traversed using
.BR KNextMenu ,
.BR KPrevMenu ,
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp .
A posted Menu system behaves somewhat like a field with the
addition of traversing among Menus in the system.
All Menu keyboard traversal actions wrap between the first
and last elements.
.mS
The system must support the Menu traversal behavior
described in the following text.
.mE
.VL 12n
.LI "\fBKNextMenu\fP"
.iX "KNextMenu"
.mS
If the component with the keyboard focus is a CascadeButton,
traverse to the default entry in the associated Cascade Menu,
or to the first traversable entry in the Menu if the Menu has
no default.
.P
Otherwise, if the current Menu has a horizontal
Menu ancestor including the MenuBar,
unpost the current Menu system pulled down from that ancestor and
traverse to the next entry in the horizontal Menu.
If that entry is a CascadeButton,
post its associated PullDown Menu and
traverse to the default entry in the Menu,
or to the first traversable entry in the Menu
if the Menu has no default.
.mE
.P
.B KNextMenu
is usually bound to
.BR "MCtrl <\(da>"
and
.BR "MCtrl <\(->>" .
.LI "\fBKPrevMenu\fP"
.iX "KPrevMenu"
.mS
If the current Menu is the child of a vertical or 2-dimensional Menu,
traverse to the parent CascadeButton.
.P
Otherwise, if the current Menu is the child of a
horizontal Menu or the MenuBar,
unpost the current Menu system pulled down from that parent and
traverse to the previous entry in the horizontal Menu.
If that entry is a CascadeButton,
post its associated PullDown Menu and
traverse to the default entry in the Menu,
or the first traversable entry in the Menu
if the Menu has no default.
.mE
.P
.B KPrevMenu
is usually bound to
.BR "MCtrl <\(ua>"
and
.BR "MCtrl <\(<->" .
.LI "\fBKDown\fP"
.iX "KDown"
.mS
If the component is in a vertical Menu or a 2-dimensional Menu,
traverse down to the next traversable component.
If the component is in a horizontal Menu or a MenuBar,
perform the same action as for
.BR KNextMenu .
.mE
.LI "\fBKUp\fP"
.iX "KUp"
.mS
If the component is in a vertical Menu or a 2-dimensional Menu,
traverse up to the next traversable component.
If the component is in a horizontal Menu,
perform the same action as for
.BR KPrevMenu .
.mE
.LI "\fBKLeft\fP"
.iX "KLeft"
.mS
If the component is in a vertical Menu,
perform the action of
.BR KPrevMenu .
If the component is in a horizontal Menu,
MenuBar, or 2-dimensional Menu,
traverse left to the next traversable component.
.mE
.LI "\fBKRight\fP"
.iX "KRight"
.mS
If the component is in a vertical Menu,
perform the same action as for
.BR KNextMenu .
If the component is in a horizontal Menu,
MenuBar, or 2-dimensional Menu,
traverse right to the next traversable component.
.BR KPrevMenu .
.mE
.LE
.P
.iX "KCancel"
.B KCancel
is used with posted Menus to back out of the Menu
hierarchy.  Its effect depends upon the location of the
cursor:
.BL
.LI
.mS
In a PullDown Menu,
.B KCancel
must dismiss the Menu and
move the location cursor to the CascadeButton used to
pull it down.
.mE
.LI
.mS
In a Popup Menu or Option Menu,
.B KCancel
must dismiss the Menu.
If an explicit focus policy is in use, the
location cursor must be moved back to the component that
previously had it.
.mE
.LI
.mS
In a MenuBar, if an explicit focus policy is in use,
.B KCancel
must move the location cursor to the component that
previously had it.
.mE
.LE
.P
The following table shows the common internal Menu navigation bindings.
.br
.ne 12
.P
.TB "Menu Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KNextMenu@MCtrl <\(da>
@MCtrl <\(->>
KPrevMenu@MCtrl <\(ua>
@MCtrl <\(<->
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
KCancel@<Escape> or <Cancel>
.TE
.H 3 "Scrollable Component Navigation"
.iX "scrolling"
.iX "scrollable component navigation"
.iX "navigation" "scrollable components"
Certain components, such as List and Text, have built-in support
for scrolling.
However, any component or group of components can be associated with
scrolling components or placed inside a ScrolledWindow and made
scrollable.
.P
A scrollable component generally has ScrollBars or some other type
of scrolling component associated with it.  When a component does not
have a scrolling component associated with it, it generally
should not be scrollable; however
components whose visible contents alone indicate that
additional items exist beyond the bounds of the visible area,
like Text,
can be scrollable even if they do not have an associated
scrolling component.
.P
.mS
.iX "scrollable component navigation"
.iX "scrolling"
.iX "navigation" "scrollable components"
Any scrollable component must support appropriate navigation
and scrolling operations.
You must use the page navigation keys
.BR KPageUp ,
.BR KPageDown ,
.BR KPageLeft ,
and
.BR KPageRight
for page-at-a-time scrolling within components or groups of
components that support scrolling.
.mE
A page is the portion of data that is visible,
not any underlying structure of the data.
.mS
When scrolling by a page, you must leave
at least one unit of overlap between the old and new pages;
.mE
for example, a line in a Text component.
.P
You should use the directional keys
.BR KUp ,
.BR KDown ,
.BR KLeft ,
and
.BR KRight
for either moving the location cursor among its elements,
moving it by increments, or
for scrolling the visible region by a regular increment.
In general, keyboard operations should traverse through the entire
scrollable component, not just through the visible portion.
.mS
If the directional keys only traverse through the visible portion,
the ScrollBar (or other scrolling component)
that components the visible portion of the scrollable component
must be traversable.
.mE
.P
.mS
In a vertically scrollable component you should support
.B KBeginData
and
.B KEndData
to move to the beginning and end of the scrollable component.
In a horizontally scrollable component you should support
.B KBeginLine
and
.B KEndLine
to move to the leftmost and rightmost edge of the scrollable component.
.mE
.P
The following table shows the common bindings for
the virtual keys used for scrolled region navigation.
.br
.ne 20
.P
.TB "Scrolled Region Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KUp@<\(ua>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KPageUp@<PageUp>
KPageDown@<PageDown>
KPageLeft@MCtrl <PageUp>
KPageRight@MCtrl <PageDown>
KBeginData@MCtrl KBeginLine
KEndData@MCtrl KEndLine
KBeginLine@<Begin> or <Home>
KEndLine@<End>
.TE
.P
When a mouse button is pressed initiating a selection operation
within a scrollable component, and the pointer
is then dragged outside of the
scrollable component, the component should
scroll towards the pointer.
Drag and drop operations should produce similar scrolling behavior within
scrollable components that force dragged elements to remain within
the component.
.P
When the location cursor is within a scrollable component,
scrolling, either by dragging if the cursor is within the scrollable
component, by using a ScrollBar, or by using the
page navigation keys, may move the cursor out of view;
.mS
however, if the location cursor is within the scrollable component,
any keyboard operation that moves the cursor or that
inserts, deletes, or modifies items at the cursor location must scroll
the component so that the cursor
is visible when the operation is complete.
.mE
When scrolling using the mouse, the location cursor
can be allowed to scroll out of the visible region.
When scrolling using the keyboard, the location cursor should
be moved so that it remains within the visible region.
