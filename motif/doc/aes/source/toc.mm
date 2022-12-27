...\"	tocmac1.mm  --  header stuff so that we can generate a toc
...\"	from local .)T calls.   This def gets closed by the tocmac2.mm
...\"	file, which also calls the .TC macro.
...\"
.de >C	\" macro that holds .)T calls
.)T 1 0 "" "Preface" ix
.)T 2 0 "" "Audience" ix
.)T 2 0 "" "Contents" ix
.)T 2 0 "" "Typographical Conventions" x
.)T 2 0 "" "Interface Definition Manual Page Format" x
.)T 1 1 "1.\ \ " "Introduction" 1 1
.)T 2 2 "1.1\ \ " "The Application Environment Specification Definition" 1 1
.)T 3 3 "1.1.1\ \ " "Introduction to the AES Definition" 1 1
.)T 3 3 "1.1.2\ \ " "Purpose of the AES" 1 1
.)T 3 3 "1.1.3\ \ " "Contents of the AES" 2 1
.)T 3 3 "1.1.4\ \ " "Organization of the AES" 2 1
.)T 3 3 "1.1.5\ \ " "The Meaning of AES Conformance" 3 1
.)T 3 3 "1.1.6\ \ " "Relationship of the AES to Standards and Other Specifications" 4 1
.)T 3 3 "1.1.7\ \ " "Relationship of the AES to Implementations" 4 1
.)T 3 3 "1.1.8\ \ " "The AES Development Process" 4 1
.)T 3 3 "1.1.9\ \ " "AES Support Levels" 5 1
.)T 2 2 "1.2\ \ " "Service Outline" 7 1
.)T 2 2 "1.3\ \ " "Overview of Services by Type and Function" 15 1
.)T 3 3 "1.3.1\ \ " "Window manager" 15 1
.)T 3 3 "1.3.2\ \ " "Widgets and Widget Functions" 15 1
.)T 3 3 "1.3.3\ \ " "Toolkit Functions" 20 1
.)T 3 3 "1.3.4\ \ " "User Interface Language" 22 1
.)T 1 1 "2.\ \ " "Reference Pages" 1 2
.)T 2 0 "" "mwm" "3" 2
.)T 2 0 "" "ApplicationShell" "33" 2
.)T 2 0 "" "Composite" "37" 2
.)T 2 0 "" "Constraint" "40" 2
.)T 2 0 "" "Core" "42" 2
.)T 2 0 "" "MrmCloseHierarchy" "45" 2
.)T 2 0 "" "MrmFetchColorLiteral" "46" 2
.)T 2 0 "" "MrmFetchIconLiteral" "47" 2
.)T 2 0 "" "MrmFetchLiteral" "48" 2
.)T 2 0 "" "MrmFetchSetValues" "49" 2
.)T 2 0 "" "MrmFetchWidget" "51" 2
.)T 2 0 "" "MrmFetchWidgetOverride" "53" 2
.)T 2 0 "" "MrmInitialize" "55" 2
.)T 2 0 "" "MrmOpenHierarchy" "56" 2
.)T 2 0 "" "MrmRegisterClass" "59" 2
.)T 2 0 "" "MrmRegisterNames" "60" 2
.)T 2 0 "" "MrmRegisterNamesInHierarchy" "62" 2
.)T 2 0 "" "Object" "64" 2
.)T 2 0 "" "OverrideShell" "65" 2
.)T 2 0 "" "RectObj" "67" 2
.)T 2 0 "" "Shell" "69" 2
.)T 2 0 "" "TopLevelShell" "72" 2
.)T 2 0 "" "TransientShell" "75" 2
.)T 2 0 "" "Uil" "78" 2
.)T 2 0 "" "VendorShell" "81" 2
.)T 2 0 "" "WMShell" "85" 2
.)T 2 0 "" "XmActivateProtocol" "91" 2
.)T 2 0 "" "XmActivateWMProtocol" "92" 2
.)T 2 0 "" "XmAddProtocolCallback" "93" 2
.)T 2 0 "" "XmAddProtocols" "94" 2
.)T 2 0 "" "XmAddTabGroup" "95" 2
.)T 2 0 "" "XmAddWMProtocolCallback" "96" 2
.)T 2 0 "" "XmAddWMProtocols" "97" 2
.)T 2 0 "" "XmArrowButton" "98" 2
.)T 2 0 "" "XmArrowButtonGadget" "103" 2
.)T 2 0 "" "XmBulletinBoard" "107" 2
.)T 2 0 "" "XmCascadeButton" "114" 2
.)T 2 0 "" "XmCascadeButtonGadget" "120" 2
.)T 2 0 "" "XmCascadeButtonGadgetHighlight" "125" 2
.)T 2 0 "" "XmCascadeButtonHighlight" "126" 2
.)T 2 0 "" "XmClipboardCancelCopy" "127" 2
.)T 2 0 "" "XmClipboardCopy" "128" 2
.)T 2 0 "" "XmClipboardCopyByName" "130" 2
.)T 2 0 "" "XmClipboardEndCopy" "132" 2
.)T 2 0 "" "XmClipboardEndRetrieve" "133" 2
.)T 2 0 "" "XmClipboardInquireCount" "134" 2
.)T 2 0 "" "XmClipboardInquireFormat" "136" 2
.)T 2 0 "" "XmClipboardInquireLength" "138" 2
.)T 2 0 "" "XmClipboardInquirePendingItems" "140" 2
.)T 2 0 "" "XmClipboardLock" "142" 2
.)T 2 0 "" "XmClipboardRegisterFormat" "143" 2
.)T 2 0 "" "XmClipboardRetrieve" "144" 2
.)T 2 0 "" "XmClipboardStartCopy" "146" 2
.)T 2 0 "" "XmClipboardStartRetrieve" "148" 2
.)T 2 0 "" "XmClipboardUndoCopy" "149" 2
.)T 2 0 "" "XmClipboardUnlock" "150" 2
.)T 2 0 "" "XmClipboardWithdrawFormat" "151" 2
.)T 2 0 "" "XmCommand" "152" 2
.)T 2 0 "" "XmCommandAppendValue" "158" 2
.)T 2 0 "" "XmCommandError" "159" 2
.)T 2 0 "" "XmCommandGetChild" "160" 2
.)T 2 0 "" "XmCommandSetValue" "161" 2
.)T 2 0 "" "XmConvertUnits" "162" 2
.)T 2 0 "" "XmCreateArrowButton" "163" 2
.)T 2 0 "" "XmCreateArrowButtonGadget" "164" 2
.)T 2 0 "" "XmCreateBulletinBoard" "165" 2
.)T 2 0 "" "XmCreateBulletinBoardDialog" "166" 2
.)T 2 0 "" "XmCreateCascadeButton" "167" 2
.)T 2 0 "" "XmCreateCascadeButtonGadget" "168" 2
.)T 2 0 "" "XmCreateCommand" "169" 2
.)T 2 0 "" "XmCreateDialogShell" "170" 2
.)T 2 0 "" "XmCreateDrawingArea" "171" 2
.)T 2 0 "" "XmCreateDrawnButton" "172" 2
.)T 2 0 "" "XmCreateErrorDialog" "173" 2
.)T 2 0 "" "XmCreateFileSelectionBox" "174" 2
.)T 2 0 "" "XmCreateFileSelectionDialog" "175" 2
.)T 2 0 "" "XmCreateForm" "176" 2
.)T 2 0 "" "XmCreateFormDialog" "177" 2
.)T 2 0 "" "XmCreateFrame" "178" 2
.)T 2 0 "" "XmCreateInformationDialog" "179" 2
.)T 2 0 "" "XmCreateLabel" "180" 2
.)T 2 0 "" "XmCreateLabelGadget" "181" 2
.)T 2 0 "" "XmCreateList" "182" 2
.)T 2 0 "" "XmCreateMainWindow" "183" 2
.)T 2 0 "" "XmCreateMenuBar" "184" 2
.)T 2 0 "" "XmCreateMenuShell" "186" 2
.)T 2 0 "" "XmCreateMessageBox" "187" 2
.)T 2 0 "" "XmCreateMessageDialog" "188" 2
.)T 2 0 "" "XmCreateOptionMenu" "189" 2
.)T 2 0 "" "XmCreatePanedWindow" "191" 2
.)T 2 0 "" "XmCreatePopupMenu" "192" 2
.)T 2 0 "" "XmCreatePromptDialog" "194" 2
.)T 2 0 "" "XmCreatePulldownMenu" "195" 2
.)T 2 0 "" "XmCreatePushButton" "197" 2
.)T 2 0 "" "XmCreatePushButtonGadget" "198" 2
.)T 2 0 "" "XmCreateQuestionDialog" "199" 2
.)T 2 0 "" "XmCreateRadioBox" "200" 2
.)T 2 0 "" "XmCreateRowColumn" "201" 2
.)T 2 0 "" "XmCreateScale" "202" 2
.)T 2 0 "" "XmCreateScrollBar" "203" 2
.)T 2 0 "" "XmCreateScrolledList" "204" 2
.)T 2 0 "" "XmCreateScrolledText" "206" 2
.)T 2 0 "" "XmCreateScrolledWindow" "208" 2
.)T 2 0 "" "XmCreateSelectionBox" "209" 2
.)T 2 0 "" "XmCreateSelectionDialog" "211" 2
.)T 2 0 "" "XmCreateSeparator" "213" 2
.)T 2 0 "" "XmCreateSeparatorGadget" "214" 2
.)T 2 0 "" "XmCreateText" "215" 2
.)T 2 0 "" "XmCreateToggleButton" "216" 2
.)T 2 0 "" "XmCreateToggleButtonGadget" "217" 2
.)T 2 0 "" "XmCreateWarningDialog" "218" 2
.)T 2 0 "" "XmCreateWorkArea" "219" 2
.)T 2 0 "" "XmCreateWorkingDialog" "220" 2
.)T 2 0 "" "XmCvtCTToXmString" "221" 2
.)T 2 0 "" "XmCvtXmStringToCT" "222" 2
.)T 2 0 "" "XmDeactivateProtocol" "223" 2
.)T 2 0 "" "XmDeactivateWMProtocol" "224" 2
.)T 2 0 "" "XmDestroyPixmap" "225" 2
.)T 2 0 "" "XmDialogShell" "226" 2
.)T 2 0 "" "XmDrawingArea" "229" 2
.)T 2 0 "" "XmDrawnButton" "233" 2
.)T 2 0 "" "XmFileSelectionBox" "239" 2
.)T 2 0 "" "XmFileSelectionBoxGetChild" "250" 2
.)T 2 0 "" "XmFileSelectionDoSearch" "252" 2
.)T 2 0 "" "XmFontList" "253" 2
.)T 2 0 "" "XmFontListAdd" "254" 2
.)T 2 0 "" "XmFontListCreate" "255" 2
.)T 2 0 "" "XmFontListFree" "256" 2
.)T 2 0 "" "XmForm" "257" 2
.)T 2 0 "" "XmFrame" "271" 2
.)T 2 0 "" "XmGadget" "274" 2
.)T 2 0 "" "XmGetAtomName" "276" 2
.)T 2 0 "" "XmGetColors" "277" 2
.)T 2 0 "" "XmGetMenuCursor" "278" 2
.)T 2 0 "" "XmGetPixmap" "279" 2
.)T 2 0 "" "XmGetPostedFromWidget" "281" 2
.)T 2 0 "" "XmInstallImage" "282" 2
.)T 2 0 "" "XmInternAtom" "283" 2
.)T 2 0 "" "XmIsMotifWMRunning" "284" 2
.)T 2 0 "" "XmLabel" "285" 2
.)T 2 0 "" "XmLabelGadget" "292" 2
.)T 2 0 "" "XmList" "298" 2
.)T 2 0 "" "XmListAddItem" "311" 2
.)T 2 0 "" "XmListAddItems" "312" 2
.)T 2 0 "" "XmListAddItemUnselected" "313" 2
.)T 2 0 "" "XmListDeleteAllItems" "314" 2
.)T 2 0 "" "XmListDeleteItem" "315" 2
.)T 2 0 "" "XmListDeleteItems" "316" 2
.)T 2 0 "" "XmListDeleteItemsPos" "317" 2
.)T 2 0 "" "XmListDeletePos" "318" 2
.)T 2 0 "" "XmListDeselectAllItems" "319" 2
.)T 2 0 "" "XmListDeselectItem" "320" 2
.)T 2 0 "" "XmListDeselectPos" "321" 2
.)T 2 0 "" "XmListGetMatchPos" "322" 2
.)T 2 0 "" "XmListGetSelectedPos" "323" 2
.)T 2 0 "" "XmListItemExists" "324" 2
.)T 2 0 "" "XmListItemPos" "325" 2
.)T 2 0 "" "XmListReplaceItems" "326" 2
.)T 2 0 "" "XmListReplaceItemsPos" "327" 2
.)T 2 0 "" "XmListSelectItem" "328" 2
.)T 2 0 "" "XmListSelectPos" "329" 2
.)T 2 0 "" "XmListSetAddMode" "330" 2
.)T 2 0 "" "XmListSetBottomItem" "331" 2
.)T 2 0 "" "XmListSetBottomPos" "332" 2
.)T 2 0 "" "XmListSetHorizPos" "333" 2
.)T 2 0 "" "XmListSetItem" "334" 2
.)T 2 0 "" "XmListSetPos" "335" 2
.)T 2 0 "" "XmMainWindow" "336" 2
.)T 2 0 "" "XmMainWindowSep1" "340" 2
.)T 2 0 "" "XmMainWindowSep2" "341" 2
.)T 2 0 "" "XmMainWindowSep3" "342" 2
.)T 2 0 "" "XmMainWindowSetAreas" "343" 2
.)T 2 0 "" "XmManager" "344" 2
.)T 2 0 "" "XmMenuPosition" "349" 2
.)T 2 0 "" "XmMenuShell" "350" 2
.)T 2 0 "" "XmMessageBox" "354" 2
.)T 2 0 "" "XmMessageBoxGetChild" "360" 2
.)T 2 0 "" "XmOptionButtonGadget" "361" 2
.)T 2 0 "" "XmOptionLabelGadget" "362" 2
.)T 2 0 "" "XmPanedWindow" "363" 2
.)T 2 0 "" "XmPrimitive" "368" 2
.)T 2 0 "" "XmProcessTraversal" "373" 2
.)T 2 0 "" "XmPushButton" "375" 2
.)T 2 0 "" "XmPushButtonGadget" "382" 2
.)T 2 0 "" "XmRemoveProtocolCallback" "388" 2
.)T 2 0 "" "XmRemoveProtocols" "389" 2
.)T 2 0 "" "XmRemoveTabGroup" "390" 2
.)T 2 0 "" "XmRemoveWMProtocolCallback" "391" 2
.)T 2 0 "" "XmRemoveWMProtocols" "392" 2
.)T 2 0 "" "XmResolvePartOffsets" "393" 2
.)T 2 0 "" "XmRowColumn" "395" 2
.)T 2 0 "" "XmScale" "407" 2
.)T 2 0 "" "XmScaleGetValue" "415" 2
.)T 2 0 "" "XmScaleSetValue" "416" 2
.)T 2 0 "" "XmScrollBar" "417" 2
.)T 2 0 "" "XmScrollBarGetValues" "425" 2
.)T 2 0 "" "XmScrollBarSetValues" "426" 2
.)T 2 0 "" "XmScrolledWindow" "427" 2
.)T 2 0 "" "XmScrolledWindowSetAreas" "433" 2
.)T 2 0 "" "XmSelectionBox" "434" 2
.)T 2 0 "" "XmSelectionBoxGetChild" "442" 2
.)T 2 0 "" "XmSeparator" "443" 2
.)T 2 0 "" "XmSeparatorGadget" "446" 2
.)T 2 0 "" "XmSetMenuCursor" "449" 2
.)T 2 0 "" "XmSetProtocolHooks" "450" 2
.)T 2 0 "" "XmSetWMProtocolHooks" "451" 2
.)T 2 0 "" "XmString" "452" 2
.)T 2 0 "" "XmStringBaseline" "453" 2
.)T 2 0 "" "XmStringByteCompare" "454" 2
.)T 2 0 "" "XmStringCompare" "455" 2
.)T 2 0 "" "XmStringConcat" "456" 2
.)T 2 0 "" "XmStringCopy" "457" 2
.)T 2 0 "" "XmStringCreate" "458" 2
.)T 2 0 "" "XmStringCreateSimple" "459" 2
.)T 2 0 "" "XmStringDirection" "460" 2
.)T 2 0 "" "XmStringDraw" "461" 2
.)T 2 0 "" "XmStringDrawImage" "462" 2
.)T 2 0 "" "XmStringDrawUnderline" "464" 2
.)T 2 0 "" "XmStringEmpty" "466" 2
.)T 2 0 "" "XmStringExtent" "467" 2
.)T 2 0 "" "XmStringFree" "468" 2
.)T 2 0 "" "XmStringHasSubstring" "469" 2
.)T 2 0 "" "XmStringHeight" "470" 2
.)T 2 0 "" "XmStringLength" "471" 2
.)T 2 0 "" "XmStringLineCount" "472" 2
.)T 2 0 "" "XmStringNConcat" "473" 2
.)T 2 0 "" "XmStringNCopy" "474" 2
.)T 2 0 "" "XmStringSeparatorCreate" "475" 2
.)T 2 0 "" "XmStringTable" "476" 2
.)T 2 0 "" "XmStringWidth" "477" 2
.)T 2 0 "" "XmText" "478" 2
.)T 2 0 "" "XmTextClearSelection" "497" 2
.)T 2 0 "" "XmTextCopy" "498" 2
.)T 2 0 "" "XmTextCut" "499" 2
.)T 2 0 "" "XmTextGetBaseline" "500" 2
.)T 2 0 "" "XmTextGetEditable" "501" 2
.)T 2 0 "" "XmTextGetInsertionPosition" "502" 2
.)T 2 0 "" "XmTextGetLastPosition" "503" 2
.)T 2 0 "" "XmTextGetMaxLength" "504" 2
.)T 2 0 "" "XmTextGetSelection" "505" 2
.)T 2 0 "" "XmTextGetSelectionPosition" "506" 2
.)T 2 0 "" "XmTextGetSource" "507" 2
.)T 2 0 "" "XmTextGetString" "508" 2
.)T 2 0 "" "XmTextGetTopCharacter" "509" 2
.)T 2 0 "" "XmTextInsert" "510" 2
.)T 2 0 "" "XmTextPaste" "511" 2
.)T 2 0 "" "XmTextPosToXY" "512" 2
.)T 2 0 "" "XmTextPosition" "513" 2
.)T 2 0 "" "XmTextRemove" "514" 2
.)T 2 0 "" "XmTextReplace" "515" 2
.)T 2 0 "" "XmTextScroll" "516" 2
.)T 2 0 "" "XmTextSetAddMode" "517" 2
.)T 2 0 "" "XmTextSetEditable" "518" 2
.)T 2 0 "" "XmTextSetHighlight" "519" 2
.)T 2 0 "" "XmTextSetInsertionPosition" "520" 2
.)T 2 0 "" "XmTextSetMaxLength" "521" 2
.)T 2 0 "" "XmTextSetSelection" "522" 2
.)T 2 0 "" "XmTextSetSource" "523" 2
.)T 2 0 "" "XmTextSetString" "524" 2
.)T 2 0 "" "XmTextSetTopCharacter" "525" 2
.)T 2 0 "" "XmTextShowPosition" "526" 2
.)T 2 0 "" "XmTextXYToPos" "527" 2
.)T 2 0 "" "XmToggleButton" "528" 2
.)T 2 0 "" "XmToggleButtonGadget" "536" 2
.)T 2 0 "" "XmToggleButtonGadgetGetState" "543" 2
.)T 2 0 "" "XmToggleButtonGadgetSetState" "544" 2
.)T 2 0 "" "XmToggleButtonGetState" "545" 2
.)T 2 0 "" "XmToggleButtonSetState" "546" 2
.)T 2 0 "" "XmTrackingLocate" "547" 2
.)T 2 0 "" "XmUninstallImage" "548" 2
.)T 2 0 "" "XmUpdateDisplay" "549" 2
.)T 1 0 "" "Index" 1 Index
...\"	tocmac2.mm  --  generate the table of contents.  This file is
...\"	the complement to tocmac1.mm.
...\"
...\"	close the definition started in tocmac1.mm:
..
...\"
...\"	call the table of contents
.TC 1 1 3 0
...\".EH ""
...\".OH ""
...\".PH ""
...\".bp
...\".af `p \\gP
...\".af P 1
...\".nr `p \\nP
...\".af P \\g(`p
...\".af `p 1
...\".if t \{\ .if !\\n(`p%2 \{\
...\".rs
...\".EF ""
...\".OF ""
...\".PF ""
...\"\ \" eject blank page
...\".bp
...\"'br\}
