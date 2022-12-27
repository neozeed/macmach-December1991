.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
...\".nr !_ 1
...\".pn 1
...\".SK
.nr H1 2
.af H1 A
.H 1 "UIL Arguments"
...\".nr !_ 0
.mc
.P 0
.mc |
This appendix provides an alphabetical listing of
the UIL arguments and their data types.
Each argument name is the same as the corresponding Motif Toolkit
resource name.
For information on which arguments are supported for which objects and
for the default values of arguments, see the widget manual pages in the
.IR "OSF/Motif Programmer's Reference" .
.mc
.P 0
.mc *
.mc
.TS H
center, tab(@);
l l 
l l.
_
.sp 6p
.mc |
\f3UIL Argument Name\fP@\f3Argument Type\fP
.mc
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
XmNaccelerator@string
XmNacceleratorText@compound_string
.mc |
XmNaccelerators@translation_table
.mc
XmNadjustLast@boolean
XmNadjustMargin@boolean
XmNalignment@integer
XmNallowOverlap@boolean
XmNallowResize@boolean
.mc *
.mc
XmNancestorSensitive@boolean
XmNapplyLabelString@compound_string
XmNarmColor@color
XmNarmPixmap@pixmap
XmNarrowDirection@integer
XmNautoShowCursorPosition@boolean
XmNautoUnmanage@boolean
XmNautomaticSelection@boolean
XmNbackground@color
XmNbackgroundPixmap@pixmap
XmNblinkRate@integer
XmNborderColor@color
XmNborderPixmap@pixmap
XmNborderWidth@integer
XmNbottomAttachment@integer
XmNbottomOffset@integer
XmNbottomPosition@integer
XmNbottomShadowColor@color
XmNbottomShadowPixmap@pixmap
.mc |
XmNbottomWidget@widget_ref
.mc
XmNbuttonFontList@font_table
.mc |
XmNcancelButton@widget_ref
.mc
XmNcancelLabelString@compound_string
XmNcascadePixmap@pixmap
.mc |
XmNcolormap@identifier
.mc
XmNcolumns@integer
XmNcommand@compound_string
.mc |
XmNcommandWindow@widget_ref
XmNcommandWindowLocation@integer
.mc
XmNcursorPosition@integer
XmNcursorPositionVisible@boolean
XmNdecimalPoints@integer
.mc |
XmNdefaultButton@widget_ref
XmNdefaultButtonShadowThickness@integer
.mc
XmNdefaultButtonType@integer
XmNdefaultPosition@boolean
.mc |
XmNdepth@identifier
.mc
XmNdialogStyle@integer
XmNdialogTitle@compound_string
XmNdialogType@integer
.mc |
XmNdirListItemCount@integer
XmNdirListItems@string_table
XmNdirListLabelString@compound_string
.mc
XmNdirMask@compound_string
.mc |
XmNdirSearchProc@any
.mc
XmNdirSpec@compound_string
.mc |
XmNdirectory@compound_string
.mc
XmNdoubleClickInterval@integer
XmNeditMode@integer
XmNeditable@boolean
XmNentryAlignment@integer
XmNentryBorder@integer
XmNentryClass@class_rec_name
.mc |
XmNfileListItemCount@integer
XmNfileListItems@string_table
XmNfileListLabelString@compound_string
.mc
XmNfileSearchProc@any
.mc |
XmNfileTypeMask@integer
.mc
XmNfillOnArm@boolean
XmNfillOnSelect@boolean
XmNfilterLabelString@compound_string
XmNfontList@font_table
XmNforeground@color
XmNfractionBase@integer
XmNheight@integer
.mc *
.mc
XmNhelpLabelString@compound_string
XmNhighlightColor@color
XmNhighlightOnEnter@boolean
XmNhighlightPixmap@pixmap
XmNhighlightThickness@integer
XmNhistoryItemCount@integer
XmNhistoryItems@string_table
XmNhistoryMaxItems@integer
XmNhistoryVisibleItemCount@integer
.mc |
XmNhorizontalScrollBar@widget_ref
.mc
XmNhorizontalSpacing@integer
XmNincrement@integer
XmNindicatorOn@boolean
.mc |
XmNindicatorSize@integer
.mc
XmNindicatorType@integer
XmNinitialDelay@integer
.mc |
XmNinitialResourcesPersistent@boolean
XmNinsertPosition@identifier
.mc
XmNisAligned@boolean
XmNisHomogeneous@boolean
XmNitemCount@integer
XmNitems@string_table
.mc *
.mc
XmNlabelFontList@font_table
XmNlabelInsensitivePixmap@pixmap
XmNlabelPixmap@pixmap
XmNlabelString@compound_string
XmNlabelType@integer
XmNleftAttachment@integer
XmNleftOffset@integer
XmNleftPosition@integer
.mc |
XmNleftWidget@widget_ref
.mc
XmNlistItemCount@integer
XmNlistItems@string_table
XmNlistLabelString@compound_string
XmNlistMarginHeight@integer
XmNlistMarginWidth@integer
XmNlistSizePolicy@integer
XmNlistSpacing@integer
XmNlistUpdated@boolean
XmNlistVisibleItemCount@integer
.mc *
.mc
XmNmainWindowMarginHeight@integer
XmNmainWindowMarginWidth@integer
XmNmappedWhenManaged@boolean
XmNmappingDelay@integer
XmNmargin@integer
XmNmarginBottom@integer
XmNmarginHeight@integer
XmNmarginLeft@integer
XmNmarginRight@integer
XmNmarginTop@integer
XmNmarginWidth@integer
.mc *
.mc
XmNmaxLength@integer
XmNmaximum@integer
.mc |
XmNmenuAccelerator@string
XmNmenuBar@widget_ref
XmNmenuHelpWidget@widget_ref
XmNmenuHistory@widget_ref
XmNmenuPost@compound_string
.mc
XmNmessageAlignment@integer
XmNmessageString@compound_string
.mc |
XmNmessageWindow@widget_ref
.mc
XmNminimizeButtons@boolean
XmNminimum@integer
.mc |
XmNmnemonic@keysym
XmNmnemonicCharSet@string
XmNmultiClick@integer
.mc
XmNmustMatch@boolean
.mc |
XmNnavigationType@integer
XmNnoMatchString@compound_string
.mc
XmNnoResize@boolean
XmNnumColumns@integer
XmNokLabelString@compound_string
XmNorientation@integer
XmNpacking@integer
XmNpageIncrement@integer
.mc |
XmNpaneMaximum@integer
XmNpaneMinimum@integer
XmNpattern@compound_string
.mc
XmNpendingDelete@boolean
XmNpopupEnabled@boolean
XmNprocessingDirection@integer
XmNpromptString@compound_string
XmNpushButtonEnabled@boolean
.mc |
XmNqualifySearchDataProc@any
.mc
XmNradioAlwaysOne@boolean
XmNradioBehavior@boolean
XmNrecomputeSize@boolean
XmNrefigureMode@boolean
XmNrepeatDelay@integer
XmNresizable@boolean
XmNresizeHeight@boolean
XmNresizePolicy@integer
XmNresizeWidth@boolean
XmNrightAttachment@integer
XmNrightOffset@integer
XmNrightPosition@integer
.mc |
XmNrightWidget@widget_ref
.mc
XmNrowColumnType@integer
XmNrows@integer
XmNrubberPositioning@boolean
XmNsashHeight@integer
XmNsashIndent@integer
XmNsashShadowThickness@integer
XmNsashWidth@integer
.mc *
.mc
XmNscaleHeight@integer
.mc |
XmNscaleMultiple@integer
.mc
XmNscaleWidth@integer
.mc |
XmNscreen@identifier
.mc
XmNscrollBarDisplayPolicy@integer
XmNscrollBarPlacement@integer
XmNscrollHorizontal@boolean
XmNscrollLeftSide@boolean
XmNscrollTopSide@boolean
.mc *
.mc
XmNscrollVertical@boolean
XmNscrolledWindowMarginHeight@integer
XmNscrolledWindowMarginWidth@integer
XmNscrollingPolicy@integer
XmNselectColor@color
XmNselectInsensitivePixmap@pixmap
XmNselectPixmap@pixmap
XmNselectThreshold@integer
XmNselectedItemCount@integer
XmNselectedItems@string_table
XmNselectionArray@any
.mc |
XmNselectionArrayCount@integer
.mc
XmNselectionLabelString@compound_string
XmNselectionPolicy@integer
XmNsensitive@boolean
XmNseparatorOn@boolean
XmNseparatorType@integer
XmNset@boolean
XmNshadowThickness@integer
XmNshadowType@integer
.mc *
.mc
XmNshowArrows@boolean
XmNshowAsDefault@integer
XmNshowSeparator@boolean
XmNshowValue@boolean
XmNskipAdjust@boolean
XmNsliderSize@integer
XmNspacing@integer
XmNstringDirection@integer
.mc |
XmNsubMenuId@widget_ref
.mc
XmNsymbolPixmap@pixmap
.mc |
XmNtextAccelerators@translation_table
.mc
XmNtextColumns@integer
XmNtextFontList@font_table
.mc |
XmNtextString@compound_string
.mc
XmNtextTranslations@translation_table
.mc *
.mc
XmNtitleString@compound_string
XmNtopAttachment@integer
.mc |
XmNtopCharacter@integer
XmNtopItemPosition@integer
.mc
XmNtopOffset@integer
XmNtopPosition@integer
XmNtopShadowColor@color
XmNtopShadowPixmap@pixmap
.mc |
XmNtopWidget@widget_ref
.mc
XmNtranslations@translation_table
XmNtraversalOn@boolean
.mc |
XmNtroughColor@color
.mc
XmNunitType@integer
.mc *
.mc
XmNuserData@any
.mc |
XmNvalue@any
XmNverifyBell@boolean
XmNverticalScrollBar@widget_ref
.mc
XmNverticalSpacing@integer
XmNvisibleItemCount@integer
XmNvisibleWhenOff@boolean
XmNvisualPolicy@integer
XmNwhichButton@integer
XmNwidth@integer
XmNwordWrap@boolean
.mc |
XmNworkWindow@widget_ref
.mc
XmNx@integer
XmNy@integer
.br
.sp 6p
.mc *
.mc
.TE
