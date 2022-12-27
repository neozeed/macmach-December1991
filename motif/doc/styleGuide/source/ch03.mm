.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 2
.H 1 "Selection and Component Activation"
.iX "selection model"
.iX "models" "selection"
.iX "object-action selection model"
.iX "component activation"
The selection model determines how elements are selected from
a group of elements.
This chapter describes the selection models that are used by
the various components of an application.  It then describes the
component activation model, which determines how to act upon
the selected elements.
.P
OSF/Motif compliant applications use an object-action selection
model.
In an object-action selection model, the user first selects an object,
and then performs an action on it.
The object-action selection model mimics real life,
so it is a
familiar process to the user and increases the user's
sense of control over applications.
.P
Objects include not only recognizable objects like windows,
PushButtons, and List elements, but also
component elements
that are not always recognized as discrete objects,
like individual letters of a text file.
Applications can also treat a group of elements as a single element,
either in a contiguous range or as a noncontiguous group.
.P
.iX "selection model"
.iX "selection actions"
.iX "component activation"
Once users select an object, they can perform an action
on it by using the components available in the application, or
by using one of the selection actions, like cut, copy, and paste.
Using components to perform actions on a selection is called activation.
This chapter describes the following topics:
.BL
.LI
Selection models, which determine how element is a collection are
selected for later action.
.LI
Selection actions, which perform actions directly on a selection,
such as cut, copy and paste.
.LI
Component activation, which is the means of using components with
a mouse or keyboard.
.LE
.H 2 "Selection Models"
Selection is used to mark one or more of a group of elements
simply for highlighting the elements, or so they can be moved,
copied, or otherwise manipulated by the application.
.P
.iX "list-like collections"
.iX "collections" "list-like"
.iX "text-like collections"
.iX "collections" "text-like"
.iX "graphics-like collections"
.iX "collections" "graphics-like"
Groups of elements can be organized into the following three types of
collections:
.BL
.LI
List-like
.LI
Text-like
.LI
Graphics-like
.LE
.P
.iX "list-like collections"
.iX "collections" "list-like"
.iX "list-like collections"
In list-like collections, when the pointer or location cursor
are in the collection, they are considered to be on an element.
The notion of "on" depends upon the manipulation model presented
by the control.  For example, "on" may mean that the cursor
is within the bounds of the element, or it may mean that
the cursor is on the boundary of the element, or the element may have
handles, and "on" means that the cursor is on one of the handles.
In text-like collections, when the pointer or
location cursor are in the collection, they are considered
to be between two elements.  The name text-like refers to
Text components in insert mode where the insertion cursor is
always between two letters.  
List-like and text-like collections are usually ordered
as linear collections.  That is, the collections are treated as 
if the elements were connected as a series of elements in
one dimension.
.P
.iX "graphics-like collections"
.iX "collections" "graphics-like"
In graphics-like collections, the pointer and location cursor
can be either on or between elements.  Graphics-like collections
are usually ordered as 2-dimensional collections.  That
is, the ordering of elements in the collection depends on
both the horizontal and vertical position of the element within
each collection.
.P
.iX "selection model"
.mS
OSF/Motif compliant systems must support
the five different selection models.
.mE
Each model is used where appropriate
in applications to limit the type of selections the
user can make.  For example, a List may limit selection to a single
element where the user is only allowed to make one choice. 
The five selection models are as follows:
.VL 25n
.mS
.LI "\fBSingle\ Selection\fP"
.iX "single selection"
.iX "selection" "single"
.mE
Used to select a single element in a collection.
Clicking
.B BSelect
on an element selects it and
deselects the previously selected element in the collection.
Single selection is described in Section 3.1.1
.mS
.LI "\fBBrowse\ Selection\fP"
.iX "browse selections"
.iX "selection" "browse"
.mE
Used to allow browsing through single selection collections.
Browse selection is also used to select a single element of
a collection.  Browse selection works just like single selection,
but additionally allows the user to browse through the elements by dragging
.B BSelect
through the elements in the collection.
Browse selection highlights each element as it is traversed, and gives
the application an opportunity to provide information about
each element as it is highlighted.  Releasing
.B BSelect
on an element selects it and deselects the previously
selected element.
Browse selection is described in Section 3.1.2
.mS
.LI "\fBMultiple\ Selection\fP"
.iX "multiple selection"
.iX "selection" "multiple"
.mE
Used to select or deselect multiple elements of a collection.
Clicking
.B BSelect
on an unselected element adds that element to
the current selection.  Clicking
.B BSelect
on a selected element removes that element from the current selection.
Multiple selection is described in Section 3.1.3
.mS
.LI "\fBRange\ Selection\fP"
.iX "range selection"
.iX "selection" "range"
.mE
Used to select a contiguous range of elements in a collection.
Clicking
.B BSelect
on an element selects the single element, and deselects any
previous selection.
.B "BSelect Drag"
over a range of elements selects all the elements within
the range, and deselects any previous selection.
Range selection is described in Section 3.1.4
.mS
.LI "\fBDiscontiguous\ Selection\fP"
.iX "discontiguous selection"
.iX "selection" "discontiguous"
.mE
Used for selecting multiple discontiguous
ranges of elements in a collection.
Clicking or dragging
.B BSelect
operates just as for range selection.
Discontiguous selection also allows
.B BToggle
to be used to add or remove selection ranges.
Discontiguous selection is described in Section 3.1.5.
.LE
.P
.iX "selection model"
The choice of the selection model should be made on a collection
by collection basis.  Some components, like Text,
enforce a selection model.  Other components, like a List,
allow the application or the user to determine the selection model.
.P
Variants of these selection models may be needed for collections that
are especially dense, when objects are organized in layers, when
3-dimensional selection is needed, when objects are hierarchicly
organized, or in other situations not covered by this
.IR "Style Guide" .
Selection models for such cases should not deviate unnecessarily from
the standard selection models.
.P
In all selection models, the selected element or group of elements
should be visually highlighted in some way.
Highlighting the selection can be done in the following of ways:
.BL
.LI
Using any one of the location cursor mechanisms
.LI
Inverting the colors of the selection
.LI
Drawing a solid or dashed box around the elements of the selection
.LE
.P
The selection can also be a null selection.  That is, it is possible
for no items to be selected in a collection.  This commonly occurs in
text-like collections where the location cursor is between
two elements, or when the user has deselected all the elements. 
.H 3 "Mouse-Based Single Selection"
.iX "single selection"
.iX "selection" "single"
The single selection model is the simplest selection model.
.mS
In the single selection model, when
.B BSelect
is clicked in a selectable element that is deselected,
that element must be selected, and any other selection
in the collection must be deselected.
The selected element must have the location cursor.
.mE
.H 3 "Mouse-Based Browse Selection"
.iX "browse selections"
.iX "selection" "browse"
The browse selection model is very similar to the single
selection model.  In browse selection, like single selection,
only a single element is selected at a time.
.mS
In the browse selection model, when
.B BSelect
is released in a selectable element,
that element must be selected, and any other selection
in the collection must be deselected.
As
.B BSelect
is dragged through selectable elements,
each element under the pointer must be selected, and the
previously selected element must be deselected.
The selection must remain on the element where
.B BSelect
is released and the location cursor must be moved
there.
.mE
The browse selection model gives the application 
an opportunity to provide information about each
element as it is highlighted.
.H 3 "Mouse-Based Multiple Selection"
.iX "multiple selection"
.iX "selection" "multiple"
Multiple selection allows the user to select or deselect
multiple single elements of a collection.
.mS
Clicking
.B BSelect
on an unselected element must add that element to the
current selection in the collection.
Clicking
.B BSelect
on a selected element must remove that element from
the current selection in the collection.
Clicking
.B BSelect
on an element
must move the location cursor to that element.
.mE
.P
With multiple selection, the user can select any group
of elements in a collection, including discontiguous
groups, all the elements, or none of the elements.
Because multiple selection requires one mouse click
for each element selected, although a simple model,
it is not well-suited for selecting large groups of elements.
.H 3 "Mouse-Based Range Selection"
.iX "range selection"
.iX "selection" "range"
The range selection model allows the user to select
multiple contiguous elements of a collection.
The description of this selection model assumes
that the drag-and-drop model is not in use in the collection.
The drag-and-drop model and its effect on this selection
model are described in Section 3.2.1.
.P
.iX "anchor"
.mS
In the range selection model, when
.B BSelect
is pressed, all elements
in the collection must be deselected.
If
.B BSelect
is clicked on a selectable element, it must
be selected.
Pressing
.B BSelect
must also set an anchor on the element, or at the
position where
.B BSelect
was pressed.
.mE
The
.B anchor
is one of the endpoints of the
selected range.
.mS
As
.B BSelect
is dragged through the collection,
every element between the pointer
and the anchor must be included in the selection.
When
.B BSelect
is released, the anchor must not move,
all the elements between the anchor and the
release position must remain in the selection,
and the location cursor must be at the location
where
.B BSelect
is released.
.mE
.P
.iX "extension models"
The user can extend the range selection using
.BR BExtend .
.B BExtend
is usually bound to
.BR "MShift BSelect" .
.mS
When
.B BExtend
is pressed, the selection must be extended according
to one of the following extension models:
.mE
.VL 15n
.mS
.LI "\fBReselect\fP"
.iX "ReSelect"
The selection must extend to include all elements
between the anchor and the pointer.
When
.B BExtend
is released, the anchor must not move,
all the elements between the anchor and the
release position must remain in the selection,
and the location cursor must be on the last element
selected or at the position of the release for graphics-like
collections.
.mE
.mS
.LI "\fBEnlarge-Only\fP"
.iX "Enlarge-Only"
.mE
The selection can only be enlarged.
.mS
The selection must extend to include all elements
between the old selection and the pointer.
In the graphics-like collections, the extended range
must be determined by the smallest rectangle that encloses
the old selection and the pointer.
When
.B BExtend
is released, the position of the anchor is irrelevant,
all the elements selected must remain in the selection,
and the location cursor must be on the last element
selected or at the position of the release for graphics-like
collections.
.mE
.mS
.LI "\fBBalance-Beam\fP"
.iX "Balance-Beam"
.mE
A balance point is defined at the midpoint of the current
selection.
.mS
When
.B BExtend
is pressed on the opposite side of the balance point
from the anchor point,
this model must work exactly like the reselect model.
When
.B BExtend
is pressed on the same side of the balance point as the
anchor point, this model must move the anchor point to
the other side of the selection, and then must work
exactly like the reselect model.
.mE
In graphics-like
collections, balancing the anchor point is done
separately in each dimension.
.mS
When
.B BExtend
is released, all elements must remain in the selection,
the anchor must remain in its last position,
and the location cursor must be on the last element selected
or at the position of the release for graphics-like collections.
.mE
.LE
.P
Text-like collections should use the balance-beam model.
The extension model used by other collections depends on
the purpose of the collection.
.P
.iX "BToggle"
In the range selection model, clicking
.B BToggle
can move the location cursor to the position
of the pointer without affecting the current
selection.
.B BToggle
is usually bound to
.BR "MCtrl BSelect"
as shown in the following table.
.P
.TB "Range Selection Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Button@Default Bindings
_
BExtend@MShift BSelect
BToggle@MCtrl BSelect
.TE
.H 3 "Mouse-Based Discontiguous Selection"
.iX "discontiguous selection"
.iX "selection" "discontiguous"
The discontiguous selection mechanism allows the user
to make multiple simultaneous range selections.
It behaves very similar to the range selection model.
The description of this selection model assumes
that the drag-and-drop model is not in use in the collection.
The drag-and-drop model and its effect on this selection
model are described in Section 3.2.1.
.P
.iX "anchor"
The behavior of
.B BSelect
in the discontiguous selection model is exactly
the same as in the range selection model.
.mS
In the discontiguous selection model, when
.B BSelect
is pressed all elements
in the collection must be deselected.
If
.B BSelect
is clicked on a selectable element, it must be selected.
Pressing
.B BSelect
must also set an anchor on the element, or at the
position where
.B BSelect
was pressed.  As
.B BSelect
is dragged through the collection,
every element between the pointer
and the anchor must be included in the selection.
When
.B BSelect
is released, the anchor must not move,
all the elements between the anchor and the
release position must remain in the selection,
and the location cursor must be on the last
element selected.
After the anchor point is set with
.BR BSelect ,
.B BExtend
must work exactly as in the range selection model.
.mE
.P
.iX "BToggle"
.iX "toggle models"
.iX "extension models"
The primary difference between the range selection model and the
discontiguous selection model is the use of
.BR BToggle .
In the range
selection mode,
.B BToggle
is only used to move the position of the
location cursor without disrupting the current selection.  In the
discontiguous model,
.B BToggle
is used to toggle the selection state of
an element or a range of elements.
.P
If the current selection is null,
.B "BToggle Click"
can leave the anchor
alone and leave all elements deselected.
.mS
Otherwise, if
.B BToggle
is clicked, the anchor must move to that point, and if
.B BToggle
is clicked on an element, the selection state of that element must be
toggled, and the anchor must be placed on that element.  In any case,
the location cursor must move to the point at which the click occurs.
.mE
.P
In list-like collections,
.B "BToggle Drag"
places the anchor on the element at which
.B BToggle
is pressed and toggles the selection state of
that element.  In text and graphics-like collections,
.B "BToggle Drag"
leaves the anchor at the point at which
.B BToggle
is pressed, but never
on an element.  In any case, the location cursor is left at the point
at which
.B BToggle
is released, and the selection state of the elements
in the range dragged out by
.B BToggle
are toggled based on one of the following toggle models.
.VL 18n
.LI "\fBAnchor\ Toggle\fP"
.iX "anchor"
.iX "toggle" "anchor"
.iX "anchor toggle"
.mS
For collections where the anchor is on an element rather than
between elements, each element in the extended
range, regardless of its current selection state,
must be set to the selection state of the element with the
anchor.
.mE
That is, if the element with the anchor is selected,
all elements in the extended range are also selected;
if the element is unselected, all the elements in the extended
range are deselected.
.LI "\fBText\ Toggle\fP"
.iX "text" "toggle"
.iX "toggle" "text"
.mS
For collections where the anchor is always between two elements, as
in Text, each element in the extended range must be set to the
inverse of the selection state of the element within the extended range that is
immediately adjacent to the anchor at the time that
.B BToggle
was pressed.
.mE
.LI "\fBFull\ Toggle\fP"
.iX "full toggle"
.iX "toggle" "full"
selection mode,
.B BToggle
is only used to move the position of the
location cursor without disrupting thel the elements in the extended
range are deselected.
.LI "\fBText\ Toggle\fP"
.iX "text" "toggle"
.iX "toggle" "text"
.mS
For collections wher
.mS
The selection state of all elements in the extended range
must be toggled.
.mE
.LE
.P
.mS
If the anchor is on an element, anchor toggling must be used; otherwise
text or full toggling must be used.
.mE
.P
Once
.B BToggle
has been used to toggle a selection, the toggled range
may be extended by
.BR BExtend .
The extended range can be determined by
any of the extension models described in Section 3.1.4; that is,
Reselect, Enlarge-Only, or Balance-Beam.
.mS
Once the extended range
is determined, the selection state of elements added to the range must be
determined by the toggle model as described above.
.mE
If
.B BExtend
removes elements from the extended range, they should revert to their
state prior to the last use of
.BR BToggle .
.H 3 "Keyboard Selection"
.iX "selection" "keyboard"
.iX "normal mode"
.iX "add mode"
Selections can be made by using the keyboard as well as
the mouse.
There are two different keyboard selection models
available, normal mode and add mode.
.P
.iX "location cursor"
.iX "cursor" "location"
Normal mode is used for making simple contiguous selections
from the keyboard.
.mS
In normal mode, keyboard navigation must never leave the
location cursor disjoint from the current selection.  
.mE
In collections where the location cursor is on an element,
the element with the location cursor is ordinarily selected;
if it is not, pressing
.B KSelect
moves the selection to the cursored element.
.P
In normal mode, if the location cursor is disjointed from the selection,
editing or inserting at the location cursor should deselect the
selection and can move the anchor to the cursor location.
.P
Add mode is used for making discontiguous selections from the keyboard.
Even in collections that do not support discontiguous selections,
add mode allows the selection to be unaffected by keyboard
navigation.
.P
.mS
Browse selections must only support normal mode.
Single and multiple selections must only support add mode.
Range and discontiguous selections must support both modes.
The default mode for range and discontiguous selections
must be normal mode.
.B KAddMode
must switch between add mode and normal mode in
range and discontiguous selections.
When switching from normal to add mode, if
the cursored element is the only element selected in
the collection, it must be deselected.
Mouse-based selection must not change based on
the keyboard selection mode.
.mE
.P
.mS
In editable components, add mode is a temporary mode
that must be exited when an operation is performed on
the selection, or when the selection is deselected.
.mE
.P
The usual bindings of the virtual keys used in this
section are shown in the following table.
.P
.TB "Keyboard Selection Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KSelect@<Select>
@<Space>
@MCtrl <Space>
KExtend@MShift KSelect
KAddMode@MShift <F8>
.TE
.H 4 "Keyboard-Based Single Selection"
.iX "single selection"
.iX "selection" "single"
.mS
Collections using the single selection model must
use add mode.
In add mode, the navigation keys for the collection must move the
location cursor independent from the selected element.
If
.B KSelect
is pressed on an unselected element, it
must select the element with the location cursor,
and must deselect any previous selection in the collection.
If
.B KSelect
is pressed on a selected element, it
must deselect the element with the location cursor.
.mE
.H 4 "Keyboard-Based Browse Selection"
.iX "browse selections"
.iX "selection" "browse"
.mS
Collections using the browse selection model must
use normal mode.
.mE
The navigation keys move the location cursor and the selection.
If the application has deselected all elements
or if the cursor is left disjoint from the selection,
.B KSelect
selects the cursored element and deselects any other elements.
.H 4 "Keyboard-Based Multiple Selection"
.iX "multiple selection"
.iX "selection" "multiple"
.mS
Collections using the multiple selection model must
use add mode.
The navigation keys for the collection must move the
location cursor independent from the current selection.
.B KSelect
on an unselected element must add the element to the
current selection.
.B KSelect
on a selected element must remove the element from the
current selection.
.mE
.H 4 "Keyboard-Based Range Selection"
.iX "range selection"
.iX "selection" "range"
.iX "anchor"
.mS
Collections using the range selection model must
initially use normal mode.
.mE
This allows a keyboard-based
behavior similar to dragging with the mouse.
.P
.mS
In normal mode, navigation keys must move the location
cursor and deselect the current selection.
If the cursor is on an element, it must be selected.
The anchor must move with the location cursor.
However,
in text-like collections, a different model can be used
in which the anchor must instead remain at its current location,
except that, if the current selection is null,
the anchor must be moved to the location of the cursor prior to navigation.
.mE
.P
.mS
.B KSelect
must move the anchor to the cursor, deselect the current selection,
and, if the cursor is on an element, select the element.
Unless the anchor is on a deselected item,
.B KExtend
must extend the selection from the cursor to the anchor, based
on the extension model used by
.BR BExtend .
.mE
.P
.mS
Using
.B MShift
in conjunction with the navigation keys must extend the selection.
If the current selection is null, or the cursor is disjoint from it,
the selection must be deselected, and the anchor must be moved
to the cursor.  The cursor must then be moved according to the
navigation keys, and the selection must be extended based on the
extension model used by
.BR BExtend .
.mE
.P
.mS
In add mode,
.B KSelect
and
.B KExtend
must work exactly as in normal mode.  However, ordinary navigation
must simply move the location cursor, but must leave the
anchor unchanged.  Shifted navigation must move the location cursor
according to the navigation keys, and the selection must be extended
based on the extension model used by
.BR BExtend .
.mE
.H 4 "Keyboard-Based Discontiguous Selection"
.iX "discontiguous selection"
.iX "selection" "discontiguous"
.mS
Collections using the discontiguous selection model must
initially use normal mode.
.mE
In the discontiguous
selection model, add mode is not only a separate model of
interaction, but an extension to the model that allows
the user to select discontiguous elements.
.P
In normal mode, keyboard-based discontiguous selection
behaves identically to keyboard-based range selection.
.mS
In add mode,
.B KSelect
must move the anchor to the location cursor
and initiate toggling.  If it is
on an element, the selection state of that element must
be toggled, but the selection state of all other elements must
remain unchanged.
.B KExtend
and shifted navigation operations
must extend the selection between the
anchor and the location cursor using one
of the toggle mechanisms described in
Section 3.1.5.
.mE
.H 3 "Canceling a Selection"
.iX "selection" "canceling"
.iX "undo"
.iX "selection" "undoing"
.iX "canceling a selection"
.iX "KCancel"
.mS
.B KCancel
must cancel or undo any incomplete drag operation used for selection,
including selection dragging using the drag-and-drop
model described in Section 3.2.1.
Once
.B KCancel
is pressed to cancel a drag operation,
the application must ignore subsequent key and button releases
until after all buttons and keys are released.
.mE
.B KCancel
is usually bound to
.B <Escape>
or
.BR <Cancel> .
.H 3 "Selecting and Deselecting All Elements"
.iX "deselecting"
.iX "SelectAll"
.iX "selection" "deselecting"
.iX "KSelectAll"
.iX "KDeselectAll"
There are two special keyboard-based selection
mechanisms for selecting (via
.BR KSelectAll )
and deselecting (via
.BR KDeselectAll )
all the elements in a collection.
.mS
.B KSelectAll
in a collection using multiple,
range, or discontiguous selection,
must select all the elements in a collection,
place the anchor at the beginning of the
collection, and leave the location cursor at
its previous position.
.mE
.P
.iX "KSelectAll"
.iX "KDeSelectAll"
Deselection differs between add mode and normal mode.
.mS
In add mode,
.B KDeselectAll
must deselect all the elements in a collection.
In normal mode,
.B KDeselectAll
must deselect all the elements in a collection, except
the element with the location cursor if the location cursor
is being displayed.  In either case,
it must leave the location
cursor at its current location and move the anchor to the location
cursor.
.mE
An application can deselect all elements in a collection regarless
of the state of add mode or the selection model.
.P
.BR KSelectAll
and
.BR KDeselectAll
are usually bound as shown in the following table.
.P
.TB "KSelectAll and KDeselectAll Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KSelectAll@MCtrl </>
KDeselectAll@MCtrl <\e>
.TE
.H 3 "Using Mnemonics for Elements"
.iX "selection" "mnemonics"
.iX "mnemonics"
Collections can also support mnemonics associated
with its elements if the elements have labels.
.mS
Typing a mnemonic while
the collection has the keyboard focus must
be equivalent to moving the location cursor to the
element and typing
.BR KSelect .
.mE
.H 2 "Selection Actions"
.iX "drag-and-drop model"
.iX "primary selection"
.iX "quick transfer"
.iX "keyboard" "selection actions"
Once a selection is made, the elements of the
selection can be acted upon using selection
actions.
Some common selection actions are delete,
cut, copy, and paste, although your application
is not limited to these.
Selection actions should be available in editable
collections.
This section describes the following ways of using
selection actions.
.BL
.LI
The drag-and-drop model
.LI
Using primary selection
.LI
Using quick transfer
.LI
Using keyboard selection actions
.LE
.H 3 "The Drag-and-Drop Model"
.iX "dragging"
Some collections can support the drag-and-drop model.
The drag-and-drop model allows the user to move or
copy the selected elements by dragging
them to a new location.
.P
Collections can allow their elements to be dragged and
dropped within the same collection and different
collections.  When the dragged
elements are dropped outside of the original collection,
an appropriate representation of the dragged elements can
be moved or copied to the destination collection, if the
destination supports dropping of that type of element.
.P
The appropriate representation
of dropped elements is determined by the collection in
which the elements are dropped.  For example, if a graphic
element representing a file were dropped in a Text component,
it could be appropriate to paste the name or the contents
of the file into the Text component.
Even collections that do not support dragging of their own elements
can support drops of elements from other collections.
.P
If a component implements dragging, it should
use
.B BDrag
to perform the drag operations.  Some implementations
may choose to use
.B BSelect
instead of 
.B BDrag
because of limited mouse buttons, consistency with existing
interfaces, or user preference.
.mS
When
.B BSelect
is used for dragging operations, its use must
supersede the use of
.B "BSelect Drag"
in the selection models described in Section 3.1.
Applications that implement the drag-and-drop model
using
.B BSelect
must also support the use of
.BR BDrag
instead if it is available.
.mE
.P
.iX "quick transfer"
.mS
When using
.B BDrag
for dragging elements,
if
.B BDrag
is pressed on a selected element or within a linear range of
selected elements,
it must drag the entire selection until
it is released.
If
.B BDrag
is pressed outside of a selected range
in a text-like collection, it must do a quick transfer instead
of dragging the element.
.mE
Quick transfer is described in
Section 3.2.3.
If
.B BDrag
is pressed on an unselected element
in a list-like collection, it can either do a quick transfer
or a dragging operation.
The behavior of
.B "BDrag Press"
in a graphics-like collection is unspecified.  It can either
do a quick transfer, a dragging operation, or pan the visible
region.
.P
.iX "moving"
.iX "copying"
.iX "pasting"
A drag operation may either copy or move the selected
elements.
.mS
If the elements are copied, a copy or representation
of the elements must be pasted into the destination collection,
but the original elements must remain unchanged.  If the
original elements were selected, they must remain selected.
.mE
.P
.mS
If the elements are moved, a copy or representation of the
elements must be pasted into the destination collection,
and the original elements must be deleted.  If the
original elements were selected, the copy of the elements
must become selected if the destination supports selection.
.mE
.P
.iX "dragging"
The type of the elements dragged, and the source and destination
of the drag-and-drop operations determine whether the operation
causes a move or a copy.
The following rules can be user customizable.
.mS
If not customized
the rules that must be followed are as follows.
.mE
.BL
.LI
.mS
Dragging from a non editable component must be treated as a copy.
.mE
.LI
.mS
Dragging to a transport device, like a printer or mailbox,
must be treated as a copy.
.mE
.LI
.mS
Dragging to a removal device, like a wastebasket or shredder,
must be treated as a move.
.mE
.LI
.mS
Dragging text to or from a Text collection must be treated as a copy.
.mE
.LE
.P
.mS
In other cases the operation must be treated as a move.
Pressing
.B MCtrl
with the drag operation must make the operation a copy.
Pressing
.B MAlt
with the drag operation must make the operation a move.
.mE
.P
.iX "drag-and-drop model"
.iX "dragging"
.mS
When the drag-and-drop model is implemented using
.BR BSelect ,
.BR "BSelect Drag"
on a selected element or within a linear range of
selected elements
must initiate a drag operation on the entire selection.
.BR "BSelect Drag"
on an unselected element
must select the element, deselecting all other elements, and initiate a
drag operation.
.mE
.P
A component may use
.B BSelect
for drag-and-drop, but not
.BR BToggle .
.mS
If
.B BToggle
is used for drag-and-drop,
.B "BToggle Drag"
on a selected element must remove the element from the
selection.
.B "BToggle Drag"
on an unselected element must add the element to the
selection and initiate a drag operation on the entire selection.
Components that implement the drag-and-drop model using
.BR BToggle ,
must also support the use of
.B BDrag
instead.
.mE
.H 3 "Using Primary Selection"
.iX "primary selection"
.iX "primary transfer"
.iX "selection" "primary"
Each collection maintains its own selection.
.mS
However, there must only be a single primary selection.
.mE
The primary selection is the selection on which selection
actions, like cut and copy, are performed.
.mS
If more than one selection is highlighted,
the primary selection must be the selection that is
most visually highlighted.
.mE
The primary selection is
associated with the collection it is in, and it is
possible for the primary selection to not contain any elements.
In this case, the primary selection is said to be null.
.P
There are two models for how the primary selection moves
among collections:
.BL
.LI
.iX "primary selection" "focus-based"
.mS
Focus-based, where the primary selection must move to a collection
whenever the collection gets the input focus.
.mE
.LI
.mS
Selection-based, where the primary selection must move to an editable
collection whenever a nonnull selection is explicitly started
in that collection using either
.BR BSelect ,
.BR BExtend ,
.BR BToggle ,
.BR KSelect ,
.BR KExtend ,
.BR KSelectAll ,
or
.BR KReselect .
Clicking
.BR BSelect
or
.BR BToggle ,
but not on an element, must not move the primary selection to
that component.
.mE
The primary selection is only moved when
a nonnull selection is made by clicking
.BR BSelect
or
.BR BToggle .
When
.B BExtend
is clicked, an implementation can move
the primary selection to the component even if the
resulting selection is null.
.mS
Dragging
.BR BSelect ,
.BR BExtend ,
or
.BR BToggle
must move the primary selection to the component even if
no elements are ultimately selected.
.mE
.LE
.P
.iX "primary selection"
.iX "selection" "primary"
When a collection loses the primary selection, it should
remember what was selected, including
the elements contained in it
and the position of the anchor.
.mS
Each collection can provide
.BR KReselect ,
which must restore the collection's selection as the primary selection.
.mE
.B KReselect
is usually bound as shown in the following table.
.P
.ne 12
.TB "KReselect Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KReselect@MCtrl MShift KSelect
KSelect@<Select>
@<Space>
@MCtrl <Space>
.TE
.P
.iX "primary transfer"
The selection-based, primary selection model should be used
whenever possible because when using the focus-based model,
it is impossible to perform primary transfer between different components
for either
a mouse user with a pointer focus policy, or for a keyboard user.
Primary transfer is used to cut or copy
the primary selection to a new location without using the clipboard.
.mS
.P
Clicking
.B BPrimaryPaste
must paste the primary selection to the position of the pointer
and set the location cursor there.
.mE
The type of elements in the primary selection,
and the source and destination
of the primary selection determine whether
.B BPrimaryPaste
causes a move or a copy in the same manner as the drag-and-drop
model described in the previous section.
.P
.mS
Clicking
.B BPrimaryCopy
must copy the primary selection to the position of the pointer
and set the location cursor there.
Clicking
.B BPrimaryCut
must cut the primary selection, paste it at the position of
the pointer, and set the location cursor there.
.mE
.P
.iX "KPrimaryPaste"
.iX "KPrimaryCopy"
.iX "KPrimaryCut"
.mS
Keyboard-based primary transfer,
.BR KPrimaryPaste ,
.BR KPrimaryCopy ,
and
.BR KPrimaryCut ,
must behave like the mouse-based primary transfer except that
the primary selection is pasted at the position of the location
cursor instead of the pointer.
.mE
.P
If a collection has a fixed insertion point (as in, append-only
text), then primary transfer can paste the selection at that
point.
.P
Primary transfer is usually bound as shown in the following
table.
.P
.TB "Primary Transfer Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key or Button@Default Bindings
_
BPrimaryPaste@BDrag
BPrimaryCopy@MCtrl BPrimaryPaste
BPrimaryCut@MAlt BPrimaryPaste
KPrimaryPaste@<vendor defined>
KPrimaryCopy@MCtrl KPrimaryPaste
@MAlt KCopy
KPrimaryCut@MAlt KPrimaryPaste
@MAlt KCut
KCut@MShift KDelete
@<Cut>
KCopy@MShift KInsert
@<Copy>
KDelete@<Delete>
KInsert@<Insert>
.TE
.H 3 "Using Quick Transfer"
.iX "quick selection"
.iX "quick transfer"
.iX "selection" "quick"
.iX "selection model" "object-action"
.iX "object-action selection model"
Quick transfer is used to make a selection, and then
immediately cut or copy that selection to the
destination cursor.  In primary transfer, the user
makes a selection, then picks a destination.
In quick transfer, the user picks the destination first,
then makes a selection to be pasted there.  This allows
multiple selections to be quickly pasted at the same location.
Note that quick transfer does not follow the object-action
selection model.
.P
The
.B "destination cursor"
is the location where selections will be pasted.
.BL
.LI
.mS
In text-like collections, the selection
must be pasted in the space before the destination cursor.
.mE
.LI
.mS
In list-like collections, the selection must be pasted
before the element with the destination cursor.
.mE
.LI
In graphics-like
collections, the position at which the selection is pasted
and the resulting position of the destination cursor are
unspecified.
.LE
.P
The destination cursor does not need to be in the same
collection in which the quick transfer is invoked.
.mS
The destination cursor must be in the component in which
the user last did a selection, insertion, or editing
operation.
.mE
It can either be at the location cursor of that
component, or determined independently.
.P
.mS
If the destination cursor is determined independently, it
must be at the location where selection, insertion or editing
was last done.  Furthermore, when the current selection is null,
the anchor must be kept at the same location as the destination cursor.
.mE
The destination cursor should be determined independently in
environments in which strong support of keyboard use is expected.
.P
The position of the destination cursor should be visually marked,
typically by a caret,
whenever it is not in the collection with the location cursor,
or when it is in the same collection, but disjointed.
.P
.iX "quick transfer"
.iX "secondary selection mechanism"
.iX "selection" "secondary"
Quick transfer is generally used as a secondary selection mechanism.
.mS
That is, it must not disturb the primary selection or
affect the clipboard, except that
if the destination cursor is in or on the boundaries of the
primary selection, quick transfer must delete the contents
of the primary selection, leaving a null primary selection,
before pasting the transferred elements.
.mE
Deleting or replacing the contents of the primary selection
is called a pending delete.
.P
.iX "dragging"
.iX "BQuickCopy"
.iX "BQuickCutDrag"
With the mouse,
the quick transfer selection is made by dragging
.BR BQuickPaste .
.B BQuickPaste
is usually bound to
.BR BDrag .
Therefore,
quick transfer can be unavailable in collections that support
the drag-and-drop model.
.mS
.B "BQuickPaste Drag"
in a collection must select all the elements
between the place where
.B BQuickPaste
was pressed and released, and either cut or copy
them to the position of the destination cursor.
.mE
Whether the operation performs a cut or a copy is
determined by the same rules as the drag-and-drop model
described in Section 3.2.1.
.mS
.B "BQuickCopy Drag"
must perform the same action as
.BR BQuickPaste ,
but must force a copy operation.
.B "BQuickCut Drag"
must perform the same action as
.BR BQuickPaste ,
but must force a cut operation.
.mE
.P
.iX "KQuickPaste"
.iX "KQuickCopy"
.iX "KQuickCut"
.mS
With the keyboard,
.B KQuickPaste
must set the quick anchor point to start either
a copy or cut operation.
.mE
Whether the operation performs a cut or a copy is
determined by the same rules as the drag-and-drop model
described in Section 3.2.1.
.mS
.B KQuickCopy
must set the quick anchor point to start a copy
operation.
.B KQuickCut
must set the quick anchor point to start a cut
operation.
.B KQuickExtend
must perform the quick operation using every
element between the quick anchor point and the location
cursor.
.mE
.P
Quick transfer is usually bound as shown in the following
table.
.br
.ne 15
.P
.TB "Quick Transfer Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key or Button@Default Bindings
_
BQuickPaste@BDrag
BQuickCopy@MCtrl BQuickPaste
BQuickCut@MAlt BQuickPaste
KQuickPaste@<vendor defined>
KQuickCopy@MCtrl KQuickPaste
KQuickCut@MAlt KQuickPaste
KQuickExtend@MShift KQuickPaste
.TE
.P
Quick transfer can be used to transfer static text or graphics that are
not ordinarily selectable.  For example, a portion of the text label
of a PushButton is not ordinarily selectable, since
.B BSelect
or
.B KSelect
activate the PushButton instead.
We recommend that developers support quick transfer for all
static text and graphics.
.P
While dragging out the quick selection, the range should be highlighted
in some way.  The highlighting mechanism should be different than
that used for the primary selection.
In text, the highlight should be underlining.
The range
determined by the quick anchor and the location cursor should
be similarly highlighted during keyboard-based quick selection. 
.H 3 "Using Keyboard Clipboard Selection Actions and Deletion"
.iX "clipboard selection actions"
.iX "selection actions" "clipboard"
.iX "KBackSpace"
.iX "KDelete"
Mouse-based clipboard selection actions are usually performed
using the
.B Edit
Menu of an application.
In addition, special bindings are available for keyboard-based
clipboard selection.
.mS
The keyboard-based clipboard selection
actions must be available in every editable collection.
In list- and graphics-like collections,
.B KDelete
and
.B KBackSpace
must delete the selected elements.
In text-like collections,
.B KDelete
and
.B KBackSpace
behave as follows.
.BL
.LI
When the location cursor is in a selection,
.B KDelete
and
.B KBackSpace
must delete the selection.
.LI
When the selection is null,
.B KDelete
must delete one character forward and
.B KBackSpace
must delete one character backward.
.mE
.LI
When the selection is not null and the location cursor is
outside of the selection,
.B KDelete
can either delete the selection or delete one character forward
and
.B KBackSpace
can either delete the selection or delete one character backward.
.LE
.P
.iX "KCut"
.iX "KCopy"
.iX "KPaste"
.iX "KUndo"
.mS
.B KCut
must cut the selected elements to the clipboard.
.B KCopy
must copy the selected elements to the clipboard.
.B KPaste
must paste the contents of the clipboard at the position
of the location or destination cursor,
except if the collection has a fixed insertion point (such
as append-only text).
.mE
.B KUndo
should undo the last operation on a selection.  There can
be additional methods of undoing operations on selections.
.B KCopy
and
.B KUndo
can also be supported on non editable collections.
.P
The keyboard-based clipboard selection actions are usually
bound as shown in the following table.
.P
.TB "Clipboard Action Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KDelete@<Delete>
KCut@MShift <Delete>
KCopy@MCtrl <Insert>
KPaste@MShift <Insert>
KUndo@MAlt <BackSpace>
.TE
.P
.mS
Pasting in Text must place the pasted text before the
cursor.
.mE
In graphics, the placement of pasted graphics and
the final position of the cursor are specific to
the graphics component.
.P
.mS
Editable components must also support pending deletion.
.mE
In pending deletion, any insert, paste or editing
operation
first deletes the contents of the current selection.
.mS
Pending
deletion must occur when the location cursor is in or on
the boundaries of the selection.
.mE
.H 2 "Component Activation"
.iX "activation" "component"
.iX "component activation"
Components are used to send information to the underlying
application.  For example, a Text component is used to
send complicated information to the application, but
most components, for example, PushButtons are simply used to
start an application process.  In fact even a complicated
component like Text may need to tell
the application that the user is finished entering
information.  Components that start some application
process are used following the activation model,
which is divided into the following areas:
.BL
.LI
Basic activation
.LI
Accelerators
.LI
Mnemonics
.LI
Help activation
.LI
Default activation
.LI
Expert activation
.LI
Previewing and autorepeat
.LE
.H 3 "Basic Activation"
.iX "basic activation"
.iX "activation" "basic"
.iX "BSelect"
.iX "KActivate"
The basic activation model mimics real life button
activation in that pressing on a button activates it.
The user selects a button with the location cursor,
which can be moved among components following the
navigation model described in Chapter 2.
.BL
.LI
.mS
Clicking
.B BSelect
on the button must activate the button.
.mE
.LI
.mS
.B KActivate
on the button with the location cursor
must activate the button.
.mE
.LI
.mS
.B KSelect
on a button with the location cursor
must activate the button.
.mE
.LE
.P
.BR KActivate
and
.B KSelect
are usually bound as shown in the following table.
.P
.TB "Activation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KActivate@KEnter
KEnter@<Return> or <Enter>
@MCtrl KEnter
KSelect@<Select>
@<Space>
@MCtrl <Space>
.TE
.P
.mS
When
.B BSelect
is pressed over a button, the appearance of the button
must change to indicate that releasing
.B BSelect
activates the button.
If while
.B BSelect
is pressed the pointer is moved outside of the button,
the visual state must be restored.
If while
.B BSelect
is still pressed the pointer is moved back inside of the button,
the visual state must again be changed to indicate the pending
activation.
If
.B BSelect
is pressed and released within
a button, the button must be activated, regardless of whether
the pointer has moved out of the button while it was pressed.
.mE
.P
An implementation can allow
.BR "BSelect Click 2+"
(multiple mouse button clicks)
to be treated as a single mouse button click on a per
component basis.
In an application where double-clicks are used heavily,
this helps prevent the user from unintentionally activating
a button twice.
.P
Note that selection and editing components are also activatable,
but
.B KSelect
and
.B BSelect
are used to select elements within the component.
In selection and editing components, only
.B KActivate
and
.B "BSelect Click 2"
can be used to activate the component.
As a further restriction,
.B "BSelect Click 2"
in a Text component is used to select a word,
and in multiline Text components,
.BR KEnter ,
the primary binding for
.BR KActivate ,
is used to insert a carriage return.
.mS
.BR "MCtrl KEnter" ,
the other binding for
.BR KActivate ,
must always be available for activating Text components.
.mE
.H 3 "Accelerators"
.iX "accelerators"
An accelerator is a key or key combination that invokes
the action of some component regardless of the position
of the location cursor when the accelerator is pressed.
Accelerators are most commonly used to activate Menu items without
first posting the Menu.
.P
.mS
If the button with the accelerator is within a primary or
secondary window, or within a PullDown Menu system
from its MenuBar, then it must be activatable whenever the
input focus is in the window.  If the button with the
accelerator is within a Popup Menu system, then it must be
activatable whenever the focus is in the component with
the Popup Menu.
.mE
.P
Applications can provide accelerators for any button
component.
.mS
All PushButtons and ToggleButtons
must support accelerators in Menus.
If a button has an accelerator, the accelerator must
be shown following the label of the button.
.mE
.H 3 "Mnemonics"
.iX "mnemonics"
A mnemonic is a single character that can be associated with
any component that contains a text label.
.mS
The label must contain
the character, and the character
must be underlined within the label.  If a label does not naturally
contain the character of the mnemonic, the mnemonic must be placed
in parentheses following the label.
.mE
Labels can also be sequentially numbered, and the number can serve
as the mnemonic.  Labels that are duplicated
within an application should be given the same mnemonic.
.mS
Mnemonics must be case insensitive for activation.
.mE
Either an uppercase or lowercase letter can be underlined in the
label.
.P
.mS
When the location cursor is on a component within a Menu, a MenuBar or
the same field as a component with a mnemonic,
typing the mnemonic character must move the location
cursor to the component and activate it.
If a mnemonic is used for an OptionButton, for a CascadeButton in a
MenuBar, or a PushButton that is not in a basic group
(that is, not in a Panel, a Menu, or a MenuBar), typing
.B MAlt
and the mnemonic anywhere in the window or its Menus must
move the cursor to the component with that mnemonic
and activate it.
Mnemonics must be supported for PushButtons in a Menu,
ToggleButtons in a Menu,
and CascadeButtons in a Menu or MenuBar.
.mE
.P
Mnemonics can also be used to select and deselect elements within
a component as described in Section 3.1.9.
.H 3 "Help Activation"
.iX "help" "activation"
.iX "KHelp"
Help is generally invoked from selections in the
.B Help
Menu of the MenuBar.
.mS
In addition,
.B KHelp
on a component must invoke any context-sensitive help
for the component or its nearest ancestor with context-sensitive
help available.
.mE
Within DialogBoxes, applications
should provide context-sensitive help for the DialogBox
as a whole.
.B KHelp
is usually bound as shown in the following table.
.P
.TB "KHelp Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KHelp@<F1>
@<Help>
.TE
.H 3 "Default Activation"
.iX "default action" "selecting"
A DialogBox should have a default action associated with it.
The default action should be the action of a default PushButton
in the bottom area of PushButtons of the DialogBox.
.mS
The default PushButton must be highlighted in some way,
usually by displaying a border around it.
.mE
Default PushButton highlighting can be removed when the
focus is outside of the DialogBox.
If the DialogBox is nested in a window the highlighting
must be removed when the focus is not the Field.
The default action of a DialogBox is activated according
to the following rules.
.BL
.LI
.mS
When the input focus is on a PushButton other than
the default, 
.B KActivate
must activate that PushButton.
The highlighting must be removed from the default PushButton,
and can be moved to the new PushButton.
.mE
When the focus policy is implicit, the changes in highlighting
should have a delay to prevent flicker when moving the pointer
through components.
.LI
.mS
When the focus is on an activatable field control, like List
or Text,
.BR "BSelect Click 2"
or
.BR KActivate
must activate both the field control's corresponding action
and the default action of the DialogBox,
.mE
except where these inputs are used internally by the component
(such as,
.B "BSelect Click 2"
in Text, and
.B KEnter
in multiline Text.)
.LI
.mS
When the input focus is on any other component,
.B KActivate
must activate
the default
action of the DialogBox, but not the
action of the component.
.mE
.LI
.mS
When the focus is not on any component,
.B KActivate
must activate the default action of the DialogBox.
.mE
.LE
.P
.mS
Except in the middle of a drag operation that can be canceled,
.B KCancel
anywhere in a DialogBox must be equivalent to activating the
.B Cancel
PushButton in the DialogBox.
.mE
.H 3 "Expert Activation"
.iX "expert action"
.iX "double-clicking"
Some elements, usually PushButtons and ToggleButtons,
can have
.B "expert activation"
actions associated with them.
For example, an icon's expert action is restoring the
window.
.BR "BSelect Click 2"
(that is, double-clicking the element)
should activate any expert action for the element.
Expert actions should only be available in a Panel of
PushButtons or in a Panel of RadioButtons where one of the RadioButtons
is always on.
.P
Where expert activation is available, the expert action
of all the buttons in the Panel should be the similar.
The expert action should include the regular action
of the component in a more global manner.  For example,
a Panel of RadioButtons in a drawing application could
include a tool for turning on the erase cursor.
Selecting the RadioButton turns on the erase cursor.
Double-clicking the RadioButton could erase the drawing
area.
.mS
To support new users and keyboard-only users,
expert actions must only be shortcuts to application features
available elsewhere.
.mE
.H 3 "Previewing and Autorepeat"
.iX "previewing"
.iX "autorepeat"
Two special actions can be used with activation: previewing
and autorepeat.  When
.B BSelect
is pressed and held over a PushButton or ToggleButton,
the application can present information in some way
that describes the effect of activating the button.  This
is called
.BR previewing .
.mS
The information must be removed when
the user releases
.BR BSelect .
.mE
Applications should provide a means to disable previewing for
experienced users.
.P
PushButtons may also
.BR autorepeat .
That is, when
.B BSelect
is pressed and held,
the PushButton activates and continues to activate
at regular intervals until the PushButton is released.
Autorepeating buttons should continue to repeat even
when the pointer moves outside the button while the
button is pressed; however, applications can suspend the activation
of the button until the pointer is moved back inside of the
button.
While the button is active, it should be drawn in the active
state.
