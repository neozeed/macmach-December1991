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
 * $Log:	Files.h,v $
 * Revision 2.2  91/09/12  16:50:30  bohman
 * 	Created.
 * 	[91/09/11  16:28:58  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Files.h
 */

/************************************************************

Created: Tuesday, September 12, 1989 at 7:45 PM
	Files.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	1985-1989
	All rights reserved

************************************************************/


#ifndef __FILES__
#define __FILES__

#ifndef __TYPES__
#include <Types.h>
#endif

/* Finder Constants */

#define fsAtMark 0
#define fOnDesk 1
#define fsCurPerm 0
#define fHasBundle 8192
#define fsRdPerm 1
#define fInvisible 16384
#define fTrash -3
#define fsWrPerm 2
#define fDesktop -2
#define fsRdWrPerm 3
#define fDisk 0
#define fsRdWrShPerm 4
#define fsFromStart 1
#define fsFromLEOF 2
#define fsFromMark 3
#define rdVerify 64
#define ioDirFlg 3
#define ioDirMask 0x10
#define fsRtParID 1
#define fsRtDirID 2

/* Version Release Stage Codes */

#define developStage 0x20
#define alphaStage 0x40
#define betaStage 0x60
#define finalStage 0x80

enum {hFileInfo,dirInfo};
typedef unsigned char CInfoType;

struct FInfo {
	OSType fdType;					/*the type of the file*/
	OSType fdCreator;				/*file's creator*/
	unsigned short fdFlags; 		/*flags ex. hasbundle,invisible,locked, etc.*/
	Point fdLocation;				/*file's location in folder*/
	short fdFldr;					/*folder containing file*/
};

typedef struct FInfo FInfo;
struct FXInfo {
	short fdIconID; 				/*Icon ID*/
	short fdUnused[4];				/*unused but reserved 8 bytes*/
	short fdComment;				/*Comment ID*/
	long fdPutAway; 				/*Home Dir ID*/
};

typedef struct FXInfo FXInfo;
struct DInfo {
	Rect frRect;					/*folder rect*/
	unsigned short frFlags; 		/*Flags*/
	Point frLocation;				/*folder location*/
	short frView;					/*folder view*/
};

typedef struct DInfo DInfo;
struct DXInfo {
	Point frScroll; 				/*scroll position*/
	long frOpenChain;				/*DirID chain of open folders*/
	short frUnused; 				/*unused but reserved*/
	short frComment;				/*comment*/
	long frPutAway; 				/*DirID*/
};

typedef struct DXInfo DXInfo;
#define ParamBlockHeader \
	Ptr qLink; 				/*queue link in header*/\
	short qType;					/*type byte for safety check*/\
	unsigned short ioTrap;				/*FS: the Trap*/\
	Ptr ioCmdAddr;					/*FS: address to dispatch to*/\
	ProcPtr ioCompletion;			/*completion routine addr (0 for synch calls)*/\
	OSErr ioResult; 				/*result code*/\
	StringPtr ioNamePtr;			/*ptr to Vol:FileName string*/\
	short ioVRefNum;				/*volume refnum (DrvNum for Eject and MountVol)*/


struct IOParam {
	ParamBlockHeader
	short ioRefNum; 				/*refNum for I/O operation*/
	char ioVersNum; 				/*version number*/
	char ioPermssn; 				/*Open: permissions (byte)*/
	Ptr ioMisc; 					/*Rename: new name (GetEOF,SetEOF: logical end of file) (Open: optional ptr to buffer) (SetFileType: new type)*/
	Ptr ioBuffer;					/*data buffer Ptr*/
	long ioReqCount;				/*requested byte count; also = ioNewDirID*/
	long ioActCount;				/*actual byte count completed*/
	short ioPosMode;				/*initial file positioning*/
	long ioPosOffset;				/*file position offset*/
};

typedef struct IOParam IOParam;
struct FileParam {
	ParamBlockHeader
	short ioFRefNum;				/*reference number*/
	char ioFVersNum;				/*version number*/
	char filler1;
	short ioFDirIndex;				/*GetFInfo directory index*/
	unsigned char ioFlAttrib;		/*GetFInfo: in-use bit=7, lock bit=0*/
	unsigned char ioFlVersNum;		/*file version number*/
	FInfo ioFlFndrInfo; 			/*user info*/
	unsigned long ioFlNum;			/*GetFInfo: file number; TF- ioDirID*/
	unsigned short ioFlStBlk;		/*start file block (0 if none)*/
	long ioFlLgLen; 				/*logical length (EOF)*/
	long ioFlPyLen; 				/*physical length*/
	unsigned short ioFlRStBlk;		/*start block rsrc fork*/
	long ioFlRLgLen;				/*file logical length rsrc fork*/
	long ioFlRPyLen;				/*file physical length rsrc fork*/
	unsigned long ioFlCrDat;		/*file creation date& time (32 bits in secs)*/
	unsigned long ioFlMdDat;		/*last modified date and time*/
};

typedef struct FileParam FileParam;
struct VolumeParam {
	ParamBlockHeader
	long filler2;
	short ioVolIndex;				/*volume index number*/
	unsigned long ioVCrDate;		/*creation date and time*/
	unsigned long ioVLsBkUp;		/*last backup date and time*/
	unsigned short ioVAtrb; 		/*volume attrib*/
	unsigned short ioVNmFls;		/*number of files in directory*/
	unsigned short ioVDirSt;		/*start block of file directory*/
	short ioVBlLn;					/*GetVolInfo: length of dir in blocks*/
	unsigned short ioVNmAlBlks; 	/*GetVolInfo: num blks (of alloc size)*/
	long ioVAlBlkSiz;				/*GetVolInfo: alloc blk byte size*/
	long ioVClpSiz; 				/*GetVolInfo: bytes to allocate at a time*/
	unsigned short ioAlBlSt;		/*starting disk(512-byte) block in block map*/
	unsigned long ioVNxtFNum;		/*GetVolInfo: next free file number*/
	unsigned short ioVFrBlk;		/*GetVolInfo: # free alloc blks for this vol*/
};

typedef struct VolumeParam VolumeParam;
struct CntrlParam {
	Ptr qLink;					/*queue link in header*/
	short qType;					/*type byte for safety check*/
	short ioTrap;					/*FS: the Trap*/
	Ptr ioCmdAddr;					/*FS: address to dispatch to*/
	ProcPtr ioCompletion;			/*completion routine addr (0 for synch calls)*/
	OSErr ioResult; 				/*result code*/
	StringPtr ioNamePtr;			/*ptr to Vol:FileName string*/
	short ioVRefNum;				/*volume refnum (DrvNum for Eject and MountVol)*/
	short ioCRefNum;				/*refNum for I/O operation*/
	short csCode;					/*word for control status code*/
	short csParam[11];				/*operation-defined parameters*/
};

typedef struct CntrlParam CntrlParam;
struct SlotDevParam {
	ParamBlockHeader
	short ioRefNum;
	char ioVersNum;
	char ioPermssn;
	Ptr ioMix;
	short ioFlags;
	char ioSlot;
	char ioID;
};

typedef struct SlotDevParam SlotDevParam;
struct MultiDevParam {
	ParamBlockHeader
	short ioRefNum;
	char ioVersNum;
	char ioPermssn;
	Ptr ioMix;
	short ioFlags;
	Ptr ioSEBlkPtr;
};

typedef struct MultiDevParam MultiDevParam;
union ParamBlockRec {
	IOParam ioParam;
	FileParam fileParam;
	VolumeParam volumeParam;
	CntrlParam cntrlParam;
	SlotDevParam slotDevParam;
	MultiDevParam multiDevParam;
};

typedef union ParamBlockRec ParamBlockRec;

typedef ParamBlockRec *ParmBlkPtr;

struct HIOParam {
	ParamBlockHeader
	short ioRefNum;
	char ioVersNum;
	char ioPermssn;
	Ptr ioMisc;
	Ptr ioBuffer;
	long ioReqCount;
	long ioActCount;
	short ioPosMode;
	long ioPosOffset;
	short filler1;
};

typedef struct HIOParam HIOParam;
struct HOpenParam {
	ParamBlockHeader
	short ioRefNum;
	char ioVersNum;
	char ioPermssn;
	Ptr ioMisc;
	short filler1[8];
	long ioDirID;
};

typedef struct HOpenParam HOpenParam;
struct HFileParam {
	ParamBlockHeader
	short ioFRefNum;
	char ioFVersNum;
	char filler1;
	short ioFDirIndex;
	char ioFlAttrib;
	char ioFlVersNum;
	FInfo ioFlFndrInfo;
	long ioDirID;
	unsigned short ioFlStBlk;
	long ioFlLgLen;
	long ioFlPyLen;
	unsigned short ioFlRStBlk;
	long ioFlRLgLen;
	long ioFlRPyLen;
	unsigned long ioFlCrDat;
	unsigned long ioFlMdDat;
};

typedef struct HFileParam HFileParam;
struct HVolumeParam {
	ParamBlockHeader
	long filler2;
	short ioVolIndex;
	unsigned long ioVCrDate;
	unsigned long ioVLsMod;
	short ioVAtrb;
	unsigned short ioVNmFls;
	short ioVBitMap;
	short ioAllocPtr;
	unsigned short ioVNmAlBlks;
	long ioVAlBlkSiz;
	long ioVClpSiz;
	short ioAlBlSt;
	long ioVNxtCNID;
	unsigned short ioVFrBlk;
	unsigned short ioVSigWord;
	short ioVDrvInfo;
	short ioVDRefNum;
	short ioVFSID;
	unsigned long ioVBkUp;
	unsigned short ioVSeqNum;
	long ioVWrCnt;
	long ioVFilCnt;
	long ioVDirCnt;
	long ioVFndrInfo[8];
};

typedef struct HVolumeParam HVolumeParam;
struct AccessParam {
	ParamBlockHeader
	short filler3;
	short ioDenyModes;				/*access rights data*/
	short filler4;
	char filler5;
	char ioACUser;					/*access rights for directory only*/
	long filler6;
	long ioACOwnerID;				/*owner ID*/
	long ioACGroupID;				/*group ID*/
	long ioACAccess;				/*access rights*/
};

typedef struct AccessParam AccessParam;
struct ObjParam {
	ParamBlockHeader
	short filler7;
	short ioObjType;				/*function code*/
	Ptr ioObjNamePtr;				/*ptr to returned creator/group name*/
	long ioObjID;					/*creator/group ID*/
	long ioReqCount;				/*size of buffer area*/
	long ioActCount;				/*length of vol parms data*/
};

typedef struct ObjParam ObjParam;
struct CopyParam {
	ParamBlockHeader
	short ioDstVRefNum; 			/*destination vol identifier*/
	short filler8;
	Ptr ioNewName;					/*ptr to destination pathname*/
	Ptr ioCopyName; 				/*ptr to optional name*/
	long ioNewDirID;				/*destination directory ID*/
	long filler14;
	long filler15;
	long ioDirID;					/*same as in FileParam*/
};

typedef struct CopyParam CopyParam;
struct WDParam {
	ParamBlockHeader
	short filler9;
	short ioWDIndex;
	long ioWDProcID;
	short ioWDVRefNum;
	short filler10;
	long filler11;
	long filler12;
	long filler13;
	long ioWDDirID;
};

typedef struct WDParam WDParam;

struct FSSpec {
    short vRefNum;
    long parID;
    Str63 name;
};

typedef struct FSSpec FSSpec;
typedef FSSpec *FSSpecPtr, **FSSpecHandle;

typedef FSSpecPtr FSSpecArrayPtr;       /* pointer to array of FSSpecs */

union HParamBlockRec {
	HIOParam ioParam;
	HOpenParam openParam;
	HFileParam fileParam;
	HVolumeParam volumeParam;
	AccessParam accessParam;
	ObjParam objParam;
	CopyParam copyParam;
	WDParam wdParam;
};

typedef union HParamBlockRec HParamBlockRec;

typedef HParamBlockRec *HParmBlkPtr;

struct HFileInfo {
	ParamBlockHeader
	short ioFRefNum;
	char ioFVersNum;
	char filler1;
	short ioFDirIndex;
	char ioFlAttrib;
	char filler2;
	FInfo ioFlFndrInfo;
	long ioDirID;
	unsigned short ioFlStBlk;
	long ioFlLgLen;
	long ioFlPyLen;
	unsigned short ioFlRStBlk;
	long ioFlRLgLen;
	long ioFlRPyLen;
	unsigned long ioFlCrDat;
	unsigned long ioFlMdDat;
	unsigned long ioFlBkDat;
	FXInfo ioFlXFndrInfo;
	long ioFlParID;
	long ioFlClpSiz;
};

typedef struct HFileInfo HFileInfo;
struct DirInfo {
	ParamBlockHeader
	short ioFRefNum;
	short filler1;
	short ioFDirIndex;
	char ioFlAttrib;
	char filler2;
	DInfo ioDrUsrWds;
	long ioDrDirID;
	unsigned short ioDrNmFls;
	short filler3[9];
	unsigned long ioDrCrDat;
	unsigned long ioDrMdDat;
	unsigned long ioDrBkDat;
	DXInfo ioDrFndrInfo;
	long ioDrParID;
};

typedef struct DirInfo DirInfo;
union CInfoPBRec {
	HFileInfo hfileInfo;
	DirInfo dirInfo;
};

typedef union CInfoPBRec CInfoPBRec;

typedef CInfoPBRec *CInfoPBPtr;

struct CMovePBRec {
	Ptr qLink;
	short qType;
	short ioTrap;
	Ptr ioCmdAddr;
	ProcPtr ioCompletion;
	OSErr ioResult;
	StringPtr ioNamePtr;
	short ioVRefNum;
	long filler1;
	StringPtr ioNewName;
	long filler2;
	long ioNewDirID;
	long filler3[2];
	long ioDirID;
};

typedef struct CMovePBRec CMovePBRec;
typedef CMovePBRec *CMovePBPtr;

struct WDPBRec {
	Ptr qLink;
	short qType;
	short ioTrap;
	Ptr ioCmdAddr;
	ProcPtr ioCompletion;
	OSErr ioResult;
	StringPtr ioNamePtr;
	short ioVRefNum;
	short filler1;
	short ioWDIndex;
	long ioWDProcID;
	short ioWDVRefNum;
	short filler2[7];
	long ioWDDirID;
};

typedef struct WDPBRec WDPBRec;
typedef WDPBRec *WDPBPtr;

struct FCBPBRec {
	Ptr qLink;
	short qType;
	short ioTrap;
	Ptr ioCmdAddr;
	ProcPtr ioCompletion;
	OSErr ioResult;
	StringPtr ioNamePtr;
	short ioVRefNum;
	short ioRefNum;
	short filler;
	short ioFCBIndx;
	short filler1;
	long ioFCBFlNm;
	short ioFCBFlags;
	unsigned short ioFCBStBlk;
	long ioFCBEOF;
	long ioFCBPLen;
	long ioFCBCrPs;
	short ioFCBVRefNum;
	long ioFCBClpSiz;
	long ioFCBParID;
};

typedef struct FCBPBRec FCBPBRec;
typedef FCBPBRec *FCBPBPtr;

struct VCB {
	Ptr qLink;
	short qType;
	short vcbFlags;
	unsigned short vcbSigWord;
	unsigned long vcbCrDate;
	unsigned long vcbLsMod;
	short vcbAtrb;
	unsigned short vcbNmFls;
	short vcbVBMSt;
	short vcbAllocPtr;
	unsigned short vcbNmAlBlks;
	long vcbAlBlkSiz;
	long vcbClpSiz;
	short vcbAlBlSt;
	long vcbNxtCNID;
	unsigned short vcbFreeBks;
	Str27 vcbVN;
	short vcbDrvNum;
	short vcbDRefNum;
	short vcbFSID;
	short vcbVRefNum;
	Ptr vcbMAdr;
	Ptr vcbBufAdr;
	short vcbMLen;
	short vcbDirIndex;
	short vcbDirBlk;
	unsigned long vcbVolBkUp;
	unsigned short vcbVSeqNum;
	long vcbWrCnt;
	long vcbXTClpSiz;
	long vcbCTClpSiz;
	unsigned short vcbNmRtDirs;
	long vcbFilCnt;
	long vcbDirCnt;
	long vcbFndrInfo[8];
	unsigned short vcbVCSize;
	unsigned short vcbVBMCSiz;
	unsigned short vcbCtlCSiz;
	unsigned short vcbXTAlBlks;
	unsigned short vcbCTAlBlks;
	short vcbXTRef;
	short vcbCTRef;
	Ptr vcbCtlBuf;
	long vcbDirIDM;
	short vcbOffsM;
};

typedef struct VCB VCB;
struct DrvQEl {
	Ptr qLink;
	short qType;
	short dQDrive;
	short dQRefNum;
	short dQFSID;
	unsigned short dQDrvSz;
	unsigned short dQDrvSz2;
};

typedef struct DrvQEl DrvQEl;
typedef DrvQEl *DrvQElPtr;

struct NumVersion {
	unsigned char majorRev; 		/*1st part of version number in BCD*/
	unsigned int minorRev : 4;		/*2nd part is 1 nibble in BCD*/
	unsigned int bugFixRev : 4; 	/*3rd part is 1 nibble in BCD*/
	unsigned char stage;			/*stage code: dev, alpha, beta, final*/
	unsigned char nonRelRev;		/*revision level of non-released version*/
};

typedef struct NumVersion NumVersion;
/* Numeric version part of 'vers' resource */
struct VersRec {
	NumVersion numericVersion;		/*encoded version number*/
	short countryCode;				/*country code from intl utilities*/
	Str255 shortVersion;			/*version number string - worst case*/
	Str255 reserved;				/*longMessage string packed after shortVersion*/
};

typedef struct VersRec VersRec;
typedef VersRec *VersRecPtr, **VersRecHndl;

OSErr	PBOpen();
OSErr	PBClose();
OSErr	PBRead();
OSErr	PBWrite();
OSErr	PBControl();
OSErr	PBStatus();
OSErr	PBCreate();
OSErr	PBSetEOF();
OSErr	PBHGetVInfo();
OSErr	PBFlushVol();
OSErr	PBHOpen();
OSErr	PBOpenRF();
OSErr	PBHOpenRF();
OSErr	PBGetEOF();
OSErr	PBAllocate();
OSErr	PBAllocContig();
OSErr	PBHCreate();
OSErr	PBDirCreate();
OSErr	PBDelete();
OSErr	PBHDelete();
OSErr	PBOpenWD();
OSErr	PBCloseWD();
OSErr	PBGetWDInfo();
OSErr	PBGetVInfo();
OSErr	PBSetVInfo();
OSErr	PBGetVol();
OSErr	PBHGetVol();
OSErr	PBSetVol();
OSErr	PBHSetVol();
OSErr	PBGetFInfo();
OSErr	PBHGetFInfo();
OSErr	PBSetFInfo();
OSErr	PBHSetFInfo();
OSErr	PBGetFCBInfo();
OSErr	PBHGetVolParams();
OSErr	PBGetCatInfo();
OSErr	PBMakeFSSpec();
OSErr	PBOpenDF();
OSErr	PBHOpenDF();
OSErr	PBSetCatInfo();

#endif
