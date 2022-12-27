#ifdef REV_INFO
#ifndef lint
static char SCCSID[] ="OSF/Motif: @(#)SquareP.h	1.2 90/07/30";
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

/******************************************************************************
*
*  SquareP.h - widget private header file
*
******************************************************************************/

#ifndef _SquareP_h
#define _SquareP_h

#include <Square.h>
#include <Xm/BulletinBP.h>

#define SquareIndex (XmBulletinBIndex + 1)

typedef struct _SquareClassPart
{
   int reserved;
} SquareClassPart;


typedef struct _SquareClassRec
{
   CoreClassPart       core_class;
   CompositeClassPart  composite_class;
   ConstraintClassPart constraint_class;
   XmManagerClassPart  manager_class;
   XmBulletinBoardClassPart  bulletin_board_class;
   SquareClassPart     square_class;
} SquareClassRec;

externalref SquareClassRec squareClassRec;

typedef struct _SquarePart
{
    int major_dimension;
} SquarePart;


/*  Full instance record declaration  */

typedef struct _SquareRec
{
   CorePart	  core;
   CompositePart  composite;
   ConstraintPart constraint;
   XmManagerPart  manager;
   XmBulletinBoardPart  bulletin_board;
   SquarePart     square;
} SquareRec;

typedef struct _SquareConstraintPart
{
   Boolean make_square;
} SquareConstraintPart;

typedef struct _SquareConstraintRec
{
   XmManagerConstraintPart manager;
   SquareConstraintPart    square;
} SquareConstraintRec, *SquareConstraint;


#endif /* _SquareP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
