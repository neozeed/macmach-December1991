.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Selection"
.hS "Description"
.iX "selection" "model"
The selection model determines how elements are selected from
a group of elements.
.mS
OSF/Motif compliant systems must support
the five different selection models.
.mE
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
.mE
.iX "browse selections"
.iX "selection" "browse"
Used to allow browsing through single selection collections.
Browse selection is also used to select a single element of
a collection.  Browse selection works just like single selection,
but additionally allows the user to browse through the elements by dragging
.B BSelect
through the elements in the collection.
Browse select highlights each element as it is traversed, and gives
the application an opportunity to provide information about
each element as it is highlighted.  Releasing
.B BSelect
on an element selects it and deselects the previously
selected element.
Browse selection is described in Section 3.1.2
.mS
.LI "\fBMultiple\ Selection\fP"
.mE
.iX "multiple selections"
.iX "selection" "multiple"
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
.mE
.iX "range selection"
.iX "selection" "range"
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
.mE
.iX "discontiguous selection"
.iX "selection" "discontiguous"
Used for simultaneously selecting multiple discontiguous
ranges of elements in a collection.
Clicking or dragging
.B BSelect
operates just the same as for range selection.
Discontiguous selection also allows
.B BToggle
to be used to add new selection ranges to or remove selection
ranges from the selection.
Discontiguous selection is described in Section 3.1.5.
.LE
.hS "Related Information"
See Chapter 3 for more information about the selection models.
