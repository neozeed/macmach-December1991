/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See xtetris.c for complete rights and liability information.
 */

/*
 * Tetris.c - bitmap editor widget.
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <stdio.h>

#include <X11/Xos.h>

#include "TetrisP.h"

#define INTERNAL_WIDTH	2
#define INTERNAL_HEIGHT 4

#define DEFAULT_PIXMAP_WIDTH	32	/* in cells */
#define DEFAULT_PIXMAP_HEIGHT	32	/* in cells */

#define DEFAULT_CELL_SIZE	30	/* in pixels */
#define DEFAULT_DELAY	750	/* in pixels */

/* values for instance variable is_drawn */
#define DRAWN 1
#define UNDRAWN 0

/* modes for drawing */
#define DRAW 1
#define UNDRAW 0

#define MAXLINES 1000	/* max of horiz or vertical cells */
#define SCROLLBARWIDTH 15

#define BLOCKS_PER_CELL 10
#define BLOCK_LENGTH 5
#define BLOCK_WIDTH 2

static short blocks[] = {
	1, 1, 0, 0, 0, 1, 0, 1, 0, 0, /* 0 vertical left L */
	1, 0, 1, 0, 0, 1, 1, 0, 0, 0, /* 1 horiz left L */
	1, 0, 1, 0, 0, 0, 1, 1, 0, 0, /* 2 vertical left L upside*/
	0, 1, 1, 0, 0, 1, 0, 1, 0, 0, /* 3 horiz left L upside*/
	1, 0, 1, 0, 0, 1, 1, 0, 0, 0, /* 4 vertical right L */
	1, 1, 0, 0, 0, 1, 0, 1, 0, 0, /* 5 horiz right L */
	0, 1, 1, 0, 0, 1, 0, 1, 0, 0, /* 6 vertical right L upside */
	1, 0, 1, 0, 0, 0, 1, 1, 0, 0, /* 7 horiz right L upside */
	1, 1, 1, 0, 0, 0, 1, 0, 0, 0, /* 8 vertical T */
	0, 1, 0, 0, 0, 1, 1, 1, 0, 0, /* 9 horiz T */
	0, 1, 0, 0, 0, 1, 1, 1, 0, 0, /* 10 vertical T upside */
	1, 1, 1, 0, 0, 0, 1, 0, 0, 0, /* 11 horiz T upside */
	1, 1, 0, 0, 0, 0, 1, 1, 0, 0, /* 12 vertical dogleg left */
	0, 1, 1, 0, 0, 1, 1, 0, 0, 0, /* 13 horiz dogleg left */
	1, 1, 0, 0, 0, 0, 1, 1, 0, 0, /* 14 vertical dogleg right */
	0, 1, 1, 0, 0, 1, 1, 0, 0, 0, /* 15 horiz dogleg right */
	0, 1, 0, 1, 0, 1, 0, 1, 0, 0, /* 16 long bar (vert) */
	0, 1, 0, 1, 0, 1, 0, 1, 0, 0, /* 17 long bar (horiz) */
	1, 1, 0, 0, 0, 1, 1, 0, 0, 0, /* 18 dummy */
	1, 1, 0, 0, 0, 1, 1, 0, 0, 0, /* 19 box (horiz) */
};

#define offset(field) XtOffset(TetrisWidget, field)

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	offset(tetris.foreground), XtRString, XtDefaultForeground },
    {XtNcallback, XtCCallback, XtRCallback, sizeof(caddr_t),
	offset(tetris.callback), XtRCallback, NULL},
    {XtNgameover, XtCGameover, XtRCallback, sizeof(caddr_t),
	offset(tetris.gameover), XtRCallback, NULL},
    {XtNlevel, XtCLevel, XtRInt, sizeof(int),
	offset(tetris.level), XtRImmediate, (caddr_t) 3},
    {XtNcellSizeInPixels, XtCCellSizeInPixels, XtRInt, sizeof(int),
	offset(tetris.cell_size_in_pixels), XtRImmediate, (caddr_t)DEFAULT_CELL_SIZE },
    {XtNpixmapWidthInCells, XtCPixmapWidthInCells, XtRDimension, sizeof(Dimension),
	offset(tetris.pixmap_width_in_cells), XtRImmediate, (caddr_t)DEFAULT_PIXMAP_WIDTH },
    {XtNpixmapHeightInCells, XtCPixmapHeightInCells, XtRDimension, sizeof(Dimension),
	offset(tetris.pixmap_height_in_cells), XtRImmediate, (caddr_t)DEFAULT_PIXMAP_HEIGHT },
    {XtNdelay, XtCDelay, XtRInt, sizeof(int),
	offset(tetris.delay), XtRImmediate, (caddr_t) DEFAULT_DELAY},
};

/* Declaration of methods */

/* 
 * These don't work under SunOS 4.0.1 and perhaps others.
 * static XtInitProc Initialize();
 * static XtExposeProc Redisplay();
 * static XtWidgetProc Destroy();
 * static XtWidgetProc Resize();
 * static XtSetValuesFunc SetValues();
 */

static void Initialize();
static void Redisplay();
static void Destroy();
static void Resize();
static Boolean SetValues();

/* these Core methods not needed by Tetris:
 *
 * static XtProc ClassInitialize();
 * static XtRealizeProc Realize();
 * static void ClassInitialize();
 * static void Realize();
 */

/* the following are private functions unique to Tetris */
static void DrawCell(), DrawPixmaps(), StartBlock(), DrawBlock(), MoveDown();
static void KillRows(), UpdateCellArray(), CheckVisibility();
static Boolean CanMoveDown(), CanMoveSideways(), CanTurn();

/* the following are actions of Tetris */
static void MoveSideways(), Drop(), Turn();

/* the following are public functions of Tetris */
void TetrisStartNewGame(), TetrisPauseGame(), TetrisContinueGame();

/* global variable :-( */
static XtIntervalId timer;

static char trans[] =
         "Shift<Btn2Down>:  Drop()           \n\
	 <Btn1Down>:    MoveSideways(l)              \n\
        !<Btn2Down>:    Turn()               \n\
         <Btn3Down>:    MoveSideways(r)";

static XtActionsRec actions[] = {
        {"MoveSideways", MoveSideways},
        {"Turn", Turn},
        {"Drop", Drop},
};

TetrisClassRec tetrisClassRec = {
    {
    /* core_class fields */
    /* superclass	  	 */ (WidgetClass) &widgetClassRec,
    /* class_name	  	 */ "Tetris",
    /* widget_size	  	 */ sizeof(TetrisRec),
    /* class_initialize   	 */ NULL,
    /* class_part_initialize	 */ NULL,
    /* class_inited       	 */ FALSE,
    /* initialize	  	 */ Initialize,
    /* initialize_hook		 */ NULL,
    /* realize		  	 */ XtInheritRealize,
    /* actions		  	 */ actions,
    /* num_actions	  	 */ XtNumber(actions),
    /* resources	  	 */ resources,
    /* num_resources	  	 */ XtNumber(resources),
    /* xrm_class	  	 */ NULLQUARK,
    /* compress_motion	  	 */ TRUE,
    /* compress_exposure  	 */ TRUE,
    /* compress_enterleave	 */ TRUE,
    /* visible_interest	  	 */ TRUE,
    /* destroy		  	 */ Destroy,
    /* resize		  	 */ Resize,
    /* expose		  	 */ Redisplay,
    /* set_values	  	 */ SetValues,
    /* set_values_hook		 */ NULL,
    /* set_values_almost	 */ XtInheritSetValuesAlmost,
    /* get_values_hook		 */ NULL,
    /* accept_focus	 	 */ NULL,
    /* version			 */ XtVersion,
    /* callback_private   	 */ NULL,
    /* tm_table		   	 */ trans,
    /* query_geometry		 */ NULL,
    /* display_accelerator       */ XtInheritDisplayAccelerator,
    /* extension                 */ NULL
    },
    {
    /* dummy_field               */ 0,
    },
};

WidgetClass tetrisWidgetClass = (WidgetClass) & tetrisClassRec;

static void
GetDrawGC(cw)
TetrisWidget cw;
{
    XGCValues values;
    XtGCMask mask = GCForeground | GCBackground | GCDashOffset | GCDashList | GCLineStyle;

    /* 
     * Setting foreground and background to 1 and 0 looks like a 
     * kludge but isn't.  This GC is used for drawing
     * into a pixmap of depth one.  Real colors are applied with a
     * separate GC when the pixmap is copied into the window.
     */
    values.foreground = 1;
    values.background = 0;
    values.dashes = 1;
    values.dash_offset = 0;
    values.line_style = LineOnOffDash;

    cw->tetris.draw_gc = XCreateGC(XtDisplay(cw), cw->tetris.big_picture, mask, &values);
}

static void
GetUndrawGC(cw)
TetrisWidget cw;
{
    XGCValues values;
    XtGCMask mask = GCForeground | GCBackground;

    /* this looks like a kludge but isn't.  This GC is used for drawing
     * into a pixmap of depth one.  Real colors are applied as the 
     * pixmap is copied into the window.
     */
    values.foreground = 0;
    values.background = 1;

    cw->tetris.undraw_gc = XCreateGC(XtDisplay(cw), cw->tetris.big_picture, mask, &values);
}

static void
GetCopyGC(cw)
TetrisWidget cw;
{
    XGCValues values;
    XtGCMask mask = GCForeground | GCBackground;

    values.foreground = cw->tetris.foreground;
    values.background = cw->core.background_pixel;

    cw->tetris.copy_gc = XtGetGC(cw, mask, &values);
}

/* ARGSUSED */
static void
Initialize(request, new)
TetrisWidget request, new;
{
    int x, y;
    /* 
     *  Check instance values set by resources that may be invalid. 
     */

    if ((new->tetris.pixmap_width_in_cells < 1) ||
		(new->tetris.pixmap_height_in_cells < 1))  {
	XtWarning("Tetris: pixmapWidth and/or pixmapHeight is too small (using 10 x 10)."); 
    	new->tetris.pixmap_width_in_cells = 10;
    	new->tetris.pixmap_height_in_cells = 10;
    }

    if (new->tetris.cell_size_in_pixels < 5) {
	XtWarning("Tetris: cellSize is too small (using 30)."); 
    	new->tetris.cell_size_in_pixels = 30;
    }

    if ((new->tetris.level < 1) || 
               (new->tetris.level > 9)) {
	XtWarning("Tetris: level range is 1 to 9 (using 3)."); 
    	new->tetris.level = 3;
    }

    new->tetris.delay_decrement = new->tetris.level;
    new->tetris.initial_delay = 350 + (500 / new->tetris.level);
    new->tetris.delay = new->tetris.initial_delay;
    new->tetris.running = True; /* start play */

    /* I'm not sure this is necessary... */
    for (y = 0;y < new->tetris.pixmap_height_in_cells; y++)
       for (x = 0;x < new->tetris.pixmap_width_in_cells; x++)
           new->tetris.cell[x][y] = 0;
       
	/*
    new->tetris.cell = XtCalloc(new->tetris.pixmap_width_in_cells * new->tetris.pixmap_height_in_cells, sizeof(int));
	*/

    new->tetris.pixmap_width_in_pixels = new->tetris.pixmap_width_in_cells * new->tetris.cell_size_in_pixels;

    new->tetris.pixmap_height_in_pixels = new->tetris.pixmap_height_in_cells * new->tetris.cell_size_in_pixels;

    new->tetris.score = 0;

    if (new->core.width == 0)
	new->core.width = new->tetris.pixmap_width_in_pixels;
    if (new->core.height == 0)
	new->core.height = new->tetris.pixmap_height_in_pixels;

    CreateBigPixmap(new);

    GetDrawGC(new);
    GetUndrawGC(new);
    GetCopyGC(new);

    /* Pixmaps start with garbage, this will clear it to zero. */

    XFillRectangle( XtDisplay((Widget) new), new->tetris.big_picture, 
		    new->tetris.undraw_gc, 0, 0,
		   (unsigned int) new->tetris.pixmap_width_in_pixels, 
		   (unsigned int) new->tetris.pixmap_height_in_pixels);

#ifdef sun
        srandom(getpid());
#else
        srand(getpid());
#endif

}


/* ARGSUSED */
static void
Redisplay(cw, event)
TetrisWidget cw;
XExposeEvent *event;
{
    register int x, y;
    unsigned int width, height;
    static int first_time = True;
    if (!XtIsRealized(cw))
	return;

    if (first_time) {
         StartBlock(cw);
         first_time = False;
    }

    if (event) {  /* called from btn-event */
	x = event->x;
        y = event->y; 
	width = event->width;
	height =  event->height;
    } 
    else {        /* called because of expose */
	x = 0;
        y = 0; 
	width = cw->tetris.pixmap_width_in_pixels;
	height = cw->tetris.pixmap_height_in_pixels;
    }

    if (DefaultDepthOfScreen(XtScreen(cw)) == 1)
	XCopyArea(XtDisplay(cw), cw->tetris.big_picture, XtWindow(cw),
	    cw->tetris.copy_gc, x, y, width, height, x, y);
    else
	XCopyPlane(XtDisplay(cw), cw->tetris.big_picture, XtWindow(cw),
	    cw->tetris.copy_gc, x, y, width, height, x, y, 1);

}

/* ARGSUSED */
static Boolean
SetValues(current, request, new)
Widget current, request, new;
{
    TetrisWidget curcw = (TetrisWidget) current;
    TetrisWidget newcw = (TetrisWidget) new;
    Boolean do_redisplay = False;

/*    
    if (curcw->core.sensitive != newcw->core.sensitive ||
	curcw->tetris.checked != newcw->tetris.checked ||
	curcw->tetris.fancy != newcw->tetris.fancy)
	do_redisplay = True;
*/

    if (curcw->tetris.foreground != newcw->tetris.foreground) {
	XtReleaseGC(curcw, curcw->tetris.copy_gc);
	GetCopyGC(newcw);
	do_redisplay = True;
    }

    if ((curcw->tetris.pixmap_width_in_cells != newcw->tetris.pixmap_width_in_cells) || 
		(curcw->tetris.pixmap_height_in_cells != newcw->tetris.pixmap_height_in_cells) || 
		(curcw->tetris.cell_size_in_pixels != newcw->tetris.cell_size_in_pixels))
	fprintf(stderr, "Tetris widget: pixmap_width_in_cells, pixmap_heigt, and cell_size_in_pixels resources can only be set before widget is created\n");

    return do_redisplay;
}


static void
Destroy(cw)
TetrisWidget cw;
{
    if (cw->tetris.big_picture)
	XFreePixmap(XtDisplay(cw), cw->tetris.big_picture);

    if (cw->tetris.draw_gc)
	XFreeGC(XtDisplay(cw), cw->tetris.draw_gc);

    if (cw->tetris.undraw_gc)
	XFreeGC(XtDisplay(cw), cw->tetris.undraw_gc);

    XtFree(cw->tetris.cell);
}

static void
CopyBlock(w)
TetrisWidget w;
{
    XExposeEvent fake_event;

    fake_event.x = w->tetris.cell_size_in_pixels * (w->tetris.cur_x - 1);
    fake_event.y = w->tetris.cell_size_in_pixels * (w->tetris.cur_y - 1);
    fake_event.width = 6 * w->tetris.cell_size_in_pixels;
    fake_event.height = 6 * w->tetris.cell_size_in_pixels;

    Redisplay(w, &fake_event);
}


CreateBigPixmap(cw)
TetrisWidget cw;
{
    /* always a 1 bit deep pixmap, regardless of screen depth */
    cw->tetris.big_picture = XCreatePixmap(XtDisplay(cw),
            RootWindow(XtDisplay(cw), DefaultScreen(XtDisplay(cw))),
            cw->tetris.pixmap_width_in_pixels, 
	    cw->tetris.pixmap_height_in_pixels, 1);
}

/* ARGSUSED */
static void
Resize(cw)
TetrisWidget cw;
{
    /* force redraw by clearing window */
    XClearArea(XtDisplay(cw), XtWindow(cw), 0, 0, cw->tetris.pixmap_width_in_pixels, cw->tetris.pixmap_height_in_pixels, True);
}

int
PickType()
{
	int num;

	num = 18;
	while (num == 18) {
	/* pick from 0 to 19, but not 18 */
		num = (int)(rand() % 20);
	}
	return(num);
}

static void
StartBlock(w)
TetrisWidget w;
{
	w->tetris.cur_x = 9;
	w->tetris.cur_y = 0;

	w->tetris.type = PickType();

	DrawBlock(w, DRAW);

	timer = XtAddTimeOut(w->tetris.delay, MoveDown, w);
}

static void
DrawBlock(w, mode)
TetrisWidget w;
int mode;
{
	int i;
	int xoffset, yoffset;
	for (i = 0; i < BLOCKS_PER_CELL; i++) {
		if (blocks[w->tetris.type * BLOCKS_PER_CELL + i]) {
			if (w->tetris.type % 2) { /* odd, horizontal */
				xoffset = i % BLOCK_LENGTH;
				yoffset = i % BLOCK_WIDTH;
			}
			else {	/* even, vertical */
				xoffset = i % BLOCK_WIDTH;
				yoffset = i % BLOCK_LENGTH;
			}
			DrawCell(w, w->tetris.cur_x + xoffset, w->tetris.cur_y + yoffset, mode);
		}
	}
}

static void
DrawCell(w, x, y, mode)
TetrisWidget w;
int x, y;
int mode;
{
    GC gc;
	/* if cell is already done, return */
    /*  taken out, to make bottom checking easier...
	if (w->tetris.cell[x + y * w->tetris.pixmap_width_in_cells] == mode)
        return; */

    if (mode == DRAW)
	gc = w->tetris.draw_gc;
    else
	gc = w->tetris.undraw_gc;

	/* otherwise, draw or undraw */
    XFillRectangle(XtDisplay(w), w->tetris.big_picture, gc,
    		(w->tetris.cell_size_in_pixels * x + 2), 
		(w->tetris.cell_size_in_pixels * y + 2), 
		(unsigned int)w->tetris.cell_size_in_pixels - 3, 
		(unsigned int)w->tetris.cell_size_in_pixels - 3);

	/* update cell array to reflect change */
	/* TAKEN OUT
    w->tetris.cell[x + y * w->tetris.pixmap_width_in_cells] = mode;
	*/
}

static void
Drop(w, event)
TetrisWidget w;
XButtonEvent *event;
{
	XtRemoveTimeOut(timer);

	while (CanMoveDown(w)) {
		DrawBlock(w, UNDRAW);
		w->tetris.cur_y++;
		DrawBlock(w, DRAW);
		CopyBlock(w);
	}

	UpdateCellArray(w);
	KillRows(w);
	w->tetris.score++;
	Score(w);
	w->tetris.delay -= w->tetris.delay_decrement;
	if (w->tetris.running)
	     StartBlock(w);
}

static Boolean
CanMoveSideways(w, direction)
TetrisWidget w;
String *direction;
{
	int i;
	int x, y, xoff;

	if (*direction[0] == 'l') {
		xoff = -1;
		/* hit left side */
		if ((w->tetris.type == 16) && (w->tetris.cur_x <= -1))
			return(False);
		else if ((w->tetris.type != 16) && (w->tetris.cur_x <= 0))
			return(False);
	}
	else  {
		xoff = 1;
/* most */	if ((w->tetris.type < 17) && (w->tetris.cur_x >= 
				w->tetris.pixmap_width_in_cells 
				- ((w->tetris.type % 2) ? 
				BLOCK_LENGTH - 2: BLOCK_WIDTH))) 
			return(False); /* block has hit right side */
/* horiz bar */	else if ((w->tetris.type == 17) && (w->tetris.cur_x >=
				w->tetris.pixmap_width_in_cells - 4))
			return(False); /* block has hit right side */
/* box */	else if ((w->tetris.type == 19) && (w->tetris.cur_x >=
				w->tetris.pixmap_width_in_cells - 2))
			return(False); /* block has hit right side */
        }

   for (i = 0;i < BLOCKS_PER_CELL; i++) {
       if (blocks[(w->tetris.type * BLOCKS_PER_CELL) + i]) {
            if (w->tetris.type % 2) { /* odd, horiz */
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_LENGTH) + xoff][w->tetris.cur_y + (i % BLOCK_WIDTH)]) {
                    return(False);
                }
            }
            else
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_WIDTH) + xoff][w->tetris.cur_y + (i % BLOCK_LENGTH)]) {
                    return(False);
            }
        }
   }
    return(True);
}


static void
MoveSideways(w, event, direction)
TetrisWidget w;
XButtonEvent *event;
String *direction;
{
	if (CanMoveSideways(w, direction)) {
		DrawBlock(w, UNDRAW);

		if (*direction[0] == 'l') 
			w->tetris.cur_x--;
		else 
			w->tetris.cur_x++;

		DrawBlock(w, DRAW);

		/* recopy to window */
		CopyBlock(w);
	}
	else
		XBell(XtDisplay(w), 100);
}

static Boolean
CanMoveDown(w)
TetrisWidget w;
{
	int i;
	int x, y, xmax, ymax;

	if ((w->tetris.type != 16) && (w->tetris.cur_y >= 
			w->tetris.pixmap_height_in_cells 
			- ((w->tetris.type % 2) ? 
			BLOCK_WIDTH : BLOCK_LENGTH - 2))) 
		return(False); /* block has hit bottom */
	else if ((w->tetris.type == 16) && (w->tetris.cur_y >=
			w->tetris.pixmap_height_in_cells - 4))
		return(False); /* block has hit bottom */

   for (i = 0;i < BLOCKS_PER_CELL; i++) {
       if (blocks[(w->tetris.type * BLOCKS_PER_CELL) + i]) {
            if (w->tetris.type % 2) { /* odd, horiz */
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_LENGTH)][w->tetris.cur_y + (i % BLOCK_WIDTH) + 1]) {
                    return(False);
                }
            }
            else
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_WIDTH)][w->tetris.cur_y + (i % BLOCK_LENGTH) + 1]) {
                    return(False);
            }
        }
   }
}

static void
Crunch(w, row)
TetrisWidget w;
int row;
{
	XExposeEvent fake_event;

	/* copy everything down in big pixmap, then redisplay */
	XCopyArea(XtDisplay(w), w->tetris.big_picture,
			w->tetris.big_picture,
                        w->tetris.draw_gc, 
			0, 0, /* source origin */
			w->tetris.pixmap_width_in_pixels, 
			w->tetris.cell_size_in_pixels * row,
			0, w->tetris.cell_size_in_pixels);

	fake_event.x = 0;
	fake_event.y = 0;
	fake_event.width = 10000; /* entire window */
	fake_event.height = 10000;

        Redisplay(w, &fake_event);
}

static void
KillRows(w)
TetrisWidget w;
{
   int x, y;
   int flag;
   int crunched_row[4];
   int i, j;

   i = 0;

   for (y = 0;y < w->tetris.pixmap_height_in_cells; y++) {
       flag = 0;
       for (x = 0;x < w->tetris.pixmap_width_in_cells; x++) {
	
	   if (w->tetris.cell[x][y])
               flag++; 
       }
       if (flag == w->tetris.pixmap_width_in_cells) {
           Crunch(w, y);
           crunched_row[i++] = y;
	   /* must move all the rows in the array too,
            * (could be as many as 4 rows crunched)
	    * but after these loops are done :-( */
       }
   }
   if (crunched_row[0]) { /* some crunching to do */
       /* now move down the array to reflect crunched rows */
       for (j = 0; j < i; j++) {  /* do up to 4 crunched rows */
           for (y = crunched_row[j]; y >= 0; y--) {
               for (x = 0;x < w->tetris.pixmap_width_in_cells; x++) {
                    w->tetris.cell[x][y] = w->tetris.cell[x][y - 1];
                }
           }
       }
       for (x = 0;x < w->tetris.pixmap_width_in_cells; x++) {
           w->tetris.cell[x][0] = 0;
           w->tetris.cell[x][1] = 0;
       }
   }
   /* clear first two rows.  For some reason they're full of garbarge */
}

Score(w)
TetrisWidget w;
{
	w->tetris.score++;

	XtCallCallbacks(w, XtNcallback, w->tetris.score);
}

static void
UpdateCellArray(w)
TetrisWidget w;	/* client_data */
{
	int i;
	int xoffset, yoffset;
	for (i = 0; i < BLOCKS_PER_CELL; i++) {
		if (blocks[w->tetris.type * BLOCKS_PER_CELL + i]) {
			if (w->tetris.type % 2) { /* odd, horizontal */
				xoffset = i % BLOCK_LENGTH;
				yoffset = i % BLOCK_WIDTH;
			}
			else {	/* even, vertical */
				xoffset = i % BLOCK_WIDTH;
				yoffset = i % BLOCK_LENGTH;
			}
    			w->tetris.cell[w->tetris.cur_x + xoffset][w->tetris.cur_y + yoffset] = True;
		}
	}
	if (w->tetris.cur_y < 3) { /* block ends up near top */
		XtCallCallbacks(w, XtNgameover, w->tetris.score);
		XtRemoveTimeOut(timer);
		w->tetris.running = False; /* stop play */
	}
}

static void
MoveDown(w)
TetrisWidget w;	/* client_data */
{
	if (CanMoveDown(w)) {
		DrawBlock(w, UNDRAW);
		w->tetris.cur_y++;
		DrawBlock(w, DRAW);
		CopyBlock(w);
		if (w->core.visible == FALSE)
			timer = XtAddTimeOut(250, CheckVisibility, w);
		else
			timer = XtAddTimeOut(w->tetris.delay, MoveDown, w);
	} else {
		UpdateCellArray(w);
		KillRows(w);
		Score(w);
		w->tetris.delay -= w->tetris.delay_decrement;
		if (w->tetris.running)
			StartBlock(w);
	}
}

static Boolean
CanTurn(w)
TetrisWidget w;
{
	int i;
	int newtype;
	static int map[] = {
		1, 2, 3, 0,  /* left L */
		5, 6, 7, 4,  /* right L */
		9, 10, 11, 8,  /* T */
		13, 12,  /* left dogleg */
		15, 14,  /* right dogleg */
		17, 16   /* long bar */
	};

	if (w->tetris.type == 19) /* box */
		return(False);

	newtype = map[w->tetris.type];

   for (i = 0;i < BLOCKS_PER_CELL; i++) {
       if (blocks[(newtype * BLOCKS_PER_CELL) + i]) {
            if (newtype % 2) { /* odd, horiz */
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_LENGTH)][w->tetris.cur_y + (i % BLOCK_WIDTH)]) {
                    return(False);
                }
            }
            else
                if (w->tetris.cell[w->tetris.cur_x + (i % BLOCK_WIDTH)][w->tetris.cur_y + (i % BLOCK_LENGTH)]) {
                    return(False);
            }
        }
   }
   return(True);
}

static void
Turn(w, event)
TetrisWidget w;
XButtonEvent *event;
{
	static int map[] = {
		1, 2, 3, 0,  /* left L */
		5, 6, 7, 4,  /* right L */
		9, 10, 11, 8,  /* T */
		13, 12,  /* left dogleg */
		15, 14,  /* right dogleg */
		17, 16   /* long bar */
	};

	/* should make sure we can turn (no settled blocks in the way) */
	if (CanTurn(w)) {
		DrawBlock(w, UNDRAW);

		/* map type to turned type */
		w->tetris.type = map[w->tetris.type];

		/* if cell at right edge and now horiz, move left one. */
		if ((w->tetris.cur_x >= w->tetris.pixmap_width_in_cells - 2) &&  (w->tetris.type % 2))
			w->tetris.cur_x--;

		DrawBlock(w, DRAW);
		CopyBlock(w);
	}
	else
		XBell(XtDisplay(w), 100);
}

static void
CheckVisibility(w)
TetrisWidget w;	/* client_data */
{
	if (w->core.visible == FALSE)
		timer = XtAddTimeOut(250, CheckVisibility, w);
	else
		timer = XtAddTimeOut(w->tetris.delay, MoveDown, w);
}

void
TetrisStartNewGame(w)
TetrisWidget w;
{
    int x, y;
    XExposeEvent fake_event;

    XtRemoveTimeOut(timer);

    w->tetris.delay = w->tetris.initial_delay;
    w->tetris.score = 0;

    for (y = 0;y < w->tetris.pixmap_height_in_cells; y++)
       for (x = 0;x < w->tetris.pixmap_width_in_cells; x++)
           w->tetris.cell[x][y] = 0;

    /* clear pixmap, no matter how big */
    XFillRectangle(XtDisplay(w), w->tetris.big_picture, w->tetris.undraw_gc,
    		0, 0, 10000, 10000);

    fake_event.x = 0;
    fake_event.y = 0;
    fake_event.width = 10000; /* entire window */
    fake_event.height = 10000;

    Redisplay(w, &fake_event);

    w->tetris.running = True; /* start play */
    StartBlock(w);
}

void
TetrisPauseGame(w)
TetrisWidget w;
{
    w->tetris.running = False; /* pause play */
    XtRemoveTimeOut(timer);
}

void
TetrisContinueGame(w)
TetrisWidget w;
{
    w->tetris.running = True; /* continue play */
    timer = XtAddTimeOut(w->tetris.delay, MoveDown, w);
}
