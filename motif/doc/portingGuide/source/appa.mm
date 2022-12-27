.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr Hu 1
.HU "Appendix A - Name Change Exceptions" 
.P
The following sections describe name changes that do not follow
the rules outlined in the porting document. 
.SP 1.5
.nr Hu 2
.HU "Widget hierarchy differences by class"
.P
The following list outlines the changes to the XUI widget hierarchy to
comply with OSF/Motif.  The XUI widget classes are listed alphabetically
with their Motif equivalents.  
Functionality changes and name changes within each class appear
in the body of the porting document.  
.VL 2.5i .25i
.LI "DwtAttachedDB"
XmForm
.LI "DwtCommandWindow"
XmCommand
.LI "DwtCommon"
Approximately equivalent to a combination of XmPrimitive and XmManager
.LI "DwtDialogBox"
XmBulletinBoard
.LI "DwtFileSelection"
XmFileSelectionBox 
.LI "DwtHelp"
Not provided in Motif
.LI "DwtLabel"
XmLabel
.LI "DwtListBox"
XmList
.LI "DwtMainWindow"
XmMainWindow
.LI "DwtMenu"
XmRowColumn
.LI "DwtMessageBox"
XmMessageBox
.LI "DwtPullDownMenuEntry"
XmCascadeButton
.LI "DwtPushButton"
XmPushButton
.LI "DwtScale"
XmScale
.LI "DwtScrollBar"
XmScrollBar
.LI "DwtScrollWindow"
XmScrolledWindow
.LI "DwtSelection"
XmSelectionBox
.LI "DwtSeparator"
XmSeparator
.LI "DwtSText"
XmText
.LI "DwtToggleButton"
XmToggleButton
.LI "DwtWindow"
XmDrawingArea
.LE
.SP 1.5
.ne 3i
.nr Hu 2
.HU "Function name changes"
.P
You must change all create function names from Dwt*Create to XmCreate*.
Changes to create function names may also require the changes
to widget class names specified in the previous section.
.SP
.VL 2.5i .25i
.LI "- DwtAttachedDBCreate"
XmCreateForm
.LI "- DwtAttachedDBPopupCreate"
XmCreateFormDialog
.LI "- DwtCautionBoxCreate"
XmCreateWarningDialog
.br
or XmCreateMessageDialog
.br
or XmCreateErrorDialog
.br
or XmCreateQuestionDialog\(dg
.LI "- DwtCommandWindowCreate"
XmCreateCommand
.LI "- DwtDialogBoxCreate"
XmCreateBulletinBoard
.LI "- DwtDialogBoxPopupCreate"
XmCreateBulletinBoardDialog
.LI "- DwtFileSelectionCreate"
XmCreateFileSelectionDialog
.LI "- DwtListBoxCreate"
XmCreateList
.LI "- DwtMenuCreate"
XmCreateRowColumn
.LI "- DwtMenuPopupCreate"
XmCreatePopupMenu
.LI "- DwtMenuPulldownCreate"
XmCreatePulldownMenu
.LI "- DwtMessageBoxCreate"
XmCreateInformationDialog\(dg
.LI "- DwtPullDownMenuEntryCreate"
XmCreateCascadeButton
.LI "- DwtScrollWindowCreate"
XmCreateScrolledWindow
.LI "- DwtSelectionCreate"
XmCreateSelectionBox
.LI "- DwtSTextCreate"
XmCreateText
.LI "- DwtWindowCreate"
XmCreateDrawingArea
.LI "- DwtWorkBoxCreate"
XmCreateWorkingDialog\(dg
.LE
.SP 
Make these other function name changes:
.SP .5
.VL 2.5i .25i
.LI "- DwtCommandAppend"
XmCommandAppendValue   
.LI "- DwtCommandErrorMessage"
XmCommandError         
.LI "- DwtCommandSet"
XmCommandSetValue      
.LI "- DwtScaleGetSlider"
XmScaleGetValue        
.LI "- DwtScaleSetSlider"
XmScaleSetValue        
.LI "- DwtScrollBarGetSlider"
XmScrollBarGetValues   
.LI "- DwtScrollBarSetSlider"
XmScrollBarSetValues   
.LE
.SP .5
.VL 1n 
.LI "\(dg"
These functions (or groups of functions)
instantiate an XmMessageBox widget inside an
XmDialogShell widget.  To instantiate just the XmMessageBox
widget, use XmCreateMessageBox.
.LE
.ne 2.5i
.SP 1.5
.nr Hu 2
.HU "Resource name changes"
.P
Change resource names from DwtN* to XmN* and DwtNadb* to XmN* with the following
exceptions: 
.SP
.VL 2.5i .25i
.LI "- DwtNautoShowInsertPoint"
XmNautoShowCursorPosition   
.LI "- DwtNcancelLabel"
XmNcancelLabelString        
.LI "- DwtNcols"
XmNcolumns                  
.LI "- DwtNdefaultPushbutton"
XmNdefaultButtonType        
.LI "- DwtNfilterLabel"
XmNfilterLabelString        
.LI "- DwtNhelpLabel"
XmNhelpLabelString          
.LI "- DwtNhorizontal"
XmNhorizontalSpacing        
.LI "- DwtNinc"
XmNincrement                
.LI "- DwtNindicator"
XmNindicatorOn              
.LI "- DwtNmenuIsHomogeneous"
XmNisHomogeneous            
.LI "- DwtNmenuNumColumns"
XmNnumColumns               
.LI "- DwtNmenuPacking"
XmNpacking                  
.LI "- DwtNinc"
XmNincrement
.LI "- DwtNokLabel"
XmNokLabelString            
.LI "- DwtNpageDecCallback"
XmNpageDecrementCallback    
.LI "- DwtNpageInc"
XmNpageIncrement            
.LI "- DwtNpageIncCallback"
XmNpageIncrementCallback    
.LI "- DwtNprompt"
XmNpromptString             
.LI "- DwtNselectionLabel"
XmNselectionLabelString     
.LI "- DwtNshape"
XmNindicatorType            
.LI "- DwtNstyle"
XmNdialogStyle              
.LI "- DwtNtitleLabel"
XmNtitleString              
.LI "- DwtNunitDecCallback"
XmNdecrementCallback
.LI "- DwtNunitIncCallback"
XmNincrementCallback
.LE
.SP 1.5
.nr Hu 2
.HU "Enumeration literal name changes"
.P
Change the enumeration literals 
of the form DwtAaaaAaaa to the
form XmAAAA_AAAA with the following exceptions:
.SP
.VL 2.5i .25i
.LI "- DwtAttachAdb"
XmATTACH_FORM
.LI "- DwtAttachOppAdb"
XmATTACH_OPPOSITE_FORM
.LI "- DwtAttachOppWidget"
XmATTACH_OPPOSITE_WIDGET
.LI "- DwtCancelButton"
XmDIALOG_CANCEL_BUTTON    
.LI "- DwtMenuWorkArea"
XmWORK_AREA               
.LI "- DwtModal"
XmDIALOG_APPLICATION_MODAL   
.LI "- DwtModal"
XmDIALOG_SYSTEM_MODAL     
.LI "- DwtModeless"
XmDIALOG_MODELESS            
.LI "- DwtOrientationHorizontal"
XmHORIZONTAL              
.LI "- DwtOrientationVertical"
XmVERTICAL                
.LI "- DwtOval"
XmONE_OF_MANY             
.LI "- DwtRectangular"
XmN_OR_MANY               
.LI "- DwtResizeFixed"
XmRESIZE_NONE             
.LI "- DwtResizeGrowOnly"
XmRESIZE_GROW             
.LI "- DwtResizeShrinkWrap"
XmRESIZE_ANY              
.LI "- DwtWorkArea"
XmDIALOG_WORK_AREA        
.LE
.SP 1.5
.nr Hu 2
.HU "Callback reason name changes"
.P
Change callback reasons from DwtCRAaaaAaaa to XmCR_AAAA_AAAA
with the following exceptions:
.SP
.VL 2.5i .25i
.LI "- DwtCRPageDec"
XmCR_PAGE_DECREMENT      
.LI "- DwtCRPageInc"
XmCR_PAGE_INCREMENT      
.LI "- DwtCRUnitDec"
XmCR_DECREMENT           
.LI "- DwtCRUnitInc"
XmCR_INCREMENT           
.LE
.SP 1.5
.nr Hu 2
.HU "Compound string name changes"
.P
The compound string functionality supported in
XUI has been altered in Motif to present a more vendor
neutral naming scheme, as well as expanded application functionality.
.SP 1.5
The table below suggests the Motif functions that should be used
to replace the corresponding Dwt functions. 
In most cases, these are simply name changes.  
In the case of certain functions,
argument order may have changed, or arguments may be missing.  
.SP 1.5
The Motif functions corresponding to DwtLatin1String, DwtCSString, 
and DwtString are suggested replacements.
.SP 1.5
Note that DwtDisplayCSMessage and DwtDisplayVMSMessage
are not part of the Motif toolkit.
.SP
.VL 2.5i .25i
.LI "- DwtCSbytecmp"
XmStringByteCompare      
.LI "- DwtCSempty"
XmStringEmpty            
.LI "- DwtCSString"
XmStringSegmentCreate    
.LI "- DwtCStrcat"
XmStringConcat           
.LI "- DwtCStrcpy"
XmStringCopy             
.LI "- DwtCStringDirection"
XmStringDirection        
.LI "- DwtCStrlen"
XmStringLength           
.LI "- DwtCStrncat"
XmStringNConcat          
.LI "- DwtCStrncpy"
XmStringNCopy            
.LI "- DwtGetNextSegment"
XmStringGetNextSegment   
.LI "- DwtInitGetSegment"
XmStringInitContext      
.LI "- DwtLatin1String"
XmStringCreateLtoR       
.LI "- DwtString"
XmStringSegmentCreate    
.LE
.SP
Also change the type DwtCompString to XmString.
.SP 1.5
.nr Hu 2
.HU "FontList name changes"
.P
Make the following FontList name changes:
.SP
.VL 2.5i .25i
.LI "- DwtAddFontList"
XmFontListAdd          
.LI "- DwtCreateFontList"
XmFontListCreate       
.LI "- DwtFreeFontList"
XmFontListFree         
.LE
.SP 1.5
.nr Hu 2
.HU "Clipboard name changes"
.P
Make the following Clipboard name changes:
.SP
.VL 2.5i .25i
.LI "- DwtBeginCopyFromClipboard"
XmClipboardStartRetrieve         
.LI "- DwtBeginCopyToClipboard"
XmClipboardStartCopy             
.LI "- DwtCancelCopyFormat"
XmClipboardWithdrawFormat        
.LI "- DwtCancelCopyToClipboard"
XmClipboardCancelCopy            
.LI "- DwtCopyFromClipboard"
XmClipboardRetrieve              
.LI "- DwtCopyToClipboard"
XmClipboardCopy                  
.LI "- DwtEndCopyFromClipboard"
XmClipboardEndRetrieve           
.LI "- DwtEndCopyToClipboard"
XmClipboardEndCopy           
.LI "- DwtInquireNextPasteCount"
XmClipboardInquireCount          
.LI "- DwtInquireNextPasteFormat"
XmClipboardInquireFormat         
.LI "- DwtInquireNextPasteLength"
XmClipboardInquireLength         
.LI "- DwtListPendingItems"
XmClipboardInquirePendingItems   
.LI "- DwtReCopyToClipboard"
XmClipboardCopyByName            
.LI "- DwtUndoCopyToClipboard"
XmClipboardUndoCopy          
.LE
.SP 1.5
.nr Hu 2
.HU "Resource Manager name changes"
.P
Make the following name changes:
.SP
.VL 2.5i .25i
.LI "- DwtCloseHierarchy"
MrmCloseHierarchy
.LI "- DwtFetchColorLiteral"
MrmFetchColorLiteral
.LI "- DwtFetchIconLiteral"
MrmFetchIconLiteral
.LI "- DwtFetchInterfaceModule"
MrmFetchInterfaceModule
.LI "- DwtFetchLiteral"
MrmFetchLiteral
.LI "- DwtFetchSetValues"
MrmFetchSetValues
.LI "- DwtFetchWidget"
MrmFetchWidget
.LI "- DwtFetchWidgetOverride"
MrmFetchWidgetOverride
.LI "- DwtInitializeDRM"
MrmInitialize          
.LI "- DwtOpenHierarchy"
MrmOpenHierarchy
.LI "- DwtRegisterClass"
MrmRegisterClass
.LI "- DwtRegisterDRMNames"
MrmRegisterNames       
.LE
.SP
The following function names have also changed, but the use of
these functions is deprecated.  The functions
MrmFetchLiteral, MrmFetchIconLiteral, and MrmFetchColorLiteral
should be used instead.
.SP 
.VL 2.5i .25i
.LI "- DwtDrmFreeResourceContext"
MrmFreeResourceContext
.LI "- DwtDrmGetResourceContext"
MrmGetResourceContext
.LI "- DwtDrmHGetIndexedLiteral"
MrmGetLiteral
.LI "- DwtDrmRCBuffer"
MrmRCBuffer
.LI "- DwtDrmRCSetType"
MrmRCSetType
.LI "- DwtDrmRCSize"
MrmRCSize
.LI "- DwtDrmRCType"
MrmRCType
.LE
