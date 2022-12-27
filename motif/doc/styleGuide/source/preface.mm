.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.de mS
.mc *
..
.de mE
.mc
..
...\" Next directive sets page number
.nr `P 7
.ds bT "OSF/Motif Style Guide
.nr Hu 1
.HU "Preface"
.iX "Microsoft Windows"
.iX "Presentation Manager"
.iX "Common User Access"
The \fIOSF/Motif Style Guide\fR provides a framework of behavior
specifications to guide application developers, widget developers, 
user interface system developers,
and window manager developers in the design and implementation of new
products consistent with the
OSF/Motif\|\s-4\uTM\d\s+4 user interface.
This
.I "Style Guide"
is also closely consistent with Microsoft Windows,
Presentation Manager, and Common User Access (CUA).
.P
The \fIStyle Guide\fP establishes a consistent behavior 
among new products by drawing out the common
elements from a variety of current behavioral models.  The \fIStyle Guide\fP
anticipates the evolution of graphical user interfaces as
new technology becomes available and as the use of the Motif\|\s-4\uTM\d\s+4
user interface spreads.
Behavioral guidelines will be added over time as they become stable.
.P
For specific details of coding the implementation into an application
program, widget, or window manager see the other volumes of the
OSF/Motif documentation set.
.nr Hu 2
.HU "Audience"
.iX "audience"
This document is written for four audiences.  The following text
suggests the sections in this guide that are relevant to each
audience.  We recommend that you read through the entire
.I "Style Guide"
once to familiarize yourself with all user interface design concepts
and to ensure that you do not miss anything.
.BL
.LI
Application Designers
.P
Should be familiar with the contents of Chapters 1, 4 and 6.
.LI
Widget Designers
.P
Should be familiar with the contents of Chapters 1, 2, 3, 4, 6, and 7.
.LI
User Interface System Designers
.P
Should be familiar with the entire contents of this guide.
.LI
Window Manager Designers
.P
Should be familiar with the contents of Chapters 1, 4, 5, and 6.
.LE
.nr Hu 2
.HU "Contents"
This document is organized into seven chapters
and two appendixes.
.BL
.LI
\fIChapter 1\fP
provides general user interface design principles.
Everyone should read this chapter.
.LI
\fIChapter 2\fP
describes the input and navigation models.  New widget
designers and user interface system designers should
read this chapter.
.LI
\fIChapter 3\fP
describes the selection and activation models.  New widget
designers and user interface system designers should
read this chapter.
.LI
\fIChapter 4\fP
describes user interface component choices, layout and interaction.
Everyone should read this chapter.
.LI
\fIChapter 5\fP
describes window manager design.  Window manager designers
and user interface system designers should read this chapter.
.LI
\fIChapter 6\fP
introduces and briefly describes
internationalization and localization concepts and
issues as they relate to user interface design.
Everyone should read this chapter.
.LI
\fIChapter 7\fP
provides reference information for concepts described
in earlier chapters, and provides detailed information
about components.
This chapter should be read by new widget designers and
can by used as a reference by everyone.
.LI
\fIAppendix A\fP
provides default keyboard and mouse bindings.
.LI
\fIAppendix B\fP
shows the correspondence between OSF/Motif widgets and
components described in this
.IR "Style Guide" .
.LE
.nr Hu 2
.HU "Typographic Conventions"
.iX "typographic conventions"
.P
The Motif document set uses the following typographic conventions: 
.BL
.LI
\fBBoldfaced\fP strings represent key and button bindings, and
literal values.
\fBBoldfaced\fP strings also represent
the first use of words described in the glossary .
.LI
.B <Key>
represents a key on the keyboard.  The word
in italics corresponds to the engraving on the actual key.
.LI
Components of the user interface are represented by
capital letters on each major word in the name of the component,
such as PushButton.
.LE
.P
See "Compliance Conventions" below for an explanation of the asterisks
(*) that appear in the margins.
.P
.iX "keyboard" "input"
.iX "keys" "virtual"
.iX "virtual keys"
.iX "virtual bindings"
.iX "input" "keyboard"
.iX "key names" "virtual"
.iX "keyboards"
Since not all keyboards are the same, it is difficult
to give style guidelines that are correct for every
manufacturer's keyboard.  To solve this problem, this
guide describes keys and buttons using a
.B "virtual binding"
mechanism, by
the action they perform.  Everywhere that keyboard
input is specified, the keys are indicated by their virtual
key name.  This guide separately explains which actual keys
are commonly used for each virtual key.  For instance, the
virtual key
.B KActivate
is used for keyboard activation of
components and is commonly bound to
.B <Return>
and
.BR "MCtrl <Return>" ,
or
.B <Enter>
and/or
.B "MCtrl <Enter>"
depending on the keyboard.
This mechanism also simplifies the descriptions of behavior
where multiple keys can be bound simultaneously to one virtual key.
.P
.iX "virtual buttons"
.iX "modifiers"
.iX "virtual keys"
.iX "keys" "virtual"
.iX "keyboards"
.iX "nominal keys"
The virtual key mechanism includes keys, modifiers, and mouse buttons.
Virtual mouse buttons start with the letter
.BR B .
For a three button mouse, the leftmost mouse button is usually
defined as
.BR BSelect ,
the middle mouse button is usually defined as
.BR BDrag ,
and the rightmost mouse button is usually defined as
.BR BCustom .
Details about how virtual mouse buttons are usually defined are described
in Section 2.2, and where a new virtual button is described for the
first time.
.P
All normal virtual keys start with the letter
.BR K .
All modifier virtual keys start with the letter
.BR M .
.P
This guide assumes that a keyboard has all the standard
alphabetic, numeric, and symbol keys,
as well as the special keys corresponding to the
actions
Enter, usually
.B <Return>
or
.B <Enter>
Tab,
Spacebar,
Backspace,
Delete,
Escape,
Insert,
\(ua,
\(da,
\(<-,
\(->,
Begin Line, usually
.BR <Begin>
or
.BR <Home> ,
End Line, usually
.BR <End> ,
Page Up,
and Page Down.
.mS
Each of this keys must be available either as specified or using
other keys or key combinations if the specified key is unavailable.
.mE
.P
This guide also requires the nominal keys corresponding to the actions
Move to the MenuBar
.RB ( <F10> ),
pop up a menu
.RB ( <Menu>
or
.BR <F4> ),
provide help
.RB ( <Help>
or
.BR <F1> ),
toggle add mode
.RB ( "<Shift> <F8>" ),
and move to the next pane
.RB ( <F6> ).
.mS
Each of this keys must be available either as specified or using
other keys or key combinations if the specified key is unavailable.
If the chosen binding involves a function key, the one listed above
must be used, unless it is not available on the keyboard.
.mE
Additional keys, including navigation keys and special keys like
.BR <Menu>
and
.BR <Help> ,
are suggested, but not necessary for full keyboard equivalence.
.P
.iX "virtual buttons"
.iX "keys" "virtual"
.iX "modifiers"
This guide also assumes that all keyboards have the three modifier
keys
.BR MShift ,
.BR MCtrl ,
and
.BR MAlt ,
which are usually bound to
.BR <Shift> ,
.BR <Ctrl> ,
and
.BR <Alt> .
Your keyboard may have the virtual buttons, keys, or modifiers
bound as appropriate.  
Appendix A
lists all of the virtual bindings used in this guide, as well
as suggested and alternative bindings.  To avoid confusion,
this guide also mentions the suggested bindings of virtual
keys throughout.
.nr Hu 2
.HU "Compliance Conventions"
.P
.iX "compliance conventions"
.iX "must"
.iX "should"
.iX "can"
.iX "certification checklist"
.iX "language direction"
.iX "scanning direction"
.iX "direction of scanning"
Throughout the \fIStyle Guide\fP
"must," "should," and "can" have special meanings.
Guidelines with "must" in them are requirements for
.I "Style Guide"
compliance.  Any guideline with "must" will be included
in the
.I "OSF/Motif Level 1 Certification Checklist"
for
.I "Style Guide"
compliance.
Any guideline with "must" will be marked in the margin with an
asterisk.
Guidelines with "should" in them are recommendations.  We
consider them important for interapplication consistency,
but we do not want to require them for compliance.
You should follow them as closely as you are able.
Guidelines with "can" in them indicate optional elements
of user interface style.
.P
The process for how
.I "Style Guide"
elements migrate from options, to recommendations, to requirements
is described in the
.IR "OSF/Motif Level 1 Certification Checklist" .
.P
Note that by default this guide assumes your application is being
designed for a left-to-right language direction environment,
and that the application is written in English.  Many of
these guidelines can, and in fact should, be modified based
on both language and scanning direction.
