.\" Input Extension Porting Document
.\"
.\" This report uses the -mm macro package for nroff.
.\" Use the script "proofit" to review this chapter using the
.\" X-windows proof utility.  
.\"
.\"
.so TitlePage.port
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
.ds Ch Initializing Extension Devices
.\"
.\"
.\"  Pull in the layout macro package.
.\"
.so Format.port
.\"
.tr ~
.P
Extension input devices are initialized in the same manner as the core 
X input devices.  Device-Independent X provides functions that can be 
called from DDX to initialize these devices.  Which functions are called
and when will vary by implementation, and will depend on whether the 
implementation opens all the input devices available to X when X is initialized,
or waits until a client requests that a device be opened.
In the simplest case, DDX will open all input devices as part of its
initialization, when the InitInput routine is called.
.H 2 "Summary of Calling Sequence"
.DS
Device-Independent X       |  Device-Dependent X
--------------------       |  -------------------             
                           |                                        
InitInput -------------->  |  - do device-specific initialization
                           |                                        
                           |  - call AddInputDevice  (deviceProc,AutoStart)
AddInputDevice             |   
  - creates DeviceIntRec   |
  - records deviceProc     |
  - adds new device to     | 
    list of off_devices.   |
sets dev->startup=AutoStart|           
                           |  - call one of:                       
                           |    - RegisterPointerDevice 
                           |      - processInputProc = ProcessPointerEvents
                           |    - RegisterKeyboardDevice 
                           |      - processInputProc = ProcessKeyboardEvents
                           |    - RegisterOtherDevice 
                           |      - processInputProc = ProcessOtherEvents
                           |                                        
                           |                                        
InitAndStartDevices -----> |  - calls deviceProc with parameters
                           |    (DEVICE_INIT, AutoStart)
sets dev->inited = return  |
  value from deviceProc    |    
                           |                                        
                           |  - in deviceProc, do one of:                       
                           |    - call InitPointerDeviceStruct (X pointer)
                           |    - call InitKeyboardDeviceStruct (X keybd)
                           |    - init extension device by calling some of:
                           |      - InitKeyClassDeviceStruct
                           |      - InitButtonClassDeviceStruct
                           |      - InitValuatorClassDeviceStruct
                           |      - InitValuatorAxisStruct
                           |      - InitFocusClassDeviceStruct
                           |      - InitProximityClassDeviceStruct
                           |      - InitKbdFeedbackClassDeviceStruct
                           |      - InitPtrFeedbackClassDeviceStruct
                           |    - init device name and type by:
                           |      - calling MakeAtom with one of the 
                           |        predefined names
                           |      - calling AssignTypeAndName
                           |                                        
                           |                                        
for each device added      |                                        
    by AddInputDevice,     |                                        
    InitAndStartDevices    |                                        
    calls EnableDevice if  |  - EnableDevice calls deviceProc with 
    dev->startup &         |    (DEVICE_ON, AutoStart)
    dev->inited            |  
                           |                                        
If deviceProc returns      |  - core devices are now enabled, extension
    Success, EnableDevice  |    devices are now available to be accessed
    move the device from   |    through the input extension protocol
    inputInfo.off_devices  |    requests.                           
    to inputInfo.devices   |                                        
.DE
.H 2 "Input Device Initialization Details"
InitInput is the first DDX routine called during X server startup.
This routine should
perform any device- and implementation- specific initialization, and call
AddInputDevice to create and initialize the DeviceIntRec structure for that
device.  AddInputDevice is passed the address of a procedure to be called
by the DIX routine InitAndStartDevices when input devices are enabled.
This procedure is expected to perform X initialization for the input device.
.P
If the device is to be used as the X pointer, DDX should then call
RegisterPointerDevice, passing the size of the motion buffer,
to initialize the device as the X pointer.
.P
If the device is to be used as the X keyboard, DDX should instead call
RegisterKeyboardDevice to initialize the device as the X keyboard.
.P
If the device is to be used as an extension device, DDX should instead
call RegisterOtherDevice, passing the DeviceIntPtr returned by
AddInputDevice.
.H 1 "Enabling Extension Devices"
InitAndStartDevices is the DIX routine that enables input devices. 
It calls the device initialization routine that was passed to AddInputDevice,
with a mode of DEVICE_INIT.  The action taken by the device initialization
routine depends on how the device is to be used.  If the device is to be
the X pointer, the device initialization routine should call
InitPointerDeviceStruct to initialize it.  If the device is to be the
X keyboard, the device initialization routine should call
InitKeyboardDeviceStruct.  If the device is to be used as an extension
device, each input class that it supports must be initialized.
.H 2 "Initializing Input Classes"
DIX procedures are defined to initialize each of the defined input classes.
The defined classes are:
.DL
.LI
KeyClass - the device has keys.
.LI
ButtonClass - the device has buttons.
.LI
ValuatorClass - the device reports motion data or positional data.
.LI
Proximitylass - the device reports proximity information.
.LI
FocusClass - the device can be focused.
.LI
PtrFeedbackClass - the device supports the setting of acceleration and threshold
.LI
KbdFeedbackClass - the device supports at least some of:  
.AL
.LI 
setting keyclick 
.LI 
setting keyclick volume
.LI
setting bell volume
.LI
setting  bell duration
.LI
setting bell pitch
.LI
setting key autorepeat
.LI
up to 32 LEDs.
.LE
.LE
.P
DIX routines are provided to initialize the X pointer and keyboard, as in
previous releases of X.  InitPointerDeviceStruct initializes the X pointer,
and InitKeyboardDeviceStruct initializes the X keyboard.  There is no
corresponding routine for extension input devices, since they do not all
support the same classes of input.  Instead, DDX is responsible for the 
initialization of the input classes supported by extension devices.  
A description of the routines provided by DIX to perform that initialization
follows.
.H 3 "InitKeyClassDeviceStruct"
This function is provided to allocate and initialize a KeyClassRec, and 
should be called for extension devices that have keys.  It is passed a pointer
to the device, and pointers to arrays of keysyms and modifiers reported by
the device.  It returns FALSE if the KeyClassRec could not be allocated,
or if the maps for the keysyms and and modifiers could not be allocated.
Its parameters are:
.P
.DS
Bool
InitKeyClassDeviceStruct(dev, pKeySyms, pModifiers)
    DeviceIntPtr dev;
    KeySymsPtr pKeySyms;
    CARD8 pModifiers[];
.DE
.H 3 "InitButtonClassDeviceStruct"
This function is provided to allocate and initialize a ButtonClassRec, and 
should be called for extension devices that have buttons.  It is passed a 
pointer to the device, the number of buttons supported, and a map of the 
reported button codes.  It returns FALSE if the ButtonClassRec could not be 
allocated.  Its parameters are:
.P
.DS
Bool
InitButtonClassDeviceStruct(dev, numButtons, map)
    register DeviceIntPtr dev;
    int numButtons;
    CARD8 *map;
.DE
.H 3 "InitValuatorClassDeviceStruct"
This function is provided to allocate and initialize a ValuatorClassRec, and 
should be called for extension devices that have valuators.  It is passed the
number of axes of motion reported by the device, the address of the motion
history procedure for the device, the size of the motion history buffer,
and the mode (Absolute or Relative) of the device.  It returns FALSE if 
the ValuatorClassRec could not be allocated.  Its parameters are:
.P
.DS
Bool
InitValuatorClassDeviceStruct(dev, numAxes, motionProc, numMotionEvents, mode)
    DeviceIntPtr dev;
    int (*motionProc)();
    int numAxes;
    int numMotionEvents;
    int mode;
.DE
.H 3 "InitValuatorAxisStruct"
This function is provided to allocate and initialize an XAxisInfoRec, and 
should be called for extension devices that have valuators.  It should
be called once for each axis of motion supported by the devices.  Each
invocation should be passed the axis number (starting with 0), the
minimum value for that axis, the maximum value for that axis, and the
resolution of the device in counts per meter.  If the device reports
relative motion, 0 should be reported as the minimum and maximum values.
InitValuatorAxisStruct has the following parameters:
.DS
InitValuatorAxisStruct(dev, axnum, minval, maxval, resolution)
    DeviceIntPtr dev;
    int axnum;
    int minval;
    int maxval;
    int resolution;
.DE
.H 3 "InitFocusClassDeviceStruct"
This function is provided to allocate and initialize a FocusClassRec, and 
should be called for extension devices that can be focused.  It is passed a
pointer to the device, and returns FALSE if the allocation fails.
It has the following parameter:
.DS
Bool
InitFocusClassDeviceStruct(dev)
    DeviceIntPtr dev;
.DE
.H 3 "InitProximityClassDeviceStruct"
This function is provided to allocate and initialize a ProximityClassRec, and 
should be called for extension absolute pointing devices that report proximity.
It is passed a pointer to the device, and returns FALSE if the allocation fails.
It has the following parameter:
.DS
Bool
InitProximityClassDeviceStruct(dev)
    DeviceIntPtr dev;
.DE
.H 3 "InitKbdFeedbackClassDeviceStruct"
This function is provided to allocate and initialize a KbdFeedbackClassRec, and 
should be called for extension devices that support some or all of the 
feedbacks that the core keyboard supports.  It is passed a
pointer to the device, a pointer to the procedure that sounds the bell,
and a pointer to the device control procedure.
It returns FALSE if the allocation fails, and has the following parameters:
.DS
Bool
InitKbdFeedbackClassDeviceStruct(dev, bellProc, controlProc)
    DeviceIntPtr dev;
    void (*bellProc)();
    void (*controlProc)();
.DE
.H 3 "InitPtrFeedbackClassDeviceStruct"
This function is provided to allocate and initialize a PtrFeedbackClassRec, and 
should be called for extension devices that allow the setting of acceleration
and threshold.  It is passed a pointer to the device,
and a pointer to the device control procedure.
It returns FALSE if the allocation fails, and has the following parameters:
.DS
Bool
InitPtrFeedbackClassDeviceStruct(dev, controlProc)
    DeviceIntPtr dev;
    void (*controlProc)();
.DE
.H 2 "Initializing The Device Name And Type"
The device name and type can be initialized by calling AssignTypeAndName
with the following parameters:
.DS
void
AssignTypeAndName(dev, type, name)
    DeviceIntPtr dev;
    Atom type;
    char *name;
.DE
.P
This will allocate space for the device name and copy the name that was passed.
The device type can be obtained by calling MakeAtom with one of the names
defined for input devices.  MakeAtom has the following parameters:
.DS
Atom
MakeAtom(name, len, makeit)
    char *name;
    int len;
    Bool makeit;
.DE
.P
Since the atom was already made when the input extension was initialized, the
value of makeit should be FALSE;
.H 2 "Initializing The Device Name And Type"
.H 1 "Other Implementation-Dependent Routines"
The input extension provides several routines that allow
implementation-specific processing to be done during calls to
some of the input extension requests.  These routines are 
initially empty, and may not be needed for all implementations.
If no implementation-specific processing is needed, the routines
can be left empty.
The following sections describe the routines provided.
.H 2 "AddOtherInputDevices"
Some implementations do not open extension input devices until requested
to do so by some client.  These implementations may not initialize
all devices when the X server starts up.  Since the XListInputDevices
function only lists those devices that have been initialized,
AddOtherInputDevices is first called to give DDX a chance to 
add any uninitialized devices.
.P
If all extension devices are initialized when the server 
starts up, this can be left an empty routine.
.H 2 "OpenInputDevice"
Some implementations may not actually open an input device until
requested by a client program.  OpenInputDevice is called when a client
makes an XOpenDevice request to allow DDX to open the device.  
.H 2 "CloseInputDevice"
Some implementations may close an input device when the last client
using that device requests that it be closed, or terminates.
CloseInputDevice is called when a client
makes an XCloseDevice request to allow DDX to close the device.  
.H 2 "SetDeviceMode"
Some input devices that report valuator data may be configured to report 
either absolute positional data or relative motion.  The XSetDeviceMode
protocol request is provided to allow DDX to change the current mode of 
such a device.
.H 2 "ChangePointerDevice"
The XChangePointerDevice request is provided to change which device is
used as the X pointer.  Some implementations may maintain information
specific to the X pointer in the private data structure pointed to by
the DeviceIntRec.  ChangePointerDevice is called to allow such 
implementations to move that information to the new pointer device.
The current location of the X cursor is an example of the type of 
information that might be affected.
.H 2 "ChangeKeyboardDevice"
The XChangeKeyboardDevice request is provided to change which device is
used as the X keyboard.  Some implementations may maintain information
specific to the X keyboard in the private data structure pointed to by
the DeviceIntRec.  ChangeKeyboardDevice is called to allow such 
implementations to move that information to the new keyboard device.
.H 1 "Input Extension Events"
Events accessed through the input extension are analogous to the core input
events, but have different event types.  They are of types 
\fBDeviceKeyPress\fP, \fBDeviceKeyRelease\fP, \fBDeviceButtonPress\fP,
\fBDeviceButtonRelease\fP, \fBDeviceDeviceMotionNotify\fP,
\fBDeviceProximityIn\fP, \fBDeviceProximityOut\fP, and \fBDeviceValuator\fP.
These event types are not constants.  Instead, they are external integers 
defined by the input extension.  The data structures that define these
events are defined in the file \fBextensions/include/XIproto.h\fP.  Other
input extension constants needed by DDX are defined in the file
\fBextensions/include/XI.h\fP.
.H 2 "Generating Multiple Events"
Some of the input extension events that clients receive are created by the
library reformatting routines from more than one wire event generated by 
the server.  For example, \fBDeviceMotionNotify\fP events are created by
the library combining a \fBDeviceMotionNotify\fP event and a 
\fBDeviceValuator\fP event.  This is necessary because the information that
is reported is larger than 32 bytes.
.P
In these cases, DDX generates multiple events and passes them to
ProcessOtherEvents as a single unit.  The number of xEvents being sent
is passed as the third parameter to ProcessOtherEvents.  DDX indicates to 
the library reformatting routines that an additional event follows by
ORing the device id in the previous event with the constant MORE_EVENTS.
.TC 1 1 6
