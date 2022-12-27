/*
 * Clock.c
 *
 * a NeWS clone clock
 */

# include <X11/Xos.h>
# include <stdio.h>
# include <X11/IntrinsicP.h>
# include <X11/StringDefs.h>
# include <X11/Xmu/Converters.h>
# include "ClockP.h"
# include <math.h>
#ifdef SHAPE
# include <X11/extensions/shape.h>
#endif

#define offset(field) XtOffset(ClockWidget,clock.field)
#define goffset(field) XtOffset(Widget,core.field)

static XtResource resources[] = {
    {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
	goffset(width), XtRString, "120"},
    {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
	goffset(height), XtRString, "120"},
    {XtNminute, XtCForeground, XtRPixel, sizeof (Pixel),
	offset(minute), XtRString, XtDefaultForeground},
    {XtNhour, XtCForeground, XtRPixel, sizeof (Pixel),
	offset(hour), XtRString, XtDefaultForeground},
    {XtNjewel, XtCForeground, XtRPixel, sizeof (Pixel),
	offset(jewel), XtRString, XtDefaultForeground},
    {XtNreverseVideo, XtCReverseVideo, XtRBoolean, sizeof (Boolean),
	offset (reverse_video), XtRString, "FALSE"},
    {XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof (int),
    	offset (backing_store), XtRString, "default"},
#ifdef SHAPE
    {XtNshapeWindow, XtCShapeWindow, XtRBoolean, sizeof (Boolean),
	offset (shape_window), XtRString, "TRUE"},
#endif
};

#undef offset
#undef goffset

static void Initialize(), Realize(), Destroy(), Redisplay(), Resize();

# define BORDER_WIDTH	(0.1)
# define WINDOW_WIDTH	(2.0 - BORDER_WIDTH*2)
# define WINDOW_HEIGHT	(2.0 - BORDER_WIDTH*2)
# define MINUTE_WIDTH	(BORDER_WIDTH / 2.0)
# define HOUR_WIDTH	(BORDER_WIDTH / 2.0)
# define JEWEL_SIZE	(BORDER_WIDTH * 0.75)
# define MINUTE_LENGTH	(0.8)
# define HOUR_LENGTH	(0.5)
# define JEWEL_X	(0.0)
# define JEWEL_Y	(1.0 - (BORDER_WIDTH + JEWEL_SIZE))

static void ClassInitialize();

ClockClassRec clockClassRec = {
    { /* core fields */
    /* superclass		*/	&widgetClassRec,
    /* class_name		*/	"Clock",
    /* size			*/	sizeof(ClockRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULL,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	NULL,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	NULL,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    }
};

static void ClassInitialize()
{
    XtAddConverter( XtRString, XtRBackingStore, XmuCvtStringToBackingStore,
		    NULL, 0 );
}

WidgetClass clockWidgetClass = (WidgetClass) &clockClassRec;

/* ARGSUSED */
static void Initialize (greq, gnew)
    Widget greq, gnew;
{
    ClockWidget w = (ClockWidget)gnew;
    XtGCMask	valuemask;
    XGCValues	myXGCV;
#ifdef SHAPE
    int shape_event_base, shape_error_base;
#endif

    valuemask = GCForeground | GCBackground;
    myXGCV.background = w->core.background_pixel;

    myXGCV.foreground = w->clock.minute;
    w->clock.minuteGC = XtGetGC(gnew, valuemask, &myXGCV);

    myXGCV.foreground = w->clock.hour;
    w->clock.hourGC = XtGetGC(gnew, valuemask, &myXGCV);

    myXGCV.foreground = w->clock.jewel;
    w->clock.jewelGC = XtGetGC (gnew, valuemask, &myXGCV);

    myXGCV.foreground = w->core.background_pixel;
    w->clock.eraseGC = XtGetGC(gnew, valuemask, &myXGCV);

    /* wait for Realize to add the timeout */
    w->clock.interval_id = 0;

#ifdef  SHAPE
    if (w->clock.shape_window && !XShapeQueryExtension (XtDisplay (w), 
							&shape_event_base, 
							&shape_error_base))
	w->clock.shape_window = False;
    w->clock.shape_mask = 0;
    w->clock.shapeGC = 0;
    w->clock.shape_width = 0;
    w->clock.shape_height = 0;
#endif
    w->clock.polys_valid = 0;
}

static void Resize (w)
    ClockWidget	w;
{
    XGCValues	xgcv;
    Widget	parent;
    XWindowChanges	xwc;
    int		face_width, face_height;
    int		x, y;
#ifdef SHAPE
    Pixmap	shape_mask;
#endif

    if (!XtIsRealized(w))
	return;

    /*
     * compute desired border size
     */

    SetTransform (&w->clock.maskt,
		  0, w->core.width,
		  w->core.height, 0,
		  -1.0, 1.0,
 		  -1.0, 1.0);

    face_width = abs (Xwidth (BORDER_WIDTH, BORDER_WIDTH, &w->clock.maskt));
    face_height = abs (Xheight (BORDER_WIDTH, BORDER_WIDTH, &w->clock.maskt));

    /*
     *  shape the windows and borders
     */

#ifdef SHAPE
    if (w->clock.shape_window) {

	SetTransform (&w->clock.t,
			face_width, w->core.width - face_width,
			w->core.height - face_height, face_height,
			-WINDOW_WIDTH/2, WINDOW_WIDTH/2,
			-WINDOW_HEIGHT/2, WINDOW_HEIGHT/2);
    
	/*
	 * allocate a pixmap to draw shapes in
	 */

    	shape_mask = XCreatePixmap (XtDisplay (w), XtWindow (w),
				    w->core.width, w->core.height, 1);
    	if (!w->clock.shapeGC)
            w->clock.shapeGC = XCreateGC (XtDisplay (w), shape_mask, 0, &xgcv);

	/* erase the pixmap */
    	XSetForeground (XtDisplay (w), w->clock.shapeGC, 0);
    	XFillRectangle (XtDisplay (w), shape_mask, w->clock.shapeGC,
			0, 0, w->core.width, w->core.height);
    	XSetForeground (XtDisplay (w), w->clock.shapeGC, 1);

	/*
	 * draw the bounding shape.  Doing this first
	 * eliminates extra exposure events.
	 */

	TFillArc (XtDisplay (w), shape_mask,
			w->clock.shapeGC, &w->clock.maskt,
			-1.0, -1.0,
			2.0, 2.0,
			0, 360 * 64);
	/*
	 * Find the highest enclosing widget and shape it
	 */

	x = 0;
	y = 0;
	for (parent = (Widget) w; XtParent (parent); parent = XtParent (parent)) {
	    x = x + parent->core.x + parent->core.border_width;
	    y = y + parent->core.y + parent->core.border_width;
	}

	XShapeCombineMask (XtDisplay (parent), XtWindow (parent), ShapeBounding,
			    x, y, shape_mask, ShapeSet);

	/* erase the pixmap */
    	XSetForeground (XtDisplay (w), w->clock.shapeGC, 0);
    	XFillRectangle (XtDisplay (w), shape_mask, w->clock.shapeGC,
			0, 0, w->core.width, w->core.height);
    	XSetForeground (XtDisplay (w), w->clock.shapeGC, 1);

	/*
	 * draw the clip shape
	 */

	TFillArc (XtDisplay (w), shape_mask,
			w->clock.shapeGC, &w->clock.t,
			-WINDOW_WIDTH/2, -WINDOW_HEIGHT/2,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 360 * 64);

	XShapeCombineMask (XtDisplay (w), XtWindow (w), ShapeClip, 
		    0, 0, shape_mask, ShapeSet);

	XFreePixmap (XtDisplay (w), shape_mask);

    } else
#endif
    {
    	/*
     	 * reconfigure the widget to split the availible
     	 * space between the window and the border
     	 */

     	if (face_width > face_height)
	    xwc.border_width = face_height;
    	else
	    xwc.border_width = face_width;
    	xwc.width = w->core.width - xwc.border_width * 2;
    	xwc.height = w->core.height - xwc.border_width * 2;
    	XConfigureWindow (XtDisplay (w), XtWindow (w),
			    CWWidth|CWHeight|CWBorderWidth,
			    &xwc);
    
    	SetTransform (&w->clock.t,
	    0, xwc.width,
	    xwc.height, 0,
	    -WINDOW_WIDTH/2, WINDOW_WIDTH/2,
	    -WINDOW_HEIGHT/2, WINDOW_HEIGHT/2);
    }
}
 
static void Realize (gw, valueMask, attrs)
     Widget gw;
     XtValueMask *valueMask;
     XSetWindowAttributes *attrs;
{
     ClockWidget	w = (ClockWidget)gw;
     static int		new_time();

    if (w->clock.backing_store != Always + WhenMapped + NotUseful) {
     	attrs->backing_store = w->clock.backing_store;
	*valueMask |= CWBackingStore;
    }
    XtCreateWindow( gw, (unsigned)InputOutput, (Visual *)CopyFromParent,
		     *valueMask, attrs );
    Resize (w);
    new_time ((caddr_t) gw, 0);
}

static void Destroy (gw)
     Widget gw;
{
     ClockWidget w = (ClockWidget)gw;
     if (w->clock.interval_id) XtRemoveTimeOut (w->clock.interval_id);
     XtDestroyGC (w->clock.minuteGC);
     XtDestroyGC (w->clock.hourGC);
#ifdef SHAPE
     if (w->clock.shapeGC)
	XtDestroyGC (w->clock.shapeGC);
#endif
}

/* ARGSUSED */
static void Redisplay(gw, event, region)
     Widget gw;
     XEvent *event;
     Region region;
{
    ClockWidget	w;

    w = (ClockWidget) gw;
    paint_jewel (w, XtWindow (w), w->clock.jewelGC);
    paint_hands (w, XtWindow (w), w->clock.minuteGC, w->clock.hourGC);
}

/*
 * routines to draw the hands and jewel
 */

# define PI (3.14159265358979323846)

/*
 * converts a number from 0..1 representing a clockwise radial distance
 * from the 12 oclock position to a radian measure of the counter-clockwise
 * distance from the 3 oclock position
 */

static double
clock_to_angle (clock)
double	clock;
{
	if (clock >= .75)
		clock -= 1.0;
	return -2.0 * PI * clock + PI / 2.0;
}

/* ARGSUSED */
static int new_time (client_data, id)
     caddr_t client_data;
     XtIntervalId id;		/* unused */
{
        ClockWidget	w = (ClockWidget)client_data;
	long		now;
	struct tm	*localtime (), *tm;
	
	if (w->clock.polys_valid) {
		paint_hands (w, XtWindow (w), w->clock.eraseGC, w->clock.eraseGC);
		check_jewel (w, XtWindow (w), w->clock.jewelGC);
	}
	(void) time (&now);
	tm = localtime (&now);
	if (tm->tm_hour >= 12)
		tm->tm_hour -= 12;
	w->clock.hour_angle = clock_to_angle ((((double) tm->tm_hour) +
				((double) tm->tm_min) / 60.0) / 12.0);
	w->clock.minute_angle =
		clock_to_angle (((double) tm->tm_min) / 60.0);
	/*
	 * add the timeout before painting the hands, that may
	 * take a while and we'd like the clock to keep up
	 * with time changes.
	 */
	w->clock.interval_id = XtAddTimeOut ((60 - tm->tm_sec) * 1000, new_time,
				client_data);
	compute_hands (w);
	paint_hands (w, XtWindow (w), w->clock.minuteGC, w->clock.hourGC);
} /* new_time */

paint_jewel (w, d, gc)
ClockWidget w;
Drawable    d;
GC	    gc;
{
	TFillArc (XtDisplay (w), d, gc, &w->clock.t,
			JEWEL_X - JEWEL_SIZE / 2.0, JEWEL_Y - JEWEL_SIZE / 2.0,
 			JEWEL_SIZE, JEWEL_SIZE, 0, 360 * 64);
}

#define sqr(x)	((x)*(x))

/*
 * check to see if the polygon intersects the circular jewel
 */

check_jewel_poly (w, poly)
ClockWidget	w;
TPoint		poly[POLY_SIZE];
{
	double	a2, b2, c2, d2;
	int	i;

	/*
	 * check each side of the polygon to see if the
	 * distance from the line to the center of the
	 * circular jewel is less than the radius.
	 */
	for (i = 0; i < POLY_SIZE-1; i++) {
		a2 = sqr (poly[i].x - JEWEL_X) + sqr (poly[i].y - JEWEL_Y);
		b2 = sqr (poly[i+1].x - JEWEL_X) + sqr (poly[i+1].y - JEWEL_Y);
		c2 = sqr (poly[i].x - poly[i+1].x) + sqr (poly[i].y - poly[i+1].y);
		d2 = a2 + b2 - c2;
		if (d2 <= sqr (JEWEL_SIZE) &&
		    a2 <= 2 * c2 && b2 <= 2 * c2 ||
 		    a2 <= sqr (JEWEL_SIZE) ||
		    b2 <= sqr (JEWEL_SIZE))
			return 1;
	}
	return 0;
}

check_jewel (w, d, gc)
ClockWidget	w;
Drawable	d;
GC		gc;
{
	if (!w->clock.polys_valid)
		return;
	if ((MINUTE_LENGTH >= (JEWEL_Y - JEWEL_SIZE/2.0) &&
	     check_jewel_poly (w, w->clock.minute_poly)) ||
	    (HOUR_LENGTH >= (JEWEL_Y - JEWEL_SIZE/2.0) &&
	     check_jewel_poly (w, w->clock.minute_poly)))
	{
		paint_jewel (w, d, gc);
	}
}

/*
 * A hand is a rectangle with a triangular cap at the far end.
 * This is represented with a five sided polygon.
 */

compute_hand (w, a, l, width, poly)
ClockWidget	w;
double		a, l, width;
TPoint		poly[POLY_SIZE];
{
	double	c, s;

	c = cos(a);
	s = sin(a);
	poly[0].x = c * l;	
	poly[0].y = s * l;
	poly[1].x = (l - width) * c - s * width;
	poly[1].y = (l - width) * s + c * width;
	poly[2].x = (-width) * c - s * width;
	poly[2].y = (-width) * s + c * width;
	poly[3].x = (-width) * c + s * width;
	poly[3].y = (-width) * s - c * width;
	poly[4].x = (l - width) * c + s * width;
	poly[4].y = (l - width) * s - c * width;
	poly[5].x = poly[0].x;
	poly[5].y = poly[0].y;
}

compute_hands (w)
ClockWidget	w;
{
	compute_hand (w, w->clock.minute_angle,
		MINUTE_LENGTH, MINUTE_WIDTH, w->clock.minute_poly);
	compute_hand (w, w->clock.hour_angle,
		HOUR_LENGTH, HOUR_WIDTH, w->clock.hour_poly);
	w->clock.polys_valid = 1;
}

paint_hand (w, d, gc, poly)
ClockWidget	w;
Drawable	d;
GC		gc;
TPoint		poly[POLY_SIZE];
{
	TFillPolygon (XtDisplay (w), d, gc, &w->clock.t, poly, POLY_SIZE,
			Convex, CoordModeOrigin);
}

paint_hands (w, d, minute_gc, hour_gc)
ClockWidget	w;
Drawable	d;
GC		minute_gc, hour_gc;
{
    if (w->clock.polys_valid) {
	paint_hand (w, d, hour_gc, w->clock.hour_poly);
	paint_hand (w, d, minute_gc, w->clock.minute_poly);
    }
}
