.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR Text
.hS "Description"
.iX "Text" "component"
.iX "components" "text"
A Text component should be used to
display and enter text.
.mS
A Text component must be composed
of an area for displaying and entering text.
.mE
The text can be either a single line or multiple
lines.
.mS
Text must support the range selection model
as described in Chapter 3
.mE
and can support the discontiguous
selection model.
.hS "Illustration"
.br
.ne 1.45i
\0
.sp 1.25i
.in +1.25i
.P! graphics/text.ps
.in -1.25i
.sp
.hS "Navigation"
The following text describes the navigation actions
of this component.
.VL 10n
.LI "\fBKUp\fP:"
.mS
In multiline Text components,
.B KUp
must move the location cursor up one line.
In single-line Text components,
.B KUp
must navigate upwards to the next component if the Text is
designed to act like a basic control.
.mE
.LI "\fBKDown\fP:"
.mS
In multiline Text components,
.B KDown
must move the location cursor down one line.
In single-line Text components,
.B KDown
must navigate downwards to the next component if the Text is
designed to act like a basic control.
.mE
.LI "\fBKLeft\fP:"
.mS
Must move the location cursor left one character.
.mE
.LI "\fBKRight\fP:"
.mS
Must move the location cursor right one character.
.mE
.LI "\fBKNextWord\fP:"
.br
.mS
Must move the location cursor to the right by a word,
that is, the first character after a space, tab,
or newline character.
.mE
In single-line Text controls,
.B KNextWord
can navigate rightwards to the next component if the Text is designed to act like
a basic control.
.LI "\fBKPrevWord\fP:"
.br
.mS
Must move the location cursor to the left by a word,
that is, the first nonblank character after the
first previous blank character.
.mE
In single-line Text controls,
.B KPrevWord
can navigate leftwards to the next component if the Text is designed to act like
a basic control.
.LI "\fBKNextPara\fP:"
.br
In multiline Text components,
.B KNextPara
can optionally move the location cursor to the beginning of the
next paragraph.
.LI "\fBKPrevPara\fP:"
.br
In multiline Text components,
.B KPrevPara
can optionally move the location cursor to the beginning of the
previous paragraph.
.LI "\fBKBeginLine\fP:"
.br
.mS
Must move the location cursor to the beginning of the line.
.mE
.LI "\fBKEndLine\fP:"
.mS
Must move the location cursor to the end of the line.
.mE
.LI "\fBKBeginData\fP:"
.br
.mS
In multiline Text components,
.B KBeginData
must move the location cursor to the beginning of the file.
.mE
.LI "\fBKEndData\fP:"
.br
.mS
In multiline Text components,
.B KEndData
must move the location cursor to the end of the file.
.mE
.LE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBKPageUp\fP:"
.mS
In multiline Text components,
.B KPageUp
must scroll the viewing window up one page of text,
displaying the previous page of text.
.mE
.LI "\fBKPageDown\fP:"
.br
.mS
In multiline Text components,
.B KPageDown
must scroll the viewing window down one page of text,
displaying the next page of text.
.mE
.LI "\fBKPageLeft\fP:"
.br
.mS
Must scroll the viewing window left one page of text.
.mE
.LI "\fBKPageRight\fP:"
.br
.mS
Must scroll the viewing window right one page of text.
.mE
.LI "\fBKSpace\fP:"
.mS
Must insert a space.
Modifying this, or any
.B KSelect
binding, with
.B MCtrl
must evoke its normal selection function.
.mE
.LI "\fBKEnter\fP:"
.mS
In multiline Text components,
.B KEnter
must insert a carriage return.
Modifying this, or any
.B KActivate
binding, with
.B MCtrl
must evoke its normal activation function.
.mE
When the location cursor is at the end of the text,
.B KEnter
can optionally traverse to the next field.
.LI "\fBKTab\fP\ and\ \fBKBackTab\fP:"
.br
.mS
In single-line Text components,
these must have no effect except that
.B KTab
can optionally traverse to the next field.
.mE
In multiline Text components,
these can optionally move the location cursor to the next or
previous tab stop.  Another option is for
.B KTab
to insert a tab character, and for
.B KBackTab
to have no effect.
.P
.mS
Modifying
.B Tab
or
.B BackTab
with
.B MCtrl
must evoke their normal field navigation functions.
.mE
When the location cursor is at the end of the text,
.B KTab
can optionally traverse to the next field.
.LI "\fBKBackSpace\fP:"
.br
.mS
If any characters are selected
and the cursor is in or at the bounds of the selection,
.B KBackSpace
must delete the selected characters; otherwise, it
must delete the character preceding the location cursor.
.mE
.LI "\fBKDelete\fP:"
.mS
If any characters are selected
and the cursor is in or at the bounds of the selection,
.B KDelete
must delete the selected characters; otherwise, it
must delete the character following the location cursor.
.mE
.LI "\fBKClear\fP:"
Can optionally delete the selected text, and replace it
with blank space.
.LI "\fBKEraseEndLine\fP:"
.br
Can optionally delete the characters following the location cursor to
the end of the line.
.LI "\fBKInsert\fP:"
Can optionally toggle between insert mode and replace mode.
.mS
Text components must start in insert mode, where the location
cursor is between two characters.  In insert mode, typing a
character must insert the character at the position of the
location cursor.
.mE
Text components can also implement replace mode,
where the location cursor is on a character.
.mS
In replace mode, typing a character must replace the current
character with that newly entered character and move the
location cursor to the next character.
.mE
.LI "\fBBSelect Click 2+\fP:"
.br
Selects text a block at a time.
.mS
Double-click must select text a word at a time.
.mE
Triple-click can optionally select text a line or sentence at a time.
Larger numbers of clicks can optionally select increasingly
larger blocks of text.
.LI "\fBBSelect Drag 2+\fP:"
.br
Can drag out a new selection by blocks as
described for
.BR "BSelect Click 2+" .
.LI "\fBBExtend Click 2+\fP:"
.br
Can extend the selection to a block boundary as
described for
.BR "BSelect Click 2+" .
.LI "\fBBExtend Drag 2+\fP:"
.br
Can extend the selection by blocks as
described for
.BR "BSelect Click 2+" .
.LI "\fBBToggle Click 2+\fP:"
.br
If the Text component supports discontiguous selection,
this can extend the discontiguous selection by blocks as
described for
.BR "BSelect Click 2+" .
.LI "\fBBToggle Drag 2+\fP:"
.br
If the Text component supports discontiguous selection,
this can drag out the discontiguous selection by blocks as
described for
.BR "BSelect Click 2+" .
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the Text control.
.mE
.LE
.hS "Common Bindings"
.TS H
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
.TH
BSelect@<Btn1>
BExtend@MShift BSelect
BToggle@MCtrl BSelect
KUp@<\(ua>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KPageUp@<PageUp>
KPageDown@<PageDown>
KPageLeft@MCtrl <PageUp>
KPageRight@MCtrl <PageDown>
KNextWord@MCtrl KRight
KPrevWord@MCtrl KLeft
KNextPara@MCtrl KDown
KPrevPara@MCtrl KUp
KEndLine@<End>
KEndData@MCtrl <End>
KBeginLine@<Begin>
@<Home>
KBeginData@MCtrl <Begin>
@MCtrl <Home>
KSelect@<Select>
@KSpace
@MCtrl KSpace
KSpace@<space>
KActivate@KEnter
@MCtrl KEnter
KEnter@<Enter>
@<Return>
KTab@<Tab>
KBackTab@<BackTab>
@MCtrl <Tab>
KBackSpace@<BackSpace>
KDelete@<Delete>
KClear@<Clear>
KEraseEndLine@MCtrl KDelete
KInsert@<Insert>
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information on Text selection,
see the description of the selection models in Chapter 3.
