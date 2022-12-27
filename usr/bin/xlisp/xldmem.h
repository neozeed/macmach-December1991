/* -*-C-*-
********************************************************************************
*
* File:         xldmem.h
* RCS:          $Header: xldmem.h,v 1.7 89/11/25 05:22:56 mayer Exp $
* Description:  dynamic memory definitions
* Author:       David Michael Betz; Niels Mayer
* Created:      
* Modified:     Sat Nov 25 05:22:46 1989 (Niels Mayer) mayer@hplnpm
* Language:     C
* Package:      N/A
* Status:       X11r4 contrib tape release
*
* WINTERP 1.0 Copyright 1989 Hewlett-Packard Company (by Niels Mayer).
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Hewlett-Packard and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Hewlett-Packard and David Betz
* make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* HEWLETT-PACKARD AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL HEWLETT-PACKARD NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* See ./winterp/COPYRIGHT for information on contacting the authors.
* 
* Please send modifications, improvements and bugfixes to mayer@hplabs.hp.com
* Post XLISP-specific questions/information to the newsgroup comp.lang.lisp.x
*
********************************************************************************
*/

/* small fixnum range */
#define SFIXMIN		(-128)
#define SFIXMAX		255
#define SFIXSIZE	384

/* character range */
#define CHARMIN		0
#define CHARMAX		255
#define CHARSIZE	256

/* new node access macros */
#define ntype(x)	((x)->n_type)

/* cons access macros */
#define car(x)		((x)->n_car)
#define cdr(x)		((x)->n_cdr)
#define rplaca(x,y)	((x)->n_car = (y))
#define rplacd(x,y)	((x)->n_cdr = (y))

/* symbol access macros */
#define getvalue(x)	 ((x)->n_vdata[0])
#define setvalue(x,v)	 ((x)->n_vdata[0] = (v))
#define getfunction(x)	 ((x)->n_vdata[1])
#define setfunction(x,v) ((x)->n_vdata[1] = (v))
#define getplist(x)	 ((x)->n_vdata[2])
#define setplist(x,v)	 ((x)->n_vdata[2] = (v))
#define getpname(x)	 ((x)->n_vdata[3])
#define setpname(x,v)	 ((x)->n_vdata[3] = (v))
#define SYMSIZE		4

/* closure access macros */
#define getname(x)     	((x)->n_vdata[0])
#define setname(x,v)   	((x)->n_vdata[0] = (v))
#define gettype(x)    	((x)->n_vdata[1])
#define settype(x,v)  	((x)->n_vdata[1] = (v))
#define getargs(x)     	((x)->n_vdata[2])
#define setargs(x,v)   	((x)->n_vdata[2] = (v))
#define getoargs(x)    	((x)->n_vdata[3])
#define setoargs(x,v)  	((x)->n_vdata[3] = (v))
#define getrest(x)     	((x)->n_vdata[4])
#define setrest(x,v)   	((x)->n_vdata[4] = (v))
#define getkargs(x)    	((x)->n_vdata[5])
#define setkargs(x,v)  	((x)->n_vdata[5] = (v))
#define getaargs(x)    	((x)->n_vdata[6])
#define setaargs(x,v)  	((x)->n_vdata[6] = (v))
#define getbody(x)     	((x)->n_vdata[7])
#define setbody(x,v)   	((x)->n_vdata[7] = (v))
#define getenv(x)	((x)->n_vdata[8])
#define setenv(x,v)	((x)->n_vdata[8] = (v))
#define getfenv(x)	((x)->n_vdata[9])
#define setfenv(x,v)	((x)->n_vdata[9] = (v))
#define getlambda(x)	((x)->n_vdata[10])
#define setlambda(x,v)	((x)->n_vdata[10] = (v))
#define CLOSIZE		11

/* vector access macros */
#define getsize(x)	((x)->n_vsize)
#define getelement(x,i)	((x)->n_vdata[i])
#define setelement(x,i,v) ((x)->n_vdata[i] = (v))

/* object access macros */
#define getclass(x)	((x)->n_vdata[0])
#define getivar(x,i)	((x)->n_vdata[i+1])
#define setivar(x,i,v)	((x)->n_vdata[i+1] = (v))

/* subr/fsubr access macros */
#define getsubr(x)	((x)->n_subr)
#define getoffset(x)	((x)->n_offset)

/* fixnum/flonum/char access macros */
#define getfixnum(x)    ((x)->n_fixnum)
#define getflonum(x)	((x)->n_flonum)
#define getchcode(x)	((x)->n_chcode)

/* string access macros */
#define getstring(x)	((x)->n_string)
#define getslength(x)	((x)->n_strlen)

/* file stream access macros */
#define getfile(x)	((x)->n_fp)
#define setfile(x,v)	((x)->n_fp = (v))
#define getsavech(x)	((x)->n_savech)
#define setsavech(x,v)	((x)->n_savech = (v))

/* unnamed stream access macros */
#define gethead(x)	((x)->n_car)
#define sethead(x,v)	((x)->n_car = (v))
#define gettail(x)	((x)->n_cdr)
#define settail(x,v)	((x)->n_cdr = (v))

#ifdef WINTERP

/* XLTYPE_XT_RESOURCE access macros */
#define get_xtresource(x)          ((x)->n_xtresource)

#ifndef Pixel
#define Pixel unsigned long	/* from <X11/Intrinsic.h> */
#endif

/* XLTYPE_Pixel access macros */
#define get_pixel(x)               ((x)->n_pixel)

#ifndef Pixmap
#define Pixmap unsigned long	/* from <X11/X.h> */
#endif

/* XLTYPE_Pixmap access macros */
#define get_pixmap(x)              ((x)->n_pixmap)

#ifndef XImage
#define XImage struct _XImage
#endif

/* XLTYPE_XImage access macros */
#define get_ximage(x)              ((x)->n_ximage)

#ifndef XmString
#define XmString char *		/* from <Xm/Xm.h> */
#endif

/* XLTYPE_XmString access macros */
#define get_xmstring(x)		   ((x)->n_xmstring)

#ifndef XEvent
#define XEvent union _XEvent	/* from <X11/Xlib.h> */
#endif

/* XLTYPE_XEvent access macros */
#define get_xevent(x)	   ((x)->n_xevent)

#ifndef Window
#define Window unsigned long
#endif

/* XLTYPE_Window access macros */
#define get_window(x)		   ((x)->n_window)

/* XLTYPE_PIXMAP_REFOBJ access macros */
#define get_pixref_pixmap(x)       ((x)->n_vdata[0])
#define set_pixref_pixmap(x,v)     ((x)->n_vdata[0] = (v))
#define get_pixref_widget(x)       ((x)->n_vdata[1])
#define set_pixref_widget(x,v)     ((x)->n_vdata[1] = (v))
#define get_pixref_resname(x)      ((x)->n_vdata[2])
#define set_pixref_resname(x,v)    ((x)->n_vdata[2] = (v))
#define PIXMAP_REFOBJ_SIZE 3

/* XLTYPE_CALLBACKOBJ access macros */
#define get_callback_widget(x)     ((x)->n_vdata[0])
#define set_callback_widget(x,v)   ((x)->n_vdata[0] = (v))
#define get_callback_name(x)       ((char *) getfixnum((x)->n_vdata[1]))
#define set_callback_name(x,v)     ((x)->n_vdata[1] = cvfixnum((FIXTYPE) (v))) /* note that (v) is of type char* */
#define get_callback_proc(x)       ((XtCallbackProc) getfixnum((x)->n_vdata[2]))
#define set_callback_proc(x,v)     ((x)->n_vdata[2] = cvfixnum((FIXTYPE) (v))) /* note that (v) is of type XtCallbackProc */
#define get_callback_closure(x)    ((x)->n_vdata[3])
#define set_callback_closure(x,v)  ((x)->n_vdata[3] = (v))
#define CALLBACKOBJ_SIZE 4

/* XLTYPE_TIMEOUTOBJ access macros */
#define get_timeout_closure(x)     ((x)->n_vdata[0])
#define set_timeout_closure(x,v)   ((x)->n_vdata[0] = (v))
#define get_timeout_id(x)          ((XtIntervalId) getfixnum((x)->n_vdata[1]))
#define set_timeout_id(x,v)        ((x)->n_vdata[1] = cvfixnum((FIXTYPE) (v))) /* note that (v) is of type XtIntervalId */
#define TIMEOUTOBJ_SIZE 2

/* XLTYPE_EVHANDLEROBJ access macros */
#define get_evhandler_widget(x)    ((x)->n_vdata[0])
#define set_evhandler_widget(x,v)  ((x)->n_vdata[0] = (v))
#define get_evhandler_mask(x)      ((long) getfixnum((x)->n_vdata[1]))
#define set_evhandler_mask(x,v)    ((x)->n_vdata[1] = cvfixnum((FIXTYPE) (v))) /* note that (v) is of type long */
#define get_evhandler_options(x)   ((long) getfixnum((x)->n_vdata[2]))
#define set_evhandler_options(x,v) ((x)->n_vdata[2] = cvfixnum((FIXTYPE) (v))) /* note that (v) is of type long */
#define get_evhandler_closure(x)   ((x)->n_vdata[3])
#define set_evhandler_closure(x,v) ((x)->n_vdata[3] = (v))
#define EVHANDLEROBJ_SIZE 4

/* XLTYPE_WIDGETOBJ access macros -- note that all OBJECT operations work
   on this type too. The main difference between this type and OBJECT
   (both of which are implemented as vectors) is that a WIDGETOBJ holds
   a single instance variable which is an immediate pointer to a WidgetID
   (as opposed to having the slot holding a pointer to an FIXNUM representing
   the WidgetID.) This slot may not be marked since it is
   not a LVAL node. Thus, special code exists in xldmem.c to prevent
   such a problem. The reason for going through all this is that we
   need to do a special operation on the widgetID when a WIDGETOBJ gets
   garbage collected (see sweep()). If the widgetID were another node,
   then the object would be considered "free" and may not hold a valid
   widgetID. Yes, this is a hack. Also, having a separate type for WIDGETOBJs
   makes some operations on WIDGETOBJs more efficient... see w_classes.c */
/* #define getclass(x)	  ((x)->n_vdata[0]) --> this macro valid for both this and OBJECT */
#define get_widgetobj_widgetID(x)   (Widget) ((x)->n_vdata[1])
#define set_widgetobj_widgetID(x,v) ((x)->n_vdata[1] = (LVAL) (v))
#define WIDGETOBJ_SIZE 1	/* this val represents the number of instance variables
				   in the object. newobject() will actually create a vector
				   of size WIDGETOBJ_SIZE+1 so as to hold the ivar and the
				   class pointer. */

#ifndef XtAccelerators
#define XtAccelerators struct _TranslationData* /* from <X11/Intrinsic.h> */
#endif

/* XLTYPE_XtAccelerators access macros */
#define get_xtaccelerators(x)         ((x)->n_xtaccelerators)

#ifndef XtTranslations
#define XtTranslations struct _TranslationData*	/* from <X11/Intrinsic.h */
#endif

/* XLTYPE_XtTranslations access macros */
#define get_xttranslations(x)	      ((x)->n_xttranslations)

#endif				/* WINTERP */

/* node types */
#define FREE	0
#define SUBR	1
#define FSUBR	2
#define CONS	3
#define SYMBOL	4
#define FIXNUM	5
#define FLONUM	6
#define STRING	7
#define OBJECT	8
#define STREAM	9
#define VECTOR	10
#define CLOSURE	11
#define CHAR	12
#define USTREAM	13
#define STRUCT	14

#ifdef WINTERP

#define XLTYPE_XtAccelerators		15
#define XLTYPE_XtTranslations		16
#define XLTYPE_XtCallbackList		17
#define XLTYPE_XEvent			18
#define XLTYPE_Window			19
#define XLTYPE_Pixel			20
#define XLTYPE_Pixmap			21
#define XLTYPE_XImage                   22
#define XLTYPE_XmString			23
#define XLTYPE_XmFontList		24
#define XLTYPE_caddr_t			25 /* generic pointer */
#define XLTYPE_XT_RESOURCE              26
#define XLTYPE_CALLBACKOBJ              27
#define XLTYPE_TIMEOUTOBJ               28
#define XLTYPE_PIXMAP_REFOBJ		29
#define XLTYPE_WIDGETOBJ                30
#define XLTYPE_EVHANDLEROBJ		31

/* XLTYPE_XT_RESOURCE node */
#define n_xtresource           n_info.n_xresource.xr_resource

/* XLTYPE_Pixel node */
#define n_pixel                n_info.n_xpixel.xp_pixel

/* XLTYPE_Pixmap node */
#define n_pixmap               n_info.n_xpixmap.xp_pixmap

/* XLTYPE_XImage node */
#define n_ximage               n_info.n_xximage.xx_ximage

/* XLTYPE_XmString node */
#define n_xmstring	       n_info.n_xxmstring.xx_xmstring

/* XLTYPE_XEvent node */
#define n_xevent	       n_info.n_xxevent.xx_xevent

/* XLTYPE_Window node */
#define n_window	       n_info.n_xwindow.xw_window

/* XLTYPE_XtAccelerators node */
#define n_xtaccelerators       n_info.n_xxtaccelerators.xx_xtaccelerators

/* XLTYPE_XtTranslations node */
#define n_xttranslations       n_info.n_xxttranslations.xx_xttranslations

#endif				/* WINTERP */

/* subr/fsubr node */
#define n_subr		n_info.n_xsubr.xs_subr
#define n_offset	n_info.n_xsubr.xs_offset

/* cons node */
#define n_car		n_info.n_xcons.xc_car
#define n_cdr		n_info.n_xcons.xc_cdr

/* fixnum node */
#define n_fixnum	n_info.n_xfixnum.xf_fixnum

/* flonum node */
#define n_flonum	n_info.n_xflonum.xf_flonum
/* character node */
#define n_chcode	n_info.n_xchar.xc_chcode

/* string node */
#define n_string	n_info.n_xstring.xs_string
#define n_strlen	n_info.n_xstring.xs_length

/* stream node */
#define n_fp		n_info.n_xstream.xs_fp
#define n_savech	n_info.n_xstream.xs_savech

/* vector/object node */
#define n_vsize		n_info.n_xvector.xv_size
#define n_vdata		n_info.n_xvector.xv_data

/* node structure */
typedef struct node {
    char n_type;		/* type of node */
    char n_flags;		/* flag bits */
    union ninfo { 		/* value */

#ifdef WINTERP

      struct xresource {	/* XLTYPE_XTRESOURCE node */
	struct _Resource_Instance *xr_resource;
      } n_xresource;
      struct xpixel {		/* XLTYPE_Pixel node */
	Pixel xp_pixel;
      } n_xpixel;
      struct xpixmap {		/* XLTYPE_Pixmap node */
	Pixmap xp_pixmap;
      } n_xpixmap;
      struct xximage {		/* XLTYPE_XImage node */
	XImage *xx_ximage;
      } n_xximage;
      struct xxmstring {	/* XLTYPE_XmString node */
	XmString xx_xmstring;
      } n_xxmstring;
      struct xxevent {		/* XLTYPE_XEvent node */
	XEvent *xx_xevent;
      } n_xxevent;
      struct xwindow {		/* XLTYPE_Window node */
	Window xw_window;
      } n_xwindow;
      struct xxtaccelerators {
	XtAccelerators xx_xtaccelerators; /* XLTYPE_XtAccelerators node */
      } n_xxtaccelerators;
      struct xxttranslations {
	XtTranslations xx_xttranslations; /* XLTYPE_XtTranslations node */
      } n_xxttranslations;

#endif				/* WINTERP */

	struct xsubr {		/* subr/fsubr node */
	    struct node *(*xs_subr)();	/* function pointer */
	    int xs_offset;		/* offset into funtab */
	} n_xsubr;
	struct xcons {		/* cons node */
	    struct node *xc_car;	/* the car pointer */
	    struct node *xc_cdr;	/* the cdr pointer */
	} n_xcons;
	struct xfixnum {	/* fixnum node */
	    FIXTYPE xf_fixnum;		/* fixnum value */
	} n_xfixnum;
	struct xflonum {	/* flonum node */
	    FLOTYPE xf_flonum;		/* flonum value */
	} n_xflonum;
	struct xchar {		/* character node */
	    int xc_chcode;		/* character code */
	} n_xchar;
	struct xstring {	/* string node */
	    int xs_length;		/* string length */
	    unsigned char *xs_string;	/* string pointer */
	} n_xstring;
	struct xstream { 	/* stream node */
	    FILE *xs_fp;		/* the file pointer */
	    int xs_savech;		/* lookahead character */
	} n_xstream;
	struct xvector {	/* vector/object/symbol/structure node */
	    int xv_size;		/* vector size */
	    struct node **xv_data;	/* vector data */
	} n_xvector;
    } n_info;
} *LVAL;

/* memory segment structure definition */
typedef struct segment {
    int sg_size;
    struct segment *sg_next;
    struct node sg_nodes[1];
} SEGMENT;

/* memory allocation functions */
extern LVAL cons();		/* (cons x y) */
extern LVAL cvsymbol();       	/* convert a string to a symbol */
extern LVAL cvstring();       	/* convert a string */
extern LVAL cvfile();		/* convert a FILE * to a file */
extern LVAL cvsubr();		/* convert a function to a subr/fsubr */
extern LVAL cvfixnum();       	/* convert a fixnum */
extern LVAL cvflonum();       	/* convert a flonum */
extern LVAL cvchar();		/* convert a character */

#ifdef WINTERP
extern LVAL cv_xtresource();	/* convert a pointer to struct _Resource_Instance */
extern LVAL cv_pixel();		/* convert a X11 Pixel to XLTYPE_Pixel */
extern LVAL cv_pixmap();	/* create a XLTYPE_Pixmap */
extern LVAL cv_ximage();	/* create a XLTYPE_XImage */
extern LVAL cv_xmstring();	/* create a XLTYPE_XmString */
extern LVAL cv_xevent();	/* create a XLTYPE_XEvent */
extern LVAL cv_window();	/* create a XLTYPE_Window */
extern LVAL cv_xtaccelerators(); /* create a XLTYPE_XtAccelerators */
extern LVAL cv_xttranslations(); /* create a XLTYPE_XtTranslations */
extern LVAL new_pixrefobj();	/* create a XLTYPE_PIXMAP_REFOBJ */
extern LVAL new_timeoutobj();	/* create a XLTYPE_TIMEOUTOBJ */
extern LVAL new_callbackobj();	/* create a XLTYPE_CALLBACKOBJ */
extern LVAL new_evhandlerobj();	/* create a XLTYPE_EVHANDLEROBJ */
#endif				/* WINTERP */

extern LVAL newstring();	/* create a new string */
extern LVAL newvector();	/* create a new vector */
extern LVAL newobject();	/* create a new object */
extern LVAL newclosure();	/* create a new closure */
extern LVAL newustream();	/* create a new unnamed stream */
extern LVAL newstruct();	/* create a new structure */
