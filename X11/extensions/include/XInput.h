/* $XConsortium: XInput.h,v 1.5 89/12/06 20:44:32 rws Exp $ */

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

#ifndef _EXTUTIL_H_
#include "extutil.h"
#define _EXTUTIL_H_
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

#define DevicePointerMotionHint(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _devicePointerMotionHint;}

#define DeviceButton1Motion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButton1Motion;}

#define DeviceButton2Motion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButton2Motion;}

#define DeviceButton3Motion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButton3Motion;}

#define DeviceButton4Motion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButton4Motion;}

#define DeviceButton5Motion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButton5Motion;}

#define DeviceButtonMotion(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButtonMotion;}

#define DeviceOwnerGrabButton(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceOwnerGrabButton;}

#define DeviceButtonPressGrab(d, type, class) \
    { class =  ((XDevice *) d)->device_id << 8 | _deviceButtonGrab;}

#define BadDevice(dpy, error) \
    {XExtDisplayInfo 	*info = (XExtDisplayInfo *) XInput_find_display (dpy); \
     error = info->codes->first_error + XI_BadDevice;}

#define BadClass(dpy, error) \
    {XExtDisplayInfo 	*info = (XExtDisplayInfo *) XInput_find_display (dpy); \
     error = info->codes->first_error + XI_BadClass;}

#define BadEvent(dpy, error) \
    {XExtDisplayInfo 	*info = (XExtDisplayInfo *) XInput_find_display (dpy); \
     error = info->codes->first_error + XI_BadEvent;}

#define BadMode(dpy, error) \
    {XExtDisplayInfo 	*info = (XExtDisplayInfo *) XInput_find_display (dpy); \
     error = info->codes->first_error + XI_BadMode;}

#define DeviceBusy(dpy, error) \
    {XExtDisplayInfo 	*info = (XExtDisplayInfo *) XInput_find_display (dpy); \
     error = info->codes->first_error + XI_DeviceBusy;}

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
    unsigned int   device_state; /* device key or button mask */
    unsigned char  axes_count;
    unsigned char  first_axis;
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
    unsigned int  device_state; /* device key or button mask */
    unsigned char axes_count;
    unsigned char first_axis;
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
    unsigned int  device_state; /* device key or button mask */
    unsigned char axes_count;
    unsigned char first_axis;
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
    unsigned int    device_state; /* device key or button mask */
    unsigned char   axes_count;
    unsigned char   first_axis;
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
    int        		valuators[6];
} XValuatorStatus;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_keys;
    char        	keys[32];
} XKeyStatus;

typedef struct {
    unsigned char	class;
    unsigned char	length;
    unsigned char	num_buttons;
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
 * An array of XDeviceList structures is returned by the 
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
