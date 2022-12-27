.\" Use tbl and -ms
.de Cp
.QP
Convention:
.I
..
.de Ce
.R
..
.de Pp
.QP
Problem:
.I
..
.de Pe
.R
..
.de Ip
.IP \(bu 3
..
.de Ls
.DS L
..
.de Le
.DE
..
.TL
Inter-Client Communication Conventions Manual
.sp
Version 1.0
.sp
MIT X Consortium Standard
.AU
David S. H. Rosenthal
.AI
Sun Microsystems, Inc.
.AB
.LP
It was an explicit design goal of the X Window System,  Version 11
to specify mechanism,  not policy.
As a result,  a client that converses with the server using the
protocol defined by the \fIX Window System Protocol,  Version 11\fP may
operate ``correctly'' in isolation,  but may not coexist properly
with others sharing the same server.  Conventions are proposed to
allow clients to cooperate in the areas of selections,  cut buffers,
window management, session management,  and resources.
.AE
.LP
.DS C
Copyright \(co 1989 by the Massachusetts Institute of Technology
Copyright \(co 1987, 1988, 1989 by Sun Microsystems, Inc.
.DE
.sp 2
.QP
``"Gentlemen."  said Lord Chancellor Thurlow to the deputation of
Nonconformists which waited on him in 1788 to ask for a repeal
of the Corporation and Test Acts,  "I'm against you,  by God!
I am for the Established Church,  damme!  Not that I have any
more regard for the Established Church than for any other
religion,  but because it is established.
And if you can get your damned religion established,  I'll be for that too!"''
.nf
           \fIYears of Endurance\fP,  Arthur Bryant
.fi
.sp 3
.LP
Permission to use, copy, modify, and distribute this documentation for any
purpose and without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies.
MIT and Sun Microsystems, Inc. make no representations about the suitability
for any purpose of the information in this document.  This documentation is
provided ``as is'' without express or implied warranty.
.sp 2
.LP
X Window System is a trademark of the Massachusetts Institute of Technology.
.br
.bp
.SH
Contents
.LP
.nf
.TA 0.5i 1.0i 1.5i 2.0i 2.5i 3.0i 3.5i
.ta 0.5i 1.0i 1.5i 2.0i 2.5i 3.0i 3.5i
1.		Introduction
	1.1.		Evolution of the Conventions
	1.2.		Atoms
		1.2.1.		What are Atoms?
		1.2.2.		Predefined Atoms
		1.2.3.		Naming Conventions
		1.2.4.		Semantics
		1.2.5.		Name Spaces

2.		Peer-to-Peer Communication via Selections
	2.1.		Acquiring Selection Ownership
	2.2.		Responsibilities of the Selection Owner
	2.3.		Giving Up Selection Ownership
		2.3.1.		Voluntarily
		2.3.2.		Forcibly
	2.4.		Requesting a Selection
	2.5.		Large Data Transfers
	2.6.		Usage of Selection Atoms
		2.6.1.		Selection Atoms
			2.6.1.1.		The PRIMARY Selection
			2.6.1.2.		The SECONDARY Selection
			2.6.1.3.		The CLIPBOARD Selection
		2.6.2.		Target Atoms
		2.6.3.		Selections with Side-Effects
			2.6.3.1.		DELETE
			2.6.3.2.		INSERT_SELECTION
			2.6.3.3.		INSERT_PROPERTY
	2.7.		Usage of Selection Properties
		2.7.1.		TEXT Properties
		2.7.2.		INCR Properties
		2.7.3.		DRAWABLE Properties
		2.7.4.		SPAN Properties

3.		Peer-to-Peer Communication via Cut-Buffers

4.		Client to Window Manager Communication
	4.1.		Client's Actions
		4.1.1.		Creating a Top-Level Window
		4.1.2.		Client Properties
			4.1.2.1.		WM_NAME
			4.1.2.2.		WM_ICON_NAME
			4.1.2.3.		WM_NORMAL_HINTS
			4.1.2.4.		WM_HINTS
			4.1.2.5.		WM_CLASS
			4.1.2.6.		WM_TRANSIENT_FOR
			4.1.2.7.		WM_PROTOCOLS
			4.1.2.8.		WM_COLORMAP_WINDOWS
		4.1.3.		Window Manager Properties
			4.1.3.1.		WM_STATE
			4.1.3.2.		WM_ICON_SIZE
		4.1.4.		Changing Window State
		4.1.5.		Configuring the Window
		4.1.6.		Changing Window Attributes
		4.1.7.		Input Focus
		4.1.8.		Colormaps
		4.1.9.		Icons
		4.1.10.		Popup Windows
		4.1.11.		Window groups
.if 0 \{
		4.1.11.		XXX - Overview of Client Actions
\}
	4.2.		Client Responses to Window Manager Actions
		4.2.1.		Reparenting
		4.2.2.		Redirection of Operations
		4.2.3.		Window Move
		4.2.4.		Window Resize
		4.2.5.		(De)Iconify
		4.2.6.		Colormap Change
		4.2.7.		Input Focus
		4.2.8.		ClientMessage Events
		4.2.9.		Redirecting Requests
	4.3		Summary of Window Manager Property Types

5.		Client to Session Manager Communication
	5.1.		Client Actions
		5.1.1.		Properties
			5.1.1.1.		WM_COMMAND
			5.1.1.2.		WM_CLIENT_MACHINE
			5.1.1.3.		WM_STATE
		5.1.2.		Termination
	5.2.		Client Responses to Session Manager Actions
		5.2.1.		Saving Client State
		5.2.2.		Window Deletion
	5.3		Summary of Session Manager Property Types

6.		Manipulation of Shared Resources
	6.1.		The Input Focus
	6.2.		The Pointer
	6.3.		Grabs
	6.4.		Colormaps
	6.5.		The Keyboard Mapping
	6.6.		The Modifier Mapping

7.		Resource Manager Conventions

8.		Conclusion
.fi
.bp
.NH
Introduction
.LP
It was an explicit design goal of X11 to specify mechanism,  not policy.
As a result,  a client that converses with the server using the
protocol defined by the \fIX Window System Protocol,  Version 11\fP may
operate ``correctly'' in isolation,  but may not coexist properly
with others sharing the same server.
.LP
Being a good citizen in the X11 world involves adhering to
conventions governing inter-client communications in a number of areas:
.Ip
The selection mechanism.
.Ip
The cut-buffers.
.Ip
The window manager.
.Ip
The session manager.
.Ip
The manipulation of shared resources.
.Ip
The resource database.
.LP
In the following sections we propose suitable conventions for each area,
in so far as it is possible to do so \fIwithout\fP enforcing a particular
user interface.
In order to permit clients written in different languages to communicate,
the conventions are expressed solely in terms of the protocol operations,
not in the (probably more familiar) Xlib interface.
The binding of these operations to the Xlib interface for C,
and to the equivalent interfaces for other languages,
is the subject of other documents.
.NH 2
Evolution of the Conventions
.LP
In the interests of timely acceptance,  this first edition of the
manual covers only a minimal set of required conventions.  It is expected
that as experience is gained,  these conventions will be added to,
and conventions governing other,  optional areas will be agreed.
The X Consortium is expected to develop mechanisms for doing this.
.LP
As far as possible,  these conventions are upwards-compatible with
those in the 25\s-2\uth\d\s0 February 1988 draft of this manual
distributed with the X11R2 release.
In some areas,  semantic problems were discovered with those conventions
and thus complete upwards compatibility could not be assured.
Areas of incompatibility are noted in the text,  and summarized
in Appendix A.
.LP
In the course of developing these conventions,  a number of minor
changes to the protocol have been identified as desirable.
They are identified in the text,  and summarized in Appendix B,
as input to a future protocol revision process.
If and
when a protocol revision incorporating them is undertaken,  this
document will need some revision.
Since it is difficult to ensure that clients and servers are upgraded
simultaneously,  clients using the revised conventions should examine
the minor protocol revision number,  and be prepared to use the older
conventions when communicating with an older server.
.LP
It is expected that the conventions will be revised
in such a way as to ensure that clients
using the conventions appropriate to protocol minor revision \fIn\fP
will inter-operate correctly with those using conventions appropriate
to protocol minor revision \fIn\fP+1 if the server supports both.
.NH 2
Atoms
.LP
Many of the conventions described below use Atoms.
The following sections amplify the description of Atoms in the protocol
specification,  to assist the reader.
.NH 3
What are Atoms?
.LP
At the conceptual level, Atoms are unique names.
Clients can use them to communicate information to each other.
They can be thought of as a bundle of octets,
like a string,
but without an encoding being specified.
The elements are not necessarily ASCII characters,  and no case folding 
happens\s-2\u1\d\s0.
.FS
1  The comment in the protocol specification for InternAtom that
ISO Latin-1 encoding should be used is in the nature of a convention;
the server treats the string as a byte sequence.
.FE
.LP
The protocol designers felt that passing these
sequences of bytes back and forth
across the wire would be too costly.
Further,  it is important that events as they appear ``on the wire'' have
a fixed size (in fact,  32 bytes),  and since some events contain Atoms a
fixed-size representation for them was needed.
.LP
To provide a fixed-size representation,  a protocol request (InternAtom)
was provided to register a byte sequence with the server,  which returns
a 32-bit value (with the top three bits zero) that maps to the byte
sequence.
The inverse operator is also available (GetAtomName).
.NH 3
Predefined Atoms
.LP
The protocol specifies a number of Atoms as being predefined:
.QP
``Predefined atoms are not strictly necessary, and may not be useful in all
environments, but will eliminate many InternAtom requests in most
applications.  Note that "predefined" is only in the sense of having numeric
values, not in the sense of having required semantics.''
.LP
They are an implementation trick to avoid the cost of Interning many atoms
that are expected to be used during the startup phase of all applications.
The results of the InternAtom requests (which require a handshake) can be
assumed \fIa priori\fP.
.LP
Language interfaces should probably cache the Atom-name mappings,  and get
them only when required.
The CLX interface, for instance, makes no distinction between predefined atoms
and other atoms; all atoms are viewed as symbols at the interface.  However, a
CLX implementation will typically keep a symbol/atom cache, and will
typically pre-fill this cache with the predefined atoms.
.NH 3
Naming Conventions
.LP
The built in atoms are composed of upper case ASCII characters with the
logical words separated by "_" (for example,  "WM_ICON_NAME").
The protocol specification
recommends that Atoms used for private vendor specific reasons should
begin with an underscore.
To prevent conflicts among organizations, additional
prefixes should be chosen (for example,  "_DEC_WM_DECORATION_GEOMETRY").
.LP
The names were chosen in this fashion to make it easy to use them in a
"natural" way within LISP. Keyword constructors allow the programmer to
specify the atoms as LISP atoms. If the atoms were not all upper case, then
special quoting conventions would have to be used.
.NH 3
Semantics
.LP
The core protocol imposes no semantics on atoms except as they are used in
FONTPROP structures.
See the definition of QueryFont in the protocol specification
for more information on FONTPROP semantics.
.NH 3
Name Spaces
.LP
The protocol defines six distinct spaces in which Atoms are interpreted,
as shown in Table 1.
Any particular Atom may or may not have some valid interpretation
with respect to each of these name spaces.
.KF
.TS
center, box;
c s s
c c c
l l l.
_
Table 1 \- Atom name spaces
_
Space	aka	Examples
_
Property name	name	(WM_HINTS, WM_NAME, RGB_BEST_MAP, etc.)
Property type	type	(WM_HINTS, CURSOR, RGB_COLOR_MAP, etc.)
Selection name	selection	(PRIMARY, SECONDARY, CLIPBOARD)
Selection target	target	(FILE_NAME, POSTSCRIPT, PIXMAP, etc.)
Font property		(QUAD_WIDTH, POINT_SIZE, etc.)
ClientMessage type		(WM_SAVE_YOURSELF, _DEC_SAVE_EDITS, etc.)
.TE
.KE
.bp
.NH
Peer-to-Peer Communication via Selections
.LP
The primary mechanism X11 defines for clients that want to
exchange information,
for example by cutting and pasting between windows,  is \fIselections\fP.
There can be an arbitrary number of selections,  each named by an atom,
and they are global to the server.
The choice of an atom to be used is discussed in Section 2.6.
Each selection is
owned by a client,  and is attached to a window.
.LP
Selections communicate between an \fIowner\fR and a \fIrequestor\fR.  The
owner has the data representing the value of its selection,  and the requestor
receives it.
A requestor wishing to obtain the value of a selection provides:
.Ip
the name of the selection
.Ip
the name of a property
.Ip
a window
.Ip
an atom representing the datatype required
.LP
If the selection is currently owned,  the owner receives an
event,  and is expected to:
.Ip
convert the contents of the selection to the requested datatype
.Ip
place this data in the named property on the named window
.Ip
send the requestor an event to let it know the property is available.
.LP
Clients are strongly encouraged to use this mechanism.  In particular,
displaying text in a permanent window without providing the ability to select it 
and convert it into a string is definitely anti-social.
.LP
Note that,  in the X11 environment,  \fIall\fP data transferred between
an owner and a requestor must normally go via the server.  An X11 client cannot
assume that another client can open the same files,  or even communicate
directly.  The other client may be talking to the server via a
completely different networking mechanism (for example,  one client might
be DECnet,  and the other TCP/IP).  Thus,  passing indirect references to data
such as file names,  hostnames & port numbers,  and so on is permitted only
if both clients specifically agree.
.NH 2
Acquiring Selection Ownership
.LP
A client wishing to acquire ownership of a particular selection
should call SetSelectionOwner:
.Ls
SetSelectionOwner
	selection:	ATOM
	owner:	WINDOW or None
	time:	TIMESTAMP or CurrentTime
.Le
.LP
The client should set "selection" to the Atom representing the selection,
set "owner" to some window that it created and set "time" to some time
between the current last-change time of the selection concerned
and the current server time.
This time value will
normally be obtained from the timestamp of the event triggering the
acquisition of the selection.  Clients should \fInot\fP set the time
value to CurrentTime,  since if they do so they have no way of finding
when they gained ownership of the selection.
Clients must use a window they created in order for requestors
to be able to route events to the owner of the selection\s-2\u*\d\s0.
.FS
*  There is at present no part of the protocol which requires requestors
to send events to the owner of a selection.
This restriction is imposed in order to prepare for possible future
extensions.
.FE
.Cp
Clients attempting to acquire a selection must set the time value of
the SetSelectionOwner request to the timestamp of the event triggering
the acquisition attempt,  not to CurrentTime.
A zero-length append to a property is a way to obtain a time-stamp for
this purpose;  the timestamp is in the corresponding PropertyNotify event.
.Ce
.LP
Note that if the time in the SetSelectionOwner request is in the future
relative to the server's current time,  or if it is in the past relative
to the last time the selection concerned changed hands,  the SetSelectionOwner
request appears to the client to succeed,  but ownership is \fInot\fP
actually transferred.
.LP
Since clients cannot name other clients directly,
the "owner" window is used to refer to the owning client
in the replies to GetSelectionOwner,  and in SelectionRequest and
SelectionClear events,
and possibly as a place to put properties describing the selection
in question.
To discover the owner of a particular selection,  a client should invoke:
.Ls
GetSelectionOwner
	selection:	ATOM
=>
	owner:	WINDOW or None
.Le
.Cp
Clients are normally expected to provide some visible confirmation
of selection ownership.
To make this feedback reliable,  a client
must perform a sequence like:
.Ls
SetSelectionOwner(selection=PRIMARY, owner=Window, time=timestamp)
owner = GetSelectionOwner(selection=PRIMARY)
if (owner != Window) Failure
.Le
.Ce
.LP
If the SetSelectionOwner request succeeds (not merely appears to succeed),
the client issuing it
is recorded by the server as being the owner of the selection for the
time period starting at "time".
.Pp
There is no way for anyone to find out the last-change time of
a selection.  At the next protocol revision,  GetSelectionOwner
should be changed to return the last-change time as well as the owner.
.Pe
.NH 2
Responsibilities of the Selection Owner
.LP
When a requestor wants the value of a selection,  the owner receives
a SelectionRequest event:
.Ls
SelectionRequest
	owner:	WINDOW
	selection:	ATOM
	target:	ATOM
	property:	ATOM or None
	requestor:	WINDOW
	time:	TIMESTAMP or CurrentTime
.Le
.LP
The owner and the selection fields will be the values specified
in the SetSelectionOwner request.
The owner should compare the time stamp with the period it has owned
the selection and,  if the time is outside,
refuse the SelectionRequest by sending the requestor window
a SelectionNotify event with the ``property'' set to None,  using
SendEvent with an empty event-mask.
.LP
More advanced selection owners are free to maintain a history
of the value of the selection,  and to respond to requests for the
value of the selection during periods they owned it
even though they do not own it now.
.LP
If the ``property'' field is None,  the requestor is an obsolete client.
Owners are encouraged to support these clients by using the ``target''
atom as the property name to be used for the reply.
.LP
Otherwise,  the owner should use the ``target'' field to decide the
form to convert the selection into,  and if the selection cannot be
converted into that form,  refuse the SelectionRequest similarly.
.LP
If the ``property'' field is not None,  the owner should place the
data resulting from converting the selection into the specified
property on the requestor window,  setting the property's type
to some appropriate value (which need not be the same as ``target'').
.Cp
All properties used to reply to SelectionRequest events must be placed
on the requestor window.
.Ce
.LP
In either case,  if the data comprising the selection cannot be
stored on the requestor window (for example,  because the server cannot
provide sufficient memory),  the owner must refuse the SelectionRequest
as above.  See the section on ``Large Data Transfers'' below.
.LP
If the property is successfully stored,
the owner should acknowledge the successful conversion
by sending the requestor window a SelectionNotify event,
using SendEvent with an empty mask:
.Ls
SelectionNotify
	requestor:	WINDOW
	selection:	ATOM
	target:	ATOM
	property:	ATOM or None
	time:	TIMESTAMP or CurrentTime
.Le
.LP
The ``selection'',  ``target'', ``time'' and ``property'' fields
of theSelectionNotify event should be
set to the values received in the SelectionRequest event
(Setting the ``property'' field to None indicates that the conversion
requested could not be made).
.Cp
The ``selection'',  ``target'',  ``time'' and ``property'' fields
in the SelectionNotify event
should be set to the values received in the SelectionRequest event.
.Ce
.LP
The data stored in the property must eventually be deleted.  A convention
is needed to assign the responsibility for doing so.
.Cp
Selection requestors are responsible for deleting properties whose
names they receive in SelectionNotify events (See section 2.4) or in
properties with type MULTIPLE.
.Ce
.LP
A selection owner will often need confirmation that the data comprising the
selection has actually been transferred (for example,  if the operation has
side-effects on the owner's internal data-structures,
these should not
take place until the requestor has indicated that it has successfully received
the data).
They should express interest in PropertyNotify events for the ``requestor''
window and wait until the property in the SelectionNotify event has
been deleted before assuming that the selection data has been transferred.
.LP
When some other client acquires a selection,  the previous owner
receives a SelectionClear event:
.Ls
SelectionClear
	owner:	WINDOW
	selection:	ATOM
	time:	TIMESTAMP
.Le
.LP
The ``timestamp'' field is the time at which the ownership changed hands,
and the ``owner'' field is the window the new owner specified in its
SetSelectionOwner request.
.LP
If an owner loses ownership while it has a transfer in progress,
that is to say before it receives notification that the requestor
has received all the data,  it must continue to service the on-going
transfer until it is complete.
.NH 2
Giving Up Selection Ownership
.LP
Clients may give up selection ownership voluntarily,  or they may
lose it forcibly as the result of some other client's actions.
.NH 3
Voluntarily
.LP
To relinquish ownership of a selection voluntarily,  a client should
execute a SetSelectionOwner request for that selection atom,
with ``owner'' specified as None,  and ``time'' the timestamp that was used to
acquire the selection.
.LP
Alternatively,  the client may destroy the window used as the "owner"
value of the SetSelectionOwner request,  or it may terminate.  In both
cases the ownership of the selection involved will revert to None.
.NH 3
Forcibly
.LP
If a client gives up ownership of a selection,
or if some other client executes a SetSelectionOwner for it and thus
reassigns it forcibly,
the previous owner will receive
a SelectionClear event:
.Ls
SelectionClear
	owner:	WINDOW
	selection:	ATOM
	time:	TIMESTAMP
.Le
.LP
The timestamp is the time the selection changed hands.
The owner argument is the
window that was specified by the current owner in its SetSelectionOwner
request.
.NH 2
Requesting a Selection
.LP
A client wishing to obtain the value of a selection in a particular
form issues a ConvertSelection request:
.Ls
ConvertSelection
	selection:	ATOM
	target:	ATOM
	property:	ATOM or None
	requestor:	WINDOW
	time:	TIMESTAMP or CurrentTime
.Le
.LP
The selection field specifies the particular selection involved,  and
the target specifies the required form of the information.
The choice of suitable atoms to use is discussed in Section 2.6.
The requestor field should be set to a window the requestor created;
the owner will place the reply property on it.
The time field should be set to the timestamp on the event triggering
the request for the selection value;  clients should \fInot\fP use
CurrentTime for this field.
.Cp
Clients should not use CurrentTime for the time field of ConvertSelection
requests.
They should use the timestamp of the event that caused the request to
be made.
.Ce
.LP
The property field should be set to the name of a property that the owner
can use to report the value of the selection.
Note that the requestor of a selection needs to know neither the client
owning the selection,  nor the window it is attached to.
.LP
Although the protocol allows the property field to be set
to None (in which case the owner is supposed chose a property name),
it is difficult for the owner to do so safely.
.Cp
Requestors should not use None for the property
field of ConvertSelection requests.
.Ce
.Cp
Owners receiving ConvertSelection requests with property field None
are talking to an obsolete client.  They should choose the target atom
as the property name to be used for the reply.
.Ce
.LP
The result of the ConvertSelection request is that a SelectionNotify event
will be received:
.Ls
SelectionNotify
	requestor:	WINDOW
	selection:	ATOM
	target:	ATOM
	property:	ATOM or None
	time:	TIMESTAMP or CurrentTime
.Le
.LP
The ``requestor'',  ``selection'',  ``time'' and ``target'' fields will be the same
as those on the ConvertSelection request.
.LP
If the ``property'' field is None,  the conversion has been refused.
This can mean that there is no owner for the selection,  that the owner
does not support the conversion implied by ``target'',  or that the
server did not have sufficient space to accommodate the data.
.LP
If the ``property'' field is not None,  then that property will exist on the
``requestor'' window.  The value of the selection can be retrieved from this
property by using the GetProperty request:
.Ls
GetProperty
	window:	WINDOW
	property:	ATOM
	type:	ATOM or AnyPropertyType
	long-offset:	CARD32
	long-length:	CARD32
	delete:	BOOL
=>
	type:	ATOM or None
	format:	{0, 8, 16, 32}
	bytes-after:	CARD32
	value:	LISTofINT8 or LISTofINT16 or LISTofINT32
.Le
.LP
When using GetProperty to retrieve the value of a selection,  the ``property''
field should be set to the corresponding value in the SelectionNotify event.
The ``type'' field should be set to AnyPropertyType,  because the requestor
has no way of knowing beforehand what type the selection owner will use.
Several GetProperty requests may be needed to retrieve all the data in the
selection;  each should set the ``long-offset'' field to the amount
of data received so far,  and the
``size'' field to some reasonable buffer size (see the section on ``Large
Data Transfers'').
If the returned value of
``bytes-after'' is zero the whole property has been transferred.
.LP
Once all the data in the selection has been retrieved,  which may require
getting the values of several properties (see the section on ``Selection
Properties''),  the property in the SelectionNotify
should be deleted by invoking GetProperty
with the ``delete'' field set True.
As discussed above,  the owner has no way of knowing when the data has been
transferred to the requestor unless the property is removed.
.Cp
The requestor must delete the property named in the SelectionNotify
once all the data has been retrieved.
They should invoke either DeleteProperty,  or
GetWindowProperty(delete==TRUE) after they have successfully retrieved
all data comprising the selection.
See the next section.
.Ce
.NH 2
Large Data Transfers
.LP
Selections can get large,  and this poses two problems:
.Ip
Transferring large amounts of data to the server is expensive.
.Ip
All servers will have limits on the amount of data that can be stored
in properties.  Exceeding this limit will result in an Alloc error
on the ChangeProperty request that the selection owner uses to store
the data.
.LP
The problem of limited server resources is addressed by the following
conventions:
.Cp
Selection owners should transfer the data describing a selection large
compared with maximum-request-size in the connection handshake
using the INCR property mechanism (see below).
.Ce
.Cp
Any client using SetSelectionOwner to acquire selection ownership should
arrange to process Alloc errors in property change requests.
For clients using Xlib,  this involves using XSetErrorHandler() to
override the default handler.
.Ce
.Cp
A selection owner must confirm that no Alloc error occurred while
storing the properties for a selection before replying with a
confirming SelectionNotify event.
.Ce
.Cp
When storing large amounts (relative to max-request-size) of data,
clients should use a sequence of ChangeProperty(mode==Append)
requests for reasonable quantities of data.
This is to avoid locking-up servers,  and to limit the waste of
data transfer caused by an Alloc error.
.Ce
.Cp
If an Alloc error occurs during storing the selection data,
all properties stored for this selection should be deleted,
and the ConvertSelection request refused by replying with a
SelectionNotify event with ``property'' set to None.
.Ce
.Cp
In order to avoid locking-up servers for inordinate lengths of time,
requestors retrieving large quantities of data from a property
should perform a series of GetProperty requests,  each asking for a
reasonable amount of data.
.Ce
.Pp
Single-threaded servers should be changed to avoid locking-up during large
data transfers.
.Pe
.NH 2
Usage of Selection Atoms
.LP
It is important to observe that defining a new atom consumes resources
in the server, and they are not released until the server re-initializes.
Thus,  it must be a goal to reduce the need for newly minted atoms.
.NH 3
Selection Atoms
.LP
There can be an arbitrary number of selections,  each named by an atom.
To conform with the inter-client conventions,  however,  clients need
deal with only these three selections:
.Ip
PRIMARY
.Ip
SECONDARY
.Ip
CLIPBOARD
.LP
Other selections may be used freely for private communication among
related groups of clients.
.Pp
How does a client find out which selection atoms are valid?
.Pe
.NH 4
The PRIMARY Selection
.LP
The selection named by the atom PRIMARY is used for all commands
which take only a single argument.
It is the principal means of communication between clients
which use the selection mechanism.
.NH 4
The SECONDARY Selection
.LP
The selection named by the atom SECONDARY is used:
.Ip
As the second argument to commands taking two arguments,  for example
``exchange primary and secondary selections''.
.Ip
As a means of obtaining data when there is a primary selection,
and the user does not wish to disturb it.
.NH 4
The CLIPBOARD Selection
.LP
The selection named by the atom CLIPBOARD is used to hold data
being transferred between clients,  normally being ``cut'' or
``copied'',  and then ``pasted''.
Whenever a client wants to transfer data to the clipboard,  it should:
.Ip
Assert ownership of the CLIPBOARD.
.Ip
If it succeeds in acquiring ownership,
it should be prepared to respond to a request for the contents of the CLIPBOARD
in the normal way,  retaining the data in order to be able to
return it.
The request may be generated by the clipboard client described below.
.Ip
If it fails to acquire ownership,
a cutting client should not actually perform the cut,
nor provide feedback suggesting that it has actually transferred
data to the clipboard.
.LP
This process should be repeated whenever the data to be transferred
would change.
.LP
Clients wishing to ``paste'' data from the clipboard should request the contents of
the CLIPBOARD selection in the usual way.
.LP
Except while a client is actually deleting data,
the owner of the CLIPBOARD selection may be a single, special client
implemented for the purpose.
It should:
.Ip
Assert ownership of the CLIPBOARD selection,
and re-assert it any time the clipboard data changes.
.Ip
If it loses the selection (which will be because someone has some
new data for the clipboard):
.RS
.Ip
Obtain the contents of the selection from the new owner,  using the timestamp
in the SelectionClear event.
.Ip
Attempt to re-assert ownership of the CLIPBOARD selection,  using the same timestamp.
.Ip
If the attempt fails,  re-start the process using a newly-acquired
timestamp.
This timestamp should be obtained by asking the current owner of the
CLIPBOARD selection to convert it to a TIMESTAMP.
If this conversion is refused,  or if the same timestamp is received
twice,  the clipboard client should acquire a fresh timestamp in the
normal way,  for example by a zero-length append to a property.
.RE
.Ip
Respond to requests for the CLIPBOARD contents in the normal way.
.LP
A special CLIPBOARD client is not necessary.  The protocol used by the
``cutting'' client,  and the ``pasting'' client,  is the
same whether the CLIPBOARD client is running or not.
The reasons for running the special client include:
.Ip
Stability \- if the ``cutting'' client were to crash or terminate the
clipboard value would still be available.
.Ip
Feedback \- the clipboard client can display the contents of the clipboard.
.Ip
Simplicity \- a client deleting data does not have to retain it
for so long,  reducing the chance of race conditions causing problems.
.LP
The reasons not to run the clipboard client include:
.Ip
Performance \- data is only transferred if it is actually required (when
some client actually wants the data).
.Ip
Flexibility \- the clipboard data may be available as more than one target.
.NH 3
Target Atoms
.LP
The atom that a requestor supplies as the ``target'' of a ConvertSelection
request determines the form of the data supplied.
The set of such atoms is extensible,  but a generally accepted base
set of target atoms is needed.
As a starting point for this,  Table 2 contains those that have been suggested
so far.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 2 \- Initial Set of Target Atoms and their Meanings
_
Atom	Type (see Table 3)	Meaning
_
TARGETS	ATOM	list of valid target atoms
MULTIPLE	ATOM_PAIR	look in the ConvertSelection property
TIMESTAMP	INTEGER	Timestamp used to acquire selection
STRING	STRING	ISO Latin 1 (+TAB+NEWLINE) text
TEXT	TEXT	Text in owner's encoding
LIST_LENGTH	INTEGER	Number of disjoint parts of selection
PIXMAP	DRAWABLE	Pixmap ID
DRAWABLE	DRAWABLE	Drawable ID
BITMAP	BITMAP	Bitmap ID
FOREGROUND	PIXEL	Pixel value
BACKGROUND	PIXEL	Pixel value
COLORMAP	COLORMAP	Colormap ID
ODIF	TEXT	ISO Office Document Interchange Format
OWNER_OS	TEXT	operating system of owner
FILE_NAME	TEXT	full path name of a file
HOST_NAME	TEXT	see WM_CLIENT_MACHINE
CHARACTER_POSITION	SPAN	start and end of selection in bytes
LINE_NUMBER	SPAN	start and end line numbers
COLUMN_NUMBER	SPAN
LENGTH	INTEGER	number of bytes in selection
USER	TEXT	name of user running owner
PROCEDURE	TEXT	name of selected procedure
MODULE	TEXT	name of selected module
PROCESS	INTEGER,	process ID of owner
	TEXT
TASK	INTEGER,	task ID of owner
	TEXT
CLASS	TEXT	class of owner - see WM_CLASS
NAME	TEXT	name of owner - see WM_NAME
CLIENT_WINDOW	WINDOW	top-level window of owner
DELETE	NULL	True if owner deleted selection
INSERT_SELECTION	NULL	Insert specified selection
INSERT_PROPERTY	NULL	Insert specified property

		This table will grow
.TE
.KE
.LP
Selection owners are required to support the following targets:
.Ip
TARGETS.
The owner should return a list of Atoms representing
the targets for which an attempt to convert the current selection
will succeed (barring unforseeable problems such as Alloc errors).
This list should include all the required Atoms.
.Ip
MULTIPLE.
The MULTIPLE target atom is valid only when a property is specified
on the ConvertSelection request.
If the property field in the SelectionRequest event is None and the
target is MULTIPLE,  it should be refused.
.IP
When a selection owner receives a SelectionRequest(target=MULTIPLE)
request,  the contents of the property named in the request will be a
list of atom pairs,  the first atom naming a target,  and the second
naming a property (None is not valid here).
The effect should be as if the owner had received a sequence of
SelectionRequest events,  one for each atom pair,  except that;
.RS
.Ip
the owner should reply with a SelectionNotify only when all the requested
conversions have been performed.
.Ip
the owner should replace in the MULTIPLE property
any property atoms for targets it failed to
convert with None.
.RE
.Cp
The entries in a MULTIPLE property must be processed in the order
they appear in the property.
See Section 2.6.3.
.Ce
.Ip
TIMESTAMP.
To avoid some race conditions,  it is important that requestors be able
to discover the timestamp the owner used to acquire ownership.
Until and unless the protocol is changed so that GetSelectionOwner
returns the timestamp used to acquire ownership,
selection owners must support conversion to TIMESTAMP,
returning the timestamp they used to obtain the selection.
.Pp
The protocol should be changed to return in response to a GetSelectionOwner
the timestamp used to acquire the selection.
.Pe
.LP
All other targets are optional.
.NH 3
Selection with Side-Effects
.LP
Some targets,  DELETE is an example,  have side-effects.  To render them
unambiguous,  the entries in a MULTIPLE property must be processed in the
order they appear in the property.
.LP
In general,  targets with side-effects will return no information (i.e.
a zero-length property of type NULL.
In all cases,  the requested side-effect must be performed before
the conversion is accepted.
If the requested side-effect cannot be performed,  the corresponding
conversion request must be refused.
.Cp
Targets with side-effects should return no information (i.e.
a zero-length property of type NULL.
.Ce
.Cp
The side-effect of a target must be performed before the conversion is accepted.
.Ce
.Cp
If the side-effect of a target cannot be performed,
the corresponding conversion request must be refused.
.Ce
.Pp
The need to delay responding to the ConvertSelection request until
a further conversion has succeeded poses problems for the Intrinsics
interface that need to be addressed.
.Pe
.LP
These side-effect targets are used to implement operations such as
``exchange PRIMARY and SECONDARY selections''.
.NH 4
DELETE
.LP
When the owner of a selection receives a request to convert it to DELETE,
it should delete the corresponding selection
(whatever doing so means for its internal data structures),
and return a zero-length property of type NULL if the deletion was successful.
.NH 4
INSERT_SELECTION
.LP
When the owner of a selection receives a request to convert it to
INSERT_SELECTION,  the property named will be of type ATOM_PAIR.
The first atom will name a selection,  and the second will name a target.
The owner should use the selection mechanism to convert the named selection
into the named target,  and insert it at the location of the selection
for which it got the INSERT_SELECTION request
(whatever doing so means for its internal data structures).
.NH 4
INSERT_PROPERTY
.LP
When the owner of a selection receives a request to convert it to
INSERT_PROPERTY,  it should insert the property named in the request
at the location of the selection
for which it got the INSERT_SELECTION request
(whatever doing so means for its internal data structures).
.NH 2
Usage of Selection Properties
.LP
The names of the properties used in selection data transfer are chosen by
the requestor.  The use of None property fields in ConvertSelection requests,
which request the selection owner to choose a name,  is not permitted by
these conventions.
.LP
The type of the property involved is always chosen by the selection owner,
and can involve some types with special semantics assigned by convention.
These special types are reviewed in the following sections.
.LP
In all cases,  a request for conversion to a target should return a property
of one of the types listed in Table 2 for that property,
or a property of type INCR and then a property of one of the listed
types.
.LP
The selection owner will return a list of zero or more items
of the type indicated by the property type.
In general,  the number of items in the list will correspond to
the number of disjoint parts of the selection.
Some targets,  side-effect targets are examples,  will be of length 0
irrespective of the number of disjoint selection parts.
In the case of fixed-size items,  the requestor may determine
the number of items by the property size;
for variable length items such as text,  the separators are listed in Table 3.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 3 \- Property Types,  Formats & Separators
_
Type Atom	Format	Separator
_
STRING	8	Null
ATOM	32	Fixed-size
ATOM_PAIR	32	Fixed-size
BITMAP	32	Fixed-size
PIXMAP	32	Fixed-size
DRAWABLE	32	Fixed-size
SPAN	32	Fixed-size
INTEGER	32	Fixed-size
WINDOW	32	Fixed-size
INCR	32	Fixed-size

		This table will grow
.TE
.KE
.NH 3
TEXT Properties
.LP
In general, the encoding for the characters in a text string property is
specified by its type. It is highly desirable for there to be a simple,
invertible mapping between string property types and any character set names
embedded within font names in any font naming standard adopted by the
Consortium.
.LP
The atom TEXT is a polymorphic target. Requesting conversion into TEXT will
convert into whatever encoding is convenient for the owner.
The encoding chosen will be indicated by the type of the property returned.
TEXT is not defined as a type; it will never be the returned type from a
selection conversion request.
.LP
If the requestor wants the owner to return the contents of the selection
in a specific encoding, it should request conversion into the name of
that encoding.
.LP
In Table 2,  the word TEXT is used to indicate one of the registered
encoding names.
The type would not actually be TEXT,  it would be STRING or some other
ATOM naming the encoding chosen by the owner.
.LP
STRING as a type or a target specifies the ISO Latin-1 character set plus the
"control" characters TAB (octal 11)  and NEWLINE (octal 12). The spacing
interpretation of TAB is context dependent. Other ASCII control characters are
explicitly not included in STRING at the present time.
.LP
Type STRING properties will consist of a list of
elements separated by NULL
characters; other encodings will need to specify an appropriate list
format.
.NH 3
INCR Properties
.LP
Requestors may receive a property of type INCR\s-2\u*\d\s0
.FS
*  These properties were called INCREMENTAL in an earlier draft.
The protocol for using them has changed,  and so the name has changed
to avoid confusion.
.FE
in response to any target that results in selection data.  This indicates that
the owner will send the actual data incrementally.
The contents of the INCR property will be an integer,  representing a
lower bound on the number of bytes of data in the selection.
The requestor and the selection owner transfer the data comprising
the selection in the following manner.
.LP
The selection requestor starts the transfer process by deleting
the (type==INCR) property forming the reply to the selection.
.LP
The selection owner then:
.Ip
Appends the data in suitable-size chunks to the
same property on the same window as the selection reply,
with a type corresponding to the actual type of the converted selection.
The size should be less than the maximum-request-size in the connection
handshake.
.Ip
Between each append,  waits for a PropertyNotify
(state==Deleted) event showing that the requestor has read the data.
The reason for doing this is to limit the consumption of space in the
server.
.Ip
When the entire data has been transferred to the server,
waits until a PropertyNotify (state==Deleted)
showing that the data has been read by the requestor,
and then writes zero-length data to the property.
.LP
The selection requestor:
.Ip
waits for the SelectionNotify event.
.Ip
loops:
.RS
.Ip
retrieving data using GetProperty with ``delete'' True
.Ip
waiting for a PropertyNotify with state==NewValue
.RE
.Ip
until a zero-length property is obtained
.Ip
deletes the zero-length property.
.LP
The type of the converted selection is the type of the first partial
property.  The remaining partial properties must have the same type.
.NH 3
DRAWABLE Properties
.LP
Requestors may receive properties of type PIXMAP,  BITMAP,  DRAWABLE or WINDOW,
containing an appropriate ID.
Some information about these drawables is available from the server via
the GetGeometry request,  but the following items are not:
.Ip
Foreground pixel.
.Ip
Background pixel.
.Ip
Colormap ID.
.LP
In general,  requestors converting into targets whose returned type in Table 2
is one of the DRAWABLE types should expect to convert also into the following
targets (using the MULTIPLE mechanism):
.Ip
FOREGROUND returns a PIXEL value.
.Ip
BACKGROUND returns a PIXEL value.
.Ip
COLORMAP returns a colormap ID.
.NH 3
SPAN Properties
.LP
Properties with type SPAN contain a list of cardinal-pairs,
with the length of the cardinals determined
by the format.  The first specifies the starting position,  and the
second the ending position plus one.
The base is zero.
If they are the same,  the span
is zero-length,  and before the specified position.  The units are
implied by the target atom,  such as LINE_NUMBER or CHARACTER_POSITION.
.bp
.NH
Peer-to-Peer Communication via Cut-Buffers
.LP
Communication via cut-buffers is much simpler,
but much less powerful than via the selection
mechanism.
The selection mechanism is active,  in that it provides a link between
the owner and requestor clients.
The cut-buffer mechanism is passive;
an owner places data in a cut-buffer,  from where a requestor retrieves
it at some later time.
.LP
The cut-buffers consist of eight properties on the root of screen 0,
named by the predefined atoms CUT_BUFFER0 to CUT_BUFFER7.
These properties must (at present) have type STRING and format 8.
A client using the cut-buffer mechanism must initially ensure that
all eight exist,  using ChangeProperty to append zero-length data to each.
.LP
A client storing data in the cut-buffers (an owner) must first rotate the
ring of buffers by +1,  using RotateProperties to rename CUT_BUFFER0 to
CUT_BUFFER1 to ..... to CUT_BUFFER7 to CUT_BUFFER0.
It must then store the data into CUT_BUFFER0, using ChangeProperty in mode
Replace.
.LP
A client obtaining data from the cut-buffers should use GetProperty to
retrieve the contents of CUT_BUFFER0.
.LP
A client may,  in response to a specific user request,  rotate the cut-buffers
by -1,  using RotateProperties to rename CUT_BUFFER7 to CUT_BUFFER6
to ..... to CUT_BUFFER0 to CUT_BUFFER7.
.LP
Data should be stored to the cut-buffers
and the ring rotated only when requested by
explicit user action.
Users depend on their mental model of cut-buffer operation,
and need to be able to identify operations that transfer data to and fro.
.bp
.NH
Client to Window Manager Communication
.LP
To permit window managers to perform their role of mediating the competing
demands for resources such as screen space,  the clients being managed must
adhere to certain conventions,  and must expect the window managers to
do likewise.
These conventions are covered here from the client's point of view,
and again from the window manager's point of view in the
.I
Window and Session Manager Conventions Manual.\s-2\u*\d\s0
.R
.FS
*  The Window and Session Manager Conventions Manual will be prepared
after this manual is finalised.
.FE
.LP
In general,  these conventions are somewhat complex,  and will
undoubtedly change as new window management paradigms
are developed.
There is thus a strong bias towards defining only those conventions
that are essential,  and which apply generally to all window management
paradigms.
Clients designed to run with a particular window manager can easily
define private protocols to add to these conventions,
but must be aware that their users may decide to run some other
window manager no matter how much the designers of the private protocol
are convinced that they have seen the ``one true light'' of user interfaces.
.LP
It is a principle of these conventions that a general client should
neither know nor care which window manager is running,  or indeed if
one is running at all.
The conventions do not support all client functions
without a window manager running \-
for example,  the concept of Iconic is not directly supported by
clients.
If no window manager is running,  the concept of Iconic does not
apply.
A goal of the conventions is to make it possible to kill and
restart window managers without loss of functionality.
.LP
Each window manager will implement a particular window management
policy;  the choice of an appropriate window management policy
for the user's circumstances is not one for an individual client to
make but will be made by the user or the user's system administrator.
This does not exclude the possibility of writing clients that
use a private protocol to restrict themselves to operating only
under a specific window manager,  it merely ensures that no 
claim of general utility is made for such programs.
.LP
For example,  the claim is often made ``the client I'm writing is
important,  and it needs to be on top''.
Perhaps it is
important when it is being run in earnest,  and it should then be run
under the control of a window manager that recognizes ``important''
windows through some private protocol and ensures that they are on top.
However,  imagine for example that the ``important'' client is being
debugged.  Then,  ensuring that it is always on top is no
longer the appropriate window management policy,  and it should be
run under a window manager that allows other windows (e.g. the
debugger) to appear on top.
.NH 2
Client's Actions
.LP
In general,  the object of the X11 design is that clients should as
far as possible do exactly what they would do in the absence of a
window manager,  except for:
.Ip
Hinting to the window manager about the resources they would like
to obtain.
.Ip
Cooperating with the window manager by accepting the resources they
are allocated,  even if they are not  those requested.
.Ip
Being prepared for resource allocations to change at any time.
.NH 3
Creating a Top-Level Window
.LP
A client would normally expect to create its top-level windows
as children of one or more of the root windows,  using some
boilerplate like:
.Ls
win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			      xsh.x, xsh.y, xsh.width, xsh.height,
			      bw, bd, bg);
.Le
or,  if a particular one of the roots was required,  like:
.Ls
win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
			      xsh.x, xsh.y, xsh.width, xsh.height,
			      bw, bd, bg);
.Le
.LP
Ideally,  it should be possible to override the choice of a root
window and allow clients (including window managers) to treat a
non-root window as a pseudo-root.
This would allow,  for example,  testing of window managers and the
use of application specific window managers to control the sub-windows
owned by the members of a related suite of clients.
Doing so properly requires an extension,
the design of which is under study\s-2\u\(dg\d\s0.
.FS
\(dg  The mechanism proposed in the earlier drafts turned out
to be inadequate to support all the proposed uses of the pseudo-root
facility.
.FE
.LP
From the client's point of view,  the window manager will regard its
top-level window as being in one of three states:
.Ip
Normal
.Ip
Iconic
.Ip
Withdrawn
.LP
Newly created windows start in the Withdrawn state.
Transitions between states happen when the top-level window is mapped
and unmapped,  and when the window manager receives certain messages.
For details,  see Sections 4.1.2.4 and 4.1.4.
.NH 3
Client Properties
.LP
Once the client has one or more top-level windows,  it should place
properties on those windows to inform the window manager of its desired
behaviour.
Window managers will assume values they find convenient for any of these
properties that are not supplied;
clients that depend on particular values must explicitly supply them.
Properties written by the client will not be changed by the window manager.
.LP
The window manager will examine the contents of these
properties when the window makes the transition from Withdrawn state,
and will monitor some for changes while the window is in Iconic
or or Normal state.
When the client changes one of these properties,  it must use
Replace mode to overwrite the entire property with new data;
the window manager will retain no memory of the old value of
the property.
All fields of the property must be set to suitable values
in a single Replace-mode ChangeProperty request.
This is to ensure that the full contents of the property will be
available to a new window manager if the existing one crashes,
or is shut down and restarted,
or if the session needs to be shut down and restarted by the
session manager.
.Cp
Clients writing or rewriting window manager properties must
ensure that the entire content of the property remains valid
at all times.
.Ce
.LP
If these properties are longer than expected,  clients
should ignore the remainder of the property.
Extending these properties is reserved to the X Consortium,
private extensions to them are forbidden.
Private additional communication between clients and window
managers should take place using separate properties.
The next sections describe each of the properties the clients
needs to set in turn.
They are summarized in Table 13 in Section 4.3.
.NH 4
WM_NAME
.LP
The WM_NAME property is an un-interpreted
string that the client wishes the window manager to display
in association with the window (for example,  in a window headline bar).
.LP
The encoding used for this string (and all other un-interpreted string
properties) is implied by the type of the property.
The ATOMS to be used for this purpose are described in Section 2.7.1.
.LP
Window managers are expected to make an effort to display this
information;  simply ignoring WM_NAME is not acceptable behaviour.
Clients can assume that at least the first part of this string
is visible to the user,  and that if the information is not
visible to the user it is because the user has taken an explicit
decision to make it invisible.
.LP
On the other hand,  there is no guarantee that the user can see
the WM_NAME string even if the window manager supports window
headlines.  The user may have placed the headline off-screen,
or have covered it by other windows.  WM_NAME should not be
used for application-critical information,  nor to announce
asynchronous changes of application state that require timely user response.
The expected uses are:
.Ip
to permit the user to identify one of a
number of instances of the same client
.Ip
To provide the user with non-critical state information.
.LP
Note that even window managers that support headline bars
will place some limit on the length of string that can be visible;
brevity here will pay dividends.
.Pp
A change is needed to XFetchName and similar Xlib routines
to allow for multiple encodings.
.Pe
.NH 4
WM_ICON_NAME
.LP
The WM_ICON_NAME property is an un-interpreted
string that the client wishes displayed
in association with the window when it is iconified (for example,
in an icon label).
In other respects,  including the type,
it is similar to WM_NAME.
Fewer characters will normally be visible in WM_ICON_NAME than WM_NAME,
for obvious geometric reasons.
.LP
Clients should not attempt to display this string in their icon pixmaps
or windows;  they should rely on the window manager to do so.
.NH 4
WM_NORMAL_HINTS
.LP
The type of the WM_NORMAL_HINTS property is WM_SIZE_HINTS.
Its contents are shown in Table 4.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 4 \- WM_SIZE_HINTS type property contents
_
Field	Type	Comments
_
flags	CARD32	see Table 5 below
pad	4*CARD32	for backwards compatibility
min_width	INT32	if missing, assume base_width
min_height	INT32	if missing, assume base_height
max_width	INT32
max_height	INT32
width_inc	INT32
height_inc	INT32
min_aspect	(INT32,INT32)
max_aspect	(INT32,INT32)
base_width	INT32	if missing, assume min-width
base_height	INT32	if missing, assume min_height
win_gravity	INT32	if missing, assume NorthWest
.TE
.KE
.KF
.TS
center, box;
c s s
c c c
l n l.
Table 5 \- WM_SIZE_HINTS.flags bit definitions
_
Name	Value	Field
_
USPosition	1	user specified x, y
USSize	2	user specified width, height
PPosition	4	program specified position
PSize	8	program specified size
PMinSize	16	program specified minimum size
PMaxSize	32	program specified maximum size
PResizeInc	64	program specified resize increments
PAspect	128	program specified min and max aspect ratios
PBaseSize	256	program specified base size
PWinGravity	512	program specified window gravity
.TE
.KE
.LP
To indicate that the size and
position of the window (when mapped from Withdrawn state)
was specified by the user, the client should set the
.B USPosition
and
.B USSize
flags. 
To indicate that it was specified by the client without any user
involvement, the client should set 
.B PPosition
and 
.B PSize .
USPosition and USSize allow a window manager to know that the
user specifically asked where
the window should be placed or how the window should be sized and that
further interaction is superfluous.
.LP
The size specifiers refer to the width and height of the client's
window excluding borders.
The window manager will interpret the position of the window,
and its border width,
to position the point of the outer rectangle of the overall window
specified by the win_gravity in the size hints.
The outer rectangle of the window includes any borders or decorations
supplied by the window manager.
In other words,
if the window manager decides to place the window where the client asked,
the position on the parent window's border
named by the win_gravity will be placed where
the client window would have been placed in the absence of a window
manager.
.LP
The defined values for win_gravity are those specified for WINGRAVITY
in the core X protocol, with the exception of Unmap and Static:
NorthWest (1), North (2), NorthEast (3), West (4), Center (5),
East (6), SouthWest (7), South (8), and SouthEast (9).
.LP
The min_width and min_height elements specify the
minimum size that the window can be for the client to be useful.
The max_width and max_height elements specify the maximum size.
The base_width and base_height elements in conjunction with width_inc
and height_inc define an arithmetic progression of preferred window
widths and heights:
.Ls    
width = base_width + ( i * width_inc )
height = base_height + ( j * height_inc )
.Le
for non-negative integers i and j.  Window managers are encouraged to
use i and j instead of width and height in reporting window sizes to
users.  If a base size is not provided, the minimum size is to be
used in its place, and vice versa.
.LP
The min_aspect and max_aspect fields are fractions,
with the numerator first and the denominator second,
and they allow a client to specify the range of aspect
ratios it prefers.
.Pp
The ``base'' and ``win_gravity'' fields need a change to Xlib.
.Pe
.NH 4
WM_HINTS
.LP
The WM_HINTS property,
whose type is WM_HINTS,
is used to communicate to the window manager
the information it needs other than the window geometry,
which is available from the window itself,
the constraints on that geometry,  which is available from the WM_NORMAL_HINTS
structure,  and various strings,  which need separate properties such
as WM_NAME.
The contents of these properties are shown in Table 6.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 6 \- WM_HINTS type property contents
_
Field	Type	Comments
_
flags	CARD32	see Table 7 below
input	CARD32	client's input model
initial_state	CARD32	state when first mapped
icon_pixmap	PIXMAP	pixmap for icon image
icon_window	WINDOW	window for icon image
icon_x	INT32	icon location
icon_y	INT32
icon_mask	PIXMAP	mask for icon shape
window_group	WINDOW	ID of group leader window
.TE
.KE
.KF
.TS
center, box;
c s s
c c c
l n l.
Table 7 \- WM_HINTS.flags bit definitions
_
Name	Value	Field
_
InputHint	1	input
StateHint	2	initial_state
IconPixmapHint	4	icon_pixmap
IconWindowHint	8	icon_window
IconPositionHint	16	icon_x & icon_y
IconMaskHint	32	icon_mask
WindowGroupHint	64	window_group
MessageHint	128	This bit is obsolete
.TE
.KE
.LP
Window managers are free to assume convenient values for all fields of
the WM_HINTS property if a window is mapped without one.
.LP
The input field is used to communicate to the window manager the input focus
model used by the client (see section 4.1.7.).
.LP
Clients with the Globally Active and No Input models should set the
``input'' flag to
.B False .
Clients with the Passive and Locally Active models should set the ``input''
flag to
.B True .
.LP
From the client's point of view,  the window manager will regard
the client's top-level window as being in one of three states:
.Ip
Normal
.Ip
Iconic
.Ip
Withdrawn
.LP
The semantics of these states are described in Section 4.1.4 below.
Newly created windows start in the Withdrawn state.
Transitions between states happen when a non-override-redirect
top-level window is mapped and unmapped,
and when the window manager receives certain messages.
.LP
The value of the initial_state field determines the state the client
wishes to be in at the time the top-level window is mapped from Withdrawn
state,
as shown in Table 8.
.KF
.TS
center, box;
c s s
c c c
l b l.
Table 8 \- WM_HINTS.initial_state values
_
State	Value	Comments
_
NormalState	1	Window is visible
IconicState	3	Icon is visible
.TE
.KE
.LP
The icon_pixmap field may specify a pixmap to be used as an icon.
This pixmap should be:
.Ip
One of the sizes specified in the WM_ICON_SIZE property on the 
root,  if it exists (see Section 4.1.3.2).
.Ip
1-bit deep.  The window manager will select,  through the defaults database,
suitable background (for the 0 bits)
and foreground (for the 1 bits) colors.  These defaults can,  of
course,  specify different colors for the icons of different clients.
.LP
The icon_mask specifies which pixels of the icon_pixmap should be used as the
icon,  allowing for icons to appear non-rectangular.
.LP
The icon_window field is the ID of a window the client wants used as its
icon.  Most,  but not all window managers will support icon windows;
those that do not are likely to have a user interface in which small
windows that behave like icons are completely inappropriate,  so that
clients should not attempt to remedy the omission by working around it.
.LP
Clients needing more capabilities from the icons than a simple two-color
bitmap should use icon windows.
Rules for clients that do are set out in Section 4.1.9.
.LP
The (icon_x,icon_y) coordinate is a hint to the window manager as to where it
should position the icon.
The policies of the window manager control the positioning of icons,  so clients
should not depend on attention being paid to this hint.
.LP
The window_group field lets the client specify that this window belongs to a group
of windows.
An example is a single client manipulating multiple 
children of the root window.
.Cp
The window_group field should be set to the ID of the
group leader.
The window group leader may be a window which exists only for that
purpose;
A place-holder group leader of this kind would never be mapped, 
either by the client or by the window manager.
.Ce
.Cp
The properties of the window group leader are those for the group as
a whole (for example,  the icon to be shown when the entire group
is iconified).
.Ce
.LP
Window managers may provide facilities for manipulating the group as 
a whole.
Clients,  at present,  have no way to operate on the group as a whole.
.LP
The ``messages'' bit,  if set in the flags field,  indicates that the
client is using an obsolete window manager communication protocol\s-2\u*\d\s0,
rather than the WM_PROTOCOLS mechanism of section 4.1.2.7.
.FS
*  This obsolete protocol was described in the 27\s-2\uth\d\s0 July 1988
draft of this manual.
Windows using it can also be detected because their WM_HINTS properties are
4 bytes longer than expected.
Window managers are free to support clients using the obsolete protocol
in a ``backwards compatibility'' mode.
.FE
.NH 4
WM_CLASS
.LP
The WM_CLASS property,
of type STRING (without control characters),
contains two consecutive null-terminated strings
specifying the Instance and Class names to be used by both the client and the
window manager for looking up resources for the application or as identifying
information.
This property must be present when the window leaves Withdrawn state,
and may be changed only while the window is in Withdrawn state.
Window managers may examine the property only when they start up and
when the Window leaves Withdrawn state,
but there should be no need for a client to change its state dynamically.
.LP
The two strings are, respectively:
.Ip
A string naming the particular instance of the application to which
the client owning this window belongs.
Resources that are specified by instance name override any resources
that are specified by class name.
Instance names may be specified by the user in an operating-system specific manner.
Under
.UX ,
the following conventions are used:
.RS
.Ip
If "-name NAME" is given on the command line, NAME is used as the instance name.
.Ip
Otherwise, if the environment variable RESOURCE_NAME is set,
its value will be used as the instance name.
.Ip
Otherwise,
the trailing part of the name used to invoke the program
(argv[0] stripped of any directory names)
is used as the instance name.
.RE
.Ip
A string naming the general class of applications to which the client owning
this window belongs.
Resources that are specified by class apply to all applications that have the
same class name.
Class names are specified by the application writer.
Examples of commonly used class names include "Emacs", "XTerm", "XClock", "XLoad", etc.
.LP
Note that WM_CLASS strings,
being null-terminated,
differ from the general conventions that STRING properties are null-separated.
This inconsistency is necessary for backwards-compatibility.
.NH 4
WM_TRANSIENT_FOR
.LP
The WM_TRANSIENT_FOR property,
of type WINDOW,
contains the ID of another top-level window.
The implication is that this window is a pop-up on behalf of the named
window,  and window managers may decide not to decorate transient windows,
or treat them differently in other ways.
In particular,  window managers should present newly-mapped WM_TRANSIENT_FOR
windows without requiring any user interaction,
even if mapping top-level windows normally does require interaction.
Dialogue boxes,  for example,  are an example of windows that should have
WM_TRANSIENT_FOR set.
.LP
It is important not to confuse WM_TRANSIENT_FOR with override-redirect.
WM_TRANSIENT_FOR should be used in those cases where the pointer
is not grabbed while the window is mapped;  in other words if other windows
are allowed to be active while the transient is up.
If other windows must be prevented from processing input
(for example,  when implementing popup menus),
use override-redirect and grab the pointer while the window is mapped.
.NH 4
WM_PROTOCOLS
.LP
The WM_PROTOCOLS property,
of type ATOM,
is a list of atoms.
Each atom identifies a communication protocol between the client and the
window manager in which the client is willing to participate.
Atoms can identify both standard protocols and private protocols
specific to individual window managers.
.LP
All the protocols in which a client can volunteer to take part involve
the window manager sending the client a ClientMessage event,  and the client
taking appropriate action.
For details of the contents of the event,
see Section 4.2.8.
In each case the protocol transactions are initiated by the window manager.
.LP
The WM_PROTOCOLS property is not required.
If it is not present, the client does not wish to participate in any
window manager protocols.
.LP
The X Consortium will maintain a registry of protocols to avoid
collisions in the name space.
Table 9 contains the protocols that have been defined to date.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 9 \- current WM_PROTOCOLS 
_
Protocol	Section	Purpose
_
WM_TAKE_FOCUS	4.1.7	Assignment of input focus.
WM_SAVE_YOURSELF	5.2.1	Save client state warning
WM_DELETE_WINDOW	5.2.2	Request to delete top-level window

		This table will grow
.TE
.KE
.NH 4
WM_COLORMAP_WINDOWS
.LP
The WM_COLORMAP_WINDOWS property,
of type WINDOW,
on a top-level window is a list of the 
the IDs of windows that may need colormaps installed
that differ from the colormap of the top-level window.
The window manager will watch this list of windows for changes in their
colormap attributes.
The top-level window is always (implicitly or explicitly) on the watch list.
.LP
See Section 4.1.8 for the details of this mechanism.
.NH 3
Window Manager Properties
.LP
The properties described above are those which the client is responsible
for maintaining on its top-level windows.
This section describes the properties that the window manager places on
client's top-level windows,
and on the root.
.NH 4
WM_STATE
.LP
The window manager will place a WM_STATE property,
of type WM_STATE,
on each top-level client window.
In general,  clients should not need to examine the contents of this property;
it is intended for communication between window and session
managers.
See Section 5.1.1.3 for more details.
.NH 4
WM_ICON_SIZE
.LP
A window manager that wishes to place constraints on the sizes of icon
pixmaps and/or windows should place a property called WM_ICON_SIZE on the root.
The contents of this property are shown in Table 10.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 10 \- WM_ICON_SIZE type property contents
_
Field	Type	Comments
_
min_width	CARD32	data for icon size series
min_height	CARD32
max_width	CARD32
max_height	CARD32
width_inc	CARD32
height_inc	CARD32
.TE
.KE
.LP
For more details see the Xlib manual,  Section 9.1.7.
.NH 3
Changing Window State
.LP
From the client's point of view,  the window manager will regard
each of the client's top-level non-override-redirect
windows as being in one of three states.
The semantics of the states are:
.Ip
NormalState.
The client's top-level window is visible.
.Ip
IconicState.
The client's top-level window is iconic,  whatever that means for this window manager.
The client can assume that its icon_window (if any) will be visible,
and failing that its icon_pixmap (if any),
or its WM_ICON_NAME will be visible.
.Ip
WithdrawnState.
Neither the client's top-level window nor its icon are visible.
.LP
In fact the window manager may implement states with semantics
other than those described above.
For example,  a window manager might implement a concept of InactiveState
in which an infrequently used client's window
would be represented as a string in a menu.
But this state is invisible to the client,  which would see itself merely
as being in IconicState.
.LP
Newly-created top-level windows are in Withdrawn state.
Once the window has been provided with suitable
properties,  the client is free to change its state as follows\s-2\u*\d\s0:
.FS
*  The conventions described in earlier drafts of this manual
had some serious semantic problems.
These new conventions are designed to be compatible with clients
using earlier conventions,  except in areas where the earlier
conventions would not actually have worked.
.FE
.Ip
Withdrawn \(-> Normal.
The client should map the window with WM_HINTS.initial_state being NormalState.
.Ip
Withdrawn \(-> Iconic.
The client should map the window with WM_HINTS.initial_state being IconicState.
.Ip
Normal \(-> Iconic.
The client should send a client message event as described below.
.Ip
Normal \(-> Withdrawn.
The client should unmap the window,  and follow it with a synthetic UnmapNotify
event as described below\s-2\u\(dg\d\s0.
.FS
\(dg  For compatibility with obsolete clients,  window managers should trigger
the transition on the real UnmapNotify rather than wait for the synthetic
one.
They should also trigger the transition if they receive a synthetic UnmapNotify
on a window for which they have not yet received a real UnmapNotify.
.FE
.Ip
Iconic \(-> Normal.
The client should map the window.  The contents of WM_HINTS.initial_state
are irrelevant in this case.
.Ip
Iconic \(-> Withdrawn.
The client should unmap the window,  and follow it with a synthetic UnmapNotify
event as described below.
.LP
Once a client's non-override-redirect top-level window has left Withdrawn state,
the client will know that the window is in Normal
state if it is mapped,
and that the window is in Iconic state if it is not mapped.
It may select for StructureNotify on the top-level window,  and it
will receive an UnmapNotify event when it moves to Iconic state,
and a MapNotify when it moves to Normal state.
This implies that a reparenting window manager will unmap the
top-level window as well as the parent window when going Iconic.
.Cp
Reparenting window managers must unmap the client's top-level window
whenever they unmap the window to which they have reparented it.
.Ce
.LP
If the transition is to Withdrawn state.
in addition to unmapping the window itself a synthetic UnmapNotify event must
be sent using SendEvent with the following parameters:
.DS 0
.TA .5i 2.5i 4.0i
.ta .5i 2.5i 4.0i

destination:	the root
propagate:	False
event-mask:	(SubstructureRedirect|SubstructureNotify)
event:	an UnmapNotify with:
	event:	the root
	window:	the window itself
	from-configure:	False
.DE
.LP
The reason for doing this is to ensure that the window manager
gets some notification of the desire to change state,  even though
the window may already be unmapped when the desire is expressed.
.LP
If the transition is from Normal to Iconic state,  the client should
send a ClientMessage event to the root with:
.Ip
``window'' == the window to be iconified
.Ip
``type'' == the atom WM_CHANGE_STATE\s-2\u\(dg\d\s0
.FS
\(dg The ``type'' field of the ClientMessage event (called the
``message_type'' field by Xlib) should not be confused with
the ``code'' field of the event itself,
which will have the value 33 (ClientMessage).
.FE
.Ip
``format'' == 32
.Ip
``data[0]'' == IconicState
.LP
Other values of data[0] are reserved for future extensions to these
conventions\s-2\u\(dd\d\s0.
.FS
\(dd  The format of this ClientMessage event does not match the
format of ClientMessages in Section 4.2.8.
This is because they are sent by the window manager to clients,
and this is sent by clients to the window manager.
.FE
The parameters of the SendEvent should be as above.
.LP
Clients can also select for VisibilityChange on their (top-level
or icon) windows.
They will then receive a VisibilityNotify (state=FullyObscured)
event when the window concerned becomes completely
obscured even though mapped (and thus perhaps a waste
of time to update),  and a VisibilityNotify (state!=FullyObscured)
when it becomes even partly viewable.
.NH 3
Configuring the Window
.LP
Clients can resize and reposition their top-level windows
using the ConfigureWindow request.
The attributes of the window that can be altered with this request are:
.Ip
The [x,y] location of the window's upper left outer corner.
.Ip
The [width,height] of the inner region of the window (excluding
borders).
.Ip
The border-width of the window.
.Ip
The window's position in the stack.
.LP
The coordinate system in which the location is expressed is that
of the root,  irrespective of any re-parenting that
may have occurred,
and the border width to be used and win_gravity position hint
to be used are those most recently requested by
the client.
Client configure requests are interpreted by the window manager
in the same manner as the initial window geometry mapped from
Withdrawn state,
as described in Section 4.1.2.3.
Clients must be aware that there is no guarantee that the window manager
will allocate them the requested size or location,  and must be prepared to
deal with \fIany\fP size and location.
If the window manager decides to respond to a ConfigureRequest by:
.Ip
Not changing the size or location of the window at all,  a client
will receive a synthetic ConfigureNotify event describing the
(unchanged) state of the window.
The (x,y) coordinates will be in the root coordinate system,
adjusted for the border width the client requested,
irrespective of any reparenting that has taken place.
The border_width will be the border width the client requested.
The client will not receive a real ConfigureNotify,
since no change has actually taken place.
.Ip
Moving the window without resizing it,  a client will
receive a synthetic ConfigureNotify event following the move
describing the new state
of the window,  whose (x,y) coordinates will be in the root
coordinate system adjusted for the border width the client requested.
The border_width will be the border width the client requested.
The client may not receive a real ConfigureNotify event describing this
change,  since the window manager may have re-parented the top-level window.
If it does receive a real event,  the synthetic event will follow
the real one.
.Ip
Resizing the window (whether or not it is moved),
a client which has selected for
StructureNotify will receive a ConfigureNotify event.  Note that
the coordinates in this event are relative to the parent,  which
may not be the root if the window has been reparented,
and will reflect the actual border width of the window,
which the window manager may have changed.
The TranslateCoordinates request can be used to convert the coordinates
if required. 
.LP
The general rule is,  coordinates in real ConfigureNotify events are
in the parent's space,  whereas in synthetic events they are in
the root space.
.LP
Clients should be aware that their borders may not be visible.
Window managers are free to use reparenting techniques to
decorate client's top-level windows with ``borders'' containing
titles,  controls,  and other details to maintain a consistent
look-and-feel.
If they do,  they are likely to override the client's attempts
to set the border width,  and set it to zero.
Clients should,  therefore,  not depend on the top-level window's
border being visible nor use it to display any critical information.
Other window managers will allow the top-level windows border to
be visible.
.Cp
Clients should set their desired border-width on all ConfigureWindow
requests,
to avoid a race condition.
.Ce
.LP
Clients changing their position in the stack must be aware that they
may have been reparented,  which means that windows that used to
be siblings no longer are.  Using a non-sibling as the sibling parameter
on a ConfigureWindow request will cause an error.
.Cp
Clients using ConfigureWindow to request a change in their position
in the stack should do so using None in the sibling field.
.Ce
.LP
Clients that must position themselves in the stack relative to some
window that was originally a sibling must do the ConfigureWindow request
(in case they are running under a non-reparenting window manager),
be prepared to deal with a resulting error,
and then follow with a synthetic ConfigureRequest event 
by invoking SendEvent with:
.DS 0
.TA .5i 2.5i 4.0i
.ta .5i 2.5i 4.0i

destination:	the root
propagate:	False
event-mask:	(SubstructureRedirect|SubstructureNotify)
event:	a ConfigureRequest with:
	event:	the root
	window:	the window itself
	....	other parameters from the ConfigureWindow
.DE
.LP
Doing this is deprecated,  and window managers are in any case free
to position windows in the stack as they see fit.
Clients should ignore the ``above'' field of both real and synthetic
ConfigureNotify events that they receive on their non-override-redirect
top-level windows
since they cannot be guaranteed to contain useful information.
.NH 3
Changing Window Attributes
.LP
The attributes that may be supplied when a window is created may be
changed using the ChangeWindowAttributes request.
They are shown in Table 11.
.KF
.TS
center, box;
c s
c c
l l.
Table 11 \- Window Attributes
_
Attribute	Private to client
_
Background pixmap	Yes
Background pixel	Yes
Border pixmap	Yes
Border pixel	Yes
Bit gravity	Yes
Window gravity	No
Backing-store hint	Yes
Save-under hint	No
Event mask	No
Do-Not-propagate mask	Yes
Override-redirect flag	No
Colormap	Yes
Cursor	Yes
.TE
.KE
.LP
Most are private to the client,  and will never be interfered with
by the window manager.
As regards the attributes that are not private to the client:
.Ip
The window manager is free to override the window gravity;
a reparenting window manager may want to set the top-level window's
window gravity for its own purposes.
.Ip
Clients are free to set the save-under hint on their top-level windows
but they must be aware that the hint may be overridden by the window
manager.
.Ip
Windows,  in effect,  have per-client event masks and so clients may
select for whatever events are convenient irrespective of any events
the window manager is selecting for.
There are some events for which only one client at a time may select,
but the window manager should not select for them on any of the client's
windows.
.Ip
Clients can set override-redirect on top-level windows but are
encouraged not to do so except as described in Sections 4.1.10
and 4.2.9.
.NH 3
Input Focus
.LP
There are four models of input handling:
.Ip
No Input.
The client never expects keyboard input.
.IP
An example would be xload,  or another output-only client.
.Ip
Passive Input.
The client expects keyboard input but never explicitly sets the input focus.
.IP
An example would be a simple client with no subwindows,  which will
accept input in PointerRoot mode,  or when the window manager sets
the input focus to its top-level window (in click-to-type mode).
.Ip
Locally Active Input.
The client expects keyboard input,  and explicitly sets the input focus,
but only does so when one of its windows already has the focus.
.IP
An example would be a client with sub-windows defining various data
entry fields that uses Next and Prev keys to move the input focus
between the fields,  once its top-level window has acquired the focus
in PointerRoot mode,  or when the window manager sets the input focus
to its top-level window (in click-to-type mode).
.Ip
Globally Active Input.
The client expects keyboard input,  and explicitly sets the input focus even when
it is in windows the client does not own.
.IP
An example would be a client with a scroll bar,  that wants to allow
users to scroll the window without disturbing the input focus even if
it is in some other window.
It wants to acquire the input focus when the user clicks in the scrolled
region,  but not when the user clicks in the scroll bar itself.  Thus,
it wants to prevent the window manager setting the input focus to any of
its windows.
.LP
The four input models and the corresponding values of the ``input'' field
and the presence or absence of the WM_TAKE_FOCUS atom in the
WM_PROTOCOLS property are shown in Table 12.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 12 \- Input Models
_
Input Model	input field	WM_TAKE_FOCUS
_
No Input	False	Absent
Passive	True	Absent
Locally Active	True	Present
Globally Active	False	Present
.TE
.KE
.LP
Passive and Locally Active clients set the ``input'' field of WM_HINTS 
.B True
to indicate that they require window manager assistance  in acquiring the
input focus.  No Input and Globally Active clients set the ``input'' field
.B False
to request that the window manager not set the input focus to their top-level
window.
.LP
Clients using SetInputFocus must set the ``time'' field to
the timestamp of the event that caused them to make the
attempt.
Note that this cannot be a FocusIn event,  since they do not
have timestamps,  and that clients may acquire the focus
without a corresponding EnterNotify.
Clients must not use CurrentTime in the ``time'' field.
.LP
Clients using the Globally Active model can only use SetInputFocus
to acquire the input focus when they do not already have it on
receipt of one of the following events:
.Ip
ButtonPress
.Ip
ButtonRelease
.Ip
Passive-grabbed KeyPress
.Ip
Passive-grabbed KeyRelease
.LP
In general,  clients should avoid using passive-grabbed Key events for
this purpose except when they are unavoidable (as for example a selection
tool that establishes a passive grab on the keys that cut,  copy,  or
paste).
.LP
The method by which the user commands the window manager to
set the focus to a window is up to the window manager.
For example,  clients cannot determine whether they
will see the click that transfers the focus.
.LP
Windows with the atom WM_TAKE_FOCUS in their WM_PROTOCOLS property
may receive a ClientMessage from the window manager as described in
Section 4.2.8. with WM_TAKE_FOCUS in their data[0] field.
If they want the focus,  they should respond with a SetInputFocus
request with its
``window'' field set to the window of theirs that last had the input
focus,  or to their ``default input window'',
and the ``time'' field set to the timestamp in the message.
See Section 4.2.7.
.LP
A client could receive WM_TAKE_FOCUS when opening from an icon,
or when the user has clicked outside the top-level window in an area that
indicates to the window manager that it should assign the focus (for example,
clicking in the headline bar can be used to assign the focus).
.LP
The goal is to support window managers that want to assign the input focus
to a top-level window in such a way that the top-level
window can either assign it to one of its sub-windows,
or decline the offer of the focus.
A clock, for example, or a text editor with
no currently open frames, might not want to take focus even though the
window manager generally believes that clients should take the input focus
after being deiconified or raised.
.Pp
There would be no need for WM_TAKE_FOCUS if the FocusIn event contained
a timestamp and a previous-focus field.
This could avoid the potential race condition.
There is space in the event for this information;
it should be added at the next protocol revision.
.Pe
.LP
Clients that set the input focus need to decide a value for the
``revert-to'' field of the SetInputFocus request.
This determines the behaviour of the input focus if the window
the focus has been set to becomes not viewable.
It can be any of:
.Ip
Parent.  In general,  clients should use this value when assigning focus
to one of their subwindows.  Unmapping the subwindow will cause focus
to revert to the parent,  which is probably what you want.
.Ip
PointerRoot.  Using this value with a click-to-type focus management
policy
leads to race conditions,  since the window becoming unviewable may
coincide with the window manager deciding to move the focus elsewhere.
.Ip
None.  Using this value causes problems if the window manager
reparents the window (most window managers will) and then crashes.
The input focus will be None,  and there will probably be no way
to change it.
.LP
The convention is:
.Cp
Clients invoking SetInputFocus should set ``revert-to'' to Parent.
.Ce
.LP
A convention is also required for clients that want to give up the
input focus.
.Cp
Clients should not give up the input focus of their own volition.
They should ignore input that they receive instead.
.Ce
.NH 3
Colormaps
.LP
The window manager is responsible for installing and uninstalling colormaps\s-2\u*\d\s0.
.FS
*  The conventions described in earlier drafts by which clients and window managers
shared responsibility for installing Colormaps suffered from semantic
problems.
.FE
Clients provide the window manager with hints on which colormaps to 
install and uninstall,
but must not install or uninstall colormaps themselves.
When a client's top-level window gets the colormap focus
(as a result of whatever colormap focus policy is implemented by the window manager)
the window manager will insure that one or more of the client's colormaps are installed.
The reason for this convention is that there is no safe way for
multiple clients to install and uninstall Colormaps.
.Cp
Clients must not use InstallColormap or UninstallColormap.
.Ce
.LP
There are two possible ways in which clients could hint to the window
manager about the Colormaps they want installed.
Using a property,  they could tell the window manager:
.Ip
a priority ordered list of the Colormaps they want installed,
.Ip
or a priority ordered list of the Windows whose Colormaps they want installed.
.LP
The second of these alternatives has been selected because:
.Ip
It allows window managers to know the Visuals for the Colormaps,
permitting Visual-dependent colormap installation policies.
.Ip
It allows window managers to select for VisibilityChange on the windows
concerned,  and ensure that maps are only installed if the windows that
need them are visible.
.LP
Clients whose top-level windows and sub-windows all use the same colormap
should set its ID in the colormap field of the window's attributes.
They should not set a WM_COLORMAP_WINDOWS property on the top-level window.
If they want to change the colormap,  they should change the window
attribute,  and the window manager will install the colormap for them.
.LP
Clients creating windows may use the value CopyFromParent to inherit
their parent's colormap.
Window managers will ensure that the root window's colormap field
contains a colormap that is suitable for clients to inherit;
in particular the colormap will provide distinguishable colors
for BlackPixel and WhitePixel.
.LP
Top-level windows that have sub-windows,
or override-redirect popup windows,
whose colormap requirements differ from the top-level window
should have a WM_COLORMAP_WINDOWS property.
This property contains a list of window IDs of windows whose
colormaps the window manager should attempt to have installed
when,  in the course of its individual colormap focus policy,
it assigns the colormap focus to the top-level window (see
Section 4.1.2.8).
The list is ordered by the importance to the client of having the
colormaps installed.
If this order changes,  the property should be updated.
The window manager will track changes to this property,
and will track changes to the colormap attribute of the windows
in the property.
.LP
WM_TRANSIENT_FOR windows can either have their own WM_COLORMAP_WINDOWS
property,  or appear in the property of the window they are transient for,
as appropriate.
.LP
Clients should be aware of the min-installed-maps and max-installed-maps fields of the
connection startup information,  and the effect that the minimum value
has on the ``required list'':
.QP
``At any time, there is a subset of the installed maps, viewed as an
ordered list, called the "required list".  The length of the required
list is at most M, where M is the min-installed-maps specified for the
screen in the connection setup.  The required list is maintained as
follows.  When a colormap is an explicit argument to InstallColormap,
it is added to the head of the list, and the list is truncated at the
tail if necessary to keep the length of the list to at most M.  When a
colormap is an explicit argument to UninstallColormap and it is in the
required list, it is removed from the list.  A colormap is not added to
the required list when it is installed implicitly by the server, and
the server cannot implicitly uninstall a colormap that is in the
required list.''
.LP
In less precise words,  the min-installed-maps most recently installed maps
are guaranteed to be installed.  This number will often be one;  clients
needing multiple colormaps should beware.
.LP
The window manager will identify and track changes to the colormap attribute
of the windows identified by the WM_COLORMAP_WINDOWS property,
and the top-level window if it does not appear in the list.
If the top-level window does not appear in the list it will be
assumed to be higher priority than any window in the list.
It will also track changes in the contents of the WM_COLORMAP_WINDOWS
property,  in case the set of windows or their relative priority changes.
The window manager will define some colormap focus policy,  and whenever
the top-level window has the colormap focus will attempt to maximize the
number of Colormaps from the head of the WM_COLORMAP_WINDOWS list that
are installed.
.NH 3
Icons
.LP
A client can hint to the window manager about the desired appearance of its
icon in several ways:
.Ip
Set a string in WM_ICON_NAME.  All clients should do this,  as it provides
a fall-back for window managers whose ideas about icons differ widely from
those of the client.
.Ip
Set a Pixmap into the ``icon_pixmap'' field of the WM_HINTS property,
and possibly another into the ``icon_mask'' field.  The window manager
is expected to display the pixmap masked by the mask.
The pixmap should be one of the sizes found in the WM_ICON_SIZE property
on the root.
If this property is not found,  the window manager is unlikely to display
icon pixmaps.
Window managers will normally clip or tile pixmaps which do not match
WM_ICON_SIZE.
.Ip
Set a window into the ``icon_window''
field of the WM_HINTS property.
The window manager is expected to map that window whenever the client is
in Iconic state.
In general,  the size of the icon window should be one of those specified
in WM_ICON_SIZE on the root,  if it exists.
Window managers are free to resize icon windows.
.LP
In Iconic state,  the window manager will normally ensure that:
.Ip
If the window's WM_HINTS.icon_window is set,
the window it names is visible.
.Ip
If not,  if the window's WM_HINTS.icon_pixmap is set,
the pixmap it names is visible.
.Ip
Otherwise,  the window's WM_ICON_NAME string is visible.
.LP
Clients should observe the following conventions about their icon
windows:
.Cp
The icon window should be an InputOutput child of the root.
.Ce
.Cp
The icon window should be one of the sizes specified in the WM_ICON_SIZE property
on the root.
.Ce
.Cp
The icon window should use the root visual & default colormap for the screen
in question.
.Ce
.Cp
Clients should not map their icon windows.
.Ce
.Cp
Clients should not unmap their icon windows.
.Ce
.Cp
Clients should not configure their icon windows.
.Ce
.Cp
Clients should not set override-redirect on their icon windows,
nor select for ResizeRedirect on them.
.Ce
.Cp
Clients must not depend on being able to receive input events
via their icon windows.
.Ce
.Cp
Clients must not manipulate the borders of their icon windows.
.Ce
.Cp
Clients must select for Exposure on their icon window,
and repaint it when requested.
.Ce
.LP
Window managers will differ as to whether they support input events
to client's icon windows;
most will allow some subset of the keys and buttons through.
.LP
Window managers will ignore any WM_NAME,
WM_ICON_NAME,
WM_NORMAL_HINTS,
WM_HINTS,
WM_CLASS,
WM_TRANSIENT_FOR,
WM_PROTOCOLS,
or WM_COLORMAP_WINDOWS
properties they find on icon windows.
Session managers will ignore any WM_COMMAND or WM_CLIENT_MACHINE
properties they find on icon windows.
.NH 3
Popup Windows
.LP
Clients wishing to pop-up a window can do one of three things:
.Ip
They can create and map another normal top-level window,  which will get
decorated and managed as normal by the window manager.
See the discussion of window groups below.
.Ip
If the window will be visible for a relatively short time,
and deserves a somewhat lighter treatment,
they can set the WM_TRANSIENT_FOR property.
They can expect less decoration,  but can set all the normal
window manager properties on the window.
An example would be a dialog box.
.Ip
If the window will be visible for a very short time, and
should not be decorated at all,  the client can set
override-redirect on the window.
In general,  this should be done only if the pointer is
grabbed while the window is mapped.
The window manager will never interfere with these windows,
which should be used with caution.
An example of an appropriate use is a pop-up menu.
.LP
Window managers are free to decide if WM_TRANSIENT_FOR windows
should be iconified when the window they are transient for is.
Clients displaying WM_TRANSIENT_FOR windows which have (or
request to have) the window they are transient for iconified
do not need to request that the same operation be performed
on the WM_TRANSIENT_FOR window;  the window manager will
change its state if that is the policy it wishes to enforce.
.NH 3
Window Groups
.LP
A set of top-level windows that should be treated from the user's
point of view as related (even though they may belong to a number
of clients) should be linked
together using the ``window_group'' field of the WM_HINTS
structure.
.LP
One of the windows (the one the others point to) will be the
group leader and will carry the group as opposed to the
individual properties.
Window managers may treat the group leader differently from
other windows in the group.
For example,  group leaders may have the full set of decorations,
and other group members a restricted set.
.LP
It is not necessary that the client ever map the group leader;
it may be a window that exists solely as a place-holder.
.LP
It is up to the window manager to determine the policy for
treating the windows in a group.
There is,  at present,  no way for a client to request a group,  as opposed
to an individual,  operation.
.if 0 \{
.NH 3
XXX - Overview of Client Actions
.IP
<XXX - insert overview here>
\}
.NH 2
Client Responses to Window Manager Actions
.LP
The window manager performs a number of operations on client
resources,  primarily on their top-level windows.
Clients must not try to fight this,  but may elect to
receive notification of the window manager's operations.
.NH 3
Reparenting
.LP
Clients must be aware that some window managers will reparent their
non-override-redirect top-level windows,
so that a window that was created as a child
of the root will be displayed as a child of some window
belonging to the window manager.
The effects that this reparenting will have on the client are:
.Ip
The parent value returned by a QueryTree request will no longer
be the value supplied to the CreateWindow request that created the
reparented window.  There should normally be no need for the client
to be aware of the identity of the window to which the top-level
window has been reparented.  In particular,  a client wishing to
create further top-level windows should continue to use the
root as the parent for these new windows.
.Ip
The server will interpret the
(x,y) coordinates in a ConfigureWindow request
in the new parent's coordinate space.
They will,  in fact,  normally not be interpreted by the server
because a reparenting window manager will normally have intercepted
these operations (see below).
Clients should use the root coordinate space for these requests
(see Section 4.1.5).
.Ip
ConfigureWindow requests that name a specific sibling window may
fail because the window named,  which used to be a sibling,
no longer is after the reparenting operation
(see Section 4.1.5).
.Ip
The (x,y) coordinates returned by a GetGeometry request are in the parent's
coordinate space,  and are thus not directly
useful after a reparent operation.
.Ip
A background of ParentRelative will have unpredictable results.
.Ip
A cursor of None will have unpredictable results.
.LP
Clients wishing to be notified when they are reparented can select
for StructureNotify on their top-level window.
They will receive a ReparentNotify event if and when reparenting
takes place.
.LP
If the window manager reparents a client's window,
the reparented window will be placed in the ``save set'' of the
parent window.
This means that,  if the window manager terminates,  the reparented window will
not be destroyed,  and will be remapped if it was unmapped.
Note that this applies to \fIall\fP client windows the window manager
reparents,  including transient windows and client icon windows.
.LP
When the window manager gives up control over a 
client's top-level window,  it will
reparent it (and any associated windows,  such as WM_TRANSIENT_FOR
windows) back to the root.
.LP
There is a potential race condition here.
A client might wish to re-use the top-level window,  reparenting it
somewhere else.
.Cp
Clients wishing to re-parent their top-level windows should do so
only when they have their original parents.
They may select for StructureNotify on their top-level windows,
and will receive ReparentNotify events informing them when this
is true.
.Ce
.NH 3
Redirection of Operations
.LP
Clients must be aware that some window managers will arrange for
some client requests to be intercepted and redirected.
Redirected requests are not executed;  they result instead in
events being sent to the window manager,  which may decide to do nothing,
to alter the arguments,  or to perform the request on behalf of
the client.
.LP
The possibility that a request may be redirected means that
a client may not assume that any redirect-able request is actually
performed when the request is issued,  or at all.
For example,  the sequence:
.DS
MapWindow A
PolyLine A GC <point> <point> ....
.DE
.LP
is incorrect,  since the MapWindow request may be intercepted and
the PolyLine output made to an unmapped window.
The client must wait for an Expose event before drawing in the window\s-2\u*\d\s0.
.FS
*  This is true even if the client set backing-store to Always.
The backing-store value is a only a hint,  and the server may stop
maintaining backing-store contents at any time.
.FE
Another example is:
.DS
ConfigureWindow width=N height=M
<output assuming window is N by M>
.DE
.LP
which incorrectly assumes that the ConfigureWindow request is actually
executed with the arguments supplied.
.LP
The requests which may be redirected are:
.Ip
MapWindow
.Ip
ConfigureWindow
.Ip
CirculateWindow
.LP
A window with the override-redirect bit set is immune from
redirection,  but the bit should be set on top-level
windows only in cases where other windows should be prevented from
processing input
while the override-redirect window is mapped (see Section 4.1.10)
and while responding to ResizeRequest events (see Section 4.2.9).
.LP
Clients which have no non-Withdrawn top-level windows and which
map an override-redirect top-level window are taking over total
responsibility for the state of the system.
It is their responsibility to:
.Ip
prevent any pre-existing window manager from interfering with their activities.
.Ip
restore the status quo exactly after they unmap the window,
so that any pre-existing window manager doesn't get confused.
.LP
In effect,  clients of this kind are acting as temporary window managers.
Doing so is strongly discouraged,  since these clients will be unaware
of the user interface policies the window manager is trying to maintain,
and their user interface behaviour is likely to conflict with that of
less demanding clients.
.NH 3
Window Move
.LP
If the window manager moves a top-level window without changing its size,
the client will receive a synthetic ConfigureNotify event following the move
describing the new location,  in terms of the root coordinate space.
Clients must not respond to being moved by attempting to move
themselves to a better location.
.LP
Any real ConfigureNotify event on a top-level window
implies that the window's position on the root may have changed,
even though the event reports that the window's position in its parent
is unchanged,
because the window may have been re-parented.
And note that the coordinates in the event will not,  in this case,
be directly useful.
.LP
The window manager will send these events using SendEvent with:
.DS 0
.TA .5i 2.5i 4.0i
.ta .5i 2.5i 4.0i

destination:	the client's window
propagate:	False
event-mask:	StructureNotify
.DE
.NH 3
Window Resize
.LP
The client can elect to receive notification of being
resized by selecting for StructureNotify on its
top-level window(s).
It will receive a ConfigureNotify event.
The size information in the event will be correct,
but the location will be in the parent window (which may not be the root).
.LP
The response of the client to being resized should be to accept
the size it has been given,  and to do its best with it.
Clients must not respond to being resized by attempting to resize
themselves to a better size.
If the size is impossible to work with,  clients are free to
request to change to Iconic state.
.NH 3
(De)Iconify
.LP
A non-override-redirect window that is not Withdrawn will be
in Normal state if it is mapped,  and in Iconic state if it is
unmapped.
This will be true even if the window has been reparented;  the
window manager will unmap the window as well as its parent when
switching to Iconic state.
.LP
The client can elect to be notified of these state changes by
selecting for StructureNotify on the top-level window.
It will receive UnmapNotify when it goes Iconic,  and MapNotify
when it goes Normal.
.NH 3
Colormap Change
.LP
Clients that wish to be notified of their colormaps being installed
or uninstalled should select for ColormapNotify on their top-level
windows,
and on any windows they have named in WM_COLORMAP_WINDOWS properties
on their top-level windows.
They will receive ColormapNotify events with the ``new'' field FALSE
when the colormap for that window is installed or uninstalled.
.Pp
There is an inadequacy in the protocol.
At the next revision,
the InstallColormap request should be changed to include a timestamp
to avoid the possibility of race conditions if more than one client
attempts to install and uninstall colormaps.
These conventions attempt to avoid the problem by restricting use
of these requests to the window manager.
.Pe
.NH 3
Input Focus
.LP
Clients can request notification that they have the input focus
by selecting for FocusChange on their top-level windows;
they will receive FocusIn and FocusOut events.
Clients that need to set the input focus to one of their
sub-windows should not do so unless
they have set WM_TAKE_FOCUS in their WM_PROTOCOLS property and:
.Ip
have set the ``input'' field of WM_HINTS to True and
actually have the input focus in (one of) their top-level windows,
.Ip
or have set the ``input'' field of WM_HINTS to False and have
received a suitable event as described in Section 4.1.7,
.Ip
or have received a WM_TAKE_FOCUS message as described in Section 4.1.7.
.LP
Clients should not warp the pointer in an attempt to transfer the
focus,  they should set the focus and leave the pointer alone.
See Section 6.2.
.LP
Once a client satisfies these conditions,  it may transfer the focus
to another of its windows using the SetInputFocus request:
.Ls
SetInputFocus
	focus:	WINDOW or PointerRoot or None
	revert-to:	{Parent, PointerRoot, None}
	time:	TIMESTAMP or CurrentTime
.Le
.LP
.Cp
Clients using SetInputFocus must set the ``time'' field to
the timestamp of the event that caused them to make the
attempt.
Note that this cannot be a FocusIn event,  since they do not
have timestamps,  and that clients may acquire the focus
without a corresponding EnterNotify.
Clients must not use CurrentTime in the ``time'' field.
.Ce
.Cp
Clients using SetInputFocus to set the focus to one of their
windows must set the revert-to field to Parent.
.Ce
.NH 3
ClientMessage Events
.LP
There is no way for clients to prevent themselves being sent
ClientMessage events.
.LP
Top-level windows with a WM_PROTOCOLS property
may be sent ClientMessage events specific to the protocols
named by the atoms in the property (see Section 4.1.2.7).
For all protocols,  the ClientMessage events:
.Ip
have WM_PROTOCOLS as the type field\s-2\u\(dg\d\s0,
.FS
\(dg The ``type'' field of the ClientMessage event (called the
``message_type'' field by Xlib) should not be confused with
the ``code'' field of the event itself,
which will have the value 33 (ClientMessage).
.FE
.Ip
have format 32,
.Ip
have the atom naming their protocol in the data[0] field\s-2\u*\d\s0,
.FS
*  We use the notation data[n] to indicate the n-th element of the
LISTofINT8,
LISTofINT16 or LISTofINT32 in the data field of the ClientMessage,
according to the format field.
The list is indexed from zero.
.FE
.Ip
have a time-stamp in their data[1] field.
.LP
The remaining fields,
including the ``window'' field,
of the event are determined by the protocol.
.LP
These events will be sent using SendEvent with:
.DS 0
.TA .5i 2.5i 4.0i
.ta .5i 2.5i 4.0i

destination:	the client's window
propagate:	False
event-mask:	() empty
event:		as specified by the protocol
.DE
.NH 3
Redirecting Requests
.LP
Normal clients can use the redirection mechanism just as window managers do,
by selecting for SubstructureRedirect on a parent window,
or ResizeRedirect on a window itself.
However,  at most one client per window can select for these events,
and a convention is needed to avoid clashes:
.Cp
Clients (including window managers) should select for SubstructureRedirect
and ResizeRedirect only on windows that they own.
.Ce
.LP
In particular,  clients that need to take some special action if they
are resized can select for ResizeRedirect on their top-level windows.
They will receive a ResizeRequest event if the window manager resizes
their window,  and the resize will not actually take place.
Clients are free to make what use they like of the information that
the window manager wants to change their size,  but they must configure
the window to the width and height specified in the event in a timely
fashion.
To ensure that the resize will actually happen at this stage,
instead of being intercepted and executed by the window manager
(and thus re-starting the process)
the client needs temporarily to set override-redirect on the window.
.Cp
Clients receiving ResizeRequest events must respond by:
(a)
setting override-redirect on the window specified in the event,
(b)
configuring the window specified in the event to the width and height
specified in the event as soon as possible,
and before making any other geometry requests,
and then (c)
clearing override-redirect on the window specified in the event.
.Ce
.LP
If a window manager detects that a client is not obeying this convention,
it is free to take whatever measures it deems appropriate to deal with
the client.
.NH 2
Summary of Window Manager Property Types
.LP
The window manager properties are summarized in Table 13.
This,  in effect,  replaces the table in Section 9.1 of
the Xlib manual.
.KF
.TS
center, box;
c s s s
c c c c
l l n l.
Table 13 \- Window Manager Properties
_
Name	Type	Format	See Section
_
WM_CLASS	STRING	8	4.1.2.5
WM_COLORMAP_WINDOWS	WINDOW	32	4.1.2.8
WM_HINTS	WM_HINTS	32	4.1.2.4
WM_ICON_NAME	TEXT		4.1.2.2
WM_ICON_SIZE	WM_ICON_SIZE	32	4.1.3.2
WM_NAME	TEXT		4.1.2.1
WM_NORMAL_HINTS	WM_SIZE_HINTS	32	4.1.2.3
WM_PROTOCOLS	ATOM	32	4.1.2.7
WM_STATE	WM_STATE	32	4.1.3.1
WM_TRANSIENT_FOR	WINDOW	32	4.1.2.6
.TE
.KE
.bp
.NH
Client to Session Manager Communication
.LP
The role of the session manager is to manage a collection of clients.
It should be capable of:
.Ip
Starting a collection of clients as a group.
.Ip
Remembering the state of a collection of clients so that
they can be re-started in the same state.
.Ip
Stopping a collection of clients in a controlled way.
.LP
It may also provide a user interface to these capabilities.
.NH 2
Client Actions
.LP
There are two ways in which clients should cooperate with the session manager:
.Ip
Stateful clients should cooperate with the session manager by providing
it with information it can use to restart them if it should become
necessary.
.Ip
Clients,  typically those with more than one top-level window,
whose server connection needs to survive the deletion of their top-level
window should take part in the WM_DELETE_WINDOW protocol
(see Section 5.2.2).
.NH 3
Properties
.LP
The client communicates with the session manager by placing two properties
(WM_COMMAND and WM_CLIENT_MACHINE)
on its top-level window.
If the client has a group of top-level windows,  these properties
should be placed on the group leader window.
.LP
The window manager is responsible for placing a WM_STATE property
on each top-level client window for use by session managers and other clients
that need to be able to identify top-level client windows and their state.
.NH 4
WM_COMMAND
.LP
The WM_COMMAND property
represents the command used to (re-)start the client.
Clients should ensure,  by re-setting this property,  that it
always reflects a command that will restart them in their current state.
The content and type of the property depends on the operating system of
the machine running the client.
In
.UX
systems using ISO Latin 1 characters for their command lines,  the property should:
.Ip
be of type STRING,
.Ip
contain a list of NULL-terminated strings,
.Ip
and be initialized from \fIargv\fP.
Other systems will need to set appropriate conventions for the type and
contents and type of WM_COMMAND properties.
Window and session managers should not assume that STRING is the type
of WM_COMMAND,
nor assume that they will be able to understand or display its contents.
.LP
Note that WM_COMMAND strings,
being null-terminated,
differ from the general conventions that STRING properties are null-separated.
This inconsistency is necessary for backwards-compatibility.
.LP
A client with multiple top-level windows should ensure that exactly
one of them should have a WM_COMMAND with non-zero length.
Zero-length WM_COMMAND properties can be used to reply to WM_SAVE_YOURSELF
messages on other top-level windows,
but will otherwise be ignored (see Section 5.2.1).
.NH 4
WM_CLIENT_MACHINE
.LP
The client should set the WM_CLIENT_MACHINE property,
of one of the TEXT types,
to a string forming the
name of the machine running the client,  as seen from the machine
running the server.
.NH 4
WM_STATE
.LP
The window manager will place a WM_STATE property,
of type WM_STATE,
on each top-level client window.
.LP
Programs like
.B xprop
that want to operate on client's top-level windows can use this
property to identify them.
A client's top-level window is one that:
.Ip
has override-redirect False,
.Ip
and has a WM_STATE property,
.Ip
or a mapped child of the root that has no descendant with a WM_STATE
property.
.LP
Recursion is necessary to cover all window manager reparenting
possibilities.
Note that clients other than window and session managers should
not need to examine the contents of WM_STATE properties;
which are not formally defined by this document.
The presence or absence of the property is all they need to know.
.LP
Suggested contents of the WM_STATE property are shown in Table 14.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 14 \- WM_STATE type property contents
_
Field	Type	Comments
_
state	CARD32	see Table 15 below
icon	WINDOW	ID of icon window
.TE
.KE
.KF
.TS
center, box;
c s s
c c c
l n l.
Table 15 \- WM_STATE.state values
_
State	Value	Comments
_
WithdrawnState	0
NormalState	1
IconicState	3
.TE
.KE
.LP
Adding other fields to this property is reserved to the X Consortium.
.LP
The icon field should contain the window
ID of the window which the window manager uses as the icon window for the
window on which this property is set,  if any,  otherwise None.
Note that this window may
not be the same as the icon window which the client may have specified.
It may be:
.Ip
the client's icon window,
.Ip
or a window that the window manager supplied which contains the client's icon
pixmap,
.Ip
or else the least ancestor of the client's icon window (or of the window
which contains the client's icon pixmap) which contains no other icons.
.LP
The state field describes the window manager's idea of
the state the window is in,  which may not match
the client's idea as expressed in the initial_state field
of the WM_HINTS property (for example,  if the user has
asked the window manager to iconify the window).
If it is NormalState,  the window manager believes
the client should be animating its window;
if it is IconicState that it should animate its icon window.
Note that in either state clients should be prepared to handle
exposure events from either window.
.LP
The contents of WM_STATE properties and other aspects of the communication
between window and session managers will be specified in the
.I
Window and Session Manager Conventions Manual.
.R
.NH 3
Termination
.LP
Since they communicate via unreliable network connections,  X11 clients must be
prepared for their connection to the server to be terminated at
any time without warning.
They cannot depend on getting notification that termination is imminent,
nor on being able to use the server to negotiate with the user (for example,
using dialog boxes for confirmation) about their fate.
.LP
Equally,  clients may terminate at any time without notice to the
session manager.
When a client terminates itself,  rather than being terminated by
the session manager,  it is viewed as having resigned from the session
in question,  and it will not be revived if the session is revived.
.NH 2
Client Responses to Session Manager Actions
.LP
Clients may need to respond to session manager actions in two ways:
.Ip
saving their internal state,
.Ip
deleting a window.
.NH 3
Saving Client State
.LP
Clients that wish to be warned when the session manager feels that
they should save their internal state (for example,
when termination impends) should
include the atom WM_SAVE_YOURSELF in the WM_PROTOCOLS property on their
top-level windows to participate in the WM_SAVE_YOURSELF protocol.
They will receive a ClientMessage as described in Section 4.2.8.
with the atom WM_SAVE_YOURSELF in its data[0] field.
.LP
Clients receiving WM_SAVE_YOURSELF should place themselves in a state from
which they can be restarted,  and should update WM_COMMAND to
be a command that will restart them in this state.
The session manager will be waiting for a PropertyNotify on WM_COMMAND
as a confirmation that the client has saved its state,  so that
WM_COMMAND should be updated (perhaps with a zero-length append)
even if its contents are correct.
No interactions with the user are permitted during this process.
.LP
Once it has received this confirmation,  the session manager will
feel free to terminate the client if that is what the user asked for.
Otherwise,  if the user asked for the session to be put to
sleep,  the session manager will ensure that the client does not
receive any mouse or keyboard events.
.LP
After receiving a WM_SAVE_YOURSELF,
saving its state,  and updating WM_COMMAND
the client should not
change its state (in the sense of doing anything that would require
a change to WM_COMMAND)
until it receives a mouse or keyboard event.
Once it does so,  it can assume that the danger is over.
The session manager will ensure that these events do not reach
clients until the danger is over,  or until the clients have been killed.
.LP
Clients with multiple top-level windows should ensure that,
irrespective of how they are arranged in window groups:
.Ip
Only one of their top-level windows has a non-zero-length WM_COMMAND
property.
.Ip
They respond to a WM_SAVE_YOURSELF message by (in this order):
.RS
.IP 1.
updating the non-zero length WM_COMMAND property if necessary,
.IP 2.
updating the WM_COMMAND property on the window for which they received
the WM_SAVE_YOURSELF message if it was not updated in step 1.
.RE
.LP
Receiving WM_SAVE_YOURSELF on a window is (conceptually) a command to save the
entire client state\s-2\u*\d\s0.
.FS
*  This convention has changed since earlier drafts because of the introduction
of the protocol in the next section.
In the public review draft,
there was ambiguity as to whether WM_SAVE_YOURSELF was a checkpoint or a shutdown
facility.
It is now unambigously a checkpoint facility;
if a shutdown facility is judged to be necessary a separate WM_PROTOCOLS protocol
will be developed and registered with the X Consortium.
.FE
.NH 3
Window Deletion
.LP
Clients,
normally those with multiple top-level windows,
whose server connection must survive the deletion of some of their
top-level windows should include the atom WM_DELETE_WINDOW
in the WM_PROTOCOLS property on each such window.
They will receive a ClientMessage as described in Section 4.2.8
whose data[0] field is WM_DELETE_WINDOW.
.LP
Clients receiving a WM_DELETE_WINDOW message should behave as if the user
selected "delete window" from a (hypothetical) menu.
They should perform any confirmation dialogue with the user,
and if they decide to complete the deletion:
.Ip
Either change the window's state to Withdrawn (as described in Section 4.1.4)
or destroy the window.
.Ip
Destroy any internal state associated with the window.
.LP
If the user aborts the deletion during the confirmation dialogue
the client should ignore the message.
.LP
Clients are permitted to interact with the user and ask (for example)
whether a file associated with the window to be deleted should be saved,
or the window deletion should be cancelled.
Clients are not required to destroy the window itself,
the resource may be reused,
but all associated state (backing store,  for example) should be released.
.LP
If the client aborts a destroy,
and the user then selects DELETE WINDOW again,
the WM should start the WM_DELETE_WINDOW protocol again.
WMs should not use DestroyWindow on a window that has
WM_DELETE_WINDOW in its WM_PROTOCOLS property.
.LP
Clients which choose not to include WM_DELETE_WINDOW in the WM_PROTOCOLS
property may be disconnected from the server
if the user asks for one of the client's top-level windows to be deleted.
.LP
Note that the WM_SAVE_YOURSELF and WM_DELETE_WINDOW protocols are
orthogonal to each other,  and may be selected independently.
.NH 2
Summary of Session Manager Property Types
.LP
The session manager properties are summarized in Table 16.
.KF
.TS
center, box;
c s s s
c c c c
l l n l.
Table 16 \- Window Manager Properties
_
Name	Type	Format	See Section
_
WM_CLIENT_MACHINE	TEXT		5.1.1.2
WM_COMMAND	TEXT		5.1.1.1
WM_STATE	WM_STATE	32	5.1.1.3
.TE
.KE
.bp
.NH
Manipulation of Shared Resources
.LP
X11 permits clients to manipulate a number of shared resources,
among them the input focus,  the pointer,  and colormaps.
Conventions are required so that clients do so in an
orderly fashion.
.if 0 \{
.IP
<XXX - grab rules>
\}
.NH 2
The Input Focus
.LP
Clients that explicitly set the input focus can do so in one of two modes:
.Cp
Locally Active clients should set the input focus to one of their windows only
when it is already in one of their windows,  or when they receive
a WM_TAKE_FOCUS message.
They should set the ``input'' field of the WM_HINTS structure TRUE.
.Ce
.Cp
Globally Active clients should set the input focus to one of their windows only
when they receive a button event,
a passive-grabbed key event,
or when they receive
a WM_TAKE_FOCUS message.
They should set the ``input'' field of the WM_HINTS structure FALSE.
.Ce
.Cp
Clients should use the timestamp of the event that caused them
to attempt to set the input focus as the ``time'' field on
the SetInputFocus request,  not CurrentTime.
.Ce
.NH 2
The Pointer
.LP
In general, clients should not warp the pointer.  Window managers may
do so,  for example to maintain the invariant that the pointer is always
in the window with the input focus.
Other window managers may wish to preserve the illusion that the user
is in sole control of the pointer.
.Cp
Clients should not warp the pointer.
.Ce
.Cp
Clients which insist on warping the pointer should do so only
with the ``src-window'' field of the WarpPointer request set to one
of their windows.
.Ce
.NH 2
Grabs
.LP
A client's attempt to establish a Button or a Key grab on a window
will fail if some other client has already established a conflicting
grab on the same window.
The grabs are,
therefore,
shared resources and their use requires conventions.
.LP
In conformance with the principle that clients should behave as far
as possible when a window manager is running as they would when it
is not, a client that has the input focus may assume that it can
receive all the available Keys and Buttons.
.Cp
Window managers should ensure that they provide some mechanism for
their clients to receive events from all keys and all buttons,
except events involving keys whose keysyms are registered as being for
window management functions (eg. a hypothetical WINDOW keysym).
.Ce
.LP
In other words,
window managers must provide some mechanism by which a client
can receive events from
.I every
key and button (regardless of modifiers) unless and
until the X Consortium registers some
keysyms as being reserved for window management functions.
No keysyms are currently registered for window management functions.
.LP
Even so, clients are well-advised to allow the key and button
combinations used to elicit program actions to be modified; since some
window managers may choose not to observe this convention,
or may not provide a convenient method for the user to transmit events
from some keys.
.Cp
Clients should establish Button and Key grabs only on windows that
they own.
.Ce
.LP
In particular, this means that a window manager wishing to establish
a grab over the client's top-level window should either establish
the grab on the root or reparent the window
and establish the grab on a proper ancestor.
In some cases, a window manager may want to consume the event received,
placing the window in a state where a subsequent such event will go to
the client.
Examples are clicking in a window to set focus, with the click not
being offered to the client, or clicking in a buried window to raise
it, again with the click not offered to the client.
More typically, a window manager should add to rather than replace
the client's semantics for key+button combinations by allowing the
event to be used by the client after the window manager is done with
it.
To ensure this,
the window manager should establish the grab on the parent using:
.Ls
pointer/keyboard-mode = Synchronous
.Le
and release the grab using AllowEvents with:
.Ls
mode = ReplayPointer/Keyboard
.Le
.LP
In this way,
the client will receive the events as if they had not been intercepted.
.LP
Obviously,
these conventions place some constraints on possible user interface policies.
There is a tradeoff here between freedom for window managers to implement
their user interface policies and freedom for clients to implement theirs.
We resolve this dilemma by:
.Ip
Allowing window managers to decide if and when a client will receive an
event from any given Key or Button.
.Ip
Placing a requirement on the window manager to provide some mechanism,
perhaps a ``Quote'' key,
by which the user can send an event from
.I any
key or button to the client.
.NH 2
Colormaps
.LP
.Cp
If a client has a top-level window that has sub-windows,
or override-redirect popup windows,
whose colormap requirements differ from the top-level window
should set a WM_COLORMAP_WINDOWS property on the top-level window.
The WM_COLORMAP_WINDOWS property contains a list of the window IDs of
windows that the window manager should track for colormap changes.
.Ce
.Cp
When a client's colormap requirements change the client
should change the colormap window attribute of a top-level window or
one of the windows indicated by a WM_COLORMAP_WINDOWS property.
.Ce
.Cp
Clients must not use InstallColormap or UninstallColormap.
.Ce
.LP
Clients with DirectColor type applications should consult Section 9.2
of the Xlib manual for conventions connected with sharing standard
colormaps.
They should look for,  and create,  the properties described there on
the root window of the appropriate Screen.
.LP
Note,  however,  that the conventions described there are not adequate
if the server supports multiple Visuals
and are not adequate if standard colormaps need to be deleted.
To address this,
two additional fields (visual_id and kill_id) are required in
RGB_COLOR_MAP type properties,  as shown in Table 17.
The Colormap described by the property is one appropriate for the
Screen on whose root the property is found.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 17 \- RGB_COLOR_MAP type property contents
_
Field	Type	Comments
_
colormap	COLORMAP	ID of the Colormap described
red_max	CARD32	Values for pixel calculations
red_mult	CARD32
green_max	CARD32
green_mult	CARD32
blue_max	CARD32
blue_mult	CARD32
base_pixel	CARD32
visual_id	VISUALID	Visual to which Colormap belongs
kill_id	CARD32	ID for destroying the resources
.TE
.KE
.LP
When deleting or replacing an RGB_COLOR_MAP, it is not sufficient to delete
the property; it is important to free the associated colormap resources as
well.  If ``kill_id'' is greater than one, then the resources should be freed
by issuing a KillClient protocol request with ``kill_id'' as the argument.
If ``kill_id'' is one, then the resources should be freed by issuing a
FreeColormap protocol request with ``colormap'' as the Colormap argument.
If ``kill_id'' is zero, then no attempt should be made to free the resources.
A client creating an RGB_COLOR_MAP for which the ``colormap'' resource is
created specifically for this purpose should set ``kill_id'' to one (and can
create more than one such standard colormap using a single connection).
A client creating an RGB_COLOR_MAP for which the ``colormap'' resource is
shared in some way (e.g., is the default colormap for the root window) should
create an arbitrary resource and use it's resource id for ``kill_id'' (and
should create no other standard colormaps on the connection).
.Cp
If an RGB_COLOR_MAP property is too short to contain the ``visual_id''
field,  it can be assumed that the visual_id is the root Visual of
the appropriate screen.
If an RGB_COLOR_MAP property is too short to contain the ``kill_id''
field, a value of zero can be assumed.
.Ce
.LP
During the connection handshake,  the server informs the client of the default
Colormap for each screen.
This is a Colormap for the root Visual,
and clients can use it to improve the extent of Colormap sharing
if they use the root Visual.
.LP
A similar capability is desirable for other Visuals,  and can be supported
by changing the definition of the RGB_DEFAULT_MAP property in Section 9.2.2
of the Xlib manual to read:
.QP
``This atom names a property.
The value of the property is an array of XStandardColormap structures
(as extended to include visual_id and kill_id fields).''
.QP
``Each entry in the array describes an RGB subset of the default color
map for the Visual specified by visual_id.''
.NH 2
The Keyboard Mapping
.LP
The X server contains a table (read by GetKeyboardMapping) that
describes,  for each keycode generated by the server,  the set of
symbols appearing on the corresponding key.  This table does not
affect the server's operations in any way,  it is simply a database
used by clients attempting to understand the keycodes they receive.
Nevertheless,  it is a shared resource and requires conventions.
.LP
It is possible for clients to modify this table,  using ChangeKeyboardMapping.
In general,  clients should not do this.
In particular,  this is
.I not
the way in which clients should implement key bindings or key remapping.
The conversion between a sequence of keycodes received from the server,
and a string in a particular encoding is a private matter for each client,
as it must be in a world where applications may be using different
encodings to support different languages and fonts.  This conversion
for ISO Latin 1 is implemented by the Xlib \fIXLookupString()\fP function;
there will presumably be equivalent functions for other encodings.
.LP
The only valid reason for using ChangeKeyboardMapping is when the symbols
written on the keys have changed;  as for example when a Dvorak key
conversion kit,  or a set of APL keycaps has been installed.
Of course,  a client may have to take the change to the keycap on trust.
.LP
It is permissible for a  client to interact with a user thus:
.Ip
``You just started me on a server without a PAUSE key.
Please choose a key to be the PAUSE key and press it now.''
.Ip
<User presses the SCROLL LOCK key>
.Ip
``Adding PAUSE to the symbols on the SCROLL LOCK key: Confirm or Abort.''
.Ip
<User confirms>
.Ip
Client uses ChangeKeyboardMapping to add PAUSE to the keycode that
already contains SCROLL LOCK.
.Ip
``Please paint PAUSE on the SCROLL LOCK key.''
.Cp
Clients should not use ChangeKeyboardMapping.
.Ce
.LP
If a client succeeds in changing the keyboard mapping table,
all clients will receive MappingNotify(request=Keyboard) events.
There is no mechanism to avoid receiving these events.
.Cp
Clients receiving MappingNotify(request=Keyboard) events should
update any internal keycode translation tables they are using.
.Ce
.NH 2
The Modifier Mapping
.LP
X11 supports 8 modifier bits,  of which 3 are pre-assigned to Shift,
Lock and Control.
Each modifier bit is controlled by the state of a set of keys,
and these sets are specified in a table accessed by GetModifierMapping and
SetModifierMapping.
This table is a shared resource,  and requires conventions.
.LP
A client needing to use one of the pre-assigned modifiers should assume that
the modifier table has been set up correctly to control these modifiers.
The Lock modifier should be interpreted as Caps Lock or Shift Lock according
as the keycodes in its controlling set include XK_Caps_Lock or XK_Shift_Lock.
.Cp
Clients should determine the meaning of a modifier bit from the keysyms
being used to control it.
.Ce
.LP
A client needing to use an extra modifier,  for example META,  should:
.Ip
Scan the existing modifier mappings.
If it finds a modifier that contains a keycode whose set of keysyms
includes XK_Meta_L or XK_Meta_R,  it should use that modifier bit.
.Ip
If there is no existing modifier controlled by  XK_Meta_L or XK_Meta_R,
it should select an unused modifier bit (one with an empty controlling set)
and:
.RS
.Ip
If there is a keycode with XL_Meta_L in its set of keysyms,  add that
keycode to the set for the chosen modifier,  then
.Ip
if there is a keycode with XL_Meta_R in its set of keysyms,  add that
keycode to the set for the chosen modifier,  then
.Ip
if the controlling set is still empty,  interact with the user to
select one or more keys to be META.
.RE
.Ip
If there are no unused modifier bits,  ask the user to take corrective
action.
.Cp
Clients needing a modifier not currently in use should assign keycodes
carrying suitable keysyms to an unused modifier bit.
.Ce
.Cp
Clients assigning their own modifier bits should ask the user politely to
remove his or her hands from the key in question if their SetModifierMapping
request returns a Busy status.
.Ce
.LP
There is no good solution to the problem of reclaiming assignments
to the 5 non-pre-assigned modifiers when they are no longer being used.
.Cp
The user has to use
.B xmodmap
or some other utility to deassign obsolete modifier mappings by hand.
.Ce
.Pp
This is kind of low-tech.
.Pe
.LP
When a client succeeds in performing a SetModifierMapping,  all clients
will receive MappingNotify(request=Modifier) events.
There is no mechanism for preventing these events being received.
A client using one of the non-pre-assigned modifiers which receives
one of these events should do a GetModifierMapping
to discover the new mapping,  and if the modifier it is using has been
cleared it should re-install the modifier.
.LP
Note that GrabServer must be used to make the GetModifierMapping, SetModifierMapping pair
in these transactions atomic.
.bp
.NH
Resource Manager Conventions
.LP
This section has yet to be generated.
.if 0 \{
.IP
<XXX - I haven't started this part yet>
.IP
<XXX - xrdb>
\}
.NH
Conclusion
.LP
This document provides the protocol-level specification of the minimal
conventions needed to ensure that X11 clients can inter-operate properly.
Further documents are required:
.Ip
A \fIWindow and Session Manager Conventions Manual\fP to cover these
convention from the opposite point of view,  and to add extra conventions
of interest to window and session manager implementors.
.Ip
A addendum to the \fIXlib \- C Language X Interface\fP manual covering
the additional routines,  XIconify() would be an example,  needed to
ensure that adhering to these conventions is convenient for the C programmer.
.SH
Acknowledgements
.LP
David Rosenthal had overall architectural responsibility for the conventions
defined in this document, wrote most of the text, and edited the document, but
the development has been a communal effort.
The details were thrashed out in meetings
at the January 1988 MIT X Conference
and at the 1988 Summer Usenix conference,
and through months (and megabytes) of argument
on the
.I wmtalk
mail alias.
Thanks are due to everyone who contributed,
and especially to the following:
.Ip
For the Selection section,
Jerry Farrell,
Phil Karlton,
Loretta Guarino Reid,
Mark Manasse,
and Bob Scheifler.
.Ip
For the Cut-Buffer section,
Andrew Palay.
.Ip
For the Window and Session Manager sections,
Todd Brunhoff,
Ellis Cohen,
Jim Fulton,
Hania Gajewska,
Jordan Hubbard,
Kerry Kimbrough,
Audrey Ishizaki,
Matt Landau,
Mark Manasse,
Bob Scheifler,
Ralph Swick,
Mike Wexler,
and Glenn Widener.
.LP
Thanks are also due to those who contributed to the public review,
including
Gary Combs,
Errol Crary,
Nancy Cyprych,
John Diamant,
Clive Feather,
Burns Fisher,
Richard Greco,
Tim Greenwood,
Kee Hinckley,
Brian Holt,
John Interrante,
John Irwin,
Vania Joloboff,
John Laporta,
Ken Lee,
Stuart Marks,
Allan Mimms,
Colas Nahaboo,
Mark Patrick,
Steve Pitschke,
Brad Reed,
and John Thomas.
.bp
.SH
Appendix A:  Compatibility with Earlier Drafts
.LP
This appendix summarizes the incompatibilities between this and earlier drafts.
.SH
A1:  The R2 Draft
.LP
The 25 February 1988 draft that was distributed as part of X11R2 was clearly
labeled as such,  and many areas were explicitly labeled as liable to change.
Nevertheless,  in the revision work since then we have been very careful not
to introduce gratuitous incompatibility.
A far as possible,  we have tried to ensure that clients obeying the conventions
in the earlier draft would still work.
.LP
The areas in which incompatibilities have become necessary are:
.Ip
The use of property None in ConvertSelection requests is no longer allowed.
Owners receiving them are free to use the target atom as the property to
respond with,  which will work in most cases.
.Ip
The protocol for INCREMENTAL type properties as selection replies has changed,
and the name has been changed to INCR.
Selection requestors are free to implement the earlier protocol if they
receive properties of type INCREMENTAL.
.Ip
The protocol for INDIRECT type properties as selection replies has changed,
and the name has been changed to MULTIPLE.
Selection requestors are free to implement the earlier protocol if they
receive properties of type INDIRECT.
.Ip
The protocol for the special CLIPBOARD client has changed.
The earlier protocol is subject to race conditions,  and should not be used.
.Ip
The set of state values in WM_HINTS.initial_state has been reduced,
but the values that are still valid are unchanged.
Window managers should treat the other values sensibly.
.Ip
The methods an application uses to change the state of its top-level window
have changed,  but in such a way that cases that used to work will still
work.
.Ip
The ``x'',  ``y'',  ``width'', and ``height'' fields have been removed from
the WM_NORMAL_HINTS property,  and replaced by pad fields.
Values set into these fields will be ignored.
The position and size of the window should be set by setting the appropriate
window attributes.
.Ip
A pair of ``base'' fields and a ``win_gravity'' field
have been added to the WM_NORMAL_HINTS property.
Window managers will assume values for these fields if the client
sets a short property.
.SH
A2:  The 27\s-2\uth\d\s0 July 1988 Draft
.LP
The Consortium review was based on a draft dated 27\s-2\uth\d\s0 July 1988.
Incompatibilities have been introduced in the following areas:
.Ip
The ``messages'' field of the WM_HINTS property was found to be unwieldy
and difficult to evolve.
It has been replaced by the WM_PROTOCOLS property,
but clients using the earlier mechanism can be detected because they set the
``messages'' bit in the flags field of the WM_HINTS property and window
managers can provide a backwards-compatibility mode.
.Ip
The mechanism described in the earlier draft by which clients installed
their own sub-window colormaps could not be made to work reliably,
and mandated some features of the look-&-feel.
It has been replaced by the WM_COLORMAP_WINDOWS property.
Clients using the earlier mechanism can be detected by the WM_COLORMAPS
property they set on their top-level window,
but providing a reliable backwards compatibility mode is not possible.
.Ip
The recommendations for window manager treatment of top-level window
borders have been changed as those in the earlier draft produced problems
with Visibility events.
For non-window-manager clients,
there is no incompatibility.
.Ip
The pseudo-root facility in the earlier draft has been removed.
Although it has been successfully implemented,
it turns out to be inadequate to support the uses envisaged.
An extension will be required to support these uses fully,
and it was felt that the maximum freedom should be left to the designers
of the extension.
In general,
the previous mechanism was invisible to clients and no incompatibility
should result.
.Ip
The addition of the WM_DELETE_WINDOW protocol (which prevents the danger
that multi-window clients may be terminated unexpectedly)
has meant some changes in the WM_SAVE_YOURSELF protocol,
to ensure that the two protocols are orthogonal.
Clients using the earlier protocol can be detected (see WM_PROTOCOLS above)
and supported in a backwards-compatibility mode.
.Ip
The conventions in Section 9.2.2. of the Xlib manual regarding properties
of type RGB_COLOR_MAP have been changed,  but clients using the earlier
conventions can be detected because their properties are 4 bytes shorter.
These clients will work correctly if the server supports only a single
Visual,  or if they use only the Visual of the root.
These are the only cases in which they would have worked,  anyway.
.SH
A3: The Public Review Drafts
.LP
The public review resulted in a set of mostly editorial changes.
The changes that introduced some degree of incompatibility are:
.Ip
A new section (6.3) was added covering the window manager's
use of Grabs.
The restrictions it imposes should affect only window managers.
.Ip
The TARGETS selection target has been clarified,
and it may be necessary for clients to add some entries to their
replies.
.Ip
A selection owner using INCR transfer should no longer replace targets in
a MULTIPLE property with the atom INCR.
.Ip
The contents of the ClientMessage sent by a client to iconify itself
has been clarified,
but there should be no incompatibility since the earlier contents
would not in fact have worked.
.Ip
The border-width in synthetic ConfigureNotify events is now specified,
but this should not cause any incompatibility.
.Ip
Clients are now asked to set a border-width on all ConfigureWindow
requests.
.Ip
Window manager properties on icon windows will now  be ignored,
but there should be no incompatibility since there was no specification
that they be obeyed previously.
.Ip
The ordering of real and synthetic ConfigureNotify events is now specified,
but any incompatibility should affect only window managers.
.Ip
The semantics of WM_SAVE_YOURSELF have been clarified and restricted to
be a checkpoint operation only.
Clients which were using it as part of a shutdown sequence may need to
be modified,
especially if they were interacting with the user during the shutdown.
.Ip
A kill_id field has been added to RGB_COLOR_MAP properties.
Clients using earlier conventions can be detected by the size of their
RGB_COLOR_MAP properties,
and the cases that would have worked will still work.
.SH
Appendix B:  Suggested Protocol Revisions
.LP
During the development of these conventions,
a number of inadequacies have been discovered in the protocol.
They are summarized here as input to an eventual protocol revision
design process.
.Ip
There is no way for anyone to find out the last-change time of
a selection.  At the next protocol revision,  GetSelectionOwner
should be changed to return the last-change time as well as the owner.
.Ip
How does a client find out which selection atoms are valid?
.Ip
The protocol should be changed to return in response to a GetSelectionOwner
the timestamp used to acquire the selection.
.Ip
There would be no need for WM_TAKE_FOCUS if the FocusIn event contained
a timestamp and a previous-focus field.
This could avoid the potential race condition.
There is space in the event for this information;
it should be added at the next protocol revision.
.Ip
There is a race condition in InstallColormap;  the request doesn't
take a timestamp,  and it may be executed after the top-level
colormap has been uninstalled.
The next protocol revision should provide the timestamp in
InstallColormap,
UninstallColormap,
ListInstalledColormaps,
and the ColormapNotify event.
The timestamp should be used in a similar way to the last-focus-change
time for the input focus.
.Ip
The protocol needs to be changed to provide some way of identifying
the Visual and the Screen of a Colormap.
.Ip
There should be some way to reclaim assignments to the 5 non-pre-assigned
modifiers when they are no longer needed.
.sp
