/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	Traps.h,v $
 * Revision 2.2  91/09/12  16:51:38  bohman
 * 	Created.
 * 	[91/09/11  16:35:56  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Traps.h
 */

/************************************************************

Created: Saturday, September 16, 1989 at 3:18 PM
	Traps.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	1985-1989
	All rights reserved

************************************************************/


#ifndef __TRAPS__
#define __TRAPS__

/*	Controls Traps	*/

#define DisposControl					0xA955
#define DragControl					0xA967
#define Draw1Control					0xA96D
#define DrawControls					0xA969
#define FindControl					0xA96C
#define GetAuxCtl						0xAA44
#define GetCRefCon 					0xA95A
#define GetCTitle						0xA95E
#define GetCtlAction					0xA96A
#define GetCtlValue					0xA960
#define GetCVariant					0xA809
#define GetMaxCtl						0xA962
#define GetMinCtl						0xA961
#define GetNewControl					0xA9BE
#define HideControl					0xA958
#define HiliteControl					0xA95D
#define KillControls					0xA956
#define MoveControl					0xA959
#define NewControl 					0xA954
#define SetCRefCon 					0xA95B
#define SetCTitle						0xA95F
#define SetCtlAction					0xA96B
#define SetCtlColor					0xAA43
#define SetCtlValue					0xA963
#define SetMaxCtl						0xA965
#define SetMinCtl						0xA964
#define ShowControl					0xA957
#define SizeControl					0xA95C
#define TestControl					0xA966
#define TrackControl					0xA968
#define UpdtControl					0xA953

/*	Desk Traps	*/

#define CloseDeskAcc					0xA9B7
#define OpenDeskAcc					0xA9B6
#define SysEdit						0xA9C2
#define SystemClick					0xA9B3
#define SystemEvent					0xA9B2
#define SystemMenu 					0xA9B5
#define SystemTask 					0xA9B4

/*	DeskBus Traps  */

#define ADBReInit						0xA07B

/*	Dialogs Traps  */

#define NewCDialog 					0xAA4B
#define Alert							0xA985
#define CautionAlert					0xA988
#define CloseDialog					0xA982
#define CouldAlert 					0xA989
#define CouldDialog					0xA979
#define DialogSelect					0xA980
#define DisposDialog					0xA983
#define DrawDialog 					0xA981
#define ErrorSound 					0xA98C
#define FindDItem						0xA984
#define FreeAlert						0xA98A
#define FreeDialog 					0xA97A
#define GetDItem						0xA98D
#define GetIText						0xA990
#define GetNewDialog					0xA97C
#define HideDItem						0xA827
#define InitDialogs					0xA97B
#define IsDialogEvent					0xA97F
#define ModalDialog					0xA991
#define NewDialog						0xA97D
#define NoteAlert						0xA987
#define ParamText						0xA98B
#define SelIText						0xA97E
#define SetDItem						0xA98E
#define SetIText						0xA98F
#define ShowDItem						0xA828
#define StopAlert						0xA986
#define UpdtDialog 					0xA978
#define StdOpcodeProc					0xABF8

/*	Events Traps  */

#define Button 						0xA974
#define EventAvail 					0xA971
#define GetKeys						0xA976
#define GetMouse						0xA972
#define GetNextEvent					0xA970
#define TickCount						0xA975
#define WaitMouseUp					0xA977
#define WaitNextEvent					0xA860

/*	FixMath Traps  */

#define FixATan2						0xA818

/*	Fonts Traps  */

#define SetFractEnable 				0xA814
#define FMSwapFont 					0xA901
#define FontMetrics					0xA835
#define GetFName						0xA8FF
#define GetFNum						0xA900
#define InitFonts						0xA8FE
#define RealFont						0xA902
#define SetFontLock					0xA903
#define SetFScaleDisable				0xA834

/*	Menus Traps  */

#define DelMCEntries					0xAA60
#define DispMCInfo 					0xAA63
#define GetMCEntry 					0xAA64
#define GetMCInfo						0xAA61
#define MenuChoice 					0xAA66
#define SetMCEntries					0xAA65
#define SetMCInfo						0xAA62
#define AddResMenu 					0xA94D
#define AppendMenu 					0xA933
#define CalcMenuSize					0xA948
#define CheckItem						0xA945
#define ClearMenuBar					0xA934
#define CountMItems					0xA950
#define DeleteMenu 					0xA936
#define DelMenuItem					0xA952
#define DisableItem					0xA93A
#define DisposMenu 					0xA932
#define DrawMenuBar					0xA937
#define EnableItem 					0xA939
#define FlashMenuBar					0xA94C
#define GetItem						0xA946
#define GetItemCmd 					0xA84E
#define GetItmIcon 					0xA93F
#define GetItmMark 					0xA943
#define GetMenuBar 					0xA93B
#define GetMHandle 					0xA949
#define GetNewMBar 					0xA9C0
#define GetRMenu						0xA9BF
#define HiliteMenu 					0xA938
#define InitMenus						0xA930
#define InitProcMenu					0xA808
#define InsertMenu 					0xA935
#define InsertResMenu					0xA951
#define InsMenuItem					0xA826
#define MenuKey						0xA93E
#define MenuSelect 					0xA93D
#define NewMenu						0xA931
#define PopUpMenuSelect				0xA80B
#define SetItem						0xA947
#define SetItemCmd 					0xA84F
#define SetItmIcon 					0xA940
#define SetItmMark 					0xA944
#define SetItmStyle					0xA942
#define SetMenuBar 					0xA93C
#define SetMFlash						0xA94A

/*	OSUtils Traps  */

#define KeyTrans						0xA9C3
#define SysBeep						0xA9C8
#define Unimplemented					0xA89F
#define HWPriv 						0xA198
#define InitDogCow 					0xA89F
#define EnableDogCow					0xA89F
#define DisableDogCow					0xA89F
#define Moof							0xA89F

/*	Packages Traps	*/

#define InitAllPacks					0xA9E6
#define InitPack						0xA9E5

/*	Quickdraw Traps  */

#define AddComp						0xAA3B
#define AddPt							0xA87E
#define AddSearch						0xAA3A
#define AllocCursor					0xAA1D
#define BackColor						0xA863
#define BackPat						0xA87C
#define BackPixPat 					0xAA0B
#define CalcMask						0xA838
#define CharExtra						0xAA23
#define CharWidth						0xA88D
#define ClipRect						0xA87B
#define CloseCport 					0xA87D
#define ClosePgon						0xA8CC
#define ClosePicture					0xA8F4
#define ClosePort						0xA87D
#define CloseRgn						0xA8DB
#define Color2Index					0xAA33
#define ColorBit						0xA864
#define CopyBits						0xA8EC
#define CopyMask						0xA817
#define CopyPixMap 					0xAA05
#define CopyPixPat 					0xAA09
#define CopyRgn						0xA8DC
#define DelComp						0xAA4D
#define DelSearch						0xAA4C
#define DiffRgn						0xA8E6
#define DisposCCursor					0xAA26
#define DisposCIcon					0xAA25
#define DisposCTable					0xAA24
#define DisposGDevice					0xAA30
#define DisposPixMap					0xAA04
#define DisposPixPat					0xAA08
#define DisposRgn						0xA8D9
#define DrawChar						0xA883
#define DrawPicture					0xA8F6
#define DrawString 					0xA884
#define DrawText						0xA885
#define EmptyRect						0xA8AE
#define EmptyRgn						0xA8E2
#define EqualPt						0xA881
#define EqualRect						0xA8A6
#define EqualRgn						0xA8E3
#define EraseArc						0xA8C0
#define EraseOval						0xA8B9
#define ErasePoly						0xA8C8
#define EraseRect						0xA8A3
#define EraseRgn						0xA8D4
#define EraseRoundRect 				0xA8B2
#define FillArc						0xA8C2
#define FillCArc						0xAA11
#define FillCOval						0xAA0F
#define FillCPoly						0xAA13
#define FillCRect						0xAA0E
#define FillCRgn						0xAA12
#define FillCRoundRect 				0xAA10
#define FillOval						0xA8BB
#define FillPoly						0xA8CA
#define FillRect						0xA8A5
#define FillRgn						0xA8D6
#define BitMapToRegion 				0xA8D7
#define FillRoundRect					0xA8B4
#define ForeColor						0xA862
#define FrameArc						0xA8BE
#define FrameOval						0xA8B7
#define FramePoly						0xA8C6
#define FrameRect						0xA8A1
#define FrameRgn						0xA8D2
#define FrameRoundRect 				0xA8B0
#define GetBackColor					0xAA1A
#define GetCCursor 					0xAA1B
#define GetCIcon						0xAA1E
#define GetClip						0xA87A
#define GetCPixel						0xAA17
#define GetCTable						0xAA18
#define GetCTSeed						0xAA28
#define GetDeviceList					0xAA29
#define GetFontInfo					0xA88B
#define GetForeColor					0xAA19
#define GetGDevice 					0xAA32
#define GetMainDevice					0xAA2A
#define GetMaxDevice					0xAA27
#define GetNextDevice					0xAA2B
#define GetPen 						0xA89A
#define GetPenState					0xA898
#define GetPixel						0xA865
#define GetPixPat						0xAA0C
#define GetPort						0xA874
#define GetSubTable					0xAA37
#define GlobalToLocal					0xA871
#define GrafDevice 					0xA872
#define HideCursor 					0xA852
#define HidePen						0xA896
#define HiliteColor					0xAA22
#define Index2Color					0xAA34
#define InitCport						0xAA01
#define InitCursor 					0xA850
#define InitGDevice					0xAA2E
#define InitGraf						0xA86E
#define InitPort						0xA86D
#define InSetRect						0xA8A9
#define InSetRgn						0xA8E1
#define InverRect						0xA8A4
#define InverRgn						0xA8D5
#define InverRoundRect 				0xA8B3
#define InvertArc						0xA8C1
#define InvertColor					0xAA35
#define InvertOval 					0xA8BA
#define InvertPoly 					0xA8C9
#define KillPicture					0xA8F5
#define KillPoly						0xA8CD
#define Line							0xA892
#define LineTo 						0xA891
#define LocalToGlobal					0xA870
#define MakeITable 					0xAA39
#define MakeRGBPat 					0xAA0D
#define MapPoly						0xA8FC
#define MapPt							0xA8F9
#define MapRect						0xA8FA
#define MapRgn 						0xA8FB
#define MeasureText					0xA837
#define Move							0xA894
#define MovePortTo 					0xA877
#define MoveTo 						0xA893
#define NewGDevice 					0xAA2F
#define NewPixMap						0xAA03
#define NewPixPat						0xAA07
#define NewRgn 						0xA8D8
#define ObscureCursor					0xA856
#define OffSetPoly 					0xA8CE
#define OffSetRect 					0xA8A8
#define OfSetRgn						0xA8E0
#define OpColor						0xAA21
#define OpenCport						0xAA00
#define OpenPicture					0xA8F3
#define OpenPoly						0xA8CB
#define OpenPort						0xA86F
#define OpenRgn						0xA8DA
#define PaintArc						0xA8BF
#define PaintOval						0xA8B8
#define PaintPoly						0xA8C7
#define PaintRect						0xA8A2
#define PaintRgn						0xA8D3
#define PaintRoundRect 				0xA8B1
#define PenMode						0xA89C
#define PenNormal						0xA89E
#define PenPat 						0xA89D
#define PenPixPat						0xAA0A
#define PenSize						0xA89B
#define PicComment 					0xA8F2
#define PlotCIcon						0xAA1F
#define PortSize						0xA876
#define ProtectEntry					0xAA3D
#define Pt2Rect						0xA8AC
#define PtInRect						0xA8AD
#define PtInRgn						0xA8E8
#define PtToAngle						0xA8C3
#define QDError						0xAA40
#define Random 						0xA861
#define RealColor						0xAA36
#define RectInRgn						0xA8E9
#define RectRgn						0xA8DF
#define ReserveEntry					0xAA3E
#define RestoreEntries 				0xAA4A
#define RGBBackColor					0xAA15
#define RGBForeColor					0xAA14
#define SaveEntries					0xAA49
#define ScalePt						0xA8F8
#define ScrollRect 					0xA8EF
#define SectRect						0xA8AA
#define SectRgn						0xA8E4
#define SeedCFill						0xAA50
#define SeedFill						0xA839
#define SetCCursor 					0xAA1C
#define SetClientID					0xAA3C
#define SetClip						0xA879
#define SetCPixel						0xAA16
#define SetPortPix 					0xAA06
#define SetCursor						0xA851
#define SetDeviceAttribute 			0xAA2D
#define SetEmptyRgn					0xA8DD
#define SetEntries 					0xAA3F
#define SetGDevice 					0xAA31
#define SetOrigin						0xA878
#define SetPBits						0xA875
#define SetPenState					0xA899
#define SetPort						0xA873
#define SetPt							0xA880
#define SetRecRgn						0xA8DE
#define SetRect						0xA8A7
#define SetStdCProcs					0xAA4E
#define SetStdProcs					0xA8EA
#define ShowCursor 					0xA853
#define ShowPen						0xA897
#define SpaceExtra 					0xA88E
#define StdArc 						0xA8BD
#define StdBits						0xA8EB
#define StdComment 					0xA8F1
#define StdGetPic						0xA8EE
#define StdLine						0xA890
#define StdOval						0xA8B6
#define StdPoly						0xA8C5
#define StdPutPic						0xA8F0
#define StdRect						0xA8A0
#define StdRgn 						0xA8D1
#define StdRRect						0xA8AF
#define StdText						0xA882
#define StdTxMeas						0xA8ED
#define StringWidth					0xA88C
#define StuffHex						0xA866
#define SubPt							0xA87F
#define TestDeviceAttribute			0xAA2C
#define TextFace						0xA888
#define TextFont						0xA887
#define TextMode						0xA889
#define TextSize						0xA88A
#define TextWidth						0xA886
#define UnionRect						0xA8AB
#define UnionRgn						0xA8E5
#define XOrRgn 						0xA8E7
#define CalcCMask						0xAA4F
#define GetMaskTable					0xA836
#define UpdatePixMap					0xAA38

/*	Resources Traps  */

#define AddResource					0xA9AB
#define ChangedResource				0xA9AA
#define CloseResFile					0xA99A
#define Count1Resources				0xA80D
#define Count1Types					0xA81C
#define CountResources 				0xA99C
#define CountTypes 					0xA99E
#define CreateResFile					0xA9B1
#define CurResFile 					0xA994
#define DetachResource 				0xA992
#define Get1IxResource 				0xA80E
#define Get1IxType 					0xA80F
#define Get1NamedResource				0xA820
#define Get1Resource					0xA81F
#define GetIndResource 				0xA99D
#define GetIndType 					0xA99F
#define GetNamedResource				0xA9A1
#define GetResAttrs					0xA9A6
#define GetResFileAttrs				0xA9F6
#define GetResInfo 					0xA9A8
#define GetResource					0xA9A0
#define HomeResFile					0xA9A4
#define InitResources					0xA995
#define LoadResource					0xA9A2
#define MaxSizeRsrc					0xA821
#define OpenResFile					0xA997
#define OpenRFPerm 					0xA9C4
#define ReleaseResource				0xA9A3
#define ResError						0xA9AF
#define RGetResource					0xA80C
#define RmveResource					0xA9AD
#define RsrcMapEntry					0xA9C5
#define RsrcZoneInit					0xA996
#define SetResAttrs					0xA9A7
#define SetResFileAttrs				0xA9F7
#define SetResInfo 					0xA9A9
#define SetResLoad 					0xA99B
#define SetResPurge					0xA993
#define SizeRsrc						0xA9A5
#define Unique1ID						0xA810
#define UniqueID						0xA9C1
#define UpdateResFile					0xA999
#define UseResFile 					0xA998
#define WriteResource					0xA9B0
#define Pack8							0xA816
#define Pack9							0xA82B
#define Pack10 						0xA82C
#define Pack11 						0xA82D
#define Pack12 						0xA82E
#define Pack13 						0xA82F
#define Pack14 						0xA830
#define Pack15 						0xA831
#define ScrnBitMap 					0xA833
#define DragTheRgn 					0xA926
#define GetItmStyle					0xA941
#define PlotIcon						0xA94B
#define Dequeue						0xA96E
#define Enqueue						0xA96F
#define StillDown						0xA973
#define AddReference					0xA9AC
#define RmveReference					0xA9AE
#define Secs2Date						0xA9C6
#define Date2Secs						0xA9C7
#define SysError						0xA9C9
#define HandToHand 					0xA9E1
#define PtrToXHand 					0xA9E2
#define PtrToHand						0xA9E3
#define HandAndHand					0xA9E4
#define Pack0							0xA9E7
#define Pack1							0xA9E8
#define Pack2							0xA9E9
#define Pack3							0xA9EA
#define FP68K							0xA9EB
#define Pack4							0xA9EB
#define Elems68K						0xA9EC
#define Pack5							0xA9EC
#define Pack6							0xA9ED
#define DECSTR68K						0xA9EE
#define Pack7							0xA9EE
#define PtrAndHand 					0xA9EF
#define LoadSeg						0xA9F0
#define Launch 						0xA9F2
#define Chain							0xA9F3
#define MethodDispatch 				0xA9F8
#define Debugger						0xA9FF
#define DebugStr						0xABFF

/*	Scrap Traps  */

#define GetScrap						0xA9FD
#define InfoScrap						0xA9F9
#define LodeScrap						0xA9FB
#define LoadScrap						0xA9FB
#define PutScrap						0xA9FE
#define UnlodeScrap					0xA9FA
#define UnloadScrap					0xA9FA
#define ZeroScrap						0xA9FC

/*	SegLoad Traps  */

#define ExitToShell					0xA9F4
#define GetAppParms					0xA9F5
#define UnLoadSeg						0xA9F1

/*	ShutDown Traps	*/

#define ShutDown						0xA895

/*	TextEdit Traps	*/

#define TEActivate 					0xA9D8
#define TEAutoView 					0xA813
#define TECalText						0xA9D0
#define TEClick						0xA9D4
#define TECopy 						0xA9D5
#define TECut							0xA9D6
#define TEDeactivate					0xA9D9
#define TEDelete						0xA9D7
#define TEDispose						0xA9CD
#define TEGetOffset					0xA83C
#define TEGetText						0xA9CB
#define TEIdle 						0xA9DA
#define TEInit 						0xA9CC
#define TEInsert						0xA9DE
#define TEKey							0xA9DC
#define TENew							0xA9D2
#define TEPaste						0xA9DB
#define TEPinScroll					0xA812
#define TEScroll						0xA9DD
#define TESelView						0xA811
#define TESetJust						0xA9DF
#define TESetSelect					0xA9D1
#define TESetText						0xA9CF
#define TEStyleNew 					0xA83E
#define TEUpdate						0xA9D3
#define TextBox						0xA9CE

/*	ToolUtils Traps  */

#define UnpackBits 					0xA8D0
#define AngleFromSlope 				0xA8C4
#define BitAnd 						0xA858
#define BitClr 						0xA85F
#define BitNot 						0xA85A
#define BitOr							0xA85B
#define BitSet 						0xA85E
#define BitShift						0xA85C
#define BitTst 						0xA85D
#define BitXOr 						0xA859
#define DeltaPoint 					0xA94F
#define FixMul 						0xA868
#define FixRatio						0xA869
#define FixRound						0xA86C
#define GetCursor						0xA9B9
#define GetIcon						0xA9BB
#define GetPattern 					0xA9B8
#define GetPicture 					0xA9BC
#define GetString						0xA9BA
#define HiWord 						0xA86A
#define LongMul						0xA867
#define LoWord 						0xA86B
#define Munger 						0xA9E0
#define NewString						0xA906
#define PackBits						0xA8CF
#define SetString						0xA907
#define ShieldCursor					0xA855
#define SlopeFromAngle 				0xA8BC
#define XMunger						0xA819
#define WriteParam 					0xA038
#define Open							0xA000
 /*
 ______________________________________________________________________

  Core routine system TRAPS

 ______________________________________________________________________


  First we have the I/O core routines. These are also used by
  the file system.

*/
#define Close							0xA001
#define Read							0xA002
#define Write							0xA003
#define Control						0xA004
#define Status 						0xA005
#define KillIO 						0xA006
#define GetVInfo 					0xA007
#define Create 						0xA008
#define Delete 						0xA009
#define OpenRF 						0xA00A
#define ReName 						0xA00B
#define GetFInfo					0xA00C
#define SetFInfo					0xA00D
#define UnmountVol 					0xA00E
#define MountVol						0xA00F
#define Allocate						0xA010
#define GetEOF 						0xA011
#define SetEOF 						0xA012
#define FlushVol						0xA013
#define GetVol 						0xA014
#define SetVol 						0xA015
#define FInitQueue 					0xA016
#define Eject							0xA017
#define GetFPos						0xA018
#define HSetVol						0xA215
#define HGetVol						0xA214
#define HOpen							0xA200
#define HGetVInfo						0xA207
#define HCreate						0xA208
#define HDelete						0xA209
#define HOpenRF						0xA20A
#define HRename						0xA20B
#define HGetFInfo					0xA20C
#define HSetFInfo					0xA20D
#define AllocContig					0xA210
#define HSetFLock						0xA241
#define HRstFLock						0xA242
#define InitZone						0xA019
 /* end of HFS additions

  Here are the memory manager core routines
*/
#define GetZone						0xA11A
#define SetZone						0xA01B
#define FreeMem						0xA01C
#define MaxMem 						0xA11D
#define NewPtr 						0xA11E
#define DisposPtr						0xA01F
#define SetPtrSize 					0xA020
#define GetPtrSize 					0xA021
#define NewHandle						0xA122
#define DisposHandle					0xA023
#define SetHandleSize					0xA024
#define GetHandleSize					0xA025
#define HandleZone 					0xA126
#define ReAllocHandle					0xA027
#define RecoverHandle					0xA128
#define HLock							0xA029
#define HUnlock						0xA02A
#define EmptyHandle					0xA02B
#define InitApplZone					0xA02C
#define SetApplLimit					0xA02D
#define BlockMove						0xA02E
#define PostEvent						0xA02F
 /*  Here are the event manager routines
*/
#define PPostEvent 					0xA12F
#define OSEventAvail					0xA030
#define GetOSEvent 					0xA031
#define FlushEvents					0xA032
#define VInstall						0xA033
 /*  Here are the utility core routines
*/
#define VRemove						0xA034
#define OffLine						0xA035
#define MoreMasters					0xA036
#define ReadDateTime					0xA039
#define SetDateTime					0xA03A
#define Delay							0xA03B
#define CmpString						0xA03C
#define DrvrInstall					0xA03D
#define DrvrRemove 					0xA03E
#define InitUtil						0xA03F
#define ResrvMem						0xA040
#define SetFilLock 					0xA041
#define RstFilLock 					0xA042
#define SetFilType 					0xA043
#define SetFPos						0xA044
#define FlushFile						0xA045
#define GetTrapAddress 				0xA146
#define SetTrapAddress 				0xA047
#define PtrZone						0xA148
#define HPurge 						0xA049
#define HNoPurge						0xA04A
#define SetGrowZone					0xA04B
#define CompactMem 					0xA04C
#define PurgeMem						0xA04D
#define AddDrive						0xA04E
#define RDrvrInstall					0xA04F
#define UprString						0xA054
#define LwrString						0xA056
#define SetApplBase					0xA057
#define OSDispatch 					0xA88F
#define RelString						0xA050
 /* new 128K ROM additions
*/
#define ReadXPRam						0xA051
#define WriteXPRam						0xA052
#define InsTime						0xA058
#define RmvTime						0xA059
#define PrimeTime						0xA05A
#define xFSDispatch					0xA060
#define HFSDispatch					0xA260
#define MaxBlock						0xA061
#define PurgeSpace 					0xA162
#define MaxApplZone					0xA063
#define MoveHHi						0xA064
#define StackSpace 					0xA065
#define NewEmptyHandle 				0xA166
#define HSetRBit						0xA067
#define HClrRBit						0xA068
#define HGetState						0xA069
#define HSetState						0xA06A
#define InitFS 						0xA06C
#define InitEvents 					0xA06D
#define StripAddress					0xA055
 /* end of System Traps

  new 256K ROM Traps
*/
#define SetAppBase 					0xA057
#define SwapMMUMode					0xA05D
#define SlotVInstall					0xA06F
#define SlotVRemove					0xA070
#define AttachVBL						0xA071
#define DoVBLTask						0xA072
#define SIntInstall					0xA075
#define SIntRemove 					0xA076
#define CountADBs						0xA077
#define GetIndADB						0xA078
#define GetADBInfo 					0xA079
#define SetADBInfo 					0xA07A
#define ADBOp							0xA07C
#define GetDefaultStartup				0xA07D
#define SetDefaultStartup				0xA07E
#define InternalWait					0xA07F
#define GetVideoDefault				0xA080
#define SetVideoDefault				0xA081
#define DTInstall						0xA082
#define SetOSDefault					0xA083
#define GetOSDefault					0xA084
#define Sleep							0xA08A
#define SysEnvirons					0xA090
#define InitPalettes					0xAA90
 /*  Palette Manager Traps, transplanted from PaletteEqu.a
*/
#define NewPalette 					0xAA91
#define GetNewPalette					0xAA92
#define DisposePalette 				0xAA93
#define ActivatePalette				0xAA94
#define SetPalette 					0xAA95
#define NSetPalette					0xAA95
#define GetPalette 					0xAA96
#define PmForeColor					0xAA97
#define PmBackColor					0xAA98
#define AnimateEntry					0xAA99
#define AnimatePalette 				0xAA9A
#define GetEntryColor					0xAA9B
#define SetEntryColor					0xAA9C
#define GetEntryUsage					0xAA9D
#define SetEntryUsage					0xAA9E
#define CTab2Palette					0xAA9F
#define Palette2CTab					0xAAA0
#define CopyPalette					0xAAA1
#define PMgrOp 						0xA085
#define HUnmountVol					0xA20E

/*	Windows Traps  */

#define BeginUpDate					0xA922
#define BringToFront					0xA920
#define CalcVBehind					0xA90A
#define CalcVis						0xA909
#define CheckUpDate					0xA911
#define ClipAbove						0xA90B
#define CloseWindow					0xA92D
#define DisposWindow					0xA914
#define DragGrayRgn					0xA905
#define DragWindow 					0xA925
#define DrawGrowIcon					0xA904
#define DrawNew						0xA90F
#define EndUpDate						0xA923
#define FindWindow 					0xA92C
#define FrontWindow					0xA924
#define GetAuxWin						0xAA42
#define GetCWMgrPort					0xAA48
#define GetNewCWindow					0xAA46
#define GetNewWindow					0xA9BD
#define GetWindowPic					0xA92F
#define GetWMgrPort					0xA910
#define GetWRefCon 					0xA917
#define GetWTitle						0xA919
#define GetWVariant					0xA80A
#define GrowWindow 					0xA92B
#define HideWindow 					0xA916
#define HiliteWindow					0xA91C
#define InitWindows					0xA912
#define InvalRect						0xA928
#define InvalRgn						0xA927
#define MoveWindow 					0xA91B
#define NewCWindow 					0xAA45
#define NewWindow						0xA913
#define PaintBehind					0xA90D
#define PaintOne						0xA90C
#define PinRect						0xA94E
#define SaveOld						0xA90E
#define SelectWindow					0xA91F
#define SendBehind 					0xA921
#define SetDeskCPat					0xAA47
#define SetWinColor					0xAA41
#define SetWindowPic					0xA92E
#define SetWRefCon 					0xA918
#define SetWTitle						0xA91A
#define ShowHide						0xA908
#define ShowWindow 					0xA915
#define SizeWindow 					0xA91D
#define TrackBox						0xA83B
#define TrackGoAway					0xA91E
#define ValidRect						0xA92A
#define ValidRgn						0xA929
#define ZoomWindow 					0xA83A
#define PutIcon						0xA9CA

/*	Notification Traps	*/

#define NMInstall						0xA05E
#define NMRemove						0xA05F

/*	SCSI Traps	*/

#define SCSIDispatch					0xA815

/*	Script Traps  */

#define ScriptUtil 					0xA8B5

/*	Slots Traps  */

#define SlotManager					0xA06E

/*	Sound Traps  */

#define SndDoCommand					0xA803
#define SndDoImmediate 				0xA804
#define SndAddModifier 				0xA802
#define SndNewChannel					0xA807
#define SndDisposeChannel				0xA801
#define SndControl 					0xA806
#define SndPlay						0xA805


#endif
