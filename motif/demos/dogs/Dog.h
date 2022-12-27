#ifdef REV_INFO
#ifndef lint
static char SCCSID[] ="OSF/Motif: @(#)Dog.h	1.5 90/07/30";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990 Alastair Gourlay
*  ALL RIGHTS RESERVED
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/*****************************************************************************
*
*  Dog.h - widget public header file
*  
******************************************************************************/

#ifndef _Dog_h
#define _Dog_h

externalref WidgetClass dogWidgetClass;

typedef struct _DogClassRec *DogWidgetClass;
typedef struct _DogRec *DogWidget;

#define DogNbarkCallback "barkCallback"
#define DogNwagTime "wagTime"
#define DogNbarkTime "barkTime"

#define DogCWagTime "WagTime"
#define DogCBarkTime "BarkTime"

#define IsDog(w) XtIsSubclass((w), dogWidgetClass)

extern Widget DogCreate();
extern int DogMrmInitialize();

#endif /* _Dog_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
