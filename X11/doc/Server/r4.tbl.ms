.\" $XConsortium: r4.tbl.ms,v 1.2 89/12/21 22:18:51 rws Exp $
.EF 'Release 4 major changes'- % -'December 4, 1989'
.OF 'Release 4 major changes'- % -'December 4, 1989'
.EH ''''
.OH ''''
.TL
Major changes in Release 4 of
the X v11 Sample Server
.AU
Keith Packard
.AI
MIT X Consortium
.LP
This is an attempted summary of major changes to the sample server from
release 3 to release 4.  It does not attempt to describe the design process
by which the various changes were reached, and does not describe the full
scope of the changes.  This document is intended as an aid to understanding
what has changed while reading the code.  To paraphrase the immortal words
of Obi-wan Kenobi:
.sp
.ce
"Use the Source, Luke"

.NH 1
Region Structure Changes
.XS
Region Structure Changes
.XE
.LP
Regions are still composed of two parts, however the data has been
reorganized.  The first part (RegionRec) now contains only the
extents and a pointer to the second part.  Regions which consist of
a single rectangle have a Null second pointer, regions which are
empty have a second pointer which is set to a static piece of memory
describing an empty region.  The second part of the region is now a
structure indicating the size and number-in-use concatenated with
the list of rectangles.  The rectangles are not explicitly declared
in the RegDataRec, but are implicitly allocated just after the
structure.  This makes it more difficult to debug region code as the
debugger will not be able to directly display the rectangle list.

Some new macros were added to make region accessing easier
with the new structures:

.TS
center;
l l.
REGION_NIL(reg)	true only for empty regions
REGION_NUM_RECTS(reg)	returns number of boxes in list
REGION_RECTS(reg)	returns pointer to first box
.TE

To make some window operations substantially faster (and to make the
interface to the Shape extension nicer), some additional region
operations were added:

.TS
center;
l l.
Bool (* RegionAppend)();	/* pRegion, pRegion */
Bool (* RegionValidate)();	/* pRegion, pOverlap */
RegionPtr (* BitmapToRegion)();	/* PixmapPtr */
RegionPtr (* RectsToRegion)();	/* nrects, pRects, ordering */
.TE

RegionAppend does a "union" operation, except that the resulting
region is not useful (e.g. is not YXBanded in the sample
implementation) in any context except either another RegionAppend or
a RegionValidate (which converts the "illegal" region into a "legal"
region).

BitmapToRegion converts a pixmap into a region, areas filled with
1's are inside, 0's are outside.  mfbPixmapToRegion provides a
sample implementation.

RectsToRegion converts a list of xRect structures into a
region, using the same interface that miRectsToRegion
already supplied to DDX layers.

Compile miregion.c -DDEBUG and it will validate regions
at every operation, checking banding and extents.  -DDEBUG
also makes the routines miRegionsEqual and miPrintRegion
available for debugging purposes.
.NH 1
Window Structure Changes
.XS
Window Structure Changes
.XE
.LP
Window structure values whose values are  have common cases moved to an optional record.
This includes values which are commonly inherited from the
parent window (visual, colormap), and values which are not
frequently used (backingBitPlanes, backingPixel).  Macros
have been added for all of the affected elements:
.nf

    wVisual(w)
    wCursor(w)
    wColormap(w)
    wDontPropagateMask(w)
    wOtherEventMasks(w)
    wOtherClients(w)
    wPassiveGrabs(w)
    wUserProps(w)
    wBackingBitPlanes(w)
    wBackingPixel(w)
    wBoundingShape(w)
    wClipShape(w)
    wClient(w)

.fi
And one macro because the type of this entry changed from
int to unsigned short and I got tired of typing
((int) w->borderWidth):
.nf

    wBorderWidth(w)

.fi
Functions removed from window and moved to screen.  All of
the old graphics functions for drawing to a window have been
moved from the window structure into the screen.  This means
that ChangeWindowAttributes no longer stores per-window
function accelerators for PaintWindowBackground/Border;
rather a general routine must be written which can accept any
possible painting style.

Valdata added.  This is a structure which contains the data
which are needed only during a validateTree operation.  When
the window is not being validated, this structure does not
exist.  The valdata member in the window subsumes the
functionality previously offered by the 'marked' member;
testing 'valdata' for non-null is equivalent to testing for
'marked'.

Regions in-lined.  Instead of having the window structure
pointing at the various region structures, they have been
inlined.  This saves the allocation overhead for the
individual regions, and saves the space previously occupied
by the pointers.  Careful here -- the compiler will not warn
you when you use these not-renamed members in the old
fashion.

DevPrivates added.  Any module in the server can request that
an arbitrary amount of storage be attached to every window
on any screen.  See the section below on devPrivates.

BackingStore state changed.  DIXSaveUnders have become a
part of the published DIX/DDX interface; a module which
implements BackingStore must also support DIXSaveUnders.
All routines which simply check if backing store is enabled
now use pWin->backStorage instead of using
pWin->backingStore.  This allows backing store to be changed
on any window without changing the state published to the
protocol.  Also, backing store functions are now per-screen
instead of per-window.  This reduces the amount of memory
per window used with backing store; it also eliminated the
backing store structure and turned backStorage into a
private pointer for use by the backing store module.

Shape Extension added.  This changes the semantics of
borders quite a bit; a new macro 'HasBorder' should be used
instead of testing pWin->borderWidth != 0.  The
implementation sets winSize and borderSize correctly, but
modules which care about window contents beyond the parent
borders must be careful when computing the appropriate
regions.  BackingStore is one such module.

ValidateTree interface changed.  Look at the code for more
details if you care.

absCorner moved from windows to Drawables.  This simplifies
quite a bit of mfb/cfb/mi code, but may make your life more
difficult.
.NH 1
Drawable Structure Changes
.XS
Drawable Structure Changes
.XE
.LP
Lots of fields which could be common between windows and
pixmaps have been moved in here.  Of note:

.TS
center;
c c
l l.
Field	Change
_
x, y	used to be only in windows as absCorner
width, height	used to be in both windows and pixmaps
class	used to be in both pixmaps and windows
bitsPerPixel	T{
added to help lots of operations
on machines where bitsPerPixel!=depth
T}
id	used to be only in windows
.TE

.fi
.NH 1
Pixmap Structure Changes
.XS
Pixmap Structure Changes
.XE
.LP
width and height were moved from here to the drawable.  Note
that the drawable also has x,y \(em when creating a pixmap
make sure that these values are set correctly (usually
0,0).  Although an effort was made to make mi always use
pDrawable->x,y for pixmaps, this code could not be tested;
if you want to set x,y to some random value, go ahead and
try it.  (mfb and cfb should also work correctly).

.NH 1
GC Structure Changes
.XS
GC Structure Changes
.XE
.LP
GC functions and operations moved from GC into separate structures.  This
allows clever DDX implementations to share these structures across multiple
GC's.  Doing this saves substantial amounts of memory over the previous
implementation; the function pointers to the various operations took 88
bytes.  Funcs are those entries which modify state inside the GC but perform
no graphics (ValdateGC, ChangeGC, ...).  Ops are those entries which change
no state in the GC but perform graphics (CopyArea, ...)

The old GC Interest queue has been removed in lieu of the
new wrapper scheme.  This new scheme is described in detail
below.

Default tiles are no longer created.  Rather, the pixel
value with which the default tile should be filled if it is
ever used is stored instead and the tileIsPixel bit set.  As
no client in existence had ever used the defaultTile, it
seemed like a good idea.  If a client should ever use the
defaultTile, ChangeGC will automatically create it; but it
normally doesn't exist (so don't rotate it).

DevPrivates were added to GC's as well; see the devPrivates
section.

.NH 1
Screen Structure Changes
.XS
Screen Structure Changes
.XE
.LP
Functions from windows and backing store merged in here.
Graphics functions for windows and the backing store
routines were merged into the screen to save memory
elsewhere.

DevPrivates added.  The interface for screens is slightly
different than that for GCs and windows; in particular,
AllocateScreenPrivateIndex may fail (due to out-of-memory)
and must be guarded against.

A new screen function was added to allow DDX's to set up the
source of a BitBlt operation before the copy occurs:
.nf

	pScreen->SourceValidate (pDrawable, srcx, srcy, width, height);

.fi
This routine should be called inside of CopyArea/CopyPlane before starting
the copy.  It is currently used by misprite to remove the cursor from
the screen when appropriate.  Do not call this function when the source and
destination drawables are the same; also do not call this function when it
is not set (the only screen proc which can be NULL).

The interface to GetSpans has changed.  It used to return an allocated block
of memory filled in with the data; now it is passed the block to use as
an additional argument.  This allows ddx/mi to reuse the same block of
memory for multiple GetSpans calls (which is quite frequent).
.NH
Out Of Memory Changes
.XS
Out Of Memory Changes
.XE
.LP
All memory allocation/deallocation should use the macros xalloc,
xrealloc, and xfree instead of the direct function interfaces, both
to get correct lint and to allow easy redefinition.  xalloc()
returns NULL on failure, but also returns NULL for zero bytes.
xrealloc() returns NULL on failure, but also returns NULL for zero
bytes.  When xrealloc fails, the original data is assumed to still
be OK.  ALLOCATE_LOCAL returns NULL on failure, but also returns
NULL for zero bytes.

Almost every allocation the server makes is now tested for failure.
Those that are not set the global Bool variable Must_have_memory to
TRUE before calling xalloc or xrealloc.  When this variable is not
set, the allocator is free to return NULL on allocation failure.
Uses of Must_have_memory should eventually be weeded from the
server; some of them are hard.

These changes were very pervasive and changed many interfaces
throughout the code.  Usually it was only necessary to change a void
function into a Bool function, returning TRUE on success, sometimes
this was not sufficient.

The most notable exception to the out-of-memory code are region
operations.  As the amount of state needed to recover from
recomputing clip-lists for a large hierarchy of windows is
frequently quite large, the server insists that all region
operations must succeed.  All the allocations within these
operations set Must_have_memory so the allocator can do the
appropriate thing.

One possibility is to reserve a chunk of memory for Must_have_memory
allocations and fail on other allocations while still having memory
free.

Note that most client applications will not live through many
allocation failures.

Most things are now completely reallocated on server reset.  This
includes screens, and root windows.

Functions whose interface has changed for out-of memory (this list
is not exhaustive)

.TS
center;
l l.
Function	Change
_
QueryTextExtents	returns success Bool
RegisterProc	returns success Bool
RegisterScreenProc	returns success Bool
(*pScreen->RegionCopy)	returns success Bool
(*pScreen->Intersect)	returns success Bool
(*pScreen->Union)	returns success Bool
(*pScreen->Subtract)	returns success Bool
(*pScreen->Inverse)	returns success Bool
(*pScreen->CreateColormap)	returns success Bool
AddResource	returns success Bool
MakeAtom	returns 0 on failure
GetScratchGC	returns NULL on failure
CreateGC	returns NULL on failure
CreatePixmap	returns NULL on failure
AddScreen	returns -1 on failure
.TE

.NH 1
Debugging Memory Allocation Troubles
.XS
Debugging Memory Allocation Troubles
.XE
.LP
New code was added to os/4.2bsd/utils.c ifdef'd DEBUG which turns
on a flock of debugging stuff.  The code still checks for
out-of-bounds storage and garbage frees, but additional hooks have
been put in to debug lost memory troubles and out-of-memory
conditions.

Each allocation is timestamped and at any time, using the debugger,
calling DumpMemorySince() will print out a list of memory allocated
since the last call to MarkMemoryTime() which have not been freed.
The variable MemoryAllocBreakpoint can be set to any time-stamp;
setting a debugger break point at line 475 in utils.c will
stop the system when that block of memory is being allocated.
.nf

    if (MemoryAllocTime == MemoryAllocBreakpoint)
	head->amount = amount;			    \(<- Stop here.

.fi
MemoryActive tracks the number of bytes currently in use by the
server (useful in addition to kernel numbers as fragmentation and
malloc overhead are ignored here).

Don't ship a production server with -DDEBUG turned on.

.NH 1
MFB Interface Changes
.XS
MFB Interface Changes
.XE
.LP
A couple of things here.  The interface to mfbScreenInit
changed; useless arguments deleted, useful arguments added.
The current interface is:
.nf

    mfbScreenInit(pScreen, pbits, xsize, ysize,
		  dpix, dpiy, width)
	ScreenPtr pScreen;
	pointer pbits; /* screen bits */
	int xsize, ysize; /* screen size in pixels */
	int dpix, dpiy; /* dots per inch */
	int width; /* pixel width */

.fi
The new argument 'width' gives the total width of the
frame buffer (including undisplayed data) in pixels.

Also, all of the colormap routines are now a part of
mfb (you don't need your own, you don't even need to
initialize them); the only routines you should initialize
are SaveScreen and the cursor routines.  Any other routine
you want to override should be stored *after* the
call to mfbScreenInit returns.

An additional routine has been created:
.nf

    mfbCreateDefColormap (pScreen)

.fi
Call this routine when the screen is completely set up and
ready for output.  It creates the default colormap,
initializes the two entries by using the values of
pScreen->whitePixel and pScreen->blackPixel and
installs it; so those need to be set.

No references to backing store need exist in any
client of mfb; mfb initializes backing store for
every screen automatically.

mfb uses the CloseScreen proc; if your device
needs to do something when closed, wrap this
function (see the section below on wrappers).  qvss
does this.  The same is true of all screen procs
not mentioned above.

.NH 1
CFB Interface Changes
.XS
CFB Interface Changes
.XE
.LP

cfbScreenInit is almost the same as mfbScreenInit.
A few more procs should be filled in by the device
to deal with pseudo color visuals:
.nf

    InstallColormap
    UninstallColormap
    ListInstalledColormap
    StoreColors
    
.fi
cfbScreenInit also initializes whitePixel and blackPixel
to zero; when cfbCreateDefColormap is called, it will
allocate whitePixel first (0) and blackPixel second (1).
If you don't like these numbers, fill in your own between
cfbScreenInit and cfbCreateDefColormap.

cfb also needs the CloseScreen proc, wrap it and
any others you need.

.NH 1
CFB Colormap Stuff
.XS
CFB Colormap Stuff
.XE
.LP
cfb now generates one of every possible kind of visual,
setting up colormaps appropriately.  You don't need
to do anything here, unless you don't like the
default visuals.  See cfbscrinit.c for the list
of default visuals.  Override this list by storing
your own after calling cfbScreenInit; before
calling cfbCreateDefColormap.

.NH 1
Zero line-width arc code
.XS
Zero line-width arc code
.XE
.LP
The performance of wide arcs is still insufferable; the expense of computing
(even approximately correct) X arcs has not changed.  But, new full-integer
algorithms have been developed which draw zero-width arcs.  Most of the code
is in mi, but cfb/mfb versions have also been written.
.nf

	void miZeroPolyArc(pDraw, pGC, narcs, parcs)

.fi
This is the most basic zero-width arc drawing routine.  It uses either PolyPoint or
FillSpans as appropriate.  Does dashed and solid arcs.
.nf

	Bool miZeroArcSetup (arc, info, ok360)
    	    register xArc *arc;
    	    register miZeroArcRec *info;
    	    Bool ok360;

.fi
This function sets up the difference equation coefficients and quadrant
masks to draw an arc; it is expected that DDX's use this routine and
device-specific pixelization code to draw solid arcs.  Both cfb and mfb have
code which uses this routine.  This routine returns TRUE if ok360 is TRUE
and if the arc spans the entire ellipse.

.NH 1
New wide-line code
.XS
New wide-line code
.XE
.LP
Mostly integer wide-line code is now included in ddx/mi.  The GC
LineHelper routine is no longer used; the old miMiter and miNonMiter
routines are now just stubs to avoid undefined values when linking.

Except for bevel joins, it is believed that this new code pixelates wide
lines in accordance with the protocol in all cases.

.NH 1
New filled-arc code
.XS
New filled-arc code
.XE
.LP
Full integer filled arc code exists in ddx/mi,mfb,cfb.  Like the
zero line-width arc code, it consists of an mi arc drawer, and a
setup routine which can be used from individual DDXs:
.nf

	void miPolyFillArc (pDraw, pGC, narcs, parcs)
		DrawablePtr pDraw;
		GCPtr pGC;
		int narcs;
		xArc *parcs;

	void miFillArcSetup (arc, info)
		xArc *arc;
		miFillArcRec *info;

.fi
Look at mfb/cfb for examples on using miFillArcSetup.

.NH 1
MI Software Cursors
.XS
MI Software Cursors
.XE
.LP
An entirely new implementation of software cursors is
included in R4.  It requires only minimal device
support:
.nf

    miDCInitialize (pScreen, pPointerCursorFuncs)
	ScreenPtr		pScreen;
	miPointerCursorFuncPtr	pPointerCursorFuncs;

.fi
miPointerCursorFuncRec is a structure containing
four pointers:
.nf

	long (*EventTime) (pScreen)
	    ScreenPtr pScreen;

	Bool (*CursorOffScreen) (ppScreen, px, py)
	    ScreenPtr	*ppScreen;
	    int		*px;
	    int		*py;

	void (*CrossScreen) (pScreen, entering)
	    ScreenPtr	*pScreen;
	    Bool	entering;

	void (*QueueEvent) (pxE, pPointer, pScreen)
	    xEvent	*pxE;
	    DevicePtr	pPointer;
	    ScreenPtr	pScreen;

.fi
EventTime returns the current event time, suitable for putting in an event.

CursorOffScreen returns TRUE if something useful happened when the cursor
goes off screen.  Must update *ppScreen and *px, *py to be reasonable when
returning TRUE.  returns FALSE if mi should keep the cursor on the current
screen.

CrossScreen is called when the cursor moves onto a new screen, entering is
TRUE for the new screen and FALSE for the old screen.

QueueEvent is called when the pointer is warped to enqueue the fake motion
event into the appropriate order in the input queue.  On devices with
shared-memory queues this may insert the new event into the queue; on other
devices it may just call
.nf

	    (*pPointer->processInputProc) (pxE, pPointer)

.fi
This is common enough that an mi routine, miPointerQueueEvent, was written;
use this if you don't care about exact event ordering.

The software cursor code was written in three distinct
layers, if you have some hardware support (like overlay
planes), you may be able to make use of some of the code
without using all of it.

.NH 1
Wrappers
.XS
Wrappers
.XE
.LP
This is the most pervasive change in the system; several
modules have needed functionality not provided in the
original sample server implementation:
.nf

	    Window interest queues.

	    more control over GC interest queues.

.fi
In particular, GC interest queues were not suitable for
either software cursors, or mi backing store; window interest
queues would eventually suffer the same fate.

Enter the "general" solution: wrappers.

A wrapper is simply a routine which has saved the previous
value for the function vector and calls it at the
appropriate point.  Every wrapper must "unwrap" before
calling down.

Screen wrappers are per-function.  That is, each
function can be wrapped separately.  For example,
to wrap CreateGC, mibstore does:
.nf

    miInitializeBackingStore (pScreen, funcs)

	...
	pScreenPriv->CreateGC = pScreen->CreateGC;
	pScreen->CreateGC = miBSCreateGC

	    static Bool
	    miBSCreateGC (pGC);
		GCPtr   pGC;
	    {
		miBSScreenPtr   pScreenPriv;
		Bool	    ret;
	    
		pScreenPriv = (miBSScreenPtr)
	    pScreen->devPrivates[miBSScreenIndex].ptr;

		pScreen->CreateGC = pScreenPriv->CreateGC;
	    
		...
	    
		ret = (*pScreen->CreateGC);
		...
	    
		pScreen->CreateGC = miBSCreateGC;
		return ret;
	    }

.fi
While the wrapped function is being called, the
screen should contain the wrapped function in the
function pointer.  This avoids troubles with recursive calls.

GC wrappers are much more complicated.  GC's have two
tables of functions; funcs and ops (see above on GC changes).
Either funcs or both funcs+ops can be wrapped.  To wrap
funcs, wrap the CreateGC screen proc; call down and then
store the func pointer in private storage and store the
wrapping func pointer in the GC.  All of the funcs
must be wrapped together; no partial solutions allowed.

As ValidateGC can be called from almost anywhere, the wrappers must
be very careful about maintaining the correct values for GC ops and
GC funcs. Some guide lines:

.IP 1
You can wrap Funcs without wrapping Ops.  Each time you
unwrap a Func, you must re-extract the Funcs from the GC
before re-wrapping:
.nf

	GC->funcs = wrapper->funcs
	...
	(*GC->funcs->func)(...);
	...
	wrapper->funcs = GC->funcs;
	GC->funcs = wrapping_funcs;

.fi
.IP 2
You cannot wrap Ops without wrapping Funcs.  Each time you
unwrap an Op, you must also unwrap Funcs, and re-extract
Ops before re-wrapping:
.nf

	GC->funcs = wrapper->funcs;
	GC->ops = wrapper->ops;
	...
	(*GC->ops->op)(...);
	...
	wrapper->ops = GC->ops;
	GC->ops = wrapping_ops;
	GC->funcs = wrapping_funcs;

.fi
The private storage used to save the wrapped functions can be allocated
automatically for your routine by using devPrivates (see the section on
devPrivates below).
.LP
To wrap the ops, wrap the funcs and, in each func
wrapper, unwrap the ops and funcs, call down, and
re-wrap.  In each op wrapper, unwrap both the
funcs and ops, call down and rewrap afterwards

The rule is:  if you wrap funcs+ops, you must
always unwrap both before down calling.  if you
wrap ops, you must always pull the ops value
out of the GC in the func wrapper and save it.  If
you wrap funcs, you needn't pull the funcs value
out of the GC to rewrap.

In this way, the wrapped validation routine can
change the op vector and not have it lost when your
wrapper routine rewraps the GC.

The corollary to this rule is:

Never change the func vector after CreateGC.  No
op can change either any op or any func.

There are two good examples of modules which do
different kinds of wrapping, mibstore and misprite.
Examine them closely.

.NH
Null function vectors disallowed
.XS
Null function vectors disallowed
.XE
.LP
To avoid disgusting complexity checking for null
function pointers in all of the possible wrapper routines,
null vectors are disallowed in screen funcs, gc ops and funcs
and window funcs.

.NH
devPrivates allocation
.XS
devPrivates allocation
.XE
.LP
To allow an arbitrary module to hang data from various server items
this scheme has been developed.  Essentially it allows non-dix code
to have dix pre-allocate space in screens, windows and gcs for
whatever the module needs.  The major reason for this is to allow
DIX to allocate all of the necessary memory in one chunk instead of
having each layer add a little piece onto the window (fragmenting
memory and slowing the process down)
.IP
Five routines were added to the DIX interface:

    int AllocateScreenPrivateIndex ()
.IP
Returns an index into every screen's devPrivates array which
can hold private data.  This routine returns -1 if it
couldn't reallocate all of the screens devPrivate arrays.
This should be called before creating any windows or gcs.

    int AllocateWindowPrivateIndex ()

.IP
Returns an index which will be a valid index for every
window's devPrivates array.  This must be called before any
windows are allocated (otherwise, some windows would not
have the array allocated correctly).  Note that this
allocation is *not* per-screen; every window in the system
is affected.  When the server is reset, this index must be
reallocated.  See the section on serverGeneration.

    Bool AllocateWindowPrivate (pScreen, index, amount)

.IP
For every window created on pScreen, amount bytes of data
will be pointed to by pWin->devPrivates[index].ptr.  If this
routine is *not* called, pWin->devPrivates[index].ptr will
be initialized to NULL.  This routine returns FALSE if it
ran out of memory reallocating some data structures in the
screen.  Everytime the server is reset, this allocation
request is forgotten and must be re-requested.

    int AllocateGCPrivateIndex ()

.IP
Just like AllocateWindowPrivateIndex, except for GC's

    Bool AllocateGCPrivate (pScreen, index, amount)

.IP
Again, just like AllocateWindowPrivate, except for GC's.
.LP
There are two common strategies for using devPrivates.  Typical
output layers need some storage on every window (for cached pixmaps
and other things) and GC (for composite clip lists, etc.) and will
use AllocateWindowPrivateIndex and AllocateWindowPrivate:
.nf

	mfbScreenInit (...)
	{
    	    ...
    	    if (serverGeneration != mfbGeneration)
    	    {
		mfbWindowPrivateIndex = AllocateWindowPrivateIndex();
		mfbGCPrivateIndex = AllocateGCPrivateIndex ();
		mfbGeneration = serverGeneration;
    	    }
    	    AllocateWindowPrivate (pScreen, mfbWindowPrivateIndex,
			    	    sizeof (mfbPrivWin));
    	    AllocateGCPrivate (pScreen, mfbGCPrivateIndex,
			    	    sizeof (mfbPrivGC));
    	    ...
	}
	
.fi
(error checking elided for clarity).  In this way, every screen
which is initialized will get a "piece of the action".

.NH 1
Cursor changes
.XS
Cursor changes
.XE
.LP
A structure "CursorBits" has been created which can be shared
by multiple cursors which use the same glyph data.  This saves
a bit of memory but changes the way in which cursor bits are
extracted from the cursor structure passed to RealizeCursor; the
change is editorial only:
.nf

	pCursor->width  ->   pCursor->bits->width
	...

.fi
Unfortunately, each cursor is still realized separately which usually means
that the bits are stored by the DDX in preparation for loading into the
hardware.  DDX can recognise this case, but must do appropriate things about
cursor color changes.  misprite does this correctly.

RGB values in the cursor structures are now unsigned short to
conform to the rest of the server.

.NH 1
Extensions interface changes
.XS
Extensions interface changes
.XE
.LP
For extensions which wish to live under multiple names, use
AddExtensionAlias().

Added additional extension proc which extracts the minor opcode from
a request packet; this is passed as an additional argument to
AddExtension.  DIX defines "StandardMinorOpcode" which can be used
by extensions which use the second byte of the packet to contain the
minor opcode.

Various byte swapping macros moved to misc.h for extension use.

.NH 1
Synchronous grab handing rewritten.
.XS
Synchronous grab handing rewritten.
.XE
.LP
After several long (and often heated) discussions, a reasonable
interpretation of the protocol has resulted in new replay
logic.  If you care about the changes, ask us.

.NH 1
Resource types reworked.
.XS
Resource types reworked.
.XE
.LP
The sample server used to have both resource types and
resource class numbers.  Resource types were 16 bit bitmasks,
resource classes were 16 bit numbers.  Because resource types
were limited in number, the PEX extension ended up using
a single resource type and 30 resource classes.  This was
silly (sort of defeats the purpose of separating type and
class if you use them backwards).

Resource types are now integer values starting at 1.  Get
a resource type by calling
.nf

    RESTYPE CreateNewResourceType(deleteFunc)

.fi
deleteFunc will be called to destroy all resources with this
type.

Resource classes are now masks starting at 1 << 31 which can
be or'ed with any resource type to provide attributes for the
type.  To allocate a new class bit, call
.nf

    RESTYPE CreateNewResourceClass()

.fi
There are now two ways of looking up resources, by type or 
by class.  Classes are non-exclusive subsets of the space of
all resources, so you can lookup the union of multiple classes.
(RC_ANY is the union of all classes).

The pre-defined resource types and classes are now:

.TS
center;
l l.
RC_CACHED	set on resources stored in dix cache
RC_DRAWABLE	set on all drawable objects (pixmaps, windows, ...)
RT_WINDOW	((RESTYPE )1 | RC_CACHED | RC_DRAWABLE)
RT_PIXMAP	((RESTYPE) 2 | RC_CACHED | RC_DRAWABLE)
RT_GC	((RESTYPE) 3 | RC_CACHED)
RT_FONT	((RESTYPE) 4)
RT_CURSOR	((RESTYPE) 5)
RT_COLORMAP	((RESTYPE) 6)
RT_CMAPENTRY	((RESTYPE) 7)
RT_OTHERCLIENT	((RESTYPE) 8)
RT_PASSIVEGRAB	((RESTYPE) 9)
.TE
(RT_VISUAL type removed, it is no longer needed by DIX)

Note that class bits must be or'ed into the value returned
by CreateNewResourceType when calling resource functions.

AddResource returns a Bool for failure.  If it fails, the destroy
proc will have been called.  The class and destroy-proc arguments to
AddResource have been dropped, since the type argument now subsumes
them.  FreeResource now takes a type instead of a class for skipping.

.NH 1
Visual Changes
.XS
Visual Changes
.XE
.LP
VisualRecs are no longer specific to a screen, the screen index has
been removed.  The bitsPerRGBValue, ColormapEntries, and nplanes
components have also been moved within the structure.  Beware, you
will need to fix up any static structure initializers for these in
your code.

The DepthRec has been changed as well, the depth component is now an
unsigned char and the numVids is now a short.  Static initializers
should still be OK.

.NH 1
New colormap routine for mi cursors
.XS
New colormap routine for mi cursors
.XE
.LP
FakeAllocColor added to aid software cursors.  The cursor "borrows"
two entries from the colormap to use in coloring the cursor; when
these two entries are stepped on by a client, the software cursor
dodges automatically.  If no free pixels are available,
FakeAllocColor returns the closest matching one.
.nf

    FakeAllocColor (pmap, pred, pgreen, pblue, pPix, read_only)
	ColormapPtr	pmap;
	unsigned short 	*pred, *pgreen, *pblue;
	Pixel		*pPix;
	Bool		*read_only;


.fi
pPix can be set as a hint in searching in a reasonable direction.
(i.e. from the top of the colormap).

This routine works with all visual types.
.NH 1
OS interface changes.
.XS
OS interface changes.
.XE
.LP
To support clients which take a long time to start up, connection
establishment is now done partly in WaitForSomething and then
on-the-fly with "magic" requests rather than waiting for
the entire setup to finish in WaitForSomething.  New OS
interfaces:
.nf

    InsertFakeRequest (client, data, count)
	ClientPtr	client;
	char		*data;
	int		count;

.fi
Splices "data" into the input stream for "client" as the
next request.  Used by dix to create fake requests for
client startup.
.nf

    ResetCurrentRequest (client)
	ClientPtr	client;

.fi
Reexecute the current request in the input buffer
(i.e. set up so that ReadRequestFromClient will
return the same request as it did last time)

ReadRequestFromClient interface has changed, now
taking only a client and returning the number of bytes available
in the client's structure.

Oops() has been eliminated, use SendErrorToClient instead.

clientsDoomed and GivingUp global variables have been folded into
a new dispatchException flags word containing (currently) two
flags:

.TS
center;
l l.
DE_RESET	server is reseting
DE_TERMINATE	server is exiting
.TE

.NH 1
Command line changes
.XS
Command line changes
.XE
.LP
Some changes to the server corrected bugs which were exploited by existing
programs (R3 xterm, for example).  A new variable, 'permitOldBugs', is
settable from the command line to retain bug-for-bug compatibility.  More
things will probably depend on this variable in the future.  This is also
settable using xset, which uses a new extension "MIT-SUNDRY-NONSTANDARD".

defaultColorVisualClass global added which sets the default
root window visual class (for cfb servers).  Accepts a
small integer which is the protocol encoding value for the
visual class.

monitorResolution global added which overrides the default device
resolution.  Useful when the graphics board has no clue about the
size of monitor attached.

.NH 1
Authorization interface added
.XS
Authorization interface added
.XE
.LP
To support MIT-MAGIC-COOKIE-1 style authorization (and probably
others), new interfaces were designed:
.nf

    InitAuthorization (file_name)
	char	*file_name;

.fi
Uses the Xau library to read in authorization-type specific
data from the specified file.
.nf

    XID
    CheckAuthorization (name_length, name, data_length, data)
	unsigned short	name_length;
	char	*name;
	unsigned short	data_length;
	char	*data;

.fi
This scans through the set of installed authorization types
looking for one with a matching name and calls the
authorization-type specific routine to validate the data.

Each active authorization entry is assigned a capability number;
every client which connects to the server using a given
authorization file element is tagged with the capability id.
Some authorization schemes may use this to revoke client access
or control client activity based on the capability number for
the client.

A formal interface is provided for each authorization type:
.nf

    (*Add) (data_len, data, ID)

.fi
.IP
Enables a new authorization data with the specified
capability number.
.nf

    XID (*Check) (data_len, data)

.fi
.IP
validate the connection setup data against the active authorization
entries.  Returns the capability index of the authorization file
data which permitted access; -1 on authorization denied.
.nf

    (*Reset) ()

.fi
.IP
Delete all active authorization entries for server reset.
.nf

    (*ToID) (data_len, data)

.fi
.IP
Returns the capability index of the authorization file data which
would be used to validate access; -1 on no match.
.nf

    Bool (*FromID) (id, data_lenp, datap)

.fi
.IP
Finds the authorization file data which matches the given ID,
returning FALSE if no match is made.
.nf

    (*Remove) (data_len, data)

.fi
.IP
The simple inverse of Add.  Some authorization schemes
may use this routine to revoke access made by clients
using this authorization data.
.LP
The vector of routines is installed in os/4.2bsd/auth.c and
is currently not dynamically configurable.

.NH 1
Server notifies parent when ready to accept connections.
.XS
Server notifies parent when ready to accept connections.
.XE
.LP
After the server initializes the network connection sockets,
it sends a SIGUSR1 to it's parent process if SIGUSR1 was
inherited as SIG_IGN from exec.  Although it seems dangerous
to send a random signal to a essentially unknown process,
this scheme is relatively safe as processes which pass SIG_IGN
on to the child process either know about this protocol, or
have SIGUSR1 set to SIG_IGN themselves.
.LP
Xdm and xinit have been modified to recognize this signal and
initiate the first connection when it is received.  In the
absence of the signal, both clients wait a "reasonable" time
(xdm: 15 seconds) and attempt the connection.
.NH 1
Structure elements which are now unsigned
.XS
Structure elements which are now unsigned
.XE
.LP
Structure elements which have changed from signed to unsigned types
will bite you because of C's poor rules on type promotion.  Make
sure you *always* cast these members to (int) before doing
arithmetic with them.  Many other members have also changed
to unsigned, but are not usually used in arithmetic operations.

.TS
center;
c c
l l l.
Object	Fields
Cursor	width, height, xhot, yhot
Drawable	width, height	(pixmaps and windows both used int)
Window	borderWidth
.TE

.NH 1
Necessary DDX Changes
.XS
Necessary DDX Changes
.XE

.LP
Update GC stuff:
.IP
\(bu Make it use op and vectors.
.br
\(bu Change references to pWin->clipList to &pWin->clipList
.br
\(bu Don't rotate non-existent tiles.
.br
\(bu Use GC devPrivates to store ddx-specific data.
.br
\(bu Eliminate Interest queue stuff.
.br
\(bu Use statically allocated regions where possible.
.LP
Update ChangeWindowAttributes:
.IP
\(bu remove function pointer stores.
.br
\(bu write new PaintWindow routines.
.LP
Update CopyWindow
.IP
\(bu to use &pWin->borderClip instead of pWin->borderClip
.LP	
Update all graphics routines:
.IP
\(bu use pDrawable->x, y instead of pWin->absCorner.x, y,
.br
\(bu use pDrawable->width, height instead of pWin->clientWinSize or
pPixmap->width,height.
.LP
Update all cursor code to indirect through the bits pointer
to get shared cursor bits.
.LP
Update all region users to use the new accessor macros.
.LP
Remove RT_VISUAL type allocation for visual data; visuals
are found by searching the screen now, instead of LookupID.
.NH 1
Server tuning parameters
.XS
Server tuning parameters
.XE
.LP
ddx/mi, ddx/mfb and ddx/cfb now utilize some hand-tuned parameters to choose
appropriate algorithms for some operations.  Cfb and mfb have been
extensively tuned for both RISC machines and CISC; unfortunately the choice
of code frequently depends on the capabilities of the native instruction
set.  ddx/mi has one tuning parameter for misprite which depends on the ddx
text implementation.  All of these parameters are currently defined in
server/include/servermd.h.
.IP
.ti 0
AVOID_MEMORY_READ
.br
For machines which can execute more than 3 or 4 instructions in a
memory-cycle time, and which have at least a 1-deep write buffer (2 deep is
better), this define causes the 8 bit cfb transparent stippling code
(including PolyText) to not read longwords from the frame buffer.  Instead,
several instructions are executed to choose the appropriate sequence of
byte/halfword/word writes.  This causes a 30% speedup on an R2000 in
PolyText speed, R3000 will see a more dramatic speedup.  If assembly code
were written for PolyText, the speedup would be ~70%.  The SPARC saw about a
15-20% speedup.  The 68020 saw about a 40% slowdown.
.IP
.ti 0
FAST_CONSTANT_OFFSET_MODE
.br
For machines which can embed a small constant offset into a load
instruction.  This define causes the BitBlt code to use constant offsets
instead of using auto-increment mode.  Speedups for using this scheme are
about 70% on the R2000 (the constant-offset-mode code is heavily tuned for
the R2000).
.IP
.ti 0
LARGE_INSTRUCTION_CACHE
.br
This simply increases the amount some loops are unrolled; the 68020 has a
256 byte instruction cache, so some of the code has been sized to fit this
cache.  Machines with larger caches (SPARC, MIPS, ...), or machines without
instruction caches (vaxstation 2000) may want to try this parameter
.IP
.ti 0
AVOID_GLYPHBLT
.br
miSprite computes bounding boxes for text to avoid removing the cursor when
it is close to the op; this routine uses GetGlyphs to create an array of
charinfos and sums the widths.  As most DDX's only implement
PolyGlyphBlt/ImageGlyphBlt, miSprite usually calls these directly, instead
of going through the mi PolyText/ImageText ops.  If your server doesn't use
mi PolyText/ImageText and has a poor PolyGlyphBlt/ImageGlyphBlt define this
macro and miSprite will use PolyText/ImageText instead.  Currently this is
only used by the apollo.
