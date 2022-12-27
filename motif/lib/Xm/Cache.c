#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Cache.c	3.9 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
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
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/LabelGP.h>
#include <Xm/CacheP.h>

/************************************************************************
 *
 *  _XmCacheDelete
 *	Delete an existing cache record.  NOTE: <data> is a pointer to the
 *      fourth field in the cache record - It is *not* a pointer to the
 *	cache record itself!
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmCacheDelete( data )
caddr_t data;
#else /* _NO_PROTO */
void _XmCacheDelete (caddr_t data)
#endif /* _NO_PROTO */
{
    XmGadgetCachePtr ptr;

    ptr = (XmGadgetCachePtr) DataToGadgetCache(data);
    if (--ptr->ref_count <= 0) {
      (ptr->prev)->next = ptr->next;
      if (ptr->next)			/* not the last record */
        (ptr->next)->prev = ptr->prev;
      XtFree( ptr );
    }
}

/************************************************************************
 *
 *  _XmCacheCopy
 *	Copy <size> bytes from <src> to <dest>. 
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmCacheCopy(src, dest, size) 
caddr_t  src, dest;
int      size;
#else /* _NO_PROTO */
void _XmCacheCopy (caddr_t src, caddr_t dest, int size)
#endif /* _NO_PROTO */
{
    bcopy(src, dest, size);
}

/************************************************************************
 *
 *  _XmCachePart
 *	Pass in a pointer, <cpart>, to <size> bytes of a temporary Cache
 *	record.  
 *	- If the Class cache head is NULL (no entries yet!), allocate a new
 *	  cache record, copy in temporary Cache bytes, append it to the 
 *	  class-cache linked list, and return the address.
 *	- Else, run through the class linked list.
 *	  = If a match is found, increment the ref_count and return the 
 *	    address.
 *	  = Else, allocate a new cache record, copy in temporary Cache bytes,
 *	    append it to the class-cache linked list, and return the address.
 *
 ************************************************************************/

#ifdef _NO_PROTO
caddr_t _XmCachePart(cp, cpart, size)
XmCacheClassPartPtr cp;
caddr_t cpart;
int size;
#else /* _NO_PROTO */
caddr_t _XmCachePart (XmCacheClassPartPtr cp, caddr_t cpart, int size)
#endif /* _NO_PROTO */
{
    XmGadgetCachePtr ptr, last;
    
    if (ClassCacheHead(cp).next == NULL)       /* First one */
    {
	ClassCacheHead(cp).next = 
	  (struct _XmGadgetCache *)XtMalloc( size + sizeof(XmGadgetCache) );
        ptr = (XmGadgetCachePtr)ClassCacheHead(cp).next;

        ClassCacheCopy(cp)(cpart, CacheDataPtr(ptr), size );  
	ptr-> ref_count = 1;
        ptr-> next = NULL;
	ptr-> prev = (struct _XmGadgetCache *)&ClassCacheHead(cp);
        return (CacheDataPtr(ptr));
    }    
    ptr = (XmGadgetCachePtr)ClassCacheHead(cp).next;
    do
    {

        if ((ClassCacheCompare(cp)( cpart, CacheDataPtr(ptr))))
        {
            ptr->ref_count++;
            return ((caddr_t) CacheDataPtr(ptr));
        }
        else
        {
            last = ptr;
            ptr = (XmGadgetCachePtr)ptr->next;
        }
    } while (ptr);
    
    /* Malloc a new rec off of last, fill it out*/
    ptr = (XmGadgetCachePtr)XtMalloc( size + sizeof(XmGadgetCache) );
    last->next = (struct _XmGadgetCache *)ptr;
    ClassCacheCopy(cp)(cpart, CacheDataPtr(ptr), size);
    ptr-> ref_count = 1;
    ptr-> next = NULL;
    ptr-> prev = last;
    return (CacheDataPtr(ptr));
}

