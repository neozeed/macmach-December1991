#ifndef lint
static char Xrcsid[] = "$XConsortium: Converters.c,v 1.53 89/12/20 16:32:37 swick Exp $";
/* $oHeader: Converters.c,v 1.6 88/09/01 09:26:23 asente Exp $ */
#endif /*lint*/
/*LINTLIBRARY*/

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Conversion.c - implementations of resource type conversion procs */

#include	"StringDefs.h"
#include	<stdio.h>
#include        <X11/cursorfont.h>
#include	<X11/keysym.h>
#include	"IntrinsicI.h"
#include	"Quarks.h"

#define	done(type, value) \
	{							\
	    if (toVal->addr != NULL) {				\
		if (toVal->size < sizeof(type)) {		\
		    toVal->size = sizeof(type);			\
		    return False;				\
		}						\
		*(type*)(toVal->addr) = (value);		\
	    }							\
	    else {						\
		static type static_val;				\
		static_val = (value);				\
		toVal->addr = (XtPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}

void XtDisplayStringConversionWarning(dpy, from, toType)
    Display* dpy;
    String from, toType;
{
    static enum {Check, Report, Ignore} report_it = Check;

    if (report_it == Check) {
	XrmDatabase rdb = XtDatabase(dpy);
	static Boolean CvtStringToBoolean();
	XrmName xrm_name[2];
	XrmClass xrm_class[2];
	XrmRepresentation rep_type;
	XrmValue value;
	xrm_name[0] = StringToName( "stringConversionWarnings" );
	xrm_name[1] = NULL;
	xrm_class[0] = StringToClass( "StringConversionWarnings" );
	xrm_class[1] = NULL;
	if (XrmQGetResource( rdb, xrm_name, xrm_class,
			     &rep_type, &value ))
	{
	    if (rep_type == StringToQuark(XtRBoolean))
		report_it = *(Boolean*)value.addr ? Report : Ignore;
	    else if (rep_type == StringToQuark(XtRString)) {
		XrmValue toVal;
		Boolean report;
		toVal.addr = (caddr_t)&report; /* Xresource.h says caddr_t */
		toVal.size = sizeof(Boolean);
		if (XtCallConverter(dpy, CvtStringToBoolean, (XrmValuePtr)NULL,
				    (Cardinal)0, &value, &toVal,
				    (XtCacheRef*)NULL))
		    report_it = report ? Report : Ignore;
	    }
	    else report_it = Report;
	}
	else report_it = Report;
    }

    if (report_it == Report) {
	String params[2];
	Cardinal num_params = 2;
	params[0] = from;
	params[1] = toType;
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		   "conversionError","string","XtToolkitError",
		   "Cannot convert string \"%s\" to type %s",
		    params,&num_params);
    }
}

void XtStringConversionWarning(from, toType)
    String from, toType;
{
	String params[2];
	Cardinal num_params = 2;
	params[0] = from;
	params[1] = toType;
	XtWarningMsg("conversionError","string","XtToolkitError",
		   "Cannot convert string \"%s\" to type %s",
		    params,&num_params);
}

static int CompareISOLatin1();

static Boolean CvtXColorToPixel();
static Boolean CvtIntToBoolean();
static Boolean CvtIntToBool();
static Boolean CvtIntToPixmap();
static Boolean CvtIntToFont();
static Boolean CvtIntOrPixelToXColor();
static Boolean CvtIntToPixel();

static Boolean CvtStringToBoolean();
static Boolean CvtStringToBool();
static Boolean CvtStringToCursor();
static Boolean CvtStringToDisplay();
static Boolean CvtStringToFile();
static Boolean CvtStringToFloat();
static Boolean CvtStringToFont();
static Boolean CvtStringToFontStruct();
static Boolean CvtStringToGeometry();
static Boolean CvtStringToInt();
static Boolean CvtStringToShort();
static Boolean CvtStringToUnsignedChar();
static Boolean CvtStringToPixel();

static Boolean IsInteger(string, value)
    String string;
    int *value;
{
    Boolean foundDigit = False;
    Boolean isNegative = False;
    Boolean isPositive = False;
    int val = 0;
    char ch;
    /* skip leading whitespace */
    while ((ch = *string) == ' ' || ch == '\t') string++;
    while (ch = *string++) {
	if (ch >= '0' && ch <= '9') {
	    val *= 10;
	    val += ch - '0';
	    foundDigit = True;
	    continue;
	}
	if (ch == ' ' || ch == '\t') {
	    if (!foundDigit) return False;
	    /* make sure only trailing whitespace */
	    while (ch = *string++) {
		if (ch != ' ' && ch != '\t')
		    return False;
	    }
	    break;
	}
	if (ch == '-' && !foundDigit && !isNegative && !isPositive) {
	    isNegative = True;
	    continue;
	}
	if (ch == '+' && !foundDigit && !isNegative && !isPositive) {
	    isPositive = True;
	    continue;
	}
	return False;
    }
    if (ch == '\0') {
	if (isNegative)
	    *value = -val;
	else
	    *value = val;
	return True;
    }
    return False;
}


/*ARGSUSED*/
static Boolean CvtIntToBoolean(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtIntToBoolean","XtToolkitError",
                  "Integer to Boolean conversion needs no extra arguments",
                   (String *)NULL, (Cardinal *)NULL);
    done(Boolean, (*(int *)fromVal->addr != 0));
}


/*ARGSUSED*/
static Boolean CvtIntToShort(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtIntToShort","XtToolkitError",
                  "Integer to Short conversion needs no extra arguments",
                   (String *)NULL, (Cardinal *)NULL);
    done(short, (*(int *)fromVal->addr));
}


/*ARGSUSED*/
static Boolean CvtStringToBoolean(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    String str = (String)fromVal->addr;
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToBoolean","XtToolkitError",
                  "String to Boolean conversion needs no extra arguments",
                   (String *)NULL, (Cardinal *)NULL);

    if (   (CompareISOLatin1(str, "true") == 0)
	|| (CompareISOLatin1(str, "yes") == 0)
	|| (CompareISOLatin1(str, "on") == 0)
	|| (CompareISOLatin1(str, "1") == 0))	done( Boolean, True );

    if (   (CompareISOLatin1(str, "false") == 0)
	|| (CompareISOLatin1(str, "no") == 0)
	|| (CompareISOLatin1(str, "off") == 0)
	|| (CompareISOLatin1(str, "0") == 0))	done( Boolean, False );

    XtDisplayStringConversionWarning(dpy, str, "Boolean");
    return False;
}


/*ARGSUSED*/
static Boolean CvtIntToBool(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtIntToBool","XtToolkitError",
                  "Integer to Bool conversion needs no extra arguments",
                   (String *)NULL, (Cardinal *)NULL);
    done(Bool, (*(int *)fromVal->addr != 0));
}


/*ARGSUSED*/
static Boolean CvtStringToBool(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    String str = (String)fromVal->addr;
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		"wrongParameters","cvtStringToBool",
		"XtToolkitError",
                 "String to Bool conversion needs no extra arguments",
                  (String *)NULL, (Cardinal *)NULL);

    if (   (CompareISOLatin1(str, "true") == 0)
	|| (CompareISOLatin1(str, "yes") == 0)
	|| (CompareISOLatin1(str, "on") == 0)
	|| (CompareISOLatin1(str, "1") == 0))	done( Bool, True );

    if (   (CompareISOLatin1(str, "false") == 0)
	|| (CompareISOLatin1(str, "no") == 0)
	|| (CompareISOLatin1(str, "off") == 0)
	|| (CompareISOLatin1(str, "0") == 0))	done( Bool, False );

    XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "Bool");
    return False;
}

XtConvertArgRec colorConvertArgs[] = {
    {XtWidgetBaseOffset, (XtPointer)XtOffset(Widget, core.screen),  sizeof(Screen *)},
    {XtWidgetBaseOffset, (XtPointer)XtOffset(Widget, core.colormap),sizeof(Colormap)}
};


/* ARGSUSED */
static Boolean CvtIntOrPixelToXColor(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{    
    XColor	c;
    Screen	*screen;
    Colormap	colormap;

    if (*num_args != 2)
      XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	 "wrongParameters","cvtIntOrPixelToXColor","XtToolkitError",
         "Pixel to color conversion needs screen and colormap arguments",
          (String *)NULL, (Cardinal *)NULL);
    screen = *((Screen **) args[0].addr);
    colormap = *((Colormap *) args[1].addr);
    c.pixel = *(int *)fromVal->addr;

    XQueryColor(DisplayOfScreen(screen), colormap, &c);
    done(XColor, c);
}


static Boolean CvtStringToPixel(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    String	    str = (String)fromVal->addr;
    XColor	    screenColor;
    XColor	    exactColor;
    Screen	    *screen;
    XtPerDisplay    pd = _XtGetPerDisplay(dpy);
    Colormap	    colormap;
    Status	    status;
    String          params[1];
    Cardinal	    num_params=1;

    if (*num_args != 2)
     XtAppErrorMsg(pd->appContext, "wrongParameters", "cvtStringToPixel",
		   "XtToolkitError",
	"String to pixel conversion needs screen and colormap arguments",
        (String *)NULL, (Cardinal *)NULL);

    screen = *((Screen **) args[0].addr);
    colormap = *((Colormap *) args[1].addr);

    if (CompareISOLatin1(str, XtDefaultBackground) == 0) {
	*closure_ret = False;
	if (pd->rv) done(Pixel, BlackPixelOfScreen(screen))
	else	    done(Pixel, WhitePixelOfScreen(screen));
    }
    if (CompareISOLatin1(str, XtDefaultForeground) == 0) {
	*closure_ret = False;
	if (pd->rv) done(Pixel, WhitePixelOfScreen(screen))
        else	    done(Pixel, BlackPixelOfScreen(screen));
    }

    if (*str == '#') {  /* some color rgb definition */

        status = XParseColor(DisplayOfScreen(screen), colormap,
			     (char*)str, &screenColor);

        if (status != 0)
           status = XAllocColor(DisplayOfScreen(screen), colormap,
                                &screenColor);
    } else  /* some color name */

        status = XAllocNamedColor(DisplayOfScreen(screen), colormap,
                                  (char*)str, &screenColor, &exactColor);
    if (status == 0) {
	params[0] = str;
	XtAppWarningMsg(pd->appContext, "noColormap", "cvtStringToPixel",
			"XtToolkitError",
                 "Cannot allocate colormap entry for \"%s\"",
                  params,&num_params);
	return False;
    } else {
	*closure_ret = (char*)True;
        done(Pixel, screenColor.pixel);
    }
}

/* ARGSUSED */
static void FreePixel(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;
    XrmValuePtr	args;
    Cardinal	*num_args;
{
    Screen	    *screen;
    Colormap	    colormap;

    if (*num_args != 2)
     XtAppErrorMsg(app, "wrongParameters","freePixel","XtToolkitError",
	"Freeing a pixel requires screen and colormap arguments",
        (String *)NULL, (Cardinal *)NULL);

    screen = *((Screen **) args[0].addr);
    colormap = *((Colormap *) args[1].addr);

    if (closure) {
	XFreeColors( DisplayOfScreen(screen), colormap,
		     (unsigned long*)toVal->addr, 1, (unsigned long)0
		    );
    }
}


/* no longer used by Xt, but it's in the spec */
XtConvertArgRec screenConvertArg[] = {
    {XtWidgetBaseOffset, (XtPointer)XtOffset(Widget, core.screen), sizeof(Screen *)}
};

/*ARGSUSED*/
static void FetchDisplayArg(widget, size, value)
    Widget widget;
    Cardinal *size;
    XrmValue* value;
{
    if (widget == NULL) {
	XtErrorMsg("missingWidget", "fetchDisplayArg", "XtToolkitError",
		   "FetchDisplayArg called without a widget to reference",
		   (String*)NULL, (Cardinal*)NULL);
    }
    value->size = sizeof(Display*);
    value->addr = (caddr_t)&DisplayOfScreen(XtScreenOfObject(widget));
}

static XtConvertArgRec displayConvertArg[] = {
    {XtProcedureArg, (XtPointer)FetchDisplayArg, 0},
};

/*ARGSUSED*/
static Boolean CvtStringToCursor(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;

    XtPointer	*closure_ret;
{
    static struct _CursorName {
	char		*name;
	unsigned int	shape;
    } cursor_names[] = {
			{"X_cursor",		XC_X_cursor},
			{"arrow",		XC_arrow},
			{"based_arrow_down",	XC_based_arrow_down},
			{"based_arrow_up",	XC_based_arrow_up},
			{"boat",		XC_boat},
			{"bogosity",		XC_bogosity},
			{"bottom_left_corner",	XC_bottom_left_corner},
			{"bottom_right_corner",	XC_bottom_right_corner},
			{"bottom_side",		XC_bottom_side},
			{"bottom_tee",		XC_bottom_tee},
			{"box_spiral",		XC_box_spiral},
			{"center_ptr",		XC_center_ptr},
			{"circle",		XC_circle},
			{"clock",		XC_clock},
			{"coffee_mug",		XC_coffee_mug},
			{"cross",		XC_cross},
			{"cross_reverse",	XC_cross_reverse},
			{"crosshair",		XC_crosshair},
			{"diamond_cross",	XC_diamond_cross},
			{"dot",			XC_dot},
			{"dotbox",		XC_dotbox},
			{"double_arrow",	XC_double_arrow},
			{"draft_large",		XC_draft_large},
			{"draft_small",		XC_draft_small},
			{"draped_box",		XC_draped_box},
			{"exchange",		XC_exchange},
			{"fleur",		XC_fleur},
			{"gobbler",		XC_gobbler},
			{"gumby",		XC_gumby},
			{"hand1",		XC_hand1},
			{"hand2",		XC_hand2},
			{"heart",		XC_heart},
			{"icon",		XC_icon},
			{"iron_cross",		XC_iron_cross},
			{"left_ptr",		XC_left_ptr},
			{"left_side",		XC_left_side},
			{"left_tee",		XC_left_tee},
			{"leftbutton",		XC_leftbutton},
			{"ll_angle",		XC_ll_angle},
			{"lr_angle",		XC_lr_angle},
			{"man",			XC_man},
			{"middlebutton",	XC_middlebutton},
			{"mouse",		XC_mouse},
			{"pencil",		XC_pencil},
			{"pirate",		XC_pirate},
			{"plus",		XC_plus},
			{"question_arrow",	XC_question_arrow},
			{"right_ptr",		XC_right_ptr},
			{"right_side",		XC_right_side},
			{"right_tee",		XC_right_tee},
			{"rightbutton",		XC_rightbutton},
			{"rtl_logo",		XC_rtl_logo},
			{"sailboat",		XC_sailboat},
			{"sb_down_arrow",	XC_sb_down_arrow},
			{"sb_h_double_arrow",	XC_sb_h_double_arrow},
			{"sb_left_arrow",	XC_sb_left_arrow},
			{"sb_right_arrow",	XC_sb_right_arrow},
			{"sb_up_arrow",		XC_sb_up_arrow},
			{"sb_v_double_arrow",	XC_sb_v_double_arrow},
			{"shuttle",		XC_shuttle},
			{"sizing",		XC_sizing},
			{"spider",		XC_spider},
			{"spraycan",		XC_spraycan},
			{"star",		XC_star},
			{"target",		XC_target},
			{"tcross",		XC_tcross},
			{"top_left_arrow",	XC_top_left_arrow},
			{"top_left_corner",	XC_top_left_corner},
			{"top_right_corner",	XC_top_right_corner},
			{"top_side",		XC_top_side},
			{"top_tee",		XC_top_tee},
			{"trek",		XC_trek},
			{"ul_angle",		XC_ul_angle},
			{"umbrella",		XC_umbrella},
			{"ur_angle",		XC_ur_angle},
			{"watch",		XC_watch},
			{"xterm",		XC_xterm},
    };
    struct _CursorName *nP;
    char *name = (char *)fromVal->addr;
    register int i;

    if (*num_args != 1)
	XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	     "wrongParameters","cvtStringToCursor","XtToolkitError",
             "String to cursor conversion needs display argument",
              (String *)NULL, (Cardinal *)NULL);

    for (i=0, nP=cursor_names; i < XtNumber(cursor_names); i++, nP++ ) {
	if (strcmp(name, nP->name) == 0) {
	    Display *display = *(Display**)args[0].addr;
	    Cursor cursor = XCreateFontCursor(display, nP->shape );
	    done(Cursor, cursor);
	}
    }
    XtDisplayStringConversionWarning(dpy, name, "Cursor");
    return False;
}

/* ARGSUSED */
static void FreeCursor(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;	/* unused */
    XrmValuePtr	args;		/* unused */
    Cardinal	*num_args;
{
    Display*	display;

    if (*num_args != 1)
     XtAppErrorMsg(app,
	     "wrongParameters","freeCursor","XtToolkitError",
             "Free Cursor requires display argument",
              (String *)NULL, (Cardinal *)NULL);

    display = *(Display**)args[0].addr;
    XFreeCursor( display, *(Cursor*)toVal->addr );
}

/*ARGSUSED*/
static Boolean CvtStringToDisplay(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    Display	*d;

    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToDisplay","XtToolkitError",
                  "String to Display conversion needs no extra arguments",
                   (String *)NULL, (Cardinal *)NULL);

    d = XOpenDisplay((char *)fromVal->addr);
    if (d != NULL)
	done(Display*, d);

    XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "Display");
    return False;
}


/*ARGSUSED*/
static Boolean CvtStringToFile(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    FILE *f;

    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		 "wrongParameters","cvtStringToFile","XtToolkitError",
                 "String to File conversion needs no extra arguments",
                 (String *) NULL, (Cardinal *)NULL);

    f = fopen((char *)fromVal->addr, "r");
    if (f != NULL)
	done(FILE*, f);

    XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "File");
    return False;
}

/* ARGSUSED */
static void FreeFile(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;	/* unused */
    XrmValuePtr	args;		/* unused */
    Cardinal	*num_args;
{
    if (*num_args != 0)
	XtAppWarningMsg(app,
		 "wrongParameters","freeFile","XtToolkitError",
                 "Free File requires no extra arguments",
                 (String *) NULL, (Cardinal *)NULL);

    fclose( *(FILE**)toVal->addr );
}

/*ARGSUSED*/
static Boolean CvtStringToFloat(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    float f;
    extern double atof();

    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		 "wrongParameters","cvtStringToFloat","XtToolkitError",
                 "String to Float conversion needs no extra arguments",
                 (String *) NULL, (Cardinal *)NULL);

    f = atof(fromVal->addr);
    done(float, f);
}

/*ARGSUSED*/
static Boolean CvtStringToFont(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    Font	f;
    Display*	display;

    if (*num_args != 1)
	XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	     "wrongParameters","cvtStringToFont","XtToolkitError",
             "String to font conversion needs display argument",
              (String *) NULL, (Cardinal *)NULL);

    display = *(Display**)args[0].addr;

    if (CompareISOLatin1((String)fromVal->addr, XtDefaultFont) != 0) {
	f = XLoadFont(display, (char *)fromVal->addr);
	if (f != 0) {
  Done:	    done( Font, f );
	}
	XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "Font");
    }
    /* try and get the default font */

    {
	XrmName xrm_name[2];
	XrmClass xrm_class[2];
	XrmRepresentation rep_type;
	XrmValue value;

	xrm_name[0] = XrmStringToName ("xtDefaultFont");
	xrm_name[1] = NULL;
	xrm_class[0] = XrmStringToClass ("XtDefaultFont");
	xrm_class[1] = NULL;
	if (XrmQGetResource(XtDatabase(dpy), xrm_name, xrm_class, 
			    &rep_type, &value)) {
	    if (rep_type == XtQString) {
		f = XLoadFont(display, (char *)value.addr);
		if (f != 0)
		    goto Done;
		else {
		    XtDisplayStringConversionWarning( dpy, (char *)value.addr,
						      "Font" );
		}
	    } else if (rep_type == XtQFont) {
		f = *(Font*)value.addr;
		goto Done;
	    } else if (rep_type == XtQFontStruct) {
		f = ((XFontStruct*)value.addr)->fid;
		goto Done;
	    }
	}
    }
    /* Should really do XListFonts, but most servers support this */
    f = XLoadFont(dpy,"-*-*-*-R-*-*-*-120-*-*-*-*-ISO8859-1");
    if (f != 0)
	goto Done;

    XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	     "noFont","cvtStringToFont","XtToolkitError",
             "Unable to load any useable ISO8859-1 font",
              (String *) NULL, (Cardinal *)NULL);
    
    return False;
}

/* ARGSUSED */
static void FreeFont(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;	/* unused */
    XrmValuePtr	args;
    Cardinal	*num_args;
{
    Display *display;
    if (*num_args != 1)
	XtAppErrorMsg(app,
	     "wrongParameters","freeFont","XtToolkitError",
             "Free Font needs display argument",
              (String *) NULL, (Cardinal *)NULL);

    display = *(Display**)args[0].addr;
    XUnloadFont( display, *(Font*)toVal->addr );
}

/*ARGSUSED*/
static Boolean CvtIntToFont(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
	   "wrongParameters","cvtIntToFont","XtToolkitError",
           "Integer to Font conversion needs no extra arguments",
            (String *) NULL, (Cardinal *)NULL);
    done(Font, *(int*)fromVal->addr);
}


/*ARGSUSED*/
static Boolean
CvtStringToFontStruct(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    XFontStruct	    *f;
    Display*	display;

    if (*num_args != 1)
     XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	     "wrongParameters","cvtStringToFontStruct","XtToolkitError",
             "String to font conversion needs display argument",
              (String *) NULL, (Cardinal *)NULL);

    display = *(Display**)args[0].addr;

    if (CompareISOLatin1((String)fromVal->addr, XtDefaultFont) != 0) {
	f = XLoadQueryFont(display, (char *)fromVal->addr);
	if (f != NULL) {
  Done:	    done( XFontStruct*, f);
	}

	XtDisplayStringConversionWarning( dpy, (char*)fromVal->addr,
					  "FontStruct" );
    }

    /* try and get the default font */

    {
	XrmName xrm_name[2];
	XrmClass xrm_class[2];
	XrmRepresentation rep_type;
	XrmValue value;

	xrm_name[0] = XrmStringToName ("xtDefaultFont");
	xrm_name[1] = NULL;
	xrm_class[0] = XrmStringToClass ("XtDefaultFont");
	xrm_class[1] = NULL;
	if (XrmQGetResource(XtDatabase(dpy), xrm_name, xrm_class, 
			    &rep_type, &value)) {
	    if (rep_type == XtQString) {
		f = XLoadQueryFont(display, (char*)value.addr);
		if (f != NULL)
		    goto Done;
		else {
		    XtDisplayStringConversionWarning( dpy, (char*)value.addr,
						      "FontStruct" );
		}
	    } else if (rep_type == XtQFont) {
		f = XQueryFont(dpy, *(Font*)value.addr );
		if (f != NULL) goto Done;
	    } else if (rep_type == XtQFontStruct) {
		f = (XFontStruct*)value.addr;
		goto Done;
	    }
	}
    }
    /* Should really do XListFonts, but most servers support this */
    f = XLoadQueryFont(dpy,"-*-*-*-R-*-*-*-120-*-*-*-*-ISO8859-1");
    if (f != NULL)
	goto Done;

    XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
	     "noFont","cvtStringToFontStruct","XtToolkitError",
             "Unable to load any useable ISO8859-1 font",
              (String *) NULL, (Cardinal *)NULL);
    
    return False;
}

/* ARGSUSED */
static void FreeFontStruct(app, toVal, closure, args, num_args)
    XtAppContext app;
    XrmValuePtr	toVal;
    XtPointer	closure;	/* unused */
    XrmValuePtr	args;
    Cardinal	*num_args;
{
    Display *display;
    if (*num_args != 1)
     XtAppErrorMsg(app,
	     "wrongParameters","freeFontStruct","XtToolkitError",
             "Free FontStruct requires display argument",
              (String *) NULL, (Cardinal *)NULL);

    display = *(Display**)args[0].addr;
    XFreeFont( display, *(XFontStruct**)toVal->addr );
}

/*ARGSUSED*/
static Boolean CvtStringToInt(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    int	i;

    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToInt","XtToolkitError",
                  "String to Integer conversion needs no extra arguments",
                  (String *) NULL, (Cardinal *)NULL);
    if (IsInteger((String)fromVal->addr, &i))
	done(int, i);

    XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "Integer");
    return False;
}

/*ARGSUSED*/
static Boolean CvtStringToShort(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
    XtPointer	*closure_ret;
{
    int i;

    if (*num_args != 0)
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
	  "wrongParameters","cvtStringToShort","XtToolkitError",
          "String to Integer conversion needs no extra arguments",
           (String *) NULL, (Cardinal *)NULL);
    if (IsInteger((String)fromVal->addr, &i))
        done(short, (short)i);

    XtDisplayStringConversionWarning(dpy, (char *) fromVal->addr, "Short");
    return False;
}

/*ARGSUSED*/
static Boolean CvtStringToUnsignedChar(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
    XtPointer	*closure_ret;
{
    int i;

    if (*num_args != 0)
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToUnsignedChar","XtToolkitError",
                  "String to Integer conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);
    if (IsInteger((String)fromVal->addr, &i)) {
        if ( i < 0 || i > 255 )
            XtDisplayStringConversionWarning(dpy, (char*)fromVal->addr,
					 "Unsigned Char");
        done(unsigned char, i);
    }
    XtDisplayStringConversionWarning(dpy, (char*)fromVal->addr, "Unsigned Char");
    return False;
}


/*ARGSUSED*/
static Boolean CvtXColorToPixel(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtXColorToPixel","XtToolkitError",
                  "Color to Pixel conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);
    done(Pixel, ((XColor *)fromVal->addr)->pixel);
}

/*ARGSUSED*/
static Boolean CvtIntToPixel(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtIntToPixel","XtToolkitError",
                  "Integer to Pixel conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);
    done(Pixel, *(int*)fromVal->addr);
}

/*ARGSUSED*/
static Boolean CvtIntToPixmap(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
        XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtIntToPixmap","XtToolkitError",
                  "Integer to Pixmap conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);
    done(Pixmap, *(Pixmap*)fromVal->addr);
}

/*ARGSUSED*/
static Boolean
CvtStringToGeometry(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToGeometry","XtToolkitError",
                  "String to Geometry conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);
    done(String, *(String*)fromVal->addr);
}

void LowerCase(source, dest)	/* %%% shouldn't be global */
    register char  *source, *dest;
{
    register char ch;
    int i;

    for (i = 0; (ch = *source) != 0 && i < 999; source++, dest++, i++) {
    	if ('A' <= ch && ch <= 'Z')
	    *dest = ch - 'A' + 'a';
	else
	    *dest = ch;
    }
    *dest = 0;
}

static int CompareISOLatin1 (first, second)
    char *first, *second;
{
    register unsigned char *ap, *bp;

    for (ap = (unsigned char *) first, bp = (unsigned char *) second;
	 *ap && *bp; ap++, bp++) {
	register unsigned char a, b;

	if ((a = *ap) != (b = *bp)) {
	    /* try lowercasing and try again */

	    if ((a >= XK_A) && (a <= XK_Z))
	      a += (XK_a - XK_A);
	    else if ((a >= XK_Agrave) && (a <= XK_Odiaeresis))
	      a += (XK_agrave - XK_Agrave);
	    else if ((a >= XK_Ooblique) && (a <= XK_Thorn))
	      a += (XK_oslash - XK_Ooblique);

	    if ((b >= XK_A) && (b <= XK_Z))
	      b += (XK_a - XK_A);
	    else if ((b >= XK_Agrave) && (b <= XK_Odiaeresis))
	      b += (XK_agrave - XK_Agrave);
	    else if ((b >= XK_Ooblique) && (b <= XK_Thorn))
	      b += (XK_oslash - XK_Ooblique);

	    if (a != b) break;
	}
    }
    return (((int) *bp) - ((int) *ap));
}


/*ARGSUSED*/
static Boolean 
CvtStringToInitialState(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    String str = (String)fromVal->addr;
    if (*num_args != 0)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToInitialState","XtToolkitError",
                  "String to InitialState conversion needs no extra arguments",
                   (String *) NULL, (Cardinal *)NULL);

    if (CompareISOLatin1(str, "NormalState") == 0) done(int, NormalState);
    if (CompareISOLatin1(str, "IconicState") == 0) done(int, IconicState);
    {
	int val;
	if (IsInteger(str, &val)) done( int, val );
    }
    XtDisplayStringConversionWarning(dpy, str, XtRInitialState);
    return False;
}

XtConvertArgRec visualConvertArgs[] = {
    {XtWidgetBaseOffset, (XtPointer)XtOffset(Widget, core.screen), sizeof(Screen *)},
    {XtWidgetBaseOffset, (XtPointer)XtOffset(Widget, core.depth), sizeof(Cardinal)}
};

/*ARGSUSED*/
static Boolean 
CvtStringToVisual(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;		/* Screen, depth */
    Cardinal    *num_args;	/* 2 */
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;	/* unused */
{
    String str = (String)fromVal->addr;
    int vc;
    XVisualInfo vinfo;
    if (*num_args != 2)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToVisual","XtToolkitError",
                  "String to Visual conversion needs screen and depth arguments",
                   (String *) NULL, (Cardinal *)NULL);

         if (CompareISOLatin1(str, "StaticGray") == 0)	vc = StaticGray;
    else if (CompareISOLatin1(str, "StaticColor") == 0)	vc = StaticColor;
    else if (CompareISOLatin1(str, "TrueColor") == 0)	vc = TrueColor;
    else if (CompareISOLatin1(str, "GrayScale") == 0)	vc = GrayScale;
    else if (CompareISOLatin1(str, "PseudoColor") == 0)	vc = PseudoColor;
    else if (CompareISOLatin1(str, "DirectColor") == 0)	vc = DirectColor;
    else if (!IsInteger(str, &vc)) {
	XtDisplayStringConversionWarning(dpy, str, "Visual class name");
	return False;
    }

    if (XMatchVisualInfo( dpy,
		     XScreenNumberOfScreen((Screen*)*(Screen**)args[0].addr),
		     (int)*(int*)args[1].addr,
		     vc,
		     &vinfo) ) {
	done( Visual*, vinfo.visual );
    }
    else {
	String params[2];
	Cardinal num_params = 2;
	params[0] = str;
	params[1] = DisplayString(dpy);
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "conversionError", "stringToVisual", "XtToolkitError",
                  "Cannot find Visual of class %s for display %s",
		  params, &num_params );
	return False;
    }
}


/*ARGSUSED*/
static Boolean 
CvtStringToAtom(dpy, args, num_args, fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    Atom atom;
    if (*num_args != 1)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
		  "wrongParameters","cvtStringToAtom","XtToolkitError",
                  "String to Atom conversion needs Display argument",
                   (String *) NULL, (Cardinal *)NULL);

    
    atom =  XInternAtom( *(Display**)args->addr, (char*)fromVal->addr, False );
    done(Atom, atom);
}


XrmQuark  XtQAtom;
XrmQuark  XtQBoolean;
XrmQuark  XtQBool;
XrmQuark  XtQColor;
XrmQuark  XtQCursor;
XrmQuark  XtQDisplay;
XrmQuark  XtQDimension;
XrmQuark  XtQFile;
XrmQuark  XtQFloat;
XrmQuark  XtQFont;
XrmQuark  XtQFontStruct;
XrmQuark  XtQGeometry;
XrmQuark  XtQInitialState;
XrmQuark  XtQInt;
XrmQuark  XtQPixel;
XrmQuark  XtQPixmap;
XrmQuark  XtQPointer;
XrmQuark  XtQPosition;
XrmQuark  XtQShort;
XrmQuark  XtQString;
XrmQuark  XtQUnsignedChar;
XrmQuark  XtQVisual;
XrmQuark  XtQWindow;

void _XtConvertInitialize()
{
/* Representation types */

    XtQAtom		= XrmStringToQuark(XtRAtom);
    XtQBoolean		= XrmStringToQuark(XtRBoolean);
    XtQColor		= XrmStringToQuark(XtRColor);
    XtQCursor		= XrmStringToQuark(XtRCursor);
    XtQDimension	= XrmStringToQuark(XtRDimension);
    XtQDisplay		= XrmStringToQuark(XtRDisplay);
    XtQFile		= XrmStringToQuark(XtRFile);
    XtQFloat		= XrmStringToQuark(XtRFloat);
    XtQFont		= XrmStringToQuark(XtRFont);
    XtQFontStruct	= XrmStringToQuark(XtRFontStruct);
    XtQGeometry		= XrmStringToQuark(XtRGeometry);
    XtQInitialState     = XrmStringToQuark(XtRInitialState);
    XtQInt		= XrmStringToQuark(XtRInt);
    XtQBool		= XrmStringToQuark(XtRBool);
    XtQPixel		= XrmStringToQuark(XtRPixel);
    XtQPixmap		= XrmStringToQuark(XtRPixmap);
    XtQPointer		= XrmStringToQuark(XtRPointer);
    XtQPosition		= XrmStringToQuark(XtRPosition);
    XtQShort            = XrmStringToQuark(XtRShort);
    XtQString		= XrmStringToQuark(XtRString);
    XtQUnsignedChar     = XrmStringToQuark(XtRUnsignedChar);
    XtQVisual	        = XrmStringToQuark(XtRVisual);
    XtQWindow		= XrmStringToQuark(XtRWindow);
}

_XtAddDefaultConverters(table)
    ConverterTable table;
{
#define Add(from, to, proc, convert_args, num_args, cache) \
    _XtTableAddConverter(table, from, to, proc, \
	    (XtConvertArgList) convert_args, (Cardinal)num_args, \
	    True, cache, (XtDestructor)NULL)

#define Add2(from, to, proc, convert_args, num_args, cache, destructor) \
    _XtTableAddConverter(table, from, to, proc, \
	    (XtConvertArgList) convert_args, (Cardinal)num_args, \
	    True, cache, destructor)

    Add(XtQColor,   XtQPixel,	    CvtXColorToPixel,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQBoolean,	    CvtIntToBoolean,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQBool,	    CvtIntToBool,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQDimension,   CvtIntToShort,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQPixel,	    CvtIntToPixel,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQPosition,    CvtIntToShort,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQPixmap,	    CvtIntToPixmap,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQFont,	    CvtIntToFont,	NULL, 0, XtCacheNone);
    Add(XtQInt,	    XtQColor,	    CvtIntOrPixelToXColor,
	colorConvertArgs, XtNumber(colorConvertArgs), XtCacheByDisplay);

    Add(XtQString,  XtQBoolean,	    CvtStringToBoolean, NULL, 0, XtCacheNone);
    Add(XtQString,  XtQBool,	    CvtStringToBool,	NULL, 0, XtCacheNone);
    Add2(XtQString,  XtQCursor,	    CvtStringToCursor,
	displayConvertArg, XtNumber(displayConvertArg),
	XtCacheByDisplay, FreeCursor);

    Add(XtQString,  XtQDimension,   CvtStringToShort,	NULL, 0, XtCacheNone);
    Add(XtQString,  XtQDisplay,	    CvtStringToDisplay, NULL, 0, XtCacheAll);
    Add2(XtQString, XtQFile,	    CvtStringToFile,	NULL, 0,
	 XtCacheAll | XtCacheRefCount, FreeFile);
    Add(XtQString,  XtQFloat,	    CvtStringToFloat,	NULL, 0, XtCacheNone);
    Add2(XtQString, XtQFont,	    CvtStringToFont,
	displayConvertArg, XtNumber(displayConvertArg),
	XtCacheByDisplay, FreeFont);
    Add2(XtQString, XtQFontStruct,  CvtStringToFontStruct,
	displayConvertArg, XtNumber(displayConvertArg),
	XtCacheByDisplay, FreeFontStruct);
    Add(XtQString,  XtQInt,	    CvtStringToInt,	NULL, 0, XtCacheAll);
    Add(XtQString,  XtQPosition,    CvtStringToShort,	NULL, 0, XtCacheAll);
    Add2(XtQString, XtQPixel,	    CvtStringToPixel,
	colorConvertArgs, XtNumber(colorConvertArgs),
	XtCacheByDisplay, FreePixel);
    Add(XtQString,  XtQShort,	    CvtStringToShort,	NULL, 0, XtCacheAll);
    Add(XtQString,  XtQUnsignedChar,CvtStringToUnsignedChar,NULL,0,XtCacheAll);

    Add(XtQPixel,   XtQColor,	    CvtIntOrPixelToXColor,
	colorConvertArgs, XtNumber(colorConvertArgs), XtCacheByDisplay);

    Add(XtQString,  XtQGeometry,    CvtStringToGeometry, NULL, 0, XtCacheNone);
    Add(XtQString,  XtQInitialState,CvtStringToInitialState, NULL, 0, XtCacheNone);
    Add2(XtQString, XtQVisual,	    CvtStringToVisual,
	visualConvertArgs, XtNumber(visualConvertArgs),
	XtCacheByDisplay, NULL);

    Add(XtQString,  XtQAtom,	    CvtStringToAtom,
	displayConvertArg, XtNumber(displayConvertArg), XtCacheByDisplay);

   _XtAddTMConverters(table);
}
