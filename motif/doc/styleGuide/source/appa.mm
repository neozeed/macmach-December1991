.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 0
.af H1 A
.H 1 "Common Bindings for Virtual Buttons and Keys"
.iX "virtual buttons"
The following table lists the virtual buttons used in this
.I "Style Guide" ,
along with their common bindings for a mouse with one,
two or three buttons.
.P
.TB "Virtual Button Bindings"
.TS
center, box, tab(@);
lb lb s s
lb lb lb lb.
@Default Bindings
Virtual Buttons@Three Buttons@Two Buttons@One Button
_
BSelect@<Btn1>@<Btn1>@<Btn1>
BDrag@<Btn2>@1: <Btn2>@undefined
@@2: <Btn2>@
@@3: undefined@
BCustom@<Btn3>@1: <Btn1> <Btn2>@undefined
@@2: or undefined@
@@3: <Btn2>@
BMenu@BCustom@1: BCustom@MCtrl MShift BSelect
@@2: MCtrl MShift BSelect@
@@3: BCustom@
BExtend@MShift BSelect@MShift BSelect@MShift BSelect
BToggle@MCtrl BSelect@MCtrl BSelect@MCtrl BSelect
BPrimaryPaste@BDrag@BDrag@undefined
BPrimaryCopy@MCtrl BPrimaryPaste@MCtrl BPrimaryPaste@undefined
BPrimaryCut@MAlt BPrimaryPaste@MCtrl BPrimaryPaste@undefined
BQuickPaste@BDrag@BDrag@undefined
BQuickCopy@MCtrl BQuickPaste@MCtrl BQuickPaste@undefined
BQuickCut@MAlt BQuickPaste@MAlt BQuickPaste@undefined
.TE
The following table lists the virtual modifier keys used in
this
.I "Style Guide" ,
along with their common bindings.
.P
.iX "virtual modifier keys"
.TB "Virtual Modifier Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
MShift@<Shift>
MCtrl@<Ctrl>
MAlt@<Alt>
@<Meta>
@<Extend char>
@<Compose Character>
.TE
.iX "virtual keys"
The following table alphabetically lists the virtual keys used in
this
.I "Style Guide" ,
along with their common bindings.
Note that some virtual keys have multiple default bindings, and
some have alternate bindings, noted by the word "or."
Where a virtual key has multiple default bindings, you should
supply all of them.
Where a virtual key has an alternate binding it is not
necessary that both bindings be available, but you can
make both bindings available.
.br
.ne 5
.P
.TB "Virtual Key Bindings"
.TS H
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
.TH
KActivate@KEnter
@MCtrl KEnter
KAddMode@MShift <F8>
KBackSpace@<BackSpace>
KBackTab@<BackTab>
@MShift KTab
KBeginData@MCtrl KBeginLine
KBeginLine@<Begin> or <Home>
KCancel@<Escape> or <Cancel>
KClear@<Clear> (optional)
KCopy@MCtrl KInsert
@<Copy>
KCut@MShift KDelete
@<Cut>
KDelete@<Delete>
KDeselectAll@MCtrl <\e>
KDown@<\(da>
KEndData@MCtrl KEndLine
KEndLine@<End>
KEnter@<Enter> or <Return>
KEraseEndLine@MCtrl KDelete
KExtend@MShift KSelect
KHelp@<Help> or <F1>
KInsert@<Insert>
KLeft@<\(<->
KMenu@<Menu> or <F4>
KMenuBar@<F10>
KNextFamilyWindow@MAlt KNextPane
KNextField@KTab
@MCtrl KTab
KNextMenu@MCtrl KDown
@MCtrl KRight
KNextPane@<F6>
KNextPara@MCtrl KDown
KNextWindow@MAlt KTab
KNextWord@MCtrl KRight
KPageDown@<PageDown>
KPageLeft@MCtrl KPageUp
KPageRight@MCtrl KPageDown
KPageUp@<PageUp>
KPaste@MShift KInsert
@<Paste>
KPrevFamilyWindow@MAlt KPrevPane
KPrevField@KBackTab
@MCtrl KBackTab
KPrevMenu@MCtrl KUp
@MCtrl KLeft
KPrevPane@MShift KNextPane
KPrevPara@MCtrl KUp
KPrevWindow@MAlt KBackTab
@MAlt MShift KTab
KPrevWord@MCtrl KLeft
KPrimaryCopy@MCtrl KPrimaryPaste
@MAlt KCopy
KPrimaryCut@MAlt KPrimaryPaste
@MAlt KCut
KPrimaryPaste@<vendor defined>
KQuickCopy@MCtrl KQuickPaste
KQuickCut@MAlt KQuickPaste
KQuickExtend@MShift KQuickPaste
KQuickPaste@<vendor defined>
KReselect@MCtrl MShift KSelect
KRestore@MCtrl MShift KInsert
KRight@<\(->>
KSelect@<Select>
@KSpace
@MCtrl KSpace
KSelectAll@MCtrl </>
KSpace@<space>
KTab@<Tab>
KUndo@MAlt KBackSpace
@<Undo>
KUp@<\(ua>
KWindowMenu@MShift <Escape>
@MAlt KSelect
.TE
