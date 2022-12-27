/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See xtetris.c for complete rights and liability information.
 */
#ifndef _XtTetris_h
#define _XtTetris_h

/*
 * Tetris Widget public include file
 */

/*
 * This include not needed unless the application includes Intrinsic.h
 * after this file.   Anyway, it doesn't hurt.
 */
#include <X11/Core.h>

/* Resources:
 * Name		     Class		RepType		Default Value
 * ----		     -----		-------		-------------
 * (from RectObj)
 * ancestorSensitive
 * x		    Position		int		0
 * y		    Position		int		0
 * width
 * height
 * borderWidth
 * sensitive
 * (from WindowObj)
 * screen		    Screen		Pointer		XtCopyScreen
 * depth		    Depth		Int		XtCopyFromParent
 * colormap	    Colormap	        Pointer	        XtCopyFromParent
 * background	    Background		pixel		White
 * backgroundPixmap    Pixmap		Pixmap		XtUnspecifiedPixmap
 * borderColor	    BorderColor		pixel		Black
 * borderPixmap	    BorderPixmap	Pixmap		XtUnspecifiedPixmap
 * mappedWhenManaged   MappedWhenManaged	Boolean		True
 * translations
 * accelerators
 * (from Core)
 * none
 * (from Tetris)
 * foregroundPixel    Foreground		pixel		Black
 * backgroundPixel    Background		pixel		White
 * callback	    Callback		Callback	NULL
 * cellSize	    CellSize		int		30
 * pixmapWidth	    PixmapWidth		int		32
 * pixmapHeight	    PixmapHeight	int		32
 * 
 */

#define XtNcellSizeInPixels		"cellSizeInPixels"
#define XtNpixmapWidthInCells		"pixmapWidthInCells"
#define XtNpixmapHeightInCells		"pixmapHeightInCells"
#define XtNcurX			"curX"
#define XtNcurY			"curY"
#define XtNdelay			"delay"
#define XtNgameover			"gameover"
#define XtNlevel			"level"
 
#define XtCCellSizeInPixels		"CellSizeInPixels"
#define XtCPixmapWidthInCells		"PixmapWidthInCells"
#define XtCPixmapHeightInCells		"PixmapHeightInCells"
#define XtCCurX			"CurX"
#define XtCCurY			"CurY"
#define XtCDelay			"Delay"
#define XtCGameover			"Gameover"
#define XtCLevel			"Level"

extern char *TetrisGetArrayString(); /* w */
    /* Widget w; */

/* Class record constants */

extern WidgetClass tetrisWidgetClass;

typedef struct _TetrisClassRec *TetrisWidgetClass;
typedef struct _TetrisRec      *TetrisWidget;

#endif /* _XtTetris_h */
/* DON'T ADD STUFF AFTER THIS #endif */
