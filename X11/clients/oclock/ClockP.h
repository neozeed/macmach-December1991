/*
 * $XConsortium: ClockP.h,v 1.5 89/07/21 13:44:45 jim Exp $
 */

#ifndef _ClockP_h
#define _ClockP_h

#include "Clock.h"
#include <X11/CoreP.h>
#include "transform.h"

#define POLY_SIZE	6

/* New fields for the clock widget instance record */
typedef struct {
	Pixel		minute;		/* minute hand pixel */
	Pixel		hour;		/* hour hand pixel */
	Pixel		jewel;		/* jewel pixel */
	GC		minuteGC;	/* pointer to GraphicsContext */
	GC		hourGC;		/* pointer to GraphicsContext */
	GC		jewelGC;	/* pointer to GraphicsContext */
	GC		eraseGC;	/* pointer to GraphicsContext */
#ifdef SHAPE
	GC		shapeGC;	/* pointer to GraphicsContext */
#endif
/* start of graph stuff */
	int		backing_store;	/* backing store variety */
	Boolean		reverse_video;	/* swap fg and bg pixels */
#ifdef SHAPE
	Boolean		shape_window;	/* use SetWindowShapeMask */
#endif
	XtIntervalId	interval_id;
	Transform	t;
	Transform	maskt;
#ifdef SHAPE
	Pixmap		shape_mask;	/* window shape */
	int		shape_width;	/* window width when shape last made */
	int		shape_height;	/* window height when shape last made */
#endif
	double		hour_angle;	/* hour hand position */
	double		minute_angle;	/* minute hand position */
	int		polys_valid;	/* polygons contain good data */
	TPoint		minute_poly[POLY_SIZE];	/* polygon for minute hand */
	TPoint		hour_poly[POLY_SIZE];	/* polygon for hour hand */
} ClockPart;

/* Full instance record declaration */
typedef struct _ClockRec {
	CorePart core;
	ClockPart clock;
} ClockRec;

/* New fields for the Clock widget class record */
typedef struct {int dummy;} ClockClassPart;

/* Full class record declaration. */
typedef struct _ClockClassRec {
	CoreClassPart core_class;
	ClockClassPart clock_class;
} ClockClassRec;

/* Class pointer. */
extern ClockClassRec clockClassRec;

#endif /* _ClockP_h */
