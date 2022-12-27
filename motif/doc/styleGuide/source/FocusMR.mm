.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Focus"
.hS "Description"
.iX "implicit focus"
.iX "explicit focus"
.iX "focus" "implicit"
.iX "focus" "explicit"
.iX "focus policies"
.iX "keyboard" "focus"
The keyboard focus model is defined by a focus policies.  A focus policy
is a specific mechanism for moving the focus among windows and components.
.mS
The implicit and explicit focus policies must be supported.
.mE
.P
.iX "implicit focus"
.iX "focus" "implicit"
In the implicit focus policy, also called the pointer,
real-estate driven, or point-to-type policy,
the keyboard focus moves to the window or component into which
a user moves the mouse pointer.
No explicit action is performed to set the keyboard focus in
the implicit focus model.
Keyboard events are sent to the window or component that the mouse pointer is
in, more specifically to the component that the mouse pointer is in.
In implicit mode, the keyboard focus tracks the mouse pointer.
Because of this, there is no
way to move the keyboard focus from the keyboard using implicit mode.
In this focus policy, the location cursor for keyboard events
does not need to be shown; however, the application can show it.
.P
The mouse pointer always behaves as if the focus policy is implicit.
That is, mouse events always go to the window and the component that the
mouse pointer is in.
.P
.iX "explicit focus"
.iX "focus" "explicit"
.iX "click-to-type"
The explicit focus policy requires the user to explicitly select
which window or component receives the keyboard focus.
.mS
Mouse events are sent to the component that the pointer is over,
so the mouse pointer must always behave as if the focus policy
is implicit, even when the focus policy is explicit.
.mE
In explicit focus mode, a
user moves the keyboard focus to a window or component by pressing
.B BSelect
while the mouse pointer is over a window or component.
Simply moving the mouse
pointer over a window or component does not give the window or component
the keyboard focus.
.mS
In this focus policy, the location of keyboard focus must be shown
by a location cursor.
.mE
.P
In explicit mode, keyboard focus moves explicitly among the components in a window
as well as among the windows.
Moving the keyboard focus among components in a window is called window
navigation.
Keyboard events go to the component
in the window with the keyboard focus.
.hS "Related Information"
See Chapter 2 for more information about the focus model.
