.sp 8
.ce 2
\s+2\fBXlib Changes From Release 3 to Release 4\fP\s-2
.sp 3
.LP
Copyright \(co 1989 by the Massachusetts Institute of Technology
.LP
Permission to use, copy, modify, and distribute this documentation for any
purpose and without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies.  MIT makes no
representations about the suitability for any purpose of the information in
this document.  This documentation is provided ``as is'' without express or
implied warranty.
.sp 3
.NH 1
Introduction
.LP
This document summarizes the major changes to Xlib since Release 3.
This document exists as an aid to those with R3-based books.
.NH 1
XOpenDisplay
.LP
The use of \fIunix\fP as a hostname in
.PN XOpenDisplay
is no longer part of the specification; use an empty
hostname to obtain the ``best'' local connection type.
.NH 1
XListDepths
.LP
To determine the number of depths that are available on a given screen, use
.PN XListDepths .
.FD 0
int *XListDepths\^(\^\fIdisplay\fP, \fIscreen_number\fP, \fIcount_return\fP\^)
.br
      Display *\fIdisplay\fP;
.br
      int \fIscreen_number\fP;
.br
      int *\fIcount_return\fP;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIscreen_number\fP 1i
Specifies the appropriate screen number on the host server.
.IP \fIcount_return\fP 1i
Returns the number of depths.
.LP
The
.PN XListDepths
function returns the array of depths 
that are available on the specified screen.
If the specified screen_number is valid and sufficient memory for the array
can be allocated,
.PN XListDepths
sets count_return to the number of available depths.
Otherwise, it does not set count_return and returns NULL.
To release the memory allocated for the array of depths, use
.PN XFree .
.NH 1
XListPixmapFormats
.LP
The
.PN XPixmapFormatValues
structure provides an interface to the pixmap format information
that is returned at the time of a connection setup.
It contains:
.LP
.Ds 0
.TA .5i 3i
.ta .5i 3i
typedef struct {
	int depth;
	int bits_per_pixel;
	int scanline_pad;
} XPixmapFormatValues;
.De
.sp
.LP
To obtain the pixmap format information for a given display, use
.PN XListPixmapFormats .
.FD 0
XPixmapFormatValues *XListPixmapFormats\^(\^\fIdisplay\fP, \fIcount_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      int *\fIcount_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIcount_return\fP 1i
Returns the number of pixmap formats that are supported by the display.
.LP
The
.PN XListPixmapFormats
function returns an array of
.PN XPixmapFormatValues
structures that describe the types of Z format images that are supported
by the specified display.
If insufficient memory is available,
.PN XListPixmapFormats
returns NULL.
To free the allocated storage for the
.PN XPixmapFormatValues
structures, use
.PN XFree .
.NH 1
XScreenNumberOfScreen
.FD 0
int XScreenNumberOfScreen\^(\^\fIscreen\fP\^)
.br
      Screen *\fIscreen\fP\^;
.FN
.IP \fIscreen\fP 1i
Specifies a pointer to the appropriate 
.PN Screen
structure.
.LP
The
.PN XScreenNumberOfScreen
function returns the screen index number of the specified screen.
.NH 1
XGetGCValues
.LP
To obtain components of a given GC, use
.PN XGetGCValues .
.FD 0
Status XGetGCValues\^(\^\fIdisplay\fP, \fIgc\fP, \fIvaluemask\fP, \
\fIvalues_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      GC \fIgc\fP\^;
.br
      unsigned long \fIvaluemask\fP\^;
.br
      XGCValues *\fIvalues_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIgc\fP 1i
Specifies the GC.
.IP \fIvaluemask\fP 1i
Specifies which components in the GC are to be returned
in the values_return argument.
This argument is the bitwise inclusive OR of one or more of the valid
GC component mask bits.
.IP \fIvalues_return\fP 1i
Returns the GC values in the specified
.PN XGCValues 
structure.
.LP
The
.PN XGetGCValues
function returns the components specified by valuemask for the specified GC.
Note that the clip mask and dash list (represented by the
.PN GCClipMask
and 
.PN GCDashList
bits, respectively, in the valuemask)
cannot be requested.
If the valuemask contains a valid set of GC mask bits
.Pn ( GCFunction ,
.PN GCPlaneMask ,
.PN GCForeground ,
.PN GCBackground ,
.PN GCLineWidth ,
.PN GCLineStyle ,
.PN GCCapStyle ,
.PN GCJoinStyle ,
.PN GCFillStyle ,
.PN GCFillRule ,
.PN GCTile ,
.PN GCStipple ,
.PN GCTileStipXOrigin ,
.PN GCTileStipYOrigin ,
.PN GCFont ,
.PN GCSubwindowMode ,
.PN GCGraphicsExposures ,
.PN GCClipXOrigin ,
.PN GCCLipYOrigin ,
.PN GCDashOffset ,
or
.PN GCArcMode )
and no error occur,
.PN XGetGCValues
sets the requested components in values_return and returns a nonzero status.
Otherwise, it returns a zero status.
.NH 1
Font Properties
.LP
The standard set of font properties has been deleted from Section 6.5,
and replaced with a reference to the X Consortium standard
\fIX Logical Font Description Conventions\fP.
.NH 1
Keyboard Groups and KeySym Mappings
.LP
A list of KeySyms is associated with each KeyCode.
The list is intended to convey the set of symbols on the corresponding key.
If the list (ignoring trailing NoSymbol entries) is 
a single KeySym ``\fIK\fP'',
then the list is treated as if it were the list 
``\fIK\fP NoSymbol \fIK\fP NoSymbol''.
If the list (ignoring trailing NoSymbol entries) is 
a pair of KeySyms ``\fIK1 K2\fP'',
then the list is treated as if it were the list ``\fIK1 K2 K1 K2\fP''.
If the list (ignoring trailing NoSymbol entries) is 
a triple of KeySyms ``\fIK1 K2 K3\fP'',
then the list is treated as if it were the list ``\fIK1 K2 K3\fP NoSymbol''.
When an explicit ``void'' element is desired in the list,
the value VoidSymbol can be used.
.LP
The first four elements of the list are split into two groups of KeySyms.
Group 1 contains the first and second KeySyms;
Group 2 contains third and fourth KeySyms.
Within each group,
if the second element of the group is NoSymbol,
then the group should be treated as if the second element were 
the same as the first element,
except when the first element is an alphabetic KeySym ``\fIK\fP'' 
for which both lowercase and uppercase forms are defined.
In that case,
the group should be treated as if the first element were 
the lowercase form of ``\fIK\fP'' and the second element were 
the uppercase form of ``\fIK\fP''.
.LP
The standard rules for obtaining a KeySym from a
.PN KeyPress
event make use of only the Group 1 and Group 2 KeySyms;
no interpretation of other KeySyms in the list is given here.
Which group to use is determined by modifier state.
Switching between groups is controlled by the KeySym named MODE SWITCH,
by attaching that KeySym to some KeyCode and attaching 
that KeyCode to any one of the modifiers Mod1 through Mod5.
This modifier is called the ``group modifier''. 
For any KeyCode,
Group 1 is used when the group modifier is off,
and Group 2 is used when the group modifier is on.
.LP
Within a group,
which KeySym to use is also determined by modifier state.
The first KeySym is used when the Shift and Lock modifiers are off.
The second KeySym is used when the Shift modifier is on,
when the Lock modifier is on and the second KeySym is uppercase alphabetic,
or when the Lock modifier is on and is interpreted as ShiftLock.
Otherwise, when the Lock modifier is on and is interpreted as CapsLock,
the state of the Shift modifier is applied first to select a KeySym,
but if that KeySym is lowercase alphabetic,
then the corresponding uppercase KeySym is used instead.
.LP
No spatial geometry of the symbols on the key is defined by
their order in the KeySym list, 
although a geometry might be defined on a
vendor-specific basis.
The X server does not use the mapping between KeyCodes and KeySyms.
Rather, it stores it merely for reading and writing by clients.
.LP
The KeyMask modifier named Lock is intended to be mapped 
to either a CapsLock or a ShiftLock key,
but which one is left as application-specific and/or user-specific.
However, it is suggested that the determination be made according to the
associated KeySym(s) of the corresponding KeyCode.
.LP
.PN XLookupString
implements this semantics.
.NH 1
XSetErrorHandler
.LP
.PN XSetErrorHandler
now returns the previous error handler.
.FD 0
int (*XSetErrorHandler\^(\^\fIhandler\fP\^)\^)\^(\^)
.br
      int (\^*\^\fIhandler\fP\^)\^(Display *, XErrorEvent *)
.FN
.NH 1
XSetIOErrorHandler
.LP
.PN XIOSetErrorHandler
now returns the previous error handler.
.FD 0
int (*XSetIOErrorHandler\^(\^\fIhandler\fP\^)\^)\^(\^)
.br
      int (\^*\^\fIhandler\fP\^)(Display *);
.FN
.NH 1
XGetErrorDatabaseText
.LP
For an extension request,
the extension name (as given by
.PN InitExtension )
followed by a period (\.) and the minor request protocol number 
is used for the message argument.
.NH 1
Inter-Client Communication Functions
.LP
The \fIInter-Client Communication Conventions Manual\fP (ICCCM) details the
X Consortium approved conventions that govern inter-client communications.
These conventions ensure peer-to-peer client cooperation in the use 
of selections, cut buffers, and shared resources as well as client cooperation
with window and session managers.
For further information,
see the \fIInter-Client Communication Conventions Manual\fP.
.NH 1
XStringToKeysym and XKeysymToString
.LP
KeySysms that are not part of the Xlib standard may be obtained by using
.PN XStringToKeysym
and
.PN XKeysymToString .
Note that the set of KeySysms that are available in this manner 
and the mechanisms by which Xlib obtains them is implementation dependent.
(In the MIT sample implementation, the resource file /usr/lib/X11/XKeysymDB
is used.  The keysym name is used as the resource name, and the resource
value is the keysym value in uppercase hexadecimal.)
.NH 1
Superceded Functions
.LP
The following functions are still supported, but are superceded by newer
interfaces:
.PN XSetStandardProperties
(by
.PN XSetWMProperties ),
.PN XSetNormalHints
(by
.PN XSetWMNormalHints ),
.PN XGetNormalHints
(by
.PN XGetWMNormalHints ),
.PN XSetSizeHints
(by
.PN XSetWMizeHints ),
.PN XGetSizeHints
(by
.PN XGetWMSizeHints ),
.PN XGetStandardColormap
(by
.PN XGetRGBColormap ),
.PN XSetStandardColormap
(by
.PN XSetRGBColormap ),
and
.PN XGeometry
(by
.PN XWMGeometry ).
.LP
The following functions still exist for compatibility,
but the functionality is not supported by the
\fIInter-Client Communication Conventions Manual\fP:
.PN XSetZoomHints
and
.PN XGetZoomHints .
.NH 1
XReadBitmapFile
.LP
The ability to read X10 format is no longer required of an implementation.
The ability to read other than the standard format is implementation dependent.
.NH 1
Resource Specifications
.LP
Resource specifications are usually stored in human-readable files
and in server properties (see
.PN XResourceManagerString ).
The BNF of a resource specification is:
.LP
.Ds 0
.TA 1.5i 1.75i
.ta 1.5i 1.75i
ResourceLine	=	Comment | ResourceSpec
Comment	=	"!" string | <empty line>
ResourceSpec	=	WhiteSpace ResourceName WhiteSpace ":" WhiteSpace value
ResourceName	=	[Binding] ComponentName {Binding ComponentName}
Binding	=	"\&." | "*"
WhiteSpace	=	{" " | "\\t"}
ComponentName	=	{"a"\-"z" | "A"\-"Z" | "0"\-"9" | "_" | "-"}
value	=	string
string	=	{<any character not including "\\n">}
.De
.LP
Note that elements enclosed in curly braces ({\&.\&.\&.}) indicate
zero or more occurrences of the enclosed elements
.LP
To allow values to contain arbitrary octets,
the 4-character sequence \\\fInnn\fP\^,
where n is a digit in the range of "0"\-"7",
is recognized and replaced with a single byte that contains
this sequence interpreted as an octal number.
For example,
a value containing a NULL byte can be stored by specifying "\\000".
.NH 1
XrmDestroyDatabase
.LP
To destroy a resource database and free its allocated memory, use
.PN XrmDestroyDatabase .
.FD 0
void XrmDestroyDatabase\^(\^\fIdatabase\fP\^)
.br
      XrmDatabase \fIdatabase\fP\^;
.FN
.IP \fIdatabase\fP 1i
Specifies the resource database.
.LP
If database is NULL,
.PN XrmDestroyDatabase
returns immediately.
.NH 1
XrmParseCommand
.LP
A new value,
.PN XrmoptionSkipNArgs ,
has been added to 
.PN XrmOptionKind .
.LP
.Ds 0
.TA .5i 2.5i
.ta .5i 2.5i
typedef enum {
	XrmoptionNoArg,	/* Value is specified in XrmOptionDescRec.value */
	XrmoptionIsArg,	/* Value is the option string itself */
	XrmoptionStickyArg,	/* Value is characters immediately following option */
	XrmoptionSepArg,	/* Value is next argument in argv */
	XrmoptionResArg,	/* Resource and value in next argument in argv */
	XrmoptionSkipArg,	/* Ignore this option and the next argument in argv */
	XrmoptionSkipLine,	/* Ignore this option and the rest of argv */
	XrmoptionSkipNArgs	/* Ignore this option and the next
		\ \ \ XrmOptionDescRec.value arguments in argv */
} XrmOptionKind;
.De
.LP
Note that
.PN XrmoptionSkipArg
is equivalent to 
.PN XrmoptionSkipNArgs
with the
.PN XrmOptionDescRec.value
field containing the value one.
Note also that the value zero for
.PN XrmoptionSkipNArgs
indicates that only the option itself is to be skipped.
.NH 1
Manipulating Top-Level Windows
.LP
Xlib provides functions that you can use to change the visibility or size
of top-level windows (that is, those that were created as a children 
of the root window).
Note that the subwindows that you create are ignored by window managers.
Therefore,
you should use the basic window functions described in Chapter 3
to manipulate your application's subwindows.
.LP
To request that a top-level window be iconified, use
.PN XIconifyWindow .
.FD 0
Status XIconifyWindow\^(\^\fIdisplay\fP, \fIw\fP, \fIscreen_number\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP;
.br
      int \fIscreen_number\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIscreen_number\fP 1i
Specifies the appropriate screen number on the host server.
.LP
The 
.PN XIconifyWindow 
function sends a WM_CHANGE_STATE 
.PN ClientMessage 
event with a format of 32 and a first data element of 
.PN IconicState 
(as described in Section 4.1.4 of the 
\fIInter-Client Communication Conventions Manual\fP)
to the root window of the specified screen.
Window managers may elect to receive this message and, 
if the window is in its normal state, 
may treat it as a request to change the window's state from normal to iconic.
If the WM_CHANGE_STATE property cannot be interned, 
.PN XIconifyWindow
does not send a message and returns a zero status.
It returns a nonzero status if the client message is sent successfully;
otherwise, it returns a zero status.
.LP
.PN XIconifyWindow 
can generate a
.PN BadWindow
error.
.sp
.LP
To request that a top-level window be withdrawn, use
.PN XWithdrawWindow .
.FD 0
Status XWithdrawWindow\^(\^\fIdisplay\fP, \fIw\fP, \fIscreen_number\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      int \fIscreen_number\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIscreen_number\fP 1i
Specifies the appropriate screen number on the host server.
.LP
The 
.PN XWithdrawWindow 
function unmaps the specified window 
and sends a synthetic 
.PN UnmapNotify 
event to the root window of the specified screen.
Window managers may elect to receive this message 
and may treat it as a request to change the window's state to withdrawn.
When a window is in the withdrawn state, 
neither its normal nor its iconic representations is visible.
It returns a nonzero status if the 
.PN UnmapNotify 
event is successfully sent; 
otherwise, it returns a zero status.
.LP
.PN XWithdrawWindow
can generate a
.PN BadWindow
error.
.sp
.LP
To request that a top-level window be reconfigured, use
.PN XReconfigureWMWindow .
.FD 0
Status XReconfigureWMWindow\^(\^\fIdisplay\fP, \fIw\fP, \fIscreen_number\fP, \
\fIvalue_mask\fP, \fIvalues\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      int \fIscreen_number\fP\^;
.br
      unsigned int \fIvalue_mask\fP\^;
.br
      XWindowChanges *\fIvalues\fP;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIscreen_number\fP 1i
Specifies the appropriate screen number on the host server.
.IP \fIvalue_mask\fP 1i
Specifies which values are to be set using information in
the values structure.
This mask is the bitwise inclusive OR of the valid configure window values bits.
.IP \fIvalues\fP 1i
Specifies a pointer to the 
.PN XWindowChanges 
structure.
.LP
The 
.PN XReconfigureWMWindow 
function issues a 
.PN ConfigureWindow 
request on the specified top-level window.
If the stacking mode is changed and the request fails with a 
.PN BadMatch 
error, 
the error event is trapped and a synthetic 
.PN ConfigureRequestEvent 
containing the same configuration parameters is sent to the root 
of the specified window.
Window managers may elect to receive this event 
and treat it as a request to reconfigure the indicated window.
.LP
.PN XReconfigureWMWindow 
can generate
.PN BadValue 
and 
.PN BadWindow 
errors.
.NH 1
Converting String Lists
.LP
Many of the text properties allow a variety of types and formats.
Because the data stored in these properties are not
simple null-terminated strings, a
.PN XTextProperty
structure is used to describe the encoding, type, and length of the text 
as well as its value.
The
.PN XTextProperty
structure contains:
.Ds 0
.TA .5i 2.5i
.ta .5i 2.5i
typedef struct 		{
	unsigned char *value;	/* property data */
	Atom encoding;	/* type of property */
	int format;	/* 8, 16, or 32 */
	unsigned long nitems;	/* number of items in value */
} XTextProperty;
.De
.LP
Xlib provides functions that you can use to convert between lists of pointers 
to character strings and text properties.
.sp
.LP
To set the specified list of strings to a 
.PN XTextProperty
structure, use
.PN XStringListToTextProperty .
.FD 0
Status XStringListToTextProperty\^(\^\fIlist\fP, \fIcount\fP, \
\fItext_prop_return\fP\^)
.br
      char **\fIlist\fP\^;
.br
      int \fIcount\fP\^;
.br
      XTextProperty *\fItext_prop_return\fP\^;
.FN
.IP \fIlist\fP 1i
Specifies a list of null-terminated character strings.
.IP \fIcount\fP 1i
Specifies the number of strings.
.IP \fItext_prop_return\fP 1i
Returns the
.PN XTextProperty
structure.
.LP
The 
.PN XStringListToTextProperty 
function sets the specified 
.PN XTextProperty
to be of type STRING (format 8) with a value representing the
concatenation of the specified list of null-separated character strings.
An extra byte containing NULL (which is not included in the nitems member) 
is stored at the end of the value field of text_prop_return.
If insufficient memory is available for the new value string, 
.PN XStringListToTextProperty
does not set any fields in the
.PN XTextProperty
structure and returns a zero status.
Otherwise, it returns a non-zero status.
To free the storage for the value field, use 
.PN XFree .
.sp
.LP
To obtain a list of strings from a specified
.PN XTextProperty
structure, use
.PN XTextPropertyToStringList .
.FD 0
Status XTextPropertyToStringList\^(\^\fItext_prop\fP, \fIlist_return\fP, \
\fIcount_return\fP\^)
.br
       XTextProperty *\fItext_prop\fP\^;
.br
       char ***\fIlist_return\fP\^;
.br
       int *\fIcount_return\fP\^;
.FN
.IP \fItext_prop\fP 1i
Specifies the
.PN XTextProperty
structure to be used.
.IP \fIlist_return\fP 1i
Returns a list of null-terminated character strings.
.IP \fIcount_return\fP 1i
Returns the number of strings.
.LP
The 
.PN XTextPropertyToStringList 
function returns a list of strings representing the null-separated elements 
of the specified
.PN XTextProperty
structure.
The data in text_prop must be of type STRING and format 8. 
Multiple elements of the property 
(for example, the strings in a disjoint text selection) 
are separated by a NULL (encoding 0).
The contents of the property are not null-terminated.
If insufficient memory is available for the list and its elements, 
.PN XTextPropertyToStringList
sets no return values and returns a zero status.
Otherwise, it returns a non-zero status.
To free the storage for the list and its contents, use 
.PN XFreeStringList .
.sp
.LP
To free the in-memory data associated with the specified string list, use
.PN XFreeStringList .
.FD 0
void XFreeStringList\^(\^\fIlist\fP\^)
.br
      char **\fIlist\fP\^;
.FN
.IP \fIlist\fP 1i
Specifies the list of strings to be freed.
.LP
The 
.PN XFreeStringList 
function releases memory allocated by 
.PN XTextPropertyToStringList .
.NH 1
Setting and Reading Text Properties
.LP
Xlib provides two functions that you can use to set and read
the text properties for a given window.
You can uses these functions set and read those properties of type TEXT
(WM_NAME, WM_ICON_NAME, WM_COMMAND, and WM_CLIENT_MACHINE).
In addition,
Xlib provides separate convenience functions that you can use to set each 
of these properties.
For further information about these convenience functions,
see sections 9.1.4, 9.1.5, 9.2.1, and 9.2.2, respectively.
.sp
.LP
To set one of a window's text properties, use
.PN XSetTextProperty .
.FD 0
void XSetTextProperty\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop\fP, \
\fIproperty\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop\fP\^;
.br
      Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop\fP 1i
Specifies the
.PN XTextProperty
structure to be used.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The
.PN XSetTextProperty
function replaces the existing, specified property for the named window 
with the data, type, format, and number of items determined by the value field,
the encoding field, the format field, and the nitems field, respectively,
of the specified
.PN XTextProperty
structure.
If the property does not already exist,
.PN XSetTextProperty
sets it for the specified window.
.LP
.PN XSetTextProperty
can generate
.PN BadAlloc ,
.PN BadAtom , 
.PN BadValue , 
and 
.PN BadWindow  
errors.
.sp
.LP
To read one of a window's text properties, use
.PN XGetTextProperty .
.FD 0
Status XGetTextProperty\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop_return\fP, \
\fIproperty\fP\^)
.br
       Display *\fIdisplay\fP\^;
.br
       Window \fIw\fP\^;
.br
       XTextProperty *\fItext_prop_return\fP\^;
.br
       Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop_return\fP 1i
Returns the
.PN XTextProperty
structure.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The
.PN XGetTextProperty 
function reads the specified property from the window
and stores the data in the returned
.PN XTextProperty
structure.
It stores the data in the value field,
the type of the data in the encoding field.
the format of the data in the format field, 
and the number of items of data in the nitems field.
The particular interpretation of the property's encoding 
and data as ``text'' is left to the calling application.
If the specified property does not exist on the window,
.PN XGetTextProperty
sets the value field to NULL, 
the encoding field to None, 
the format field to 0, 
and the nitems field to zero.
.LP
If it was able to set these files in the
.PN XTextProperty
structure,
.PN XGetTextProperty
returns a non-zero status; 
otherwise, it returns a zero status.
.LP
.PN XGetTextProperty
can generate
.PN BadAtom 
and 
.PN BadWindow 
errors.
.NH 1
Setting and Reading the WM_NAME Property
.LP
Xlib provides convenience functions that you can use to set and read 
the WM_NAME property for a given window.
.sp
.LP
To set a window's WM_NAME property with the supplied convenience function, use
.PN XSetWMName .
.FD 0
void XSetWMName\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop\fP 1i
Specifies the
.PN XTextProperty
structure to be used.
.LP
The
.PN XSetWMName
convenience function performs a 
.PN XSetTextProperty 
on the WM_NAME property (see section 9.1.3).
.sp
.LP
To read a window's WM_NAME property with the supplied convenience function, use
.PN XGetWMName .
.FD 0
Status XGetWMName\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop_return\fP 1i
Returns the
.PN XTextProperty
structure.
.LP
The
.PN XGetWMName 
convenience function performs an 
.PN XGetTextProperty 
on the WM_NAME property (see section 9.1.3).
.LP
The next two functions have been superceded by
.PN XSetWMName
and
.PN XGetWMName ,
respectively. 
You can use these additional convenience functions 
for window names that are encoded as STRING properties.
.NH 1
Setting and Reading the WM_ICON_NAME Property
.LP
Xlib provides convenience functions that you can use to set and read 
the WM_ICON_NAME property for a given window.
.LP
.sp
To set a window's WM_ICON_NAME property,
use
.PN XSetWMIconName .
.FD 0
void XSetWMIconName\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop\fP 1i
Specifies the
.PN XTextProperty
structure to be used.
.LP
The
.PN XSetWMIconName
convenience function performs a
.PN XSetTextProperty
on the WM_ICON_NAME property (see section 9.1.3).
.sp
.LP
To read a window's WM_ICON_NAME property,
use
.PN XGetWMIconName .
.FD 0
Status XGetWMIconName\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop_return\fP 1i
Returns the
.PN XTextProperty
structure.
.LP
The 
.PN XGetWMIconName 
convenience function performs an 
.PN XGetTextProperty 
on the WM_ICON_NAME property (see section 9.1.3).
.LP
The next two functions have been superceded by
.PN XSetWMIconName
and
.PN XGetWMIconName ,
respectively.
You can use these additional convenience functions 
for window names that are encoded as STRING properties.
.NH 1
Setting and Reading the WM_HINTS Property
.LP
Xlib provides functions that you can use to set and read 
the WM_HINTS property for a given window.
These functions use the flags and the
.PN XWMHints 
structure, as defined in the
.Pn < X11/Xutil.h >
header file.
.sp
.LP
To allocate an
.PN XWMHints
structure, use
.PN XAllocWMHints .
.FD 0
XWMHints *XAllocWMHints\^(\|)
.FN
.LP
The
.PN XAllocWMHints
function allocates and returns a pointer to a
.PN XWMHints
structure.
Note that all fields in the
.PN XWMHints
structure are initially set to zero.
If insufficient memory is available, 
.PN XAllocWMHints
returns NULL.
To free the memory allocated to this structure,
use
.PN XFree .
.LP
The
.PN XWMHints
structure contains:
.LP
/* Window manager hints mask bits */
.TS
lw(.5i) lw(2.5i) lw(2.5i).
T{
#define
T}	T{
.PN InputHint
T}	T{
(1L << 0)
T}
T{
#define
T}	T{
.PN StateHint
T}	T{
(1L << 1)
T}
T{
#define
T}	T{
.PN IconPixmapHint
T}	T{
(1L << 2)
T}
T{
#define
T}	T{
.PN IconWindowHint
T}	T{
(1L << 3)
T}
T{
#define
T}	T{
.PN IconPositionHint
T}	T{
(1L << 4)
T}
T{
#define
T}	T{
.PN IconMaskHint
T}	T{
(1L << 5)
T}
T{
#define
T}	T{
.PN WindowGroupHint
T}	T{
(1L << 6)
T}
T{
#define
T}	T{
.PN AllHints
T}	T{
(InputHint|StateHint|IconPixmapHint|
.br
IconWindowHint|IconPositionHint|
.br
IconMaskHint|WindowGroupHint)
T}
.TE
.Ds 0
.TA .5i 2.5i
.ta .5i 2.5i
/* Values */

typedef struct {
	long flags;	/* marks which fields in this structure are defined */
	Bool input;	/* does this application rely on the window manager to
			get keyboard input? */
	int initial_state;	/* see below */
	Pixmap icon_pixmap;	/* pixmap to be used as icon */
	Window icon_window;	/* window to be used as icon */
	int icon_x, icon_y;	/* initial position of icon */
	Pixmap icon_mask;	/* pixmap to be used as mask for icon_pixmap */
	XID window_group;	/* id of related window group */
	/* this structure may be extended in the future */
} XWMHints;
.De
.LP
The input member is used to communicate to the window manager the input focus
model used by the application.
Applications that expect input but never explicitly set focus to any 
of their subwindows (that is, use the push model of focus management), 
such as X10-style applications that use real-estate
driven focus, should set this member to 
.PN True .  
Similarly, applications
that set input focus to their subwindows only when it is given to their
top-level window by a window manager should also set this member to 
.PN True .
Applications that manage their own input focus by explicitly setting
focus to one of their subwindows whenever they want keyboard input 
(that is, use the pull model of focus management) should set this member to 
.PN False .
Applications that never expect any keyboard input also should set this member
to 
.PN False .
.LP
Pull model window managers should make it possible for push model
applications to get input by setting input focus to the top-level windows of
applications whose input member is 
.PN True .  
Push model window managers should
make sure that pull model applications do not break them 
by resetting input focus to 
.PN PointerRoot 
when it is appropriate (for example, whenever an application whose
input member is 
.PN False 
sets input focus to one of its subwindows).
.LP
The definitions for the initial_state flag are:
.TS
lw(.5i) lw(2i) lw(.15i) lw(2.75i).
T{
#define
T}	T{
.PN WithdrawnState
T}	T{
0
T}	T{
T}
T{
#define
T}	T{
.PN NormalState
T}	T{
1
T}	T{
/* most applications start this way */
T}
T{
#define
T}	T{
.PN IconicState
T}	T{
3
T}	T{
/* application wants to start as an icon */
T}
.TE
The icon_mask specifies which pixels of the icon_pixmap should be used as the
icon.  
This allows for nonrectangular icons.
Both icon_pixmap and icon_mask must be bitmaps.
The icon_window lets an application provide a window for use as an icon
for window managers that support such use.
The window_group lets you specify that this window belongs to a group
of other windows.
For example, if a single application manipulates multiple 
top-level windows, this allows you to provide enough
information that a window manager can iconify all of the windows
rather than just the one window.
.LP
.sp
To set a window's WM_HINTS property, use
.PN XSetWMHints .
.FD 0
XSetWMHints\^(\^\fIdisplay\fP, \fIw\fP, \fIwmhints\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XWMHints *\fIwmhints\fP\^;

.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIwmhints\fP 1i
Specifies the 
.PN XWMHints
structure to be used.
.LP
The
.PN XSetWMHints
function sets the window manager hints that include icon information and location,
the initial state of the window, and whether the application relies on the
window manager to get keyboard input.
.LP
.PN XSetWMHints
can generate
.PN BadAlloc
and
.PN BadWindow 
errors.
.LP
.sp
To read a window's WM_HINTS property, use
.PN XGetWMHints .
.FD 0
XWMHints *XGetWMHints\^(\^\fIdisplay\fP, \fIw\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.LP
The
.PN XGetWMHints
function reads the window manager hints and 
returns NULL if no WM_HINTS property was set on the window 
or returns a pointer to a 
.PN XWMHints 
structure if it succeeds.
When finished with the data,
free the space used for it by calling
.PN XFree .
.LP
.PN XGetWMHints
can generate a
.PN BadWindow 
error.
.NH 1
Setting and Reading the WM_NORMAL_HINTS Property
.LP
Xlib provides functions that you can use to set or read 
the WM_NORMAL_HINTS property for a given window.
The functions use the flags and the
.PN XSizeHints 
structure, as defined in the
.Pn < X11/Xutil.h >
header file.
.sp
.LP
To allocate an
.PN XSizeHints
structure, use
.PN XAllocSizeHints .
.FD 0
XSizeHints *XAllocSizeHints\^(\|)
.FN
.LP
The
.PN XAllocSizeHints
function allocates and returns a pointer to a
.PN XSizeHints
structure.
Note that all fields in the
.PN XSizeHints
structure are initially set to zero.
If insufficient memory is available, 
.PN XAllocSizeHints
returns NULL.
To free the memory allocated to this structure,
use
.PN XFree .
.LP
The
.PN XSizeHints
structure contains:
.LP
/* Size hints mask bits */
.TS
lw(.5i) lw(1.25i) lw(.75i) lw(3.25i).
T{
#define
T}	T{
.PN USPosition
T}	T{
(1L << 0)
T}	T{
/* user specified x, y */
T}
T{
#define
T}	T{
.PN USSize
T}	T{
(1L << 1)
T}	T{
/* user specified width, height */
T}
T{
#define
T}	T{
.PN PPosition
T}	T{
(1L << 2)
T}	T{
/* program specified position */
T}
T{
#define
T}	T{
.PN PSize
T}	T{
(1L << 3)
T}	T{
/* program specified size */
T}
T{
#define
T}	T{
.PN PMinSize
T}	T{
(1L << 4)
T}	T{
/* program specified minimum size */
T}
T{
#define
T}	T{
.PN PMaxSize
T}	T{
(1L << 5)
T}	T{
/* program specified maximum size */
T}
T{
#define
T}	T{
.PN PResizeInc
T}	T{
(1L << 6)
T}	T{
/* program specified resize increments */
T}
T{
#define
T}	T{
.PN PAspect
T}	T{
(1L << 7)
T}	T{
/* program specified min and max aspect ratios */
T}
T{
#define
T}	T{
.PN PBaseSize
T}	T{
(1L << 8)
T}
T{
#define
T}	T{
.PN PWinGravity
T}	T{
(1L << 9)
T}
T{
#define
T}	T{
.PN PAllHints
T}	T{
T}	T{
(PPosition|PSize|PMinSize|PMaxSize|
.br
PResizeInc|PAspect)
T}
.TE
.Ds 0
.TA .5i 2.5i
.ta .5i 2.5i
/* Values */

typedef struct {
	long flags;	/* marks which fields in this structure are defined */
	int x, y;	/* Obsolete */
	int width, height;	/* Obsolete */
	int min_width, min_height;
	int max_width, max_height;
	int width_inc, height_inc;
	struct {
	       int x;	/* numerator */
	       int y;	/* denominator */
	} min_aspect, max_aspect;
	int base_width, base_height;	/* New */
	int win_gravity;	/* New */
} XSizeHints;
.De
.LP
The x, y, width, and height members are now obsolete
and are left solely for compatibility reasons.
The min_width and min_height members specify the
minimum window size that still allows the application to be useful.
The max_width and max_height members specify the maximum window size.
The width_inc and height_inc members define an arithmetic progression of
sizes (minimum to maximum) into which the window prefers to be resized.
The min_aspect and max_aspect members are expressed
as ratios of x and y, 
and they allow an application to specify the range of aspect
ratios it prefers.
The base_width and base_height members define the desired size of the window.
The win_gravity member defines the region of the window that is to be retained
when it is resized.
.LP
Note that use of the
.PN PAllHints
macro is highly discouraged.
.sp
.LP
To set a window's WM_NORMAL_HINTS property, use
.PN XSetWMNormalHints .
.FD 0
void XSetWMNormalHints\^(\^\fIdisplay\fP, \fIw\fP, \fIhints\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XSizeHints *\fIhints\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIhints\fP 1i
Specifies the size hints for the window in its normal state.
.LP
The 
.PN XSetWMNormalHints 
function replaces the size hints for the WM_NORMAL_HINTS property 
on the specified window.
If the property does not already exist,
.PN XSetWMNormalHints
sets the size hints for the WM_NORMAL_HINTS property on the specified window.
The property is stored with a type of WM_SIZE_HINTS and a format of 32.
.LP
.PN XSetWMNormalHints
can generate
.PN BadAlloc
and
.PN BadWindow
errors.
.sp
.LP
To read a window's WM_NORMAL_HINTS property, use
.PN XGetWMNormalHints .
.FD 0
Status XGetWMNormalHints\^(\^\fIdisplay\fP, \fIw\fP, \fIhints_return\fP, \
\fIsupplied_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XSizeHints *\fIhints_return\fP\^;
.br
      long *\fIsupplied_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIhints_return\fP 1i
Returns the size hints for the window in its normal state.
.IP \fIsupplied_return\fP 1i
Returns the hints that were supplied by the user.
.LP
The 
.PN XGetWMNormalHints 
function returns the size hints stored in the WM_NORMAL_HINTS property 
on the specified window.
If the property is of type WM_SIZE_HINTS, of format 32,
and is long enough to contain either an old (pre-ICCCM) 
or new size hints structure, 
.PN XGetWMNormalHints
sets the various fields of the 
.PN XSizeHints
structure, sets the supplied_return argument to the list of fields 
that were supplied by the user (whether or not they contained defined values)
and returns a non-zero status.
Otherwise, it returns a zero status.
.LP
If 
.PN XGetWMNormalHints
returns successfully and a pre-ICCCM size hints property is read, 
the supplied_return argument will contain the following bits:
.LP
.Ds
(USPosition|USSize|PPosition|PSize|PMinSize|
 PMaxSize|PResizeInc|PAspect)
.De
.LP
If the property is large enough to contain the base size 
and window gravity fields as well, 
the supplied_return argument will also contain the following bits:
.LP
.Ds
PBaseSize|PWinGravity
.De
.LP
.PN XGetWMNormalHints
can generate a
.PN BadWindow
error.
.sp
.LP
To set a window's WM_SIZE_HINTS property, use
.PN XSetWMSizeHints .
.FD 0
void XSetWMSizeHints\^(\^\fIdisplay\fP, \fIw\fP, \fIhints\fP, \fIproperty\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XSizeHints *\fIhints\fP\^;
.br
      Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIhints\fP 1i
Specifies the
.PN XSizeHints
structure to be used.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The 
.PN XSetWMSizeHints 
function replaces the size hints for the specified property 
on the named window.
If the specified property does not already exist,
.PN XSetWMSizeHints
sets the size hints for the specified property
on the named window.
The property is stored with a type of WM_SIZE_HINTS and a format of 32.
To set a window's normal size hints, 
you can use the 
.PN XSetWMNormalHints
function.
.LP
.PN XSetWMSizeHints
can generate
.PN BadAlloc ,
.PN BadAtom , 
and 
.PN BadWindow
errors.
.sp
.LP
To read a window's WM_SIZE_HINTS property, use
.PN XGetWMSizeHints .
.FD 0
Status XGetWMSizeHints\^(\^\fIdisplay\fP, \fIw\fP, \fIhints_return\fP, \
\fIsupplied_return\fP, \fIproperty\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XSizeHints *\fIhints_return\fP\^;
.br
      long *\fIsupplied_return\fP\^;
.br
      Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIhints_return\fP 1i
Returns the
.PN XSizeHints
structure.
.IP \fIsupplied_return\fP 1i
Returns the hints that were supplied by the user.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The 
.PN XGetWMSizeHints
function returns the size hints stored in the specified property 
on the named window.
If the property is of type WM_SIZE_HINTS, of format 32, 
and is long enough to contain either an old (pre-ICCCM) 
or new size hints structure, 
.PN XGetWMSizeHints
sets the various fields of the 
.PN XSizeHints
structure, sets the supplied_return argument to the
list of fields that were supplied by the user 
(whether or not they contained defined values), 
and returns a non-zero status.
Otherwise, it returns a zero status.
To get a window's normal size hints, 
you can use the 
.PN XGetWMNormalHints 
function.
.LP
If 
.PN XGetWMSizeHints
returns successfully and a pre-ICCCM size hints property is read, 
the supplied_return argument will contain the following bits:
.LP
.Ds
(USPosition|USSize|PPosition|PSize|PMinSize|
 PMaxSize|PResizeInc|PAspect)
.De
.LP
If the property is large enough to contain the base size 
and window gravity fields as well, 
the supplied_return argument will also contain the following bits:
.LP
.Ds
PBaseSize|PWinGravity
.De
.LP
.PN XGetWMSizeHints
can generate
.PN BadAtom 
and 
.PN BadWindow
errors.
.NH 1
XAllocClassHint
.LP
To allocate an
.PN XClassHint
structure, use
.PN XAllocClassHint .
.FD 0
XClassHint *XAllocClassHint\^(\|)
.FN
.LP
The
.PN XAllocClassHint
function allocates and returns a pointer to a
.PN XClassHint
structure.
Note that the pointer fields in the
.PN XClassHint
structure are initially set to NULL.
If insufficient memory is available, 
.PN XAllocClassHint
returns NULL.
To free the memory allocated to this structure,
use
.PN XFree .
.NH 1
Setting and Reading the WM_PROTOCOLS Property
.LP
Xlib provides functions that you can use to set and read
the WM_PROTOCOLS property for a given window.
.LP
.sp
To set a window's WM_PROTOCOLS property, use
.PN XSetWMProtocols .
.FD 0
Status XSetWMProtocols\^(\^\fIdisplay\fP, \fIw\fP, \fIprotocols\fP, \
\fIcount\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Atom *\fIprotocols\fP\^;
.br
      int \fIcount\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIprotocols\fP 1i
Specifies the list of protocols.
.IP \fIcount\fP 1i
Specifies the number of protocols in the list.
.LP
The 
.PN XSetWMProtocols 
function replaces the WM_PROTOCOLS property on the specified window 
with the list of atoms specified by the protocols argument.
If the property does not already exist,
.PN XSetWMProtocols
sets the WM_PROTOCOLS property on the specified window
to the list of atoms specified by the protocols argument.
The property is stored with a type of ATOM and a format of 32.
If it cannot intern the WM_PROTOCOLS atom, 
.PN XSetWMProtocols
returns a zero status.
Otherwise, it returns a non-zero status.
.LP
.PN XSetWMProtocols
can generate
.PN BadAlloc
and
.PN BadWindow
errors.
.sp
.LP
To read a window's WM_PROTOCOLS property, use
.PN XGetWMProtocols .
.FD 0
Status XGetWMProtocols\^(\^\fIdisplay\fP, \fIw\fP, \fIprotocols_return\fP, \
\fIcount_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Atom **\fIprotocols_return\fP\^;
.br
      int *\fIcount_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIprotocols_return\fP 1i
Returns the list of protocols.
.IP \fIcount_return\fP 1i
Returns the number of protocols in the list
.LP
The 
.PN XGetWMProtocols 
function returns the list of atoms stored in the WM_PROTOCOLS property 
on the specified window.
These atoms describe window manager protocols in which the owner 
of this window is willing to participate.
If the property exists, is of type ATOM, is of format 32, 
and the atom WM_PROTOCOLS can be interned, 
.PN XGetWMProtocols
sets the protocols_return argument to a list of atoms, 
sets the count_return argument to the number of elements in list, 
and returns a a non-zero status.
Otherwise, it sets neither of the return arguments
and returns a zero status.
To release the list of atom, use
.PN XFree .
.LP
.PN XGetWMProtocols
can generate a
.PN BadWindow
error.
.NH 1
Setting and Reading the WM_COLORMAP_WINDOWS Property
.LP
Xlib provides functions that you can use to set and read
the WM_COLORMAP_WINDOWS property for a given window.
.sp
.LP
To set a window's WM_COLORMAP_WINDOWS property, use
.PN XSetWMColormapWindows .
.FD 0
Status XSetWMColormapWindows\^(\^\fIdisplay\fP, \fIw\fP, \
\fIcolormap_windows\fP, \fIcount\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Window *\fIcolormap_windows\fP\^;
.br
      int \fIcount\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIcolormap_windows\fP 1i
Specifies the list of windows.
.IP \fIcount\fP 1i
Specifies the number of windows in the list.
.LP
The 
.PN XSetWMColormapWindows 
function replaces the WM_COLORMAP_WINDOWS property on the specified
window with the list of windows specified by the colormap_windows argument.
It the property does not already exist,
.PN XSetWMColormapWindows
sets the WM_COLORMAP_WINDOWS property on the specified
window to the list of windows specified by the colormap_windows argument.
The property is stored with a type of WINDOW and a format of 32.
If it cannot intern the WM_COLORMAP_WINDOWS atom,
.PN XSetWMColormapWindows
returns a zero status.
Otherwise, it returns a non-zero status.
.LP
.PN XSetWMColormapWindows
can generate
.PN BadAlloc
and
.PN BadWindow
errors.
.sp
.LP
To read a window's WM_COLORMAP_WINDOWS property, use
.PN XGetWMColormapWindows .
.FD 0
Status XGetWMColormapWindows\^(\^\fIdisplay\fP, \fIw\fP, \
\fIcolormap_windows_return\fP, \fIcount_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Window **\fIcolormap_windows_return\fP\^;
.br
      int *\fIcount_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIcolormap_windows_return\fP 1i
Returns the list of windows.
.IP \fIcount_return\fP 1i
Returns the number of windows in the list.
.LP
The 
.PN XGetWMColormapWindows 
function returns the list of window identifiers stored 
in the WM_COLORMAP_WINDOWS property on the specified window.
These identifiers indicate the colormaps that the window manager
may need to install for this window.
If the property exists, is of type WINDOW, is of format 32, 
and the atom WM_COLORMAP_WINDOWS can be interned, 
.PN XGetWMColormapWindows
sets the windows_return argument to a list of window identifiers, 
sets the count_return argument to the number of elements in list, 
and returns a non-zero status.
Otherwise, it sets neither of the return arguments
and returns a zero status.
To release the list of window identifiers, use
.PN XFree . 
.LP
.PN XGetWMColormapWindows
can generate a
.PN BadWindow
error.
.NH 1
XAllocIconSize
.LP
To allocate an
.PN XIconSize
structure, use
.PN XAllocIconSize .
.FD 0
XIconSize *XAllocIconSize\^(\|)
.FN
.LP
The
.PN XAllocIconSize 
function allocates and returns a pointer to a
.PN XIconSize 
structure.
Note that all fields in the
.PN XIconSize
structure are initially set to zero.
If insufficient memory is available, 
.PN XAllocIconSize
returns NULL.
To free the memory allocated to this structure,
use
.PN XFree .
.NH 1
Using Window Manager Convenience Functions
.LP
Xlib provides two additional convenience functions that you can use to:
.IP \(bu 5
Set the standard window manager properties for a given window
.IP \(bu 5
Obtain window geometry information
.sp
.LP
To set a window's standard window manager properties, use
.PN XSetWMProperties .
The standard window manager properties for a given window are
WM_NAME, WM_ICON_NAME, WM_HINTS, WM_NORMAL_HINTS, WM_CLASS,
WM_COMMAND, and WM_CLIENT_MACHINE.
.FD 0
void XSetWMProperties\^(\^\fIdisplay\fP, \fIw\fP, \fIwindow_name\fP, \
\fIicon_name\fP, \fIargv\fP, \fIargc\fP, \fInormal_hints\fP, \fIwm_hints\fP, \
\fIclass_hints\fP\^)
.br
      Display *\fIdisplay\fP\^; 
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fIwindow_name\fP\^;
.br
      XTextProperty *\fIicon_name\fP\^;
.br
      char **\fIargv\fP\^;
.br
      int \fIargc\fP\^;
.br
      XSize_hints *\fInormal_hints\fP\^;
.br
      XWMHints *\fIwm_hints\fP\^;
.br
      XClassHint *\fIclass_hints\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIwindow_name\fP 1i
Specifies the window name,
which should be a null-terminated string.
.IP \fIicon_name\fP 1i
Specifies the icon name,
which should be a null-terminated string.
.IP \fIargv\fP 1i
Specifies the application's argument list.
.IP \fIargc\fP 1i
Specifies the number of arguments.
.IP \fInormal_hints\fP 1i
Specifies the size hints for the window in its normal state.
.IP \fIwm_hints\fP 1i
Specifies the
.PN XWMHints
structure to be used.
.IP \fIclass_hints\fP 1i
Specifies the
.PN XClassHint
structure to be used.
.LP
The 
.PN XSetWMProperties 
convenience function provides a single programming interface 
for setting those essential window properties that are used 
for communicating with other clients (particularly window and session
managers).
.LP
If the window_name argument is non-null, 
.PN XSetWMProperties
calls
.PN XSetWMName ,
which, in turn, sets the WM_NAME property (see section 9.1.4).
If the icon_name argument is non-null,
.PN XSetWMProperties
calls
.PN XSetWMIconName ,
which sets the WM_ICON_NAME property (see section 9.1.5).
If the argv argument is non-null, 
.PN XSetWMProperties
calls
.PN XSetCommand ,
which sets the WM_COMMAND property (see section 9.2.1).
Note that an argc of 0 is allowed to indicate a zero-length command.
Note also that the hostname of this machine is stored using
.PN XSetWMClientMachine 
(see section 9.2.2).
.LP
If the normal_hints argument is non-null, 
.PN XSetWMProperties
calls
.PN XSetWMNormalHints ,
which sets the WM_NORMAL_HINTS property (see section 9.1.7).
If the wm_hints argument is non-null, 
.PN XSetWMProperties
calls
.PN XSetWMHints ,
which sets the WM_HINTS property (see section 9.1.6).
.LP
If the class_hints argument is non-null, 
.PN XSetWMProperties
calls
.PN XSetClassHint ,
which sets the WM_CLASS property (see section 9.1.8).
If the res_name member in the
.PN XClassHint
structure is set to the null pointer and the RESOURCE_NAME environment 
variable is set, 
then value of the environment variable is substituted for res_name.
If the res_name member is null, 
the environment variable is not set, 
and argv and argv[0] are set, 
then the value of argv[0], stripped of
any directory prefixes, is substituted for res_name.
.LP
.PN XSetWMProperties
can generate
.PN BadAlloc
and
.PN BadWindow
errors.
.sp
.LP
To obtain a window's geometry information, use
.PN XWMGeometry .
.FD 0
int XWMGeometry\^(\^\fIdisplay\fP, \fIscreen\fP, \fIuser_geom\fP, \
\fIdef_geom\fP, \fIbwidth\fP, \fIhints\fP, \fIx_return\fP, \fIy_return\fP,
.br
                \fIwidth_return\fP, \fIheight_return\fP, \fIgravity_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      int \fIscreen\fP\^;
.br
      char *\fIuser_geom\fP\^;
.br
      char *\fIdef_geom\fP\^;
.br
      unsigned int \fIbwidth\fP\^;
.br
      XSizeHints *\fIhints\fP\^;
.br
      int *\fIx_return\fP, *\fIy_return\fP\^; 
.br
      int *\fIwidth_return\fP\^;
.br
      int *\fIheight_return\fP\^;
.br
      int *\fIgravity_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIscreen\fP 1i
Specifies the screen.
.IP \fIuser_geom\fP 1i
Specifies the user-specified geometry or NULL.
.IP \fIdef_geom\fP 1i
Specifies the application's default geometry or NULL.
.IP \fIbwidth\fP 1i
Specifies the border width.
.IP \fIhints\fP 1i
Specifies the size hints for the window in its normal state.
.IP \fIx_return\fP 1i
.br
.ns
.IP \fIy_return\fP 1i
Return the x and y offsets.
.IP \fIwidth_return\fP 1i
.br
.ns
.IP \fIheight_return\fP 1i
Return the width and height determined.
.IP \fIgravity_return\fP 1i
Returns the window gravity.
.LP
The 
.PN XWMGeometry 
function combines any geometry information (given in the format used by 
.PN XParseGeometry )
specified by the user and by the calling program with size hints 
(usually the ones to be stored in WM_NORMAL_HINTS) and returns the position, 
size, and gravity
.Pn ( NorthWestGravity , 
.PN NorthEastGravity , 
.PN SouthEastGravity
or
.PN SouthWestGravity ) 
that describe the window.
If the base size is not set in the 
.PN XSizeHints
structure, 
the minimum size is used if set.
Otherwise, a base size of 0 is assumed.
If no minimum size is set in the hints structure, 
the base size is used.
A mask (in the form returned by 
.PN XParseGeometry ) 
that describes which values came from the user specification 
and whether or not the position coordinates are relative
to the right and bottom edges is returned (which will have already been
accounted for in the x_return and y_return values).
.LP
Note that invalid geometry specifications can cause a width or height of 0 
to be returned.
The caller may pass the address of the hints win_gravity field 
as gravity_return to update the hints directly.
.NH 1
Client to Session Manager Communication
.LP
This section discusses how to:
.IP \(bu 5
Set and read the WM_COMMAND property
.IP \(bu 5
Set and read the WM_CLIENT_MACHINE_PROPERTY
.NH 1
XGetCommand
.LP
To read a window's WM_COMMAND property, use
.PN XGetCommand .
.FD 0
Status XGetCommand\^(\^\fIdisplay\fP, \fIw\fP, \fIargv_return\fP, \
\fIargc_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      char ***\fIargv_return\fP\^;
.br
      int *\fIargc_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIargv_return\fP 1i
Returns the application's argument list.
.IP \fIargc_return\fP 1i
Returns the number of arguments returned.
.LP
The 
.PN XGetCommand 
function reads the WM_COMMAND property from the specified window 
and returns a string list.
If the WM_COMMAND property exists, 
it is of type STRING and format 8.
If sufficient memory can be allocated to contain the string list, 
.PN XGetCommand
fills in the argv_return and argc_return arguments
and returns a non-zero status.
Otherwise, it returns a zero status.
To free the memory allocated to the string list, use
.PN XFreeStringList .
.NH 1
Setting and Reading the WM_CLIENT_MACHINE property
.LP
Xlib provides functions that you can use to set and read 
the WM_CLIENT_MACHINE property for a given window.
.sp
.LP
To set a window's WM_CLIENT_MACHINE property, use
.PN XSetWMClientMachine .
.FD 0
void XSetWMClientMachine\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop\fP 1i
Specifies the
.PN XTextProperty
structure to be used.
.LP
The
.PN XSetWMClientMachine
convenience function performs a
.PN XSetTextProperty
on the WM_CLIENT_MACHINE property.
Note that you also can set the client machine property by using
.PN XSetTextProperty
(see section 9.1.3).
.sp
.LP
To read a window's WM_CLIENT_MACHINE property, use
.PN XGetWMClientMachine .
.FD 0
Status XGetWMClientMachine\^(\^\fIdisplay\fP, \fIw\fP, \fItext_prop_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XTextProperty *\fItext_prop_return\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fItext_prop_return\fP 1i
Returns the
.PN XTextProperty
structure.
.LP
The
.PN XGetWMClientMachine
convenience function performs an 
.PN XGetTextProperty 
on the WM_CLIENT_MACHINE property.
Note that you also can read the client machine property by using
.PN XGetTextProperty
(see section 9.1.3).
.NH 1
Standard Colormaps
.LP
To allocate an
.PN XStandardColormap
structure, use
.PN XAllocStandardColormap .
.FD 0
XStandardColormap *XAllocStandardColormap\^(\|)
.FN
.LP
The
.PN XAllocStandardColormap
function allocates and returns a pointer to a
.PN XStandardColormap
structure.
Note that all fields in the
.PN XStandardColormap
structure are initially set to zero.
If insufficient memory is available, 
.PN XAllocStandardColormap
returns NULL.
To free the memory allocated to this structure,
use
.PN XFree .
.LP
Two new members have been added to the
.PN XStandardColormap 
structure:
.LP
/* Hints */
.TS
lw(.5i) lw(2i) lw(1i).
T{
#define
T}	T{
.PN ReleaseByFreeingColormap
T}	T{
( (XID) 1L)
T}
.TE
/* Values */
.Ds 0
.TA .5i 2.5i
.ta .5i 2.5i
typedef struct {
	Colormap colormap;
	unsigned long red_max;
	unsigned long red_mult;
	unsigned long green_max;
	unsigned long green_mult;
	unsigned long blue_max;
	unsigned long blue_mult;
	unsigned long base_pixel;
	VisualID visualid;	/* New */
	XID killid;	/* New */
} XStandardColormap;
.De
.LP
The visualid member gives the ID number of the visual from which the
colormap was created.
The killid member gives a resource ID that indicates whether
the cells held by this standard colormap are to be released 
by freeing the colormap ID or by calling the
.PN XKillClient
function on the indicated resource.
(Note that this method is necessary for allocating out of an existing colormap).
.LP
Xlib provides functions that you can use to set and obtain an
.PN XStandardColormap
structure.
.sp
.LP
To set an
.PN XStandardColormap
structure, use
.PN XSetRGBColormaps .
.FD 0
void XSetRGBColormaps\^(\^\fIdisplay\fP, \fIw\fP, \fIstd_colormap\fP, \
\fIcount\fP, \fIproperty\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XStandardColormap *\fIstd_colormap\fP\^;
.br
      int \fIcount\fP\^;
.br
      Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIstd_colormap\fP 1i
Specifies the
.PN XStandardColormap
structure to be used.
.IP \fIcount\fP 1i
Specifies the number of colormaps.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The 
.PN XSetRGBColormaps 
function replaces the RGB colormap definition in the specified property 
on the named window.
If the property does not already exist,
.PN XSetRGBColormaps
sets the RGB colormap definition in the specified property
on the named window.
The property is stored with a type of RGB_COLOR_MAP and a format of 32.
Note that it is the caller's responsibility to honor the ICCCM
restriction that only RGB_DEFAULT_MAP contain more than one definition.
.LP
.PN XSetRGBColormaps
can generate
.PN BadAlloc ,
.PN BadAtom ,
and
.PN BadWindow
errors.
.LP
To obtain the 
.PN XStandardColormap 
structure associated with the specified property, use
.PN XGetRGBColormaps .
.FD 0
Status XGetRGBColormaps\^(\^\fIdisplay\fP, \fIw\fP, \fIstd_colormap_return\fP, \
\fIcount_return\fP, \fIproperty\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      XStandardColormap **\fIstd_colormap_return\fP\^;
.br
      int *\fIcount_return\fP\^;
.br
      Atom \fIproperty\fP\^;
.FN
.IP \fIdisplay\fP 1i
Specifies the connection to the X server.
.IP \fIw\fP 1i
Specifies the window.
.IP \fIstd_colormap_return\fP 1i
Returns the
.PN XStandardColormap
structure.
.IP \fIcount_return\fP 1i
Returns the number of colormaps.
.IP \fIproperty\fP 1i
Specifies the property name.
.LP
The 
.PN XGetRGBColormaps
function returns the RGB colormap definitions stored 
in the specified property on the named window.
If the property exists, is of type RGB_COLOR_MAP, is of format 32, 
and is long enough to contain a colormap definition (if the visualid is not
present, 
.PN XGetRGBColormaps 
assumes the default visual for the screen on which the window is
located; if the killid is not present, 
.PN None , 
which indicates that the resources cannot be released, is assumed), 
.PN XGetRGBColormaps
allocates and fills in space for the returned colormaps, 
and returns a non-zero status.
Otherwise, 
none of the fields are set, and 
.PN XGetRGBColormaps
returns a zero status.
Note that it is the caller's responsibility to honor the ICCCM
restriction that only RGB_DEFAULT_MAP contain more than one definition.
.LP
.PN XGetRGBColormaps
can generate
.PN BadAtom
and
.PN BadWindow
errors.
