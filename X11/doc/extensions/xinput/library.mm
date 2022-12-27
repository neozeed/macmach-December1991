.\" Input Extension
.\"
.\" This report uses the -mm macro package for nroff.
.\" Use the script "proofit" to review this chapter using the
.\" X-windows proof utility.  
.\"
.\"
.so TitlePage
.\"  Force the heading counter for level 1 to one
.\"
.nr Ej 1
.\"
.\"
.\"  Print table of contents to level 4 headings
.\"
.nr Cl 4
.\"
.\"  Page eject for each level 1 heading
.\"
.nr H1 1
.nr P 1
.\"
.\"  Define Ch to contain the chapter string.
.\"
.ds Ch Input Extension Overview
.\"
.\"
.\"  Pull in the layout macro package.
.\"
.so Format
.\"
.tr ~
This document describes an extension to
the X11 server.  The purpose of this extension is to support the use
of additional input devices beyond the pointer and keyboard devices
defined by the core X protocol.  This first section gives an overview
of the input extension.  The following sections correspond to 
chapters 7 and 8, "Window Manager functions" and 
"Events and Event-Handling Functions" of the "Xlib - C Language Interface"
manual and describe how to use the input extension.
.H 2 "Design Approach"
The design approach of the extension is to define functions
and events analogous to the core functions and events. This allows
extension input devices and events to be individually distinguishable from each other 
and from the core input devices and events .  These functions and events make use
of a device identifier and support the
reporting of n-dimensional motion data as well as other data that
is not currently reportable via the core input events.
.H 2 "Core Input Devices"
The X server core protocol supports two input devices:  a pointer and a
keyboard.  The pointer device has two major functions. 
First, it may be used to generate motion information
that client programs can detect. Second, it may also be used to indicate the
current location and focus of the X keyboard.  To accomplish this, the server 
echoes a cursor at the current position of the X pointer.  Unless the X
keyboard has been explicitly focused, this cursor also shows the current
location and focus of the X keyboard.
.P
The X keyboard is used to generate input that client programs can detect.
.P
The X keyboard and X pointer are referred to in this document as 
the \fIcore devices\fP, and the input
events they generate (\fBKeyPress\fP, \fBKeyRelease\fP, \fBButtonPress\fP, 
\fBButtonRelease\fP, and
\fBMotionNotify\fP) are known as the \fIcore input events\fP.  All other
input devices are referred to as \fIextension input devices\fP and the 
input events they generate are referred to as \fIextension input events\fP.
.nS
This input extension does not change the behavior or functionality of the
core input devices, core events, or core protocol requests, with the
exception of the core grab requests.  These requests may affect the
synchronization of events from extension devices.  See the explanation
in the section titled "Event Synchronization and Core Grabs".
.xE
.P
Selection of the physical devices to be initially used by the server as the 
core devices is left implementation-dependent.  Functions are defined that
allow client programs to change which physical devices are used as the
core devices.
.H 2 "Extension Input Devices"
The input extension controls access to input devices other than the X keyboard
and X pointer.  It allows client programs to select input from these devices 
independently
from each other and independently from the core devices.  Input events from
these devices are of extension types (\fBDeviceKeyPress\fP, 
\fBDeviceKeyRelease\fP,
\fBDeviceButtonPress\fP, \fBDeviceButtonRelease\fP, \fBDeviceMotionNotify\fP, 
etc.) and contain
a device identifier so that events of the same type coming from different
input devices can be distinguished.
.P
Extension input events are not limited in size by the size of the server
32-byte wire events.  Extension input events
may be constructed by the server sending as many
wire sized events as necessary to return the information required for
that event.
The library event reformatting routines
are responsible for combining these into one or more client XEvents.
.P
Any input device that generates key, button or motion data may be used as
an extension input device.
Extension input devices may have 0 or more keys, 0 or more buttons,
and may report 0 or more axes of motion.  Motion may be reported 
as relative movements from a previous position or as an absolute
position.  All valuators reporting motion information for a given
extension input device must report the same kind of motion information
(absolute or relative).
.P
This extension is designed to accommodate new types of input devices that
may be added in the future.  The protocol requests that refer to
specific characteristics of input devices organize that information
by \fBinput device classes\fP.  Server implementors may add new
classes of input devices without changing the protocol requests.
.P
All extension input
devices are treated like the core X keyboard in determining their location
and focus.  The server does not track the location of these devices on an
individual basis, and therefore
does not echo a cursor to indicate their current location.
Instead, their location is determined by the location of the core X pointer.
Like the core X keyboard, they may be explicitly focused. If they are
not explicitly focused,  their focus
is determined by the location of the core X pointer.
.H 3 "Input Device Classes"
Some of the input extension functions divide input devices into classes
based on their functionality.  This is intended to allow new classes of input
devices to be defined at a later time without changing the semantics of 
these functions.  The following input device classes are currently
defined:
.VL 20
.LI \fBKEY\fP
The device reports key events.
.LI \fBBUTTON\fP
The device reports button events.
.LI \fBVALUATOR\fP
The device reports valuator data in motion events.
.LI \fBPROXIMITY\fP
The device reports proximity events.
.LI \fBFOCUS\fP
The device can be focused.
.LI \fBFEEDBACK\fP
The device supports feedbacks.
.LE
.P
Additional classes may be added in the future.
Functions that support multiple input classes, such as the 
\fBXListInputDevices\fP function that lists all available input devices,
organize the data they return by input class.  Client programs that
use these functions should not access data unless it matches a 
class defined at the time those clients were compiled.  In this way,
new classes can be added without forcing existing clients that use
these functions to be recompiled.
.H 1 "Library Extension Functions"
Extension input devices are accessed by client programs through the 
use of new protocol requests.
The following requests are provided as extensions to Xlib.  Constants
and structures referenced by these functions may be found in the 
file \fBXInput.h\fP, which is attached to this document as appendix A.
.P
The library will return NoSuchExtension if an extension request
is made to a server that does not support the input extension.
.H 2 "Window Manager Functions"
.H 3 "Changing The Core Devices"
These functions are provided to change which physical device is used
as the X pointer or X keyboard.
.nS
Using these functions may change the characteristics of the core devices.
The new pointer device may have a different number of buttons than the 
old one did, or the new keyboard device may have a different number of
keys or report a different range of keycodes.  Client programs may be
running that depend on those characteristics.  For example, a client
program could allocate an array based on the number of buttons on the
pointer device, and then use the button numbers received in button events
as indicies into that array.  Changing the core devices could cause
such client programs to behave improperly or abnormally terminate,
if they ignore the ChangeDeviceNotify event generated by these requests.
.xE
.P
These functions change the X keyboard or X pointer device and generate
an \fBXChangeDeviceNotify\fP event and a \fBMappingNotify\fP event.  
The specified device becomes the
new X keyboard or X pointer device.  The location of the core device
does not change as a result of this request.
.P
These requests fail and return \fBDeviceGrabbed\fP if either the specified
device or the core device it would replace are grabbed by some other
client.  They fail and return \fBDeviceFrozen\fP if either device is frozen
by the active grab of another client.
.P
These requests fail with a \fBBadDevice\fP error if the specified device is
invalid, or has not previously been opened via \fBXOpenDevice\fP.
.SP 2
.P
The specified device must support input class \fBKeys\fP (as reported in the
\fBListInputDevices\fP request) or the request will fail with a 
\fBBadMatch\fP error.
Once the device has successfully replaced one of the core devices, it
is treated as a core device until it is in turn replaced by another
\fBChangeDevice\fP request, or until the server terminates.  The termination
of the client that changed  the device will not cause it to change back.
Attempts to use the \fBXCloseDevice\fP request to close the new core device will
fail with a \fBBadDevice\fP error.
.P
To change which physical device is used as the X keyboard, use
the \fBXChangeKeyboardDevice\fP function.
.P
.DS
\f(CW
int
XChangeKeyboardDevice (display, device)
    Display *display;
    XDevice *device;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LE
.P
If no error occurs, this function returns \fBSuccess\fP.
.mc >
The focus state of the new keyboard is the same as
the focus state of the old X keyboard.
.mc
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP,
\fBDeviceGrabbed\fP.
.SP 2
.P
To change which physical device is used as the X pointer,
use the \fBXChangePointerDevice\fP function.
The specified device must support input class \fBValuators\fP (as reported in 
the \fBXListInputDevices request\fP) and report at least two axes of motion,
or the request will fail with a \fBBadMatch\fP error.
If the specified device reports more than two axes, the two specified in
the xaxis and yaxis arguments will be used.  Data from other
valuators on the device will be ignored.
.P
.mc >
If the specified device reports absolute positional information, and the 
server implementation does not allow such a device to be used as the 
X pointer, the request will fail with a BadDevice error.
.mc
Once the device has successfully replaced one of the core devices, it
is treated as a core device until it is in turn replaced by another
\fBXChangeDevice\fP request, or until the server terminates.  The termination
of the client that changed  the device will not cause it to change back.
Attempts to use the \fBXCloseDevice\fP request to close the new core device will
fail with a \fBBadDevice\fP error.
.mc
.DS
\f(CW
int
XChangePointerDevice (display, device, xaxis, yaxis)
    Display *display;
    XDevice *device;
    int     xaxis;
    int     yaxis;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIxaxis\fP
Specifies the index of the axis to be used as the x-axis of the pointer device.
.LI \fIyaxis\fP
Specifies the index of the axis to be used as the y-axis of the pointer device.
.LE
.P
If no error occurs, this function returns \fBSuccess\fP.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP,
\fBDeviceGrabbed\fP.
.H 3 "Event Synchronization And Core Grabs"
Implementation of the input extension requires an extension of the
meaning of event synchronization for the core grab requests.  This is
necessary in order to allow window managers to freeze all input devices
with a single request.
.P
The core grab requests require a \fBpointer_mode\fP and \fBkeyboard_mode\fP
argument.  The meaning of these modes is changed by the input extension.
For the \fBXGrabPointer\fP and \fBXGrabButton\fP requests, \fBpointer_mode\fP
controls synchronization of the pointer device, and \fBkeyboard_mode\fP
controls the synchronization of all other input devices.  
For the \fBXGrabKeyboard\fP
and \fBXGrabKey\fP requests, \fBpointer_mode\fP controls the synchronization
of all input devices except the X keyboard, while \fBkeyboard_mode\fP controls
the synchronization of the keyboard.  When using one of the core grab
requests, the synchronization of extension devices
is controlled by the mode specified for the device not being grabbed.
.H 3 "Extension Active Grabs"
Active grabs of
extension devices are supported via the 
\fBXGrabDevice\fP
function in the same way that core devices are grabbed using
the core \fBXGrabKeyboard\fP function, except that a \fIDevice\fP is passed as
a function parameter.  
The \fBXUngrabDevice\fP function allows a
previous active grab for an extension device to be released.
.P
Passive grabs of buttons and keys on extension devices are supported
via the \fBXGrabDeviceButton\fP and \fBXGrabDeviceKey\fP functions.
These passive grabs are released via the \fBXUngrabDeviceKey\fP and
\fBXUngrabDeviceButton\fP functions.
.SP 2
To grab an extension device, use the \fBXGrabDevice\fP function.
The device must have previously been opened using the
\fBXOpenDevice\fP function.
.DS
\f(CW
int
XGrabDevice (display, device, grab_window, owner_events, 
    event_count, event_list, this_device_mode, other_device_mode, time)
        Display     *display;
        XDevice     *device;
        Window      grab_window;
        Bool        owner_events;
        int         event_count;
        XEventClass *event_list;
        int         this_device_mode;
        int         other_device_mode;
        Time        time;
\fP
.DE
.VL 25
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIgrab_window\fP
Specifies the ID of a window associated with the device specified above.
.LI \fIowner_events\fP
Specifies a boolean value of either \fBTrue\fP or \fBFalse\fP.
.LI \fIevent_count\fP
Specifies the number of elements in the event_list array.
.LI \fIevent_list\fP
Specifies a pointer to a list of event classes that indicate which events
the client wishes to receive.  
.mc >
These event classes must have been obtained
using the device being grabbed.
.LI \fIthis_device_mode\fP
.mc
Controls further processing of events from this device.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LI \fIother_device_mode\fP
Controls further processing of events from all other devices.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LI \fItime\fP
Specifies the time.  This may be either a timestamp expressed in
milliseconds, or \fBCurrentTime\fP.
.LE
.P
The \fBXGrabDevice\fP function actively grabs an 
extension input device, and generates 
\fBDeviceFocusIn\fP and \fBDeviceFocusOut\fP events.  Further input events from
this device are reported only to the grabbing client.  This function
overrides any previous active grab by this client for this 
device.  
.P
The event-list parameter is a pointer to a list of event classes.  This list
indicates which events the client wishes to receive while the grab is active.
If owner_events
is \fBFalse\fP, input events from this device are reported with respect to 
grab_window and are only reported if specified in event_list.
If owner_events is \fBTrue\fP, then if a generated event would
normally be reported to this client, it is reported normally. Otherwise
the event is reported with respect to the grab_window, and is only
reported if specified in event_list.
.P
The this_device_mode argument controls the further processing 
of events from this device, and the other_device_mode argument controls
the further processing of input events from all other devices.
.BL
.LI
If the this_device_mode argument is 
\fBGrabModeAsync\fP,
device event processing continues
normally; if the device is currently frozen by this client, then
processing of device events is resumed.
If the this_device_mode  argument is
\fBGrabModeSync\fP,
the state of the grabbed device
(as seen by client applications) appears to freeze,
and no further device events are generated by the server until the
grabbing client issues a releasing 
\fBXAllowDeviceEvents\fP
call or until the device grab is released.
Actual
device input events are not lost while the device is frozen; they are
simply queued for later processing.
.LI
If the other_device_mode is 
\fBGrabModeAsync\fP,
event processing from other devices is unaffected
by activation of the grab.  
If other_device_mode is \fBGrabModeSync\fP,
the state of all devices except the grabbed device
(as seen by client applications) appears to freeze, and no further
events are generated by the server until the grabbing client issues a
releasing 
\fBXAllowEvents\fP or \fBXAllowDeviceEvents\fP
call or until the device grab is released.
Actual events are not lost
while the other devices are frozen; they are simply queued for later
processing.
.P
\fBXGrabDevice\fP fails and returns:
.BL
.LI
\fBAlreadyGrabbed\fP
If the device is actively grabbed by some other client.
.LI
\fBGrabNotViewable\fP
If grab_window is not viewable.
.LI
\fBGrabInvalidTime\fP
If the specified time is earlier
than the last-grab-time for the specified device
or later than the current X server time. Otherwise,
the last-grab-time for the specified device is set
to the specified time and 
\fBCurrentTime\fP
is replaced by the current X server time.
.LI
\fBGrabFrozen\fP
If the device is frozen by an active grab of another client.
.LE
.P
If a grabbed device is closed by a client while an active grab by that 
client is in
effect, that active grab will be released.  Any passive grabs established by
that client will be released.  If the device is frozen only by an active grab
of the requesting client, it is thawed.
.mc >
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadWindow\fP, 
\fBBadValue\fP, \fBBadClass\fP.
.mc
.SP 2
To release a grab of an extension device, use \fBXUngrabDevice\fP.
.DS
\f(CW
int
XUngrabDevice (display, device, time)
        Display *display;
        XDevice *device;
        Time    time;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fItime\fP
Specifies the time.  This may be either a timestamp expressed in
milliseconds, or \fBCurrentTime\fP.
.LE
.P
This function allows a client to release an extension input device and any
queued events if this client has it grabbed from either \fBXGrabDevice\fP
or \fBXGrabDeviceKey\fP.  If any other devices are frozen by the grab,
\fBXUngrabDevice\fP thaws them. 
The function does not release the device and any
queued events if the specified time is earlier than the last-device-grab
time or is later than the current X server time.  It also generates 
\fBDeviceFocusIn\fP and \fBDeviceFocusOut\fP events.  The X server 
automatically performs an \fBXUngrabDevice\fP if the event window for an
active device grab becomes not viewable, or if the client terminates without
releasing the grab.
.P
Errors returned by this function:  \fBBadDevice\fP.
.SP 2
.H 3 "Passively Grabbing A Key"
To passively grab a single key on an extension device, use \fBXGrabDeviceKey\fP.
That device must have previously been opened using the
\fBXOpenDevice\fP function, or the request will fail with a \fBBadDevice\fP
error.  
If the specified device does not support input class \fBKeys\fP,
the request will fail with a \fBBadMatch\fP error.
.DS
\f(CW
int
XGrabDeviceKey (display, device, keycode, modifiers, modifier_device
    grab_window, owner_events, event_count, event_list, this_device_mode, 
    other_device_mode)
        Display     *display;
        XDevice     *device;
        int         keycode;
        unsigned    int modifiers;
        XDevice     *modifier_device;
        Window      grab_window;
        Bool        owner_events;
        int         event_count;
        XEventClass *event_list;
        int         this_device_mode;
        int         other_device_mode;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIkeycode\fP
Specifies the keycode of the key that is to be grabbed.  You can pass
either the keycode or \fBAnyKey\fR.
.LI \fImodifiers\fP
Specifies the set of keymasks.  This mask is the bitwise inclusive OR
of these keymask bits:  \fBShiftMask\fR, \fBLockMask\fR, \fBControlMask\fR, 
\fBMod1Mask\fR, \fBMod2Mask\fR, \fBMod3Mask\fR, \fBMod4Mask\fR, \fBMod5Mask\fR.
.LI ~
You can also pass \fBAnyModifier\fP, which is equivalent to issuing the grab 
key request for all possible modifier combinations (including the combination
of no modifiers).
.LI \fImodifier_device\fP
Specifies the device whose modifiers are to be used.
.LI \fIgrab_window\fP
Specifies the ID of a window associated with the device specified above.
.LI \fIowner_events\fP
Specifies a boolean value of either \fBTrue\fR or \fBFalse\fR.
.LI \fIevent_count\fP
Specifies the number of elements in the event_list array.
.LI \fIevent_list\fP
Specifies a pointer to a list of event classes that indicate which events
the client wishes to receive.
.LI \fIthis_device_mode\fP
Controls further processing of events from this device.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LI \fIother_device_mode\fP
Controls further processing of events from all other devices.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LE
.P
This function is analogous to the core \fBXGrabKey\fP function.  It creates an
explicit passive grab for a key on an extension device.
.P
The \fBXGrabDeviceKey\fP function establishes a passive grab on a device.
Consequently, in the future, 
.BL
.LI
IF the device is not grabbed and the specified key, 
which itself can be a modifier key, is logically pressed
when the specified modifier keys logically are down on the specified
modifier device
(and no other keys are down),
.LI
AND no other modifier keys logically are down,
.LI
AND EITHER the grab window is an ancestor of (or is) the focus window
OR the grab window is a descendent of the focus window and contains the pointer,
.LI
AND a passive grab on the same device and key combination does not exist on any
ancestor of the grab window,
.LI
THEN the device is actively grabbed, as for \fBXGrabDevice\fP,
the last-device-grab time is set to the time at which the key was pressed
(as transmitted in the \fBDeviceKeyPress\fP event), and the 
\fBDeviceKeyPress\fP event is reported.
.LE
.P
The interpretation of the remaining arguments is as for \fBXGrabDevice\fP.
The active grab is terminated automatically when logical state of the
device has the specified key released
(independent of the logical state of the modifier keys).
.P
Note that the logical state of a device (as seen by means of the X protocol)
may lag the physical state if device event processing is frozen.
.P
A modifier of \fBAnyModifier\fP is equivalent to issuing the request for all
possible modifier combinations (including the combination of no modifiers).  
It is not required that all modifiers specified have
currently assigned keycodes.
A key of \fBAnyKey\fP is equivalent to issuing
the request for all possible keycodes.  Otherwise, the key must be in
the range specified by min_keycode and max_keycode in the 
information returned by the \fBXListInputDevices\fP
function.
If it is not within that range, \fBXGrabDeviceKey\fP generates a
\fBBadValue\fP error.
.P
A \fBBadAccess\fP error is generated if some other client has issued a 
\fBXGrabDeviceKey\fP with the same device and key combination on the 
same window.  When using \fBAnyModifier\fP or \fBAnyKey\fP,
the request fails completely and the X server generates a \fBBadAccess\fP
error and no grabs are
established if there is a conflicting grab for any combination.
.P
\fBXGrabDeviceKey\fP can generate \fBBadDevice\fP, \fBBadAccess\fP,
\fBBadMatch\fP, \fBBadWindow\fP, and \fBBadValue\fP errors.
.P
\fBXGrabDeviceKey\fP returns \fBSuccess\fP upon successful completion of the
request.
.P
This function cannot be used to grab a key on the X keyboard
device.  The core \fBXGrabKey\fP function should be used for that purpose.
.SP 2
To release a passive grab of a single key on an extension device, 
use \fBXUngrabDeviceKey\fP.
.DS
\f(CW
int
XUngrabDeviceKey (display, device, keycode, modifiers, modifier_device,
    ungrab_window)
        Display  *display;
        XDevice  *device;
        int      keycode;
        unsigned int modifiers;
        XDevice  *modifier_device;
        Window   ungrab_window;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIkeycode\fP
Specifies the keycode of the key that is to be ungrabbed.  You can pass
either the keycode or \fBAnyKey\fR.
.LI \fImodifiers\fP
Specifies the set of keymasks.  This mask is the bitwise inclusive OR
of these keymask bits:  \fBShiftMask\fR, \fBLockMask\fR, \fBControlMask\fR, 
\fBMod1Mask\fR, \fBMod2Mask\fR, \fBMod3Mask\fR, \fBMod4Mask\fR, \fBMod5Mask\fR.
.LI ~
You can also pass \fBAnyModifier\fP, which is equivalent to issuing the ungrab key 
request for all possible modifier combinations (including the combination
of no modifiers).
.LI \fImodifier_device\fP
Specifies the device whose modifiers are to be used.
.LI \fIungrab_window\fP
Specifies the ID of a window associated with the device specified above.
.LE
.PP
.SH DESCRIPTION
.P
This function is analogous to the core \fBXUngrabKey\fP function.  It releases
an explicit passive grab for a key on an extension input device.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP.
.SP 2
.H 3 "Passively Grabbing A Button"
To establish a passive grab for a single button on an extension device,
use \fBXGrabDeviceButton\fP.
The specified device must have previously been opened using the
\fBXOpenDevice\fP function, or the request will fail with a \fBBadDevice\fP
error.  If the specified device does not support input class \fBButtons\fP,
the request will fail with a \fBBadMatch\fP error.
.DS
\f(CW
int
XGrabDeviceButton (display, device, button, modifiers, modifier_device,
    grab_window, owner_events, event_mask, this_device_mode, other_device_mode)
        Display      *display;
        XDevice      *device;
        unsigned int button;
        unsigned int modifiers;
        XDevice      *modifier_device;
        Window       grab_window;
        Bool         owner_events;
        int          event_count;
        XEventClass  *event_list;
        int          this_device_mode;
        int          other_device_mode;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIbutton\fP
Specifies the code of the button that is to be grabbed.  You can pass
either the button or \fBAnyButton\fR.
.LI \fImodifiers\fP
Specifies the set of keymasks.  This mask is the bitwise inclusive OR
of these keymask bits:  \fBShiftMask\fR, \fBLockMask\fR, \fBControlMask\fR, 
\fBMod1Mask\fR, \fBMod2Mask\fR, \fBMod3Mask\fR, \fBMod4Mask\fR, \fBMod5Mask\fR.
.LI ~
You can also pass \fBAnyModifier\fP, which is equivalent to issuing the grab
request for all possible modifier combinations (including the combination
of no modifiers).
.LI \fImodifier_device\fP
Specifies the device whose modifiers are to be used.
.LI \fIgrab_window\fP
Specifies the ID of a window associated with the device specified above.
.LI \fIowner_events
Specifies a boolean value of either \fBTrue\fR or \fBFalse\fR.
.LI \fIevent_count\fP
Specifies the number of elements in the event_list array.
.LI \fIevent_list\fP
Specifies a list of event classes that indicates which device events are to be 
reported to the client.
.LI \fIthis_device_mode\fP
Controls further processing of events from this device.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LI \fIother_device_mode\fP
Controls further processing of events from all other devices.  You can pass one
of these constants: \fBGrabModeSync\fP or \fBGrabModeAsync\fP.
.LE
.P
This function is analogous to the core \fBXGrabButton function\fP.  
It creates an
explicit passive grab for a button on an extension input device.  Since the
server does not track extension devices, no cursor is specified with this
request.  For the same reason, there is no confine_to parameter.
The device must have previously been opened using the
\fBXOpenDevice\fP function.
.P
The \fBXGrabDeviceButton\fP function establishes a passive grab on a device.
Consequently, in the future, 
.BL
.LI
IF the device is not grabbed and the specified button is logically pressed
when the specified modifier keys logically are down 
(and no other buttons or modifier keys are down),
.LI
AND EITHER the grab window is an ancestor of (or is) the focus window
OR the grab window is a descendent of the focus window and contains the pointer,
.LI
AND a passive grab on the same device and button/ key combination does not 
exist on any ancestor of the grab window,
.LI
THEN the device is actively grabbed, as for \fBXGrabDevice\fP,
the last-grab time is set to the time at which the button was pressed
(as transmitted in the \fBDeviceButtonPress\fP event), and the 
\fBDeviceButtonPress\fP event is reported.
.LE
.P
The interpretation of the remaining arguments is as for 
\fBXGrabDevice\fP.
The active grab is terminated automatically when logical state of the
device has all buttons released (independent of the logical state of 
the modifier keys).
.P
Note that the logical state of a device (as seen by means of the X protocol)
may lag the physical state if device event processing is frozen.
.P
A modifier of \fBAnyModifier\fP
is equivalent to issuing the request for all
possible modifier combinations (including the combination of no
modifiers).  
It is not required that all modifiers specified have
currently assigned keycodes.
A button of \fBAnyButton\fP is equivalent to issuing
the request for all possible buttons.
Otherwise, it is not required that the 
specified button be assigned to a physical button.
.P
A 
\fBBadAccess\fP error is generated if some other client has issued a 
\fBXGrabDeviceButton\fP
with the same device and button combination on the same window.  
When using \fBAnyModifier\fP or \fBAnyButton\fP, the request fails completely 
and the X server generates a \fBBadAccess\fP
error and no grabs are
established if there is a conflicting grab for any combination.
.P
\fBXGrabDeviceButton\fP can generate \fBBadDevice\fP, \fBBadMatch\fP,
\fBBadAccess\fP, \fBBadWindow\fP, and \fBBadValue\fP errors.
.P
This function cannot be used to grab a button on the X pointer
device.  The core \fBXGrabButton\fP function should be used for that
purpose.
.SP 2
To release a passive grab of a button on an extension device, use 
\fBXUngrabDeviceButton\fP.
.DS
\f(CW
int
XUngrabDeviceButton (display, device, button, modifiers, modifier_device,
    ungrab_window)
        Display  *display;
        XDevice  *device;
        unsigned int button;
        unsigned int modifiers;
        XDevice  *modifier_device;
        Window   ungrab_window;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIbutton\fP
Specifies the code of the button that is to be ungrabbed.  You can pass
either a button or \fBAnyButton\fR.
.LI \fImodifiers\fP
Specifies the set of keymasks.  This mask is the bitwise inclusive OR
of these keymask bits:  \fBShiftMask\fR, \fBLockMask\fR, \fBControlMask\fR, 
\fBMod1Mask\fR, \fBMod2Mask\fR, \fBMod3Mask\fR, \fBMod4Mask\fR, \fBMod5Mask\fR.
.LI ~
You can also pass \fBAnyModifier\fP, which is equivalent to issuing the ungrab key 
request for all possible modifier combinations (including the combination
of no modifiers).
.LI \fImodifier_device\fP
Specifies the device whose modifiers are to be used.
.LI \fIungrab_window\fP
Specifies the ID of a window associated with the device specified above.
.LE
.P
This function is analogous to the core \fBXUngrabDeviceButton\fP function.  It
releases an explicit passive grab for a button on an extension device.
That device must have previously been opened using the
\fBXOpenDevice\fP function, or a \fBBadDevice\fP error will result.
.P
A modifier of \fBAnyModifier\fP
is equivalent to issuing the request for all
possible modifier combinations (including the combination of no
modifiers).  
.P
\fBXUngrabDeviceButton\fP can generate \fBBadDevice\fP, \fBBadMatch\fP, and
\fBBadWindow\fP errors.
.P
This function cannot be used to ungrab a button on the X pointer
device.  The core \fBXUngrabButton\fP function
should be used for that purpose.
.H 3 "Thawing A Device"
To allow further events to be processed when a device has been frozen,
use \fBXAllowDeviceEvents\fR.
.DS
\f(CW
int
XAllowDeviceEvents (display, device, event_mode, time)
        Display *display;
        XDevice *device;
        int     event_mode;
        Time    time;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIevent_mode\fP
Specifies the event mode.  You can pass one of these constants:
\fBAsyncThisDevice\fP, \fBSyncThisDevice\fP, \fBAsyncOtherDevices\fP,
\fBReplayThisDevice\fP, \fBAsyncAllDevices\fP, or \fBSyncAllDevices\fP.
.LI \fItime\fP
Specifies the time.  This may be either a timestamp expressed in
milliseconds, or \fBCurrentTime\fP.
.LE
.P
The \fBXAllowDeviceEvents\fP function releases some queued events if the client
has caused a device to freeze.
The function has no effect if the specified time is earlier than the last-grab 
time of the most recent active grab for the client, or if the specified time
is later than the current X server time.
The following describes the processing that occurs depending on what constant
you pass to the event_mode argument:
.BL
.LI \fBAsyncThisDevice\fP
If the specified device is frozen by the client, event processing for that 
.mc >
continues as usual.  If the device is frozen multiple times  by the client on 
behalf of multiple separate grabs, AsyncThisDevice thaws for all.
.mc
AsyncThisDevice has no effect if the specified device is not frozen by the 
client, but the device need not be grabbed by the client.
.LI \fBSyncThisDevice\fP 
If the specified device is frozen and actively grabbed by the client,
event processing for that device continues normally until the next 
key or button event is reported to the client.
At this time, 
the specified device again appears to freeze.
However, if the reported event causes the grab to be released,
the specified device does not freeze.
SyncThisDevice has no effect if the specified device is not frozen by the client
or is not grabbed by the client.
.LI \fBReplayThisDevice\fP
If the specified device is actively grabbed by the client and is frozen as the result of
an event having been sent to the client (either from the activation of a 
GrabDeviceButton or from a previous AllowDeviceEvents with mode SyncThisDevice, 
but not from a Grab),
the grab is released and that event is completely reprocessed.
This time, however, the request ignores any passive grabs at or above 
(towards the root) the grab-window of the grab just released.
The request has no effect if the specified device is not grabbed by the client
or if it is not frozen as the result of an event.
.LI \fBAsyncOtherDevices\fP
If the remaining devices are frozen by the client,
event processing for them continues as usual.
.mc >
If the other devices are frozen multiple times  by the client on behalf of 
multiple separate grabs,
AsyncOtherDevices ``thaws'' for all.
.mc
AsyncOtherDevices has no effect if the devices are not frozen by the client,
but those devices need not be grabbed by the client.
.LI \fBSyncAllDevices\fP
If all devices are frozen by the client,
event processing (for all devices) continues normally until the next
button or key event is reported
to the client for a grabbed device (button event for the grabbed device, key
or motion event for the device), at which time the devices again appear to
freeze.  However, if the reported event causes the grab to be released,
then the devices do not freeze (but if any device is still
grabbed, then a subsequent event for it will still cause all devices
to freeze).  
SyncAllDevices has no effect unless all devices
are frozen by the client.  If any device is frozen twice
by the client on behalf of two separate grabs, 
SyncAllDevices "thaws" for both (but a subsequent freeze for SyncAllDevices
will only freeze each device once).
.LI \fBAsyncAllDevices\fP
If all devices are frozen by the
client, event processing (for all devices) continues normally.
.mc >
If any device is frozen multiple times by the client on behalf of multiple
separate grabs, AsyncAllDevices "thaws" for all.
.mc
If any device is frozen twice by the client on behalf of two separate grabs,
AsyncAllDevices "thaws" for both.
AsyncAllDevices has no effect unless all
devices are frozen by the client.
.LE
.P
AsyncThisDevice, SyncThisDevice, and ReplayThisDevice 
have no effect on the processing of events from the remaining devices.
AsyncOtherDevices
has no effect on the processing of events from the specified device.
When the event_mode is SyncAllDevices or AsyncAllDevices, the 
device parameter is ignored.
.P
It is possible for several grabs of different devices (by the same 
or different clients) to be active simultaneously.
If a device is frozen on behalf of any grab,
no event processing is performed for the device.
It is possible for a single device to be frozen because of several grabs.
In this case,
the freeze must be released on behalf of each grab before events can 
again be processed.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadValue\fP.
.H 3 "Controlling Device Focus"
The current focus window for an extension input device can be 
determined using the \fBXGetDeviceFocus\fP function.
Extension devices are focused using the \fBXSetDeviceFocus\fP
function in the same way that the keyboard is focused using
the core \fBXSetInputFocus\fP function, except that a device id is passed as
a function parameter.  One additional focus state, \fBFollowKeyboard\fP,
is provided for extension devices.
.P
To get the current focus state, revert state, and focus time of an extension device,
use \fBXGetDeviceFocus\fP.
.DS
\f(CW
int
XGetDeviceFocus (display, device, focus_return, revert_to_return,
    focus_time_return)
        Display *display;
        XDevice *device;
        Window  *focus_return;
        int     *revert_to_return;
        Time    *focus_time_return;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIfocus_return\fP
Specifies the address of a variable into which the server can return the ID of
the window that contains the device focus, 
or one of the constants \fBNone\fP,
\fBPointerRoot\fP, or \fBFollowKeyboard\fP.  
.LI \fIrevert_to_return\fP
Specifies the address of a variable into which the server can
return the current revert_to status for the device.
.LI \fIfocus_time_return\fP
Specifies the address of a variable into which the server can
return the focus time last set for the device.
.LE
.P
This function returns the focus state, the revert-to state,
and the last-focus-time for an extension input device.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP.
.SP 2
To set the focus of an extension device, use \fBXSetDeviceFocus\fP.
.DS
\f(CW
int
XSetDeviceFocus (display, device, focus, revert_to, time)
        Display *display;
        XDevice *device;
        Window  focus;
        int     revert_to;
        Time    time;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIfocus\fP
Specifies the id of the window to which the device's focus should be set.
This may be a window id, or
\fBPointerRoot\fP, \fBFollowKeyboard\fP, or \fBNone\fP.
.LI \fIrevert_to\fP
Specifies to which window the focus of the device should revert
if the focus window becomes not viewable.  One of the following
constants may be passed:
\fBRevertToParent\fP, \fBRevertToPointerRoot\fP,
\fBRevertToNone\fP, or \fBRevertToFollowKeyboard\fP.
.LI \fItime\fP
Specifies the time.  You can pass either a timestamp, expressed in
milliseconds, or \fBCurrentTime\fP.
.LE
.P
This function changes the focus for an extension input device and the 
last-focus-change-time.  The function has no effect if the specified 
time is earlier than the last-focus-change-time or is later than the
current X server time.  Otherwise, the last-focus-change-time is set to the
specified time.
This function causes the X server to generate \fBDeviceFocusIn\fP and 
\fBDeviceFocusOut\fP events.
.P
The action taken by the server when this function is requested depends
on the value of the focus argument:
.BL
.LI
If the focus argument is \fBNone\fP, all input events from this device
will be discarded until a new focus window is set.  In this case, the
revert_to argument is ignored.
.LI
If a window ID is assigned to the focus argument, it becomes the focus
window of the device.  If an input event from the device would normally
be reported to this window or to one of its inferiors, the event is 
reported normally.  Otherwise, the event is reported relative to the focus 
window.
.LI
If you assign \fBPointerRoot\fP to the focus argument, the focus window is 
dynamically taken to be the root window of whatever screen the pointer is
on at each input event.  In this case, the revert_to argument is ignored.
.LI
If you assign \fBFollowKeyboard\fP to the focus argument, the focus window is 
dynamically taken to be the same as the focus of the X keyboard at each
input event.
.LE
.P
The specified focus window must be viewable at the time \fBXSetDeviceFocus\fP
is called.  Otherwise, it generates a BadMatch error.  If the focus window
later becomes not viewable, the X server evaluates the revert_to argument
to determine the new focus window.
.BL
.LI
If you assign \fBRevertToParent\fP
to the revert_to argument, the focus reverts to the parent
(or the closest viewable ancestor), and the new revert_to value is taken to
be \fBRevertToNone\fP.  
.LI
If you assign \fBRevertToPointerRoot\fP, \fBRevertToFollowKeyboard\fP,
or \fBRevertToNone\fP
to the revert_to argument, the focus reverts to that value.
.LE
.P
When the focus reverts,
the X server generates \fBDeviceFocusIn\fP
and \fBDeviceFocusOut\fP
events, but the last-focus-change time is not affected.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP, 
\fBBadValue\fP, and \fBBadWindow\fP.
.H 3 "Controlling Device Feedback"
To determine the current feedback settings of an extension input device, 
use \fBXGetFeedbackControl]\fP.
To change the current feedback settings of an extension input device, use 
\fBXChangeFeedbackControl\fP.
.P
You should use \fBXFreeFeedbackList\fP to free the data returned by this 
function.
.P
.DS
\f(CW
XFeedbackState
*XGetFeedbackControl (display, device, num_feedbacks_return)
        Display          *display;
        XDevice          *device;
        int              *num_feedbacks_return;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fInum_feedbacks\fP
Returns the number of feedbacks supported by the device.
.LE
.P
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
} XFeedbackState;
\fP
.DE
.mc >
.P
Feedbacks are grouped into classes.  Those feedbacks equivalent to those
supported by the core keyboard are reported in class \fBKbdFeedback\fP
using he \fBXKbdFeedbackState\fP structure.
The members of that structure are as follows:
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     click;
    int     percent;
    int     pitch;
    int     duration;
    int     led_mask;
    int     global_auto_repeat;
    char    auto_repeats[32];
} XKbdFeedbackState;
\fP
.DE
.P
Those feedbacks equivalent to those
supported by the core pointer are reported in class \fBPtrFeedback\fP
using he \fBXPtrFeedbackState\fP structure.
The members of that structure are as follows:
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     accelNum;
    int     accelDenom;
    int     threshold;
} XPtrFeedbackState;
\fP
.DE
.mc
.P
\fBInteger\fP feedbacks are those capable of displaying integer numbers.  
The minimum and maximum values that they can display are reported.
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     resolution;
    int     minVal;
    int     maxVal;
} XIntegerFeedbackState;
\fP
.DE
.P
\fBString\fP feedbacks are those that can display character information.  
The set
of key symbols that they can display is reported, as well as the maximum number
of characters they can display.
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     max_symbols;
    int     num_syms_supported;
    KeySym  *syms_supported;
} XStringFeedbackState;
\fP
.DE
.P
\fBBell\fP feedbacks are those that can generate a sound.
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     percent;
    int     pitch;
    int     duration;
} XBellFeedbackState;
\fP
.DE
.P
\fBLed\fP feedbacks are those that can generate a light.
Up to 32 lights per feedback are supported.  
.mc >
Each bit in led_mask
corresponds to one light, and the corresponding bit in led_values
indicates whether that light should be turned on or off.
.mc
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
.mc >
    MASK    led_values;
.mc
} XLedFeedbackState;
\fP
.DE
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP.
.P
To free the information returned by the \fBXGetFeedbackControl\fP function,
use \fBXFreeFeedbackList\fP.
.DS
\f(CW
XFreeFeedbackList (list)
        XFeedbackState *list;
\fP
.DE
.P
.VL 20
.LI \fIlist\fP
Specifies the pointer to the \fBXFeedbackState\fP structure returned by
a previous call to \fBXGetFeedbackControl\fP.
.LE
.P
This function frees the list of feedback control information.
.P
To change the settings of a feedback
on an extension device, use \fBXChangeFeedbackControl\fP.
This function modifies the current control values of the specified feedback
using information passed in the appropriate \fBXFeedbackControl\fP structure
for the feedback.
Which values are modified depends on the valuemask passed.
.P
.DS
\f(CW
int
XChangeFeedbackControl (display, device, valuemask, values)
        Display          *display;
        XDevice          *device;
        unsigned long    valuemask;
        XFeedbackControl *value;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIvaluemask\fP
Specifies one value for each bit in the mask (least to most significant
bit).  The values are associated with the feedbacks for the specified
device.
.LI \fIvalues\fP
Specifies a pointer to the \fBXFeedbackControl\fP structure.
.LE
.P
This function controls the device characteristics described by the
\fBXFeedbackControl\fP structure. 
.P
.DS
\f(CW
typedef struct {
    XID            feedbackClass;
    int		   length;
} XFeedbackControl;
\fP
.DE
.P
Feedback class \fBKbdFeedback\fP controls feedbacks equivalent to those
provided by the core keyboard using the \fBKbdFeedbackControl\fP structure.
The members of that structure are:
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     click;
    int     percent;
    int     pitch;
    int     duration;
    int     led_mask;
    int     led_value;
    int     key;
    int     auto_repeat_mode;
} XKbdFeedbackControl;
\fP
.DE
.P
This class controls the device characteristics described by the
\fBXKbdFeedbackControl\fP structure.  These include the key_click_percent,
global_auto_repeat and individual key auto-repeat.  Valid modes
are \fBAutoRepeatModeOn\fP, \fBAutoRepeatModeOff\fP,
\fBAutoRepeatModeDefault\fP.
.P
Valid masks are as follows:
.P
.DS
#define DvKeyClickPercent            (1L << 0)
.mc >
#define DvPercent                    (1L << 1)
#define DvPitch                      (1L << 2)
#define DvDuration                   (1L << 3)
#define DvLed                        (1L << 4)
#define DvLedMode                    (1L << 5)
#define DvKey                        (1L << 6)
#define DvAutoRepeatMode             (1L << 7)
.mc
.DE
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP, 
\fBBadValue\fP.
.P
Feedback class \fBPtrFeedback\fP controls feedbacks equivalent to those
provided by the core pointer using the \fBPtrFeedbackControl\fP structure.
The members of that structure are:
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     accelNum;
    int     accelDenom;
    int     threshold;
} XPtrFeedbackControl;
\fP
.DE
.P
Which values are modified depends on the valuemask passed.
.P
Valid masks are as follows:
.P
.DS
#define DvAccelnum                   (1L << 0)
#define DvAccelDenom                 (1L << 1)
#define DvThreshold                  (1L << 2)
.DE
.P
The acceleration, expressed as a fraction, is a multiplier for movement. 
For example,
specifying 3/1 means the device moves three times as fast as normal.
The fraction may be rounded arbitrarily by the X server.  
Acceleration
only takes effect if the device moves more than threshold pixels at
once and only applies to the amount beyond the value in the threshold argument.
Setting a value to -1 restores the default.
The values of the accelNumerator and threshold fields must be nonzero for
the pointer values to be set.
Otherwise, the parameters will be unchanged.
Negative values generate a \fBBadValue\fP
error, as does a zero value
for the accelDenominator field.
.P
This request fails with a \fBBadMatch\fP error if the specified device is not
currently reporting relative motion.  If a that is capable of reporting both
relative and absolute motion has its mode changed from \fBRelative\fP to
\fBAbsolute\fP by an \fBXSetDeviceMode\fP request, valuator control values
will be ignored by the server while the device is in that mode.
.P
Feedback class \fBIntegerFeedback\fP controls integer feedbacks displayed
on input devices, using the \fBIntegerFeedbackControl\fP structure.
The members of that structure are:
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     int_to_display;
} XIntegerFeedbackControl;
\fP
.DE
.P
Valid masks are as follows:
.P
.DS
#define DvInteger                    (1L << 0)
.DE
.P
Feedback class \fBStringFeedback\fP controls string feedbacks displayed
on input devices, using the \fBStringFeedbackControl\fP structure.
The members of that structure are:
.P
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     num_keysyms;
    KeySym  *syms_to_display;
} XStringFeedbackControl;
\fP
.DE
.P
Valid masks are as follows:
.P
.DS
#define DvString                     (1L << 0)
.DE
.P
Feedback class \fBBellFeedback\fP controls a bell on an input device,
using the \fBBellFeedbackControl\fP structure.
The members of that structure are:
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     percent;
    int     pitch;
    int     duration;
} XBellFeedbackControl;
\fP
.DE
.P
Valid masks are as follows:
.P
.DS
#define DvPercent                    (1L << 1)
#define DvPitch                      (1L << 2)
#define DvDuration                   (1L << 3)
.DE
.P
Feedback class \fBLedFeedback\fP controls lights on an input device,
using the \fBLedFeedbackControl\fP structure.
The members of that structure are:
.DS
\f(CW
typedef struct {
    XID     class;
    int     length;
    int     led_mask;
    int     led_values;
} XLedFeedbackControl;
\fP
.DE
Valid masks are as follows:
.P
.DS
#define DvLed                        (1L << 4)
#define DvLedMode                    (1L << 5)
.DE
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP,
\fPBadFeedBack\fP.
.H 3 "Controlling Device Encoding"
To get the key mapping of an extension device that supports input class 
\fBKeys\fP, use 
\fBXGetDeviceKeyMapping\fP.
.DS
\f(CW
KeySym
*XGetDeviceKeyMapping (display, device, first_keycode_wanted, 
    keycode_count, keysyms_per_keycode_return)
        Display *display;
        XDevice *device;
        KeyCode first_keycode_wanted;
        int     keycode_count;
        int     *keysyms_per_keycode_return;
\fP
.DE
.VL 30
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIfirst_keycode_wanted\fP
Specifies the first keycode that is to be returned.
.LI \fIkeycode_count\fP
Specifies the number of keycodes that are to be returned.
.LI \fIkeysyms_per_keycode_return\fP
Returns the number of keysyms per keycode.
.LE
.P
This function is analogous to the core \fBXGetKeyboardMapping\fP function.  
It returns the symbols for the specified number of keycodes for the 
specified extension device.
.P
\fBXGetDeviceKeyMapping\fP returns the symbols for the 
specified number of keycodes for the 
specified extension device, starting with the specified keycode.
The first-keycode must be greater than or equal to
min-keycode as returned 
by the \fBXListInputDevices\fP request (else a \fBBadValue\fP error),
and
.P
.DS
first-keycode + count \- 1
.DE
.P
must be less than or equal to max-keycode as returned 
by the \fBXListInputDevices\fP request
(else a \fBBadValue\fP error).
.P
The number of elements in the keysyms list is
.P
.DS
count * keysyms-per-keycode
.DE
and KEYSYM number N (counting from zero) for keycode K has an index
(counting from zero) of
.P
.DS
(K \- first-keycode) * keysyms-per-keycode + N
.DE
.P
in keysyms.
The keysyms-per-keycode value is chosen arbitrarily by the server
to be large enough to report all requested symbols.
A special KEYSYM value of
\fBNoSymbol\fP
is used to fill in unused elements for individual keycodes.
.P
You should use XFree to free the data returned by this function.
.P
If the specified device has not first been opened by this client via
\fBXOpenDevice\fP, this request will fail with a \fBBadDevice\fP error.
If that device does not support input class Keys,
this request will fail with a \fBBadMatch\fP error.
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP,
\fBBadValue\fP.
.SP 2
To change the keyboard mapping of an extension device that supports input
class \fBKeys\fP, use 
\fBXChangeDeviceKeyMapping\fP.
.DS
\f(CW
int
XChangeDeviceKeyMapping (display, device, first_keycode, 
    keysyms_per_keycode, keysyms, num_codes)
        Display *display;
        XDevice *device;
        int     first_keycode;
        int     keysyms_per_keycode;
        KeySym  *keysyms;
        int     num_codes;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIfirst_keycode\fP
Specifies the first keycode that is to be changed.
.LI \fIkeysyms_per_keycode\fP
Specifies the keysyms that are to be used.
.LI \fIkeysyms\fP
Specifies a pointer to an array of keysyms.
.LI \fInum_codes\fP
Specifies the number of keycodes that are to be changed.
.LE
.P
This function is analogous to the core \fBXChangeKeyboardMapping\fP function.  
It defines the symbols for the specified number of keycodes for the 
specified extension keyboard device.
.P
If the specified device has not first been opened by this client via
\fBXOpenDevice\fP,
this request will fail with a \fBBadDevice\fP error.
If the specified device does not support input class Keys,
this request will fail with a \fBBadMatch\fP error.
.P 
The number of elements in the keysyms list must be a multiple of
keysyms_per_keycode.  Otherwise, \fBXChangeDeviceKeyMapping\fP generates
a \fBBadLength\fP error.  The specified first_keycode must be greater
than or equal to the min_keycode value returned by the \fBListInputDevices\fP
request, or this request will fail with a \fBBadValue\fP error.  In addition,
if the following expression is not less than the max_keycode value returned by
the ListInputDevices request, the request will fail with a BadValue
error:
.P
.DS
	  first_keycode + (num_codes / keysyms_per_keycode) - 1
.DE
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP,
\fBBadValue\fP, \fBBadAlloc\fP.
.SP 2
.P
To obtain the keycodes that are used as modifiers on an 
extension device that supports input class \fBKeys\fP, use
\fBXGetDeviceModifierMapping\fP.
.DS
\f(CW
XModifierKeymap 
*XGetDeviceModifierMapping (display, device)
        Display *display;
        XDevice *device;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LE
.P
This function is analogous to the core \fBXGetModifierMapping\fP function.  
The \fBXGetDeviceModifierMapping\fP function returns a newly created
\fBXModifierKeymap\fP structure that contains the keys being used as
modifiers for the specified device.  The structure should be freed after
use with \fBXFreeModifierMapping\fP.  If only zero values appear in the set
for any modifier, that modifier is disabled.
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP.
.SP 2
.P
To set which keycodes that are to be used as modifiers for an extension
device, use \fBXSetDeviceModifierMapping\fP.
.DS
\f(CW
int 
XSetDeviceModifierMapping (display, device, modmap)
        Display         *display;
        XDevice         *device;
        XModifierKeymap *modmap;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fImodmap\fP
Specifies a pointer to the \fBXModifierKeymap\fP structure.
.LE
.P
This function is analogous to the core XSetModifierMapping function.  
The \fBXSetDeviceModifierMapping\fP function specifies the keycodes of 
the keys, if any, that are to be used as modifiers.  A zero value means
that no key should be used.  No two arguments can have the same nonzero
keycode value.  Otherwise, \fBXSetDeviceModifierMapping\fP generates a
\fBBadValue\fP error.
There are eight modifiers, and the modifiermap member of the 
\fBXModifierKeymap\fP structure contains eight sets of max_keypermod 
keycodes, one for each modifier in the
order Shift, Lock, Control, Mod1, Mod2, Mod3, Mod4, and Mod5.
Only nonzero keycodes have meaning in each set, and nonzero keycodes
are ignored.
In addition, all of the nonzero keycodes must be in the range specified by 
min_keycode and max_keycode reported by the \fBXListInputDevices\fP function.
Otherwise, \fBXSetModifierMapping\fP generates a \fBBadValue\fP
error.
No keycode may appear twice in the entire map. 
Otherwise, it generates a \fBBadValue\fP
error.
.P
A X server can impose restrictions on how modifiers can be changed, 
for example,
if certain keys do not generate up transitions in hardware or if multiple
modifier keys are not supported.  
If some such restriction is violated, 
the status reply is
\fBMappingFailed,\fP
and none of the modifiers are changed.
If the new keycodes specified for a modifier differ from those
currently defined and any (current or new) keys for that modifier are
in the logically down state, 
the status reply is \fBMappingBusy\fP, 
and none of the modifiers are changed.
\fBXSetModifierMapping\fP
generates a \fBDeviceMappingNotify\fP
event on a \fBMappingSuccess\fP
status.
.P
\fBXSetDeviceModifierMapping\fP can generate \fBBadDevice\fR, \fBBadMatch\fP,
\fBBadAlloc\fP, and \fBBadValue\fP errors.
.SP 2
.H 3 "Controlling Button Mapping"
To set the mapping of the buttons on an extension device, use
\fBXSetDeviceButtonMapping\fP.
.DS
\f(CW
int
XSetDeviceButtonMapping (display, device, map, nmap)
        Display          *display;
        XDevice          *device;
        unsigned char    map[];
        int              nmap;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fImap\fP
Specifies the mapping list.
.LI \fInmap\fP
Specifies the number of items in the mapping list.
.LE
.P
The
\fBXSetDeviceButtonMapping\fP function sets the mapping of the buttons on
an extension device.  If it succeeds, the X server generates a
\fBDeviceMappingNotify\fP event, and \fBXSetDeviceButtonMapping\fP
returns \fBMappingSuccess\fP.
Elements of the list are indexed starting from one.
The length of the list must be the same as
\fBXGetDeviceButtonMapping\fP would return, or a \fBBadValue\fP
error results.
The index is a button number, and the element of the list
defines the effective number.
A zero element disables a button, and elements are not restricted in
value by the number of physical buttons.
However, no two elements can have the same nonzero value,
or a \fBBadValue\fP error results.
If any of the buttons to be altered are logically in the down state,
\fBXSetDeviceButtonMapping\fP returns \fBMappingBusy\fP,
and the mapping is not changed.
.P
\fBXSetDeviceButtonMapping\fP can generate \fBBadDevice\fP, \fBBadMatch\fP,
and \fBBadValue\fP errors.
.P
.sp
To get the button mapping, use \fBXGetDeviceButtonMapping\fP.
.SP 2
.DS
\f(CW
int
XGetDeviceButtonMapping (display, device, map_return, nmap)
        Display          *display;
        XDevice          *device;
        unsigned char    map_return[];
        int              nmap;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fImap_return\fP
Specifies the mapping list.
.LI \fInmap\fP
Specifies the number of items in the mapping list.
.LE
.P
The \fBXGetDeviceButtonMapping\fP
function returns the current mapping of the pointer.
Elements of the list are indexed starting from one.
\fBXGetDeviceButtonMapping\fP returns the number of physical buttons actually 
on the pointer.
The nominal mapping for the buttons is the identity mapping: map[i]=i.
The nmap argument specifies the length of the array where the button
mapping is returned, and only the first nmap elements are returned 
in map_return.
.P
Errors returned by this function: \fBBadDevice\fP, \fBBadMatch\fP.
.SP 2
.H 3 "Obtaining The State Of A Device"
To obtain information that describes the state of the keys, buttons and 
valuators of an extension device, use \fBXQueryDeviceState\fP.
.DS
\f(CW
XDeviceState 
*XQueryDeviceState (display, device)
        Display *display;
        XDevice *device;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LE
.P
The \fBXQueryDeviceState\fP function returns a pointer to an
\fBXDeviceState\fP structure.  This structure points to a list of
\fBXInputClassState\fP structures that describe
the state of the keys, buttons, and valuators on the device.
.DS
\f(CW
typedef struct {
        XID		device_id;
        int		num_classes;
        XInputClass	*data;
} XDeviceState;

typedef struct
    {
    unsigned char	class;
    unsigned char	length;
    } XInputClass;
\fP
.DE
.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_valuators;
    unsigned char	mode;
    int        		*valuators;
} XValuatorState;
\fP
.DE

.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_keys;
    char        	keys[32];
} XKeyState;
\fP
.DE

.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_buttons;
    char        	buttons[32];
} XButtonState;
\fP
.DE
.P
In the \fBXButtonState\fP and \fBXKeyState\fP structures, 
byte N (from 0) contains the
bits for key or button 8N to 8N+7 with the least significant bit in the
byte representing key or button 8N.
.P
You should use \fBXFreeDeviceState\fP to free the data returned by this 
function.
.P
Errors returned by this function: \fBBadDevice\fP.
.P
.DS
\f(CW
XFreeDeviceState (state)
        XDeviceState *state;
\fP
.DE
.P
.VL 20
.LI \fIstate\fP
Specifies the pointer to the \fBXDeviceState\fP data returned by
a previous call to \fBXQueryDeviceState\fP.
.LE
.P
This function frees the device state data.
.H 2 "Events and Event-Handling Functions"
The input extension creates input events analogous to the core input events.
These extension input events are generated by manipulating one of the
extension input devices.  The following sections describe these events
and explain how a client program can receive them.
.H 3 "Event Types"
Event types are integer numbers that a client can use to determine what
kind of event it has received.  The client compares the type field of 
the event structure with known event types to make this determination.
.P 
The core input event types are constants and are defined in the header
file \fB<X11/X.h>\fP.  Extension event types are not constants.  Instead, they
are dynamically allocated by the extension's request to the X server
when the extension is initialized.  Because of this, extension event
types must be obtained by the client from the server.
.P
The client program determines the event type for an extension event by using
the information returned by the \fBXOpenDevice\fP request.
This type can then be used for comparison with the type field
of events received by the client.
.P
Extension events propagate up the window hierarchy in the same manner
as core events.  If a window is not interested in an extension event, 
it usually propagates to the closest ancestor that is interested,
unless the dont_propagate list prohibits it.
Grabs of extension devices may alter the set of windows that receive a particular
extension event.
.P
The following table lists the event category and its associated event
type or types.
.SP
.TS
box center;
c | c
l | l.
Event Category	Event Type
=
Device key events	\fIDeviceKeyPress\fP,
	\fIDeviceKeyRelease\fP
_
Device motion events	\fIDeviceButtonPress\fP,
	\fIDeviceButtonRelease\fP,
	\fIDeviceMotionNotify\fP
_
Device input focus events	\fIDeviceFocusIn\fP,
	\fIDeviceFocusOut\fP
_
Device state notification events	\fIDeviceStateNotify\fP
_
Device proximity events	\fIProximityIn\fP,
	\fIProximityOut\fP
_
Device mapping events	\fIDeviceMappingNotify\fP
_
Device change events	\fIChangeDeviceNotify\fP
.TE
.SP
.H 3 "Event Classes"
Event classes are integer numbers that are used in the same way as the
core event masks.  They are used by a client program to indicate to the
server which events that client program wishes to receive.
.P
The core input event masks are constants and are defined in the header
file \fB<X11/X.h>\fP.  Extension event classes are not constants.  Instead, 
they are dynamically allocated by the extension's request to the X server
when the extension is initialized.  Because of this, extension event
classes must be obtained by the client from the server.
.P
The event class for an extension event and device is returned by the 
\fBXOpenDevice\fP function. 
This class can then be used in an \fBXSelectExtensionEvent\fP
request to ask that events of that type from that device be sent to
the client program.
.P
.mc >
For \fBDeviceButtonPress\fP events, the client may specify whether
or not an implicit passive grab should be done when the button is
pressed.  If the client wants to guarantee that it will receive
a \fBDeviceButtonRelease\fP event for each \fBDeviceButtonPress\fP
event it receives, it should specify the  \fBDeviceButtonPressGrab\fP
class in addition to the \fBDeviceButtonPress\fP class.
This restricts the client in that only one client at a time
may request \fBDeviceButtonPress\fP events from the same device and
window if any client specifies this class.
.P
If any client has specified the \fBDeviceButtonPressGrab\fP class, any requests
by any other client that specify the same device and window and specify
either \fBDeviceButtonPress\fP or \fBDeviceButtonPressGrab\fP will
cause an \fBAccess\fP error to be generated.
.mc
.P
If only the \fBDeviceButtonPress\fP class is specified, no implicit
passive grab will be done when a button is pressed on the device.
Multiple clients may use this class to specify the same device and
window combination.
.P
The client may also select \fBDeviceMotion\fP events only when a 
button is down.  It does this by specifying the event classes 
\fBDeviceButtonMotion1\fP through \fBDeviceButtonMotion5\fP.  
An input device will only support
as many button motion classes as it has buttons.
.H 3 "Event Structures"
Each extension event type has a corresponding structure declared in
\fB<X11/XInput.h>\fP.  All event structures have the following members:
.VL 20
.LI \fItype\fP
Set to the event type number that uniquely identifies it.  For example,
when the X server reports a \fBDeviceKeyPress\fP event to a client 
application, it sends an \fBXDeviceKeyPressEvent\fP structure.
.LI \fIdisplay\fP
Set to a pointer to a structure that defines the display the event was read on.
.LI \fIsend_event\fP
Set to \fBTrue\fP if the event came from an \fBXSendEvent\fP request.
.LI \fIserial\fP
Set from the serial number reported in the protocol but expanded from the
16-bit least-significant bits to a full 32-bit value.
.LE
.P
Extension event structures report the current position of the X pointer.
In addition, if the device reports motion data and is reporting absolute data,
the current value of any valuators the device contains is also reported.
.H 4 "Device Key Events"
Key events from extension devices contain all the information that is
contained in a key event from the X keyboard.  In addition, they contain
a deviceid and report the current value of any valuators on the device,
if that device is reporting absolute data.
If data for more than six valuators is being reported, more than one
key event will be sent.
The axes_count field contains the number of axes that are being
reported.  The server sends as many of these events as are
needed to report the device data.  Each event contains the total number
of axes reported in the axis_count field, and the first axis reported
in the current event in the first_axis field.
If the device supports input class \fBValuators\fP, 
but is not reporting absolute mode data,
the axes_count field contains 0.
.P
The location reported in 
the x,y and x_root,y_root fields is the location of the core X pointer.
.P
The XDeviceKeyEvent structure is defined as follows:
.DS
\f(CW
typedef struct 
    {
    int            type;         /* of event */
    unsigned long  serial;       /* # of last request processed */
    Bool           send_event;   /* true if from SendEvent request */
    Display        *display;     /* Display the event was read from */
    Window         window;       /* "event" window reported relative to */
    XID            deviceid;
    Window         root;         /* root window event occured on */
    Window         subwindow;    /* child window */
    Time           time;         /* milliseconds */
    int            x, y;         /* x, y coordinates in event window */
    int            x_root;       /* coordinates relative to root */
    int            y_root;       /* coordinates relative to root */
    unsigned int   state;        /* key or button mask */
    unsigned int   keycode;      /* detail */
    Bool           same_screen;  /* same screen flag */
    unsigned char  axes_count;
    unsigned char  first_axis;
    unsigned int   device_state; /* device key or button mask */
    int            axis_data[6];
    } XDeviceKeyEvent;

typedef XDeviceKeyEvent XDeviceKeyPressedEvent;
typedef XDeviceKeyEvent XDeviceKeyReleasedEvent;
\fP
.DE
.H 4 "Device Button Events"
Button events from extension devices contain all the information that is
contained in a button event from the X pointer.  In addition, they contain
a deviceid and report the current value of any valuators on the device,
if that device is reporting absolute data.
If data for more than six valuators is being reported, more than one
button event may be sent.
The axes_count field contains the number of axes that are being
reported.  The server sends as many of these events as are
needed to report the device data.  Each event contains the total number
of axes reported in the axis_count field, and the first axis reported
in the current event in the first_axis field.
If the device supports input class \fBValuators\fP, 
but is not reporting absolute mode data,
the axes_count field contains 0.
.P
The location reported in 
the x,y and x_root,y_root fields is the location of the core X pointer.
.DS
\f(CW
typedef struct {
    int           type;         /* of event */
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;       /* "event" window reported relative to */
    XID           deviceid;
    Window        root;         /* root window that the event occured on */
    Window        subwindow;    /* child window */
    Time          time;         /* milliseconds */
    int           x, y;         /* x, y coordinates in event window */
    int           x_root;       /* coordinates relative to root */
    int           y_root;       /* coordinates relative to root */
    unsigned int  state;        /* key or button mask */
    unsigned int  button;       /* detail */
    Bool          same_screen;  /* same screen flag */
    unsigned char axis_count;
    unsigned char first_axis;
    unsigned int  device_state; /* device key or button mask */
    int           axis_data[6];
    } XDeviceButtonEvent;

typedef XDeviceButtonEvent XDeviceButtonPressedEvent;
typedef XDeviceButtonEvent XDeviceButtonReleasedEvent;
\fP
.DE
.H 4 "Device Motion Events"
Motion events from extension devices contain all the information that is
contained in a motion event from the X pointer.  In addition, they contain
a deviceid and report the current value of any valuators on the device.
.P
The location reported in 
the x,y and x_root,y_root fields is the location of the core X pointer, 
and so is 2-dimensional.
.P
Extension motion devices may report motion data for a variable number of 
axes.  
The axes_count field contains the number of axes that are being
reported.  The server sends as many of these events as are
needed to report the device data.  Each event contains the total number
of axes reported in the axis_count field, and the first axis reported
in the current event in the first_axis field.
.P
.DS
\f(CW
typedef struct 
    {
    int           type;        /* of event */
    unsigned long serial;      /* # of last request processed by server */
    Bool          send_event;  /* true if from a SendEvent request */
    Display       *display;    /* Display the event was read from */
    Window        window;      /* "event" window reported relative to */
    XID           deviceid;
    Window        root;        /* root window that the event occured on */
    Window        subwindow;   /* child window */
    Time          time;        /* milliseconds */
    int           x, y;        /* x, y coordinates in event window */
    int           x_root;      /* coordinates relative to root */
    int           y_root;      /* coordinates relative to root */
    unsigned int  state;       /* key or button mask */
    char          is_hint;     /* detail */
    Bool          same_screen; /* same screen flag */
    unsigned int  device_state; /* device key or button mask */
    unsigned char axes_count;
    unsigned char first_axis;
    int           axis_data[6];
    } XDeviceMotionEvent;
\fP
.DE
.H 4 "Device Focus Events"
These events are equivalent to the core focus events.
They contain the same information, with the addition
of a deviceid to identify which device has had a focus change,
and a timestamp.
.P
\fBDeviceFocusIn\fP and \fBDeviceFocusOut\fP events are generated for
focus changes of extension devices in the same manner as core focus
events are generated.
.P
.DS
\f(CW
typedef struct 
    {
    int           type;       /* of event */
    unsigned long serial;     /* # of last request processed by server */
    Bool          send_event; /* true if this came from a SendEvent request */
    Display       *display;   /* Display the event was read from */
    Window        window;     /* "event" window it is reported relative to */
    XID           deviceid;
    int           mode;       /* NotifyNormal, NotifyGrab, NotifyUngrab */
    int           detail;
	/*
	 * NotifyAncestor, NotifyVirtual, NotifyInferior, 
	 * NotifyNonLinear,NotifyNonLinearVirtual, NotifyPointer,
	 * NotifyPointerRoot, NotifyDetailNone 
	 */
    Time                time;
    } XDeviceFocusChangeEvent;

typedef XDeviceFocusChangeEvent XDeviceFocusInEvent;
typedef XDeviceFocusChangeEvent XDeviceFocusOutEvent;
\fP
.DE
.H 4 "Device StateNotify Event"
This event is analogous to the core keymap event, but
reports the current state of the device for each
input class that it supports.
It is generated after every \fBDeviceFocusIn\fP event
and \fBEnterNotify\fP  event and is delivered to clients 
who have selected \fBXDeviceStateNotify\fP events.
.P
If the device supports more valuators than can be reported in a single
\fBXEvent\fP, multiple \fBXDeviceStateNotify\fP events will be generated.
.P
.DS
\f(CW
typedef struct
    {
    unsigned char	class;
    unsigned char	length;
    } XInputClass;

typedef struct {
    int           type;
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if this came from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;
    XID           deviceid;
    Time          time;
    int           num_classes;
    char	  data[64];
} XDeviceStateNotifyEvent;	
\fP
.DE
.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_valuators;
    unsigned char	mode;
    int        		valuators[6];
} XValuatorStatus;
\fP
.DE
.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_keys;
    char        	keys[32];
} XKeyStatus;
\fP
.DE
.P
.DS
\f(CW
typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_buttons;
    char        	buttons[32];
} XButtonStatus;
\fP
.DE
.H 4 "Device Mapping Event"
This event is equivalent to the core MappingNotify event.
It notifies client programs when the mapping of keys,
modifiers, or buttons on an extension device has changed.
.P
.DS
\f(CW
typedef struct {
    int            type;
    unsigned long  serial;	
    Bool           send_event;
    Display        *display;
    Window         window;
    XID            deviceid;
    int            request;
    int            first_keycode;
    int            count;
    Time           time;
} XDeviceMappingEvent;
\fP
.DE
.H 4 "ChangeDeviceNotify Event"
This event has no equivalent in the core protocol.  It notifies client
programs when one of the core devices has been changed.
.P
.DS
\f(CW
typedef struct {
    int            type;
    unsigned long  serial;
    Bool           send_event;
    Display        *display;
    Window         window;
    XID            deviceid;
    Time           time;
    int            request;
} XChangeDeviceNotifyEvent;
\fP
.DE
.H 4 "Proximity Events"
These events have no equivalent in the core protocol.  Some input
devices such as graphics tablets or touchscreens may send these
events to indicate that a stylus has moved into or out of contact
with a positional sensing surface.
.P
The event contains the current value of any valuators on the device,
if that device is reporting absolute data.
If data for more than six valuators is being reported, more than one
proximity event may be sent.
The axes_count field contains the number of axes that are being
reported.  The server sends as many of these events as are
needed to report the device data.  Each event contains the total number
of axes reported in the axis_count field, and the first axis reported
in the current event in the first_axis field.
If the device supports input class \fBValuators\fP, 
but is not reporting absolute mode data,
the axes_count field contains 0.
.P
.DS
\f(CW
typedef struct 
    {
    int             type;      /* ProximityIn or ProximityOut */        
    unsigned long   serial;    /* # of last request processed by server */
    Bool            send_event; /* true if this came from a SendEvent request */
    Display         *display;  /* Display the event was read from */
    Window          window;      
    XID	            deviceid;
    Window          root;            
    Window          subwindow;      
    Time            time;            
    int             x, y;            
    int             x_root, y_root;  
    unsigned int    state;           
    Bool            same_screen;     
    unsigned char   axes_count;
    unsigned char   first_axis;
    unsigned int    device_state; /* device key or button mask */
    int             axis_data[6];
    } XProximityNotifyEvent;
typedef XProximityNotifyEvent XProximityInEvent;
typedef XProximityNotifyEvent XProximityOutEvent;
\fP
.DE
.H 3 "Determining The Extension Version"
.DS
\f(CW
XExtensionVersion
*XGetExtensionVersion (display, name)
        Display *display;
        char    *name;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIname\fP
Specifies the name of the desired extension.
.LE
.P
This function allows a client to determine if a server supports
the desired version of the input extension.
.P
The \fBXExtensionVersion\fP structure returns information about the
version of the extension supported by the server.  The structure is
defined as follows:
.P
.DS
\f(CW
typedef struct
    {
    Bool  present;   
    short major_version;
    short minor_version;
    } XExtensionVersion;
\fP
.DE
.P
You should use \fBXFree\fP to free the data returned by this function.
.H 3 "Listing Available Devices"
A client program that wishes to access a specific device
must first determine whether that device is connected to the X server.  This
is done through the
\fBXListInputDevices\fP function, which will return a list of all devices that
can be opened by the X server.  The client program can use the \fBtype\fP field
in an \fBXGetAtomName\fP request to determine the name of the device.  These names
can be compared against the names defined in the \fBXInput.h\fP
file to determine if the device is the one desired by the client.
.P
.DS
\f(CW
XDeviceInfo 
*XListInputDevices (display, ndevices)
        Display *display;
        int     *ndevices;            /* RETURN */
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIndevices\fP
Specifies the address of a variable into which the server
can return the number of input devices available to the X server.
.LE
.P
This function allows a client to determine which devices 
are available for X input and information about those devices.
An array of \fBXDeviceInfo\fP structures is returned, with one
element in the array for each device.  The number of devices
is returned in the \fBndevices\fP argument.
.P
.mc >
The X pointer device and X keyboard device are reported, as well as
all available extension input devices.  The use field of the 
\fBXDeviceInfo\fP structure specifies the current use of the device.
If the value of this field is \fBIsXPointer\fP, the device is the 
X pointer device.  If the value is \fBIsXKeyboard\fP, the device is
the X keyboard device.  If the value is \fBIsXExtensionDevice\fP, the
device is available for use as an extension input device.
.mc
.P
Each \fBXDeviceInfo\fP entry contains a pointer to a list of
structures that describe the characteristics of each class
of input supported by that device.  The num_entries field
contains the number of entries in that list.
.P
If the device supports input class \fBValuators\fP,
one of the structures pointed to by 
the \fBXDeviceInfo\fP structure will be an
\fBXValuatorInfo\fP structure.  The axes field of that structure
contains the address of an array of \fBXAxisInfo\fP structures.
There is one element in this array for each axis of motion
reported by the device.  The number of elements in this 
array is contained in the num_axes element of the 
\fBXValuatorInfo\fP structure.
The size of the motion buffer for the device is
reported in the motion_buffer field of the \fBXValuatorInfo\fP
structure.
.P
The \fBXDeviceInfo\fP structure contains the following information:
.DS
\f(CW
typedef struct _XDeviceInfo
    {
    XID                 id;        
    Atom                type;
    char                *name;
    int                 num_classes;
    int                 use;
    XAnyClassPtr 	inputclassinfo;
    } XDeviceInfo;
\fP
.DE
.P
The structures pointed to by the \fBXDeviceInfo\fP structure contain
the following information:
.DS
\f(CW
typedef struct _XKeyInfo
    {
    XID			class;
    int			length;
    unsigned short      min_keycode;
    unsigned short      max_keycode;
    unsigned short      num_keys;
    } XKeyInfo;

typedef struct _XButtonInfo {
    XID		class;
    int		length;
    short 	num_buttons;
    } XButtonInfo;

typedef struct	_XValuatorInfo
    {
    XID			class;
    int			length;
    unsigned char       num_axes;
    unsigned char       mode;
    unsigned long       motion_buffer;
    XAxisInfoPtr        axes;
    } XValuatorInfo;
\fP
.DE
.P
The \fBXAxisInfo\fP structure pointed to by the \fBXValuatorInfo\fP structure 
contains the following information.
.DS
\f(CW
typedef struct _XAxisInfo {
    int 	resolution;
    int 	min_value;
    int 	max_value;
    } XAxisInfo;
\fP
.DE
.P
The following atom names are defined in the file \fBXI.h\fP:
.DS
\f(CW
\fBMOUSE\fP	
\fBTABLET\fP
\fBKEYBOARD\fP	
\fBTOUCHSCREEN\fP
\fBTOUCHPAD\fP	
\fBBUTTONBOX\fP	
\fBBARCODE\fP
\fBKNOB_BOX\fP
\fBTRACKBALL\fP	
\fBQUADRATURE\fP
\fBSPACEBALL\fP
\fBDATAGLOVE\fP
\fBEYETRACKER\fP
\fBCURSORKEYS\fP
\fBFOOTMOUSE\fP
\fBID_MODULE\fP
\fBONE_KNOB\fP
\fBNINE_KNOB\fP
\fP
.DE
.P
These names can be used in an \fBXGetAtom\fP request to return an atom that
can be used for comparison with the type field of the 
\fBXDeviceInfo\fP structure.
.P
This function returns NULL if there are no input devices to list.
.P
.DS
\f(CW
XFreeDeviceList (list)
        XDeviceInfo *list;
\fP
.DE
.P
.VL 20
.LI \fIlist\fP
Specifies the pointer to the \fBXDeviceInfo\fP array returned by
a previous call to \fBXListInputDevices\fP.
.LE
.P
This function frees the list of input device information.
.H 3 "Enabling And Disabling Extension Devices"
Each
client program that wishes to access an extension device must request that
the server open that device.  This is done via the \fBXOpenDevice\fP
request.  That request is defined as follows:
.P
.DS
\f(CW
XDevice 
*XOpenDevice(display, device_id)
        Display *display;
        XID     device_id;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice_id\fP
Specifies the ID that uniquely identifies the device to be opened.  This ID
is obtained from the \fBXListInputDevices\fP request.
.LE
This function opens the device for the requesting client and returns an
\fBXDevice\fP structure on success.  That structure is defined as follows:
.P
.DS
\f(CW
typedef struct {
        XID                    device_id;
        int                    num_classes;
        XInputClassInfo        *classes;
} XDevice;
\fP
.DE
.P
The \fBXDevice\fP structure contains a pointer to 
an array of \fBXInputClassInfo\fP structures.  Each element in that array
contains information about events of a particular input class supported
by the input device.
.P
The XInputClassInfo structure is defined as follows:
.P
.DS
\f(CW
typedef struct {
        unsigned char input_class;
        unsigned char event_type_base;
} XInputClassInfo;
\fP
.DE
.P
A client program can determine the event
type and event class for a given event by using macros defined by the 
input extension.  The name of the macro corresponds to the desired event,
and the macro is passed the structure that describes the device from which
input is desired, i.e.
.DS
\f(CW
	DeviceKeyPress (XDevice *device, &event_type, &event_class)
\fP
.DE
.P
The macro will fill in the values of the event class to be used in an
\fBXSelectExtensionEvent\fP request to select the event, and the event
type to be used in comparing with the event types of events received
via \fBXNextEvent\fP.
.P
Errors returned by this function:  \fBBadDevice\fP.
.SP 2
.P
Before terminating, the client program should request that the server close
the device. This is done via the \fBXCloseDevice\fP request.
.P
A client may open the same extension device more than once.  Requests
after the first successful one return an additional XDevice structure
with the same information as the first, but otherwise have no effect.
A single CloseDevice request will terminate that client's access to
the device.
.P
Closing a device releases any active or passive grabs the requesting client
has established.  If the device is frozen only by an active grab of the
requesting client, any queued events are released.
.P
If a client program terminates without closing a device, the server will
automatically close that device on behalf of the client.  This does not
affect any other clients that may be accessing that device.
.DS
\f(CW
int
XCloseDevice(display, device)
        Display *display;
        XDevice *device;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the device to be closed.
.LE	
.P
This function closes the device for the requesting client, and frees
the \fBXDevice\fP structure.
.P
Errors returned by this function:  \fBBadDevice\fP.
.H 3 "Changing The Mode Of A Device"
Some devices are capable of reporting either relative or absolute motion
data.  To change the mode of a device from relative to absolute, use the
\fBXSetDeviceMode\fP function.  The valid values are \fBAbsolute\fP or 
\fBRelative\fP.
.DS
\f(CW
int
XSetDeviceMode (display, device, mode)
        Display *display;
        XDevice *device;
        int     mode;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the device whose mode should be changed.
.LI \fImode\fP
Specifies the mode.  You can specify one of these constants:
\fBAbsolute\fP or \fBRelative\fP.
.LE	
.P
This function allows a client to request the server to change the mode of a 
device that is capable of reporting either absolute positional data or relative
motion data.  If the device is invalid, or the client has not previously 
requested that the server open the device via an \fBXOpenDevice\fP request, 
this request will fail with a \fBBadDevice\fP error.
If the device does not support input class \fBValuators\fP, or if it is not
capable of reporting the specified mode, the request will fail with a 
\fBBadMatch\fP error.
.P
This request will fail and return \fBDeviceBusy\fP if another client has 
already opened the device and requested a different mode.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP,
\fBBadMode\fP, \fBDeviceBusy\fP.
.H 3 "Selecting Extension Device Events"
Device input events are selected using the \fBXSelectExtensionEvent\fP
function.
The parameters passed are a pointer to 
a list of classes that define the desired event types and devices, a count
of the number of elements in the list, and the id of the window from which 
events are desired.
.DS
\f(CW
int
XSelectExtensionEvent (display, window, event_list, count)
        Display     *display;
        Window      window;
        XEventClass *event_list;
        int         event_count;
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIwindow\fP
Specifies the ID of the window from which the client wishes to receive events.
.LI \fIevent_list\fP
Specifies a pointer to a list of XEventClasses that specify which events are
desired.
.LI \fIevent_count\fP
Specifies the number of elements in the event_list.
.LE	
.P
This function requests the server to send events that match the events and
devices described by the event list and that come from the requested 
window.  
.mc >
The elements of the XEventClass array are the event_class values
returned obtained by invoking a macro with the pointer to a Device
structure returned by the \fBXOpenDevice\fP request.
For example, the DeviceKeyPress macro, invoked in the form:
.DS
\f(CW
	DeviceKeyPress (XDevice *device, &event_type, &event_class)
\fP
.DE
.P
returns the XEventClass for DeviceKeyPress events from the specified device.
.P
Macros are defined for the following event classes:  \fBDeviceKeyPress,
DeviceKeyRelease, DeviceButtonPress, DeviceButtonRelease, DeviceMotionNotify,
DeviceFocusIn, DeviceFocusOut, ProximityIn, ProximityOut, DeviceStateNotify,
DeviceMappingNotify, ChangeDeviceNotify, DevicePointerMotionHint,
DeviceButton1Motion, DeviceButton2Motion, DeviceButton3Motion, 
DeviceButton4Motion, DeviceButton5Motion, DeviceButtonMotion,
DeviceOwnerGrabButton,\fP and \fBDeviceButtonPressGrab\fP.
.mc
To get the next available event from within a client program, use the 
core \fBXNextEvent\fP function.  This returns the next event whether it
came from a core device or an extension device.
.P
.mc >
Succeeding XSelectExtensionEvent requests using XEventClasses for the same 
device as was specified on a previous request will replace the previous
set of selected events from that device with the new set.
.mc
.P
Errors returned by this function:  \fBBadWindow\fP, \fBBadAccess\fP, 
\fBBadClass\fP, \fBBadLength\fP.
.H 3 "Determining Selected Device Events"
To determine which extension events are currently selected from a given
window, use \fBXGetSelectedExtensionEvents\fP.
.DS
\f(CW
int
XGetSelectedExtensionEvents (display, window, this_client_count, this_client, 
        all_clients_count, all_clients)
        Display     *display;
        Window      window;
        int         *this_client_count;  /* RETURN */
.mc >
        XEventClass **this_client;       /* RETURN */
        int         *all_clients_count;  /* RETURN */
        XEventClass **all_clients;       /* RETURN */
.mc
\fP
.DE
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIwindow\fP
Specifies the ID of the window from which the client wishes to receive events.
.LI \fIthis_client_count\fP
Specifies the number of elements in the this_client list.
.LI \fIthis_client\fP
Specifies a pointer to a list of XEventClasses that specify which events are
selected by this client.
.LI \fIall_client_count\fP
Specifies the number of elements in the all_clients list.
.LI \fIall_clients\fP
Specifies a pointer to a list of XEventClasses that specify which events are
selected by all clients.
.LE	
.P
This function returns pointers to two event list arrays.  
One lists the extension events selected by this client from
the specified window.  The other lists the extension events selected by
all clients from the specified window.  This information is analogous
to that returned in the fields your_event_mask and all_event_masks of the
\fBXWindowAttributes\fP structure when an 
\fBXGetWindowAttributes\fP request is made.
.P
You should use \fBXFree\fP to free the two arrays returned by this function.
.P
Errors returned by this function: \fBBadWindow\fP.
.H 3 "Controlling Event Propagation"
Extension events propagate up the window hierarchy in the same manner
as core events.  If a window is not interested in an extension event, 
it usually propagates to the closest ancestor that is interested,
unless the dont_propagate list prohibits it.
Grabs of extension devices may alter the set of windows that receive a 
particular extension event.
.P
Client programs may control event propagation through the use
of the following two functions.  
.P
\fBXChangeDeviceDontPropagateList\fP adds an event to or deletes an event from 
the do_not_propagate list of extension events for the specified window.
There is one list per window, and the list remains for the life of the window.
The list is not altered if a client that changed the list terminates.
.P
Suppression of event propagation is not allowed for all events.  If a
specified XEventClass is invalid because suppression of that event is not
allowed, a \fBBadClass\fP error will result.
.P
.DS
\f(CW
int
XChangeDeviceDontPropagateList (display, window, event_count, events, mode)
	Display     *display;
	Window      window;
	int         event_count;
	XEventClass *events;
	int         mode;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIwindow\fP
Specifies the desired window.
.LI \fIevent_count\fP
Specifies the number of elements in the events list.
.LI \fIevents\fP
Specifies a pointer to the list of XEventClasses.
.LI \fImode\fP
Specifies the mode.  You may use the constants \fBAddToList\fP or
\fBDeleteFromList\fP.
.LE
.P
.mc >
This function can return \fBBadWindow\fP, \fBBadClass\fP, and
\fBBadMode\fP errors.
.mc
.SP 2
.P
\fBXGetDeviceDontPropagateList\fP allows a client to determine the 
do_not_propagate list of extension events for the specified window.
.P
.DS
\f(CW
XEventClass
*XGetDeviceDontPropagateList (display, window, event_count)
	Display *display;
	Window  window;
.mc >
	int     *event_count;	/*RETURN */
.mc 
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIwindow\fP
Specifies the desired window.
.LI \fIevent_count\fP
Specifies the number of elements in the array returned by this function.
.LE
.P
An array of \fBXEventClasses\fP is returned by this function.
Each XEventClass represents a device/ event type pair.
.P
This function can return a \fBBadWindow\fP or a \fBBadClass\fP error.
.P
You should use \fBXFree\fP to free the data returned by this function.
.mc >
.H 3 "Sending An Event"
\fBXSendExtensionEvent\fP allows a client to send an extension event
to another client.
.P
.DS
\f(CW
int
XSendExtensionEvent (display, device, window, propagate, event_count, 
	event_list, event)
	Display     *display;
	XDevice     *device;
	Window      window;
	Bool        propagate;
	int         event_count;
	XEventClass *event_list;
	XEvent      *event
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the device whose ID is recorded in the event.
.LI \fIwindow\fP
Specifies the destination window ID.  You can pass a window ID,
\fBPointerWindow\fP or \fBFocusWindow\fP.
.LI \fIpropagate\fP
Specifies a boolean value that is either True or False.
.LI \fIevent_count\fP
Specifies the number of elements in the event_list array.
.LI \fIevent_list\fP
Specifies a pointer to an array of XEventClasses.
.LI \fIevent\fP
Specifies a pointer to the event that is to be sent.
.LE
.P
The XSendExtensionEvent function identifies the destination window,
determines which clients should receive the specified event, and ignores
any active grabs.  This function requires a list of XEventClasses to be
specified.  These are obtained by opening an input device with the
XOpenDevice request.
.P
This function uses the \fBwindow\fP argument to identify the destination
window as follows:
.BL
.LI
If you pass \fBPointerWindow\fP, the destination window is the window
that contains the pointer.
.BL
If you pass \fBFocusWindow\fP, and if the focus window contains the pointer,
the destination window is the window that contains the pointer.  If the 
focus window does not contain the pointer, the destination window is the 
focus window.
.LE
.P
To determine which clients should receive the specified events, 
\fBXSendExtensionEvent\fP uses the propagate argument as follows:
.BL
.LI
If propagate is \fBFalse\fP, the event is sent to every client selecting
from the destination window
any of the events specified in the event_list array.
.LI
If propagate is \fBTrue\fP, and no clients have selected
from the destination window
any of the events specified in the event_list array, the destination is 
replaced with the closest ancestor of destination for which some client
has selected one of the specified events, and for which no intervening
window has that event in its do_not_propagate mask.  If no such window
exists, or if the window is an ancestor of the focus window, and 
\fBFocusWindow\fP was originally specified as the destination, the event
is not sent to any clients.  Otherwise, the event is reported to every
client selecting on the final destination any of the events specified
in event_list.
.P
The event in the \fBXEvent\fP structure must be one of the events defined
by a loaded extension, so that the X server can correctly byte swap the
contents as necessary.  The contents of the event are otherwise unaltered
and unchecked by the X server except to force send_event to \fBTrue\fP
in the forwarded event and to set the sequence number in the event correctly.
.P
XSendExtensionEvent returns zero if the conversion-to-wire protocol
failed, otherwise it returns nonzero.
.P
This function can generate \fBBadDevice\fP, \fBBadValue\fP, \fBBadWindow\fP, 
or \fBBadClass\fP errors.
.mc
.H 3 "Getting Motion History"
.DS
\f(CW
XDeviceTimeCoord 
*XGetDeviceMotionEvents (display, device, start, stop, nevents_return, 
	mode_return, axis_count_return);
        Display *display;
        XDevice *device;
        Time    start, stop;
        int     *nevents_return;
        int     *mode_return;
        int     *axis_count_return;
\fP
.DE
.P
.VL 20
.LI \fIdisplay\fP
Specifies the connection to the X server.
.LI \fIdevice\fP
Specifies the desired device.
.LI \fIstart\fP
Specifies the start time.
.LI \fIstop\fP
Specifies the stop time.
.LI \fInevents_return\fP
Specifies the address of a variable into which the server
will return the number of positions in the motion buffer returned
for this request.
.LI \fImode_return\fP
Specifies the address of a variable into which the server
will return the mode of the nevents information.  The mode will be
one of the following: \fBAbsolute\fP or \fBRelative\fP.
.LI \fIaxis_count_return\fP
Specifies the address of a variable into which the server
will return the number of axes reported in each of the positions returned.
.LE
.P
This function returns all positions in the device's motion history buffer
that fall between the specified start and stop times inclusive.  If the
start time is in the future, or is later than the stop time, no positions
are returned.
.P
The return type for this function is a structure defined as follows:
.DS
\f(CW
typedef struct {
        Time time;
        unsigned int *data;
} XDeviceTimeCoord;
\fP
.DE
.P
The data field of the \fBXDeviceTimeCoord\fP structure is a pointer to an 
array of data items.  Each item is of type int, and there is one data item
per axis of motion reported by the device.  The number of axes reported
by the device is returned in the axis_count variable.
.P
The value of the data items depends on the mode of the device. 
The mode is returned in the mode variable.  If the 
mode is \fBAbsolute\fP, the data items are the raw values generated by
the device.  These may be scaled by the client program using the
maximum values that the device can generate for each axis of motion
that it reports.  The maximum value for each axis is reported in
the max_val field of the \fBXAxisInfo\fP structure.  This structure is 
part of the information returned by the \fBXListInputDevices\fP request.
.P
If the mode is \fBRelative\fP, the data items are the relative values
generated by the device.  The client program must choose an initial
position for the device and maintain a current position by 
accumulating these relative values.
.P
Consecutive calls to this function may return data of different modes, if
if some client program has changed the mode of the device via an
\fBXSetDeviceMode\fP request.
.P
You should use \fBXFreeDeviceMotionEvents\fP to free the data returned by this 
function.
.P
Errors returned by this function:  \fBBadDevice\fP, \fBBadMatch\fP,
\fBBadWindow\fP.
.P
.DS
\f(CW
XFreeDeviceMotionEvents (events)
        XDeviceTimeCoord *events;
\fP
.DE
.P
.VL 20
.LI \fIevents\fP
Specifies the pointer to the \fBXDeviceTimeCoord\fP array returned by
a previous call to \fBXGetDeviceMotionEvents\fP.
.LE
.P
This function frees the array of motion information.
.\"
.\"
.\"  Appendicies
.\"
.\"
.SK
.ds Ch <X11/XInput.h> Header File
.nr H1 1
.so S.aformat
.P
The following information is contained in the \fB<X11/XInput.h>\fP
header file:
.DS
\f(CW
/* $Header: XInput.h,v 1.7 89/10/13 14:03:24 gms Exp $ */

/************************************************************
Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, California, and the 
Massachusetts Institute of Technology, Cambridge, Massachusetts.

			All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Hewlett-Packard or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
HEWLETT-PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

********************************************************/

/* Definitions used by the library and client */

#ifndef _XLIB_H_
#include <X11/Xlib.h>
#endif

#ifndef _XI_H_
#include "XI.h"
#endif

#define _deviceKeyPress		0
#define _deviceKeyRelease	1

#define _deviceButtonPress	0
#define _deviceButtonRelease	1

#define _deviceMotionNotify	0

#define _deviceFocusIn		0
#define _deviceFocusOut		1

#define _proximityIn		0
#define _proximityOut		1

#define _deviceStateNotify	0
#define _deviceMappingNotify	1
#define _changeDeviceNotify	2

#define FindTypeAndClass(d, type, class, classid, offset) \
    { int i; XInputClassInfo *ip; \
    type = 0; class = 0; \
    for (i=0, ip= ((XDevice *) d)->classes; \
	 i< ((XDevice *) d)->num_classes; \
	 i++, ip++) \
	if (ip->input_class == classid) \
	    {type =  ip->event_type_base + offset; \
	     class =  ((XDevice *) d)->device_id << 8 | type;}}

#define DeviceKeyPress(d, type, class) \
    FindTypeAndClass(d, type, class, KeyClass, _deviceKeyPress)

#define DeviceKeyRelease(d, type, class) \
    FindTypeAndClass(d, type, class, KeyClass, _deviceKeyRelease)

#define DeviceButtonPress(d, type, class) \
    FindTypeAndClass(d, type, class, ButtonClass, _deviceButtonPress)

#define DeviceButtonRelease(d, type, class) \
    FindTypeAndClass(d, type, class, ButtonClass, _deviceButtonRelease)

#define DeviceMotionNotify(d, type, class) \
    FindTypeAndClass(d, type, class, ValuatorClass, _deviceMotionNotify)

#define DeviceFocusIn(d, type, class) \
    FindTypeAndClass(d, type, class, FocusClass, _deviceFocusIn)

#define DeviceFocusOut(d, type, class) \
    FindTypeAndClass(d, type, class, FocusClass, _deviceFocusOut)

#define ProximityIn(d, type, class) \
    FindTypeAndClass(d, type, class, ProximityClass, _proximityIn)

#define ProximityOut(d, type, class) \
    FindTypeAndClass(d, type, class, ProximityClass, _proximityOut)

#define DeviceStateNotify(d, type, class) \
    FindTypeAndClass(d, type, class, OtherClass, _deviceStateNotify)

#define DeviceMappingNotify(d, type, class) \
    FindTypeAndClass(d, type, class, OtherClass, _deviceMappingNotify)

#define ChangeDeviceNotify(d, type, class) \
    FindTypeAndClass(d, type, class, OtherClass, _changeDeviceNotify)


/***************************************************************
 *
 * DeviceKey events.  These events are sent by input devices that
 * support input class Keys.
 * The location of the X pointer is reported in the coordinate
 * fields of the x,y and x_root,y_root fields.
 *
 */

typedef struct 
    {
    int            type;         /* of event */
    unsigned long  serial;       /* # of last request processed */
    Bool           send_event;   /* true if from SendEvent request */
    Display        *display;     /* Display the event was read from */
    Window         window;       /* "event" window reported relative to */
    XID            deviceid;
    Window         root;         /* root window event occured on */
    Window         subwindow;    /* child window */
    Time           time;         /* milliseconds */
    int            x, y;         /* x, y coordinates in event window */
    int            x_root;       /* coordinates relative to root */
    int            y_root;       /* coordinates relative to root */
    unsigned int   state;        /* key or button mask */
    unsigned int   keycode;      /* detail */
    Bool           same_screen;  /* same screen flag */
    unsigned char  axes_count;
    unsigned char  first_axis;
    unsigned int   device_state; /* device key or button mask */
    int            axis_data[6];
    } XDeviceKeyEvent;

typedef XDeviceKeyEvent XDeviceKeyPressedEvent;
typedef XDeviceKeyEvent XDeviceKeyReleasedEvent;

/*******************************************************************
 *
 * DeviceButton events.  These events are sent by extension devices
 * that support input class Buttons.
 *
 */

typedef struct {
    int           type;         /* of event */
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;       /* "event" window reported relative to */
    XID           deviceid;
    Window        root;         /* root window that the event occured on */
    Window        subwindow;    /* child window */
    Time          time;         /* milliseconds */
    int           x, y;         /* x, y coordinates in event window */
    int           x_root;       /* coordinates relative to root */
    int           y_root;       /* coordinates relative to root */
    unsigned int  state;        /* key or button mask */
    unsigned int  button;       /* detail */
    Bool          same_screen;  /* same screen flag */
    unsigned char axes_count;
    unsigned char first_axis;
    unsigned int  device_state; /* device key or button mask */
    int           axis_data[6];
    } XDeviceButtonEvent;

typedef XDeviceButtonEvent XDeviceButtonPressedEvent;
typedef XDeviceButtonEvent XDeviceButtonReleasedEvent;

/*******************************************************************
 *
 * DeviceMotionNotify event.  These events are sent by extension devices
 * that support input class Valuators.
 *
 */

typedef struct 
    {
    int           type;        /* of event */
    unsigned long serial;      /* # of last request processed by server */
    Bool          send_event;  /* true if from a SendEvent request */
    Display       *display;    /* Display the event was read from */
    Window        window;      /* "event" window reported relative to */
    XID           deviceid;
    Window        root;        /* root window that the event occured on */
    Window        subwindow;   /* child window */
    Time          time;        /* milliseconds */
    int           x, y;        /* x, y coordinates in event window */
    int           x_root;      /* coordinates relative to root */
    int           y_root;      /* coordinates relative to root */
    unsigned int  state;       /* key or button mask */
    char          is_hint;     /* detail */
    Bool          same_screen; /* same screen flag */
    unsigned char axes_count;
    unsigned char first_axis;
    unsigned int  device_state; /* device key or button mask */
    int           axis_data[6];
    } XDeviceMotionEvent;

/*******************************************************************
 *
 * DeviceFocusChange events.  These events are sent when the focus
 * of an extension device that can be focused is changed.
 *
 */

typedef struct 
    {
    int           type;       /* of event */
    unsigned long serial;     /* # of last request processed by server */
    Bool          send_event; /* true if from a SendEvent request */
    Display       *display;   /* Display the event was read from */
    Window        window;     /* "event" window reported relative to */
    XID           deviceid;
    int           mode;       /* NotifyNormal, NotifyGrab, NotifyUngrab */
    int           detail;
	/*
	 * NotifyAncestor, NotifyVirtual, NotifyInferior, 
	 * NotifyNonLinear,NotifyNonLinearVirtual, NotifyPointer,
	 * NotifyPointerRoot, NotifyDetailNone 
	 */
    Time                time;
    } XDeviceFocusChangeEvent;

typedef XDeviceFocusChangeEvent XDeviceFocusInEvent;
typedef XDeviceFocusChangeEvent XDeviceFocusOutEvent;

/*******************************************************************
 *
 * ProximityNotify events.  These events are sent by those absolute
 * positioning devices that are capable of generating proximity information.
 *
 */

typedef struct 
    {
    int             type;      /* ProximityIn or ProximityOut */        
    unsigned long   serial;    /* # of last request processed by server */
    Bool            send_event; /* true if this came from a SendEvent request */
    Display         *display;  /* Display the event was read from */
    Window          window;      
    XID	            deviceid;
    Window          root;            
    Window          subwindow;      
    Time            time;            
    int             x, y;            
    int             x_root, y_root;  
    unsigned int    state;           
    Bool            same_screen;     
    unsigned char   axes_count;
    unsigned char   first_axis;
    unsigned int    device_state; /* device key or button mask */
    int             axis_data[6];
    } XProximityNotifyEvent;
typedef XProximityNotifyEvent XProximityInEvent;
typedef XProximityNotifyEvent XProximityOutEvent;

/*******************************************************************
 *
 * DeviceStateNotify events are generated on EnterWindow and FocusIn 
 * for those clients who have selected DeviceState.
 *
 */

typedef struct
    {
    unsigned char	class;
    unsigned char	length;
    } XInputClass;

typedef struct {
    int           type;
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if this came from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;
    XID           deviceid;
    Time          time;
    int           num_classes;
    char	  data[64];
} XDeviceStateNotifyEvent;	

typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_valuators;
    unsigned char	mode;
    int        		valuators[6];
} XValuatorStatus;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_keys;
    char        	keys[32];
} XKeyStatus;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_buttons;
    char        	buttons[32];
} XButtonStatus;

/*******************************************************************
 *
 * DeviceMappingNotify event.  This event is sent when the key mapping,
 * modifier mapping, or button mapping of an extension device is changed.
 *
 */

typedef struct {
    int           type;
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if this came from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;       /* unused */
    XID           deviceid;
    Time          time;
    int           request;      /* one of MappingModifier, MappingKeyboard,
                                    MappingPointer */
    int           first_keycode;/* first keycode */
    int           count;        /* defines range of change w. first_keycode*/
} XDeviceMappingEvent;

/*******************************************************************
 *
 * ChangeDeviceNotify event.  This event is sent when an 
 * XChangeKeyboard or XChangePointer request is made.
 *
 */

typedef struct {
    int           type;
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if this came from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    Window        window;       /* unused */
    XID           deviceid;
    Time          time;
    int           request;      /* NewPointer or NewKeyboard */
} XChangeDeviceNotifyEvent;

/*******************************************************************
 *
 * Control structures for input devices that support input class
 * Feedback.  These are used by the XGetFeedbackControl and 
 * XChangeFeedbackControl functions.
 *
 */

typedef struct {
     XID            class;
     int            length;
} XFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     click;
    int     percent;
    int     pitch;
    int     duration;
    int     led_mask;
    int     global_auto_repeat;
    char    auto_repeats[32];
} XKbdFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     accelNum;
    int     accelDenom;
    int     threshold;
} XPtrFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     resolution;
    int     minVal;
    int     maxVal;
} XIntegerFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     max_symbols;
    int     num_syms_supported;
    KeySym  *syms_supported;
} XStringFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     percent;
    int     pitch;
    int     duration;
} XBellFeedbackState;

typedef struct {
    XID     class;
    int     length;
    int     led_values;
} XLedFeedbackState;

typedef struct {
     XID            class;
     int            length;
} XFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     accelNum;
    int     accelDenom;
    int     threshold;
} XPtrFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     click;
    int     percent;
    int     pitch;
    int     duration;
    int     led_mask;
    int     led_value;
    int     key;
    int     auto_repeat_mode;
} XKbdFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     num_keysyms;
    KeySym  *syms_to_display;
} XStringFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     int_to_display;
} XIntegerFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     percent;
    int     pitch;
    int     duration;
} XBellFeedbackControl;

typedef struct {
    XID     class;
    int     length;
    int     led_mask;
    int     led_values;
} XLedFeedbackControl;

/*******************************************************************
 *
 * An array of XDeviceInfo structures is returned by the 
 * XListInputDevices function.  Each entry contains information
 * about one input device.  Among that information is an array of 
 * pointers to structures that describe the characteristics of 
 * the input device.
 *
 */

typedef struct _XAnyClassinfo *XAnyClassPtr;

typedef struct _XAnyClassinfo {
    XID 	class;
    int 	length;
    } XAnyClassInfo;

typedef struct _XDeviceInfo *XDeviceInfoPtr;

typedef struct _XDeviceInfo
    {
    XID                 id;        
    Atom                type;
    char                *name;
    int                 num_classes;
    int                 use;
    XAnyClassPtr 	inputclassinfo;
    } XDeviceInfo;

typedef struct _XKeyInfo *XKeyInfoPtr;

typedef struct _XKeyInfo
    {
    XID			class;
    int			length;
    unsigned short      min_keycode;
    unsigned short      max_keycode;
    unsigned short      num_keys;
    } XKeyInfo;

typedef struct _XButtonInfo *XButtonInfoPtr;

typedef struct _XButtonInfo {
    XID		class;
    int		length;
    short 	num_buttons;
    } XButtonInfo;

typedef struct _XAxisInfo *XAxisInfoPtr;

typedef struct _XAxisInfo {
    int 	resolution;
    int 	min_value;
    int 	max_value;
    } XAxisInfo;

typedef struct _XValuatorInfo *XValuatorInfoPtr;

typedef struct	_XValuatorInfo
    {
    XID			class;
    int			length;
    unsigned char       num_axes;
    unsigned char       mode;
    unsigned long       motion_buffer;
    XAxisInfoPtr        axes;
    } XValuatorInfo;


/*******************************************************************
 *
 * An XDevice structure is returned by the XOpenDevice function.  
 * It contains an array of pointers to XInputClassInfo structures.
 * Each contains information about a class of input supported by the
 * device, including a pointer to an array of data for each type of event
 * the device reports.
 *
 */


typedef struct {
        unsigned char   input_class;
        unsigned char   event_type_base;
} XInputClassInfo;

typedef struct {
        XID                    device_id;
        int                    num_classes;
        XInputClassInfo        *classes;
} XDevice;


/*******************************************************************
 *
 * The following structure is used to return information for the 
 * XGetSelectedExtensionEvents function.
 *
 */

typedef struct {
        XEventClass     event_type;
        XID             device;
} XEventList;

/*******************************************************************
 *
 * The following structure is used to return motion history data from 
 * an input device that supports the input class Valuators.
 * This information is returned by the XGetDeviceMotionEvents function.
 *
 */

typedef struct {
        Time            time;
        unsigned int    *data;
} XDeviceTimeCoord;


/*******************************************************************
 *
 * Device state structure.
 *
 */

typedef struct {
        XID		device_id;
        int		num_classes;
        XInputClass	*data;
} XDeviceState;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_valuators;
    unsigned char	mode;
    int        		*valuators;
} XValuatorState;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_keys;
    char        	keys[32];
} XKeyState;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    short		num_buttons;
    char        	buttons[32];
} XButtonState;

/*******************************************************************
 *
 * Function definitions.
 *
 */

XDevice                *XOpenDevice();
XDeviceInfo            *XListInputDevices();
XDeviceTimeCoord       *XGetDeviceMotionEvents();
KeySym                 *XGetDeviceKeyMapping();
XModifierKeymap        *XGetDeviceModifierMapping();
XFeedbackState         *XGetFeedbackControl();
XExtensionVersion      *XGetExtensionVersion();
XDeviceState           *XQueryDeviceState();
XEventClass            *XGetDeviceDontPropagateList();
\fP
.DE
.\"
.\"
.\"  Dump out a table of contents at the end.
.\"
.\"
.TC 1 1 6
