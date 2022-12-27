/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See xtetris.c for complete rights and liability information.
 */

/* 
 * TetrisP.h - Private definitions for Tetris widget
 */

#ifndef _XtTetrisP_h
#define _XtTetrisP_h

/*
 * This include not needed unless the .c file includes IntrinsicP.h
 * after this file.   Anyway, it doesn't hurt.
 */
#include <X11/CoreP.h>

/*
 * This one is always needed!
 */
#include "Tetris.h"

/* New fields for the Tetris widget class record */

typedef struct {
	int dummy_field;	/* keep compiler happy */
} TetrisClassPart;

/* Full class record declaration */
typedef struct _TetrisClassRec {
    CoreClassPart	core_class;
    TetrisClassPart	tetris_class;
} TetrisClassRec;

extern TetrisClassRec tetrisClassRec;

/* New fields for the Tetris widget record */
typedef struct {
    /* resources */
    Pixel	foreground;
    XtCallbackList callback;	/* application installed callback function(s) */
    XtCallbackList gameover;
    Dimension	pixmap_width_in_cells;
    Dimension	pixmap_height_in_cells;
    int cell_size_in_pixels;
    int delay;
    int level;

    /* private state */
    int cur_x, cur_y;  /* position of current cell */
    int type;	/* current type */
    Dimension	pixmap_width_in_pixels;
    Dimension	pixmap_height_in_pixels;
    Pixmap big_picture;
    GC		draw_gc;	/* one plane, for drawing into pixmap */
    GC		undraw_gc;	/* one plane, for drawing into pixmap */
    GC		copy_gc;	/* defaultdepthofscreen, for copying pixmap into window */
    int cell[20][20];	/* array for keeping track of array of bits */
    int delay_decrement; /* amount delay is reduced each time a block falls */
    int initial_delay;
    int score;
    Boolean running;	/* is game being played? */
} TetrisPart;

/*
 * Full instance record declaration
 */
typedef struct _TetrisRec {
    CorePart		core;
    TetrisPart	tetris;
} TetrisRec;

#endif /* _XtTetrisP_h */
