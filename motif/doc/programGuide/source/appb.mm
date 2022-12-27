.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".nr !_ 1
...\".pn 1
...\".SK
.nr H1 1
.af H1 A
.H 1 "UIL Built-In Tables"
...\".nr !_ 0
.P 0
This appendix contains a listing of part of the
UIL built-in tables used during
compilation to check that your UIL specification is consistent with the
Motif Toolkit.
.P 0
For each object in the Motif Toolkit, this appendix contains a table
that lists the reasons and controls (children)
supported by UIL for that object.
The arguments supported by UIL for each object are the same as the
Motif Toolkit resources for that object.
Appendix C lists the name and UIL data type of each UIL argument.
For information on which arguments are supported for which objects and
for the default values of arguments, see the widget manual pages in the
.IR "OSF/Motif Programmer's Reference" .
.P
.ds dH ArrowButton
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
.TE
.ds dH "XmArrowButtonGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
.TE
.ds dH BulletinBoard
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNfocusCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmBulletinBoardDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNfocusCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmCascadeButton
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPulldownMenu@MrmNcreateCallback
@XmNactivateCallback
@XmNcascadingCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmCascadeButtonGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPulldownMenu@MrmNcreateCallback
@XmNactivateCallback
@XmNcascadingCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmCommand
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNcommandChangedCallback
@XmNcommandEnteredCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNunmapCallback
.TE
.ds dH "XmDrawingArea
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNexposeCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNinputCallback
XmCascadeButtonGadget@XmNresizeCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmDrawnButton
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNexposeCallback
@XmNhelpCallback
@XmNresizeCallback
.TE
.ds dH "XmErrorDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmMessageBox@MrmNcreateCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmFileSelectionBox
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmBulletinBoard@XmNapplyCallback
XmBulletinBoardDialog@XmNcancelCallback
XmCascadeButton@XmNdestroyCallback
XmCommand@XmNfocusCallback
XmDrawingArea@XmNhelpCallback
XmDrawnButton@XmNmapCallback
XmErrorDialog@XmNnoMatchCallback
XmFileSelectionBox@XmNokCallback
XmFileSelectionDialog@XmNunmapCallback
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmText@
XmTextField@
XmToggleButton@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmFileSelectionDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmBulletinBoard@XmNapplyCallback
XmBulletinBoardDialog@XmNcancelCallback
XmCascadeButton@XmNdestroyCallback
XmCommand@XmNfocusCallback
XmDrawingArea@XmNhelpCallback
XmDrawnButton@XmNmapCallback
XmErrorDialog@XmNnoMatchCallback
XmFileSelectionBox@XmNokCallback
XmFileSelectionDialog@XmNunmapCallback
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmText@
XmTextField@
XmToggleButton@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmForm
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNfocusCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmFormDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNfocusCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmFrame
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNhelpCallback
XmBulletinBoardDialog@
XmCascadeButton@
XmCascadeButtonGadget@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmInformationDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmMessageBox@MrmNcreateCallback
XmPushButton@XmNcancelCallback
user_defined@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmLabel
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmLabelGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmList
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNbrowseSelectionCallback
@XmNdefaultActionCallback
@XmNdestroyCallback
@XmNextendedSelectionCallback
@XmNhelpCallback
@XmNmultipleSelectionCallback
@XmNsingleSelectionCallback
.TE
.ds dH "XmMainWindow
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNhelpCallback
XmBulletinBoardDialog@
XmCascadeButton@
XmCascadeButtonGadget@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmMenuBar
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmMessageBox
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmMessageDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmOptionMenu
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPulldownMenu@MrmNcreateCallback
@XmNdestroyCallback
@XmNentryCallback
@XmNhelpCallback
@XmNmapCallback
@XmNunmapCallback
.TE
.ds dH "XmPanedWindow
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNhelpCallback
XmBulletinBoardDialog@
XmCascadeButton@
XmCascadeButtonGadget@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmPopupMenu
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmPromptDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPushButton@MrmNcreateCallback
user_defined@XmNapplyCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNnoMatchCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmPulldownMenu
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmPushButton
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
.TE
.ds dH "XmPushButtonGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
.TE
.ds dH "XmQuestionDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmMessageBox@MrmNcreateCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmRadioBox
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmRowColumn
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmScale
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNdragCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNvalueChangedCallback
XmCascadeButtonGadget@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmScrollBar
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNdecrementCallback
@XmNdestroyCallback
@XmNdragCallback
@XmNhelpCallback
@XmNincrementCallback
@XmNpageDecrementCallback
@XmNpageIncrementCallback
@XmNtoBottomCallback
@XmNtoTopCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmScrolledList
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNbrowseSelectionCallback
@XmNdefaultActionCallback
@XmNdestroyCallback
@XmNextendedSelectionCallback
@XmNhelpCallback
@XmNmultipleSelectionCallback
@XmNsingleSelectionCallback
.TE
.ds dH "XmScrolledText
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNgainPrimaryCallback
@XmNhelpCallback
@XmNlosePrimaryCallback
@XmNlosingFocusCallback
@XmNmodifyVerifyCallback
@XmNmotionVerifyCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmScrolledWindow
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNhelpCallback
XmBulletinBoardDialog@
XmCascadeButton@
XmCascadeButtonGadget@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmSelectionBox
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPushButton@MrmNcreateCallback
user_defined@XmNapplyCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNnoMatchCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmSelectionDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPushButton@MrmNcreateCallback
user_defined@XmNapplyCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNnoMatchCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmSeparator
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmSeparatorGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNdestroyCallback
@XmNhelpCallback
.TE
.ds dH "XmText
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNgainPrimaryCallback
@XmNhelpCallback
@XmNlosePrimaryCallback
@XmNlosingFocusCallback
@XmNmodifyVerifyCallback
@XmNmotionVerifyCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmTextField
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNactivateCallback
@XmNdestroyCallback
@XmNgainPrimaryCallback
@XmNhelpCallback
@XmNlosePrimaryCallback
@XmNlosingFocusCallback
@XmNmodifyVerifyCallback
@XmNmotionVerifyCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmToggleButton
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmToggleButtonGadget
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNarmCallback
@XmNdestroyCallback
@XmNdisarmCallback
@XmNhelpCallback
@XmNvalueChangedCallback
.TE
.ds dH "XmWarningDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
No children are supported@MrmNcreateCallback
@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "XmWorkArea
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@MrmNcreateCallback
XmArrowButtonGadget@XmNdestroyCallback
XmBulletinBoard@XmNentryCallback
XmBulletinBoardDialog@XmNhelpCallback
XmCascadeButton@XmNmapCallback
XmCascadeButtonGadget@XmNunmapCallback
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmLabelGadget@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmPushButtonGadget@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmSeparatorGadget@
XmText@
XmTextField@
XmToggleButton@
XmToggleButtonGadget@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
.ds dH "XmWorkingDialog
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmPushButton@MrmNcreateCallback
user_defined@XmNcancelCallback
@XmNdestroyCallback
@XmNfocusCallback
@XmNhelpCallback
@XmNmapCallback
@XmNokCallback
@XmNunmapCallback
.TE
.ds dH "user_defined
.TS H
tab(@);
lB lB
lB lB
l l.
\\\\*(dH
_
.sp 6p
Controls@Reasons
.sp 6p
_
.sp 6p
.TH
.ds dH "\*(dH (Continued)
XmArrowButton@
XmBulletinBoard@
XmBulletinBoardDialog@
XmCascadeButton@
XmCommand@
XmDrawingArea@
XmDrawnButton@
XmErrorDialog@
XmFileSelectionBox@
XmFileSelectionDialog@
XmForm@
XmFormDialog@
XmFrame@
XmInformationDialog@
XmLabel@
XmList@
XmMenuBar@
XmMessageBox@
XmMessageDialog@
XmOptionMenu@
XmPanedWindow@
XmPopupMenu@
XmPromptDialog@
XmPulldownMenu@
XmPushButton@
XmQuestionDialog@
XmRadioBox@
XmRowColumn@
XmScale@
XmScrollBar@
XmScrolledList@
XmScrolledText@
XmScrolledWindow@
XmSelectionBox@
XmSelectionDialog@
XmSeparator@
XmText@
XmTextField@
XmToggleButton@
XmWarningDialog@
XmWorkArea@
XmWorkingDialog@
user_defined@
.TE
