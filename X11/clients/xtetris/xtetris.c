/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * online documentation for any purpose is hereby granted without 
 * fee, provided that the above copyright notice appear in all copies 
 * and that both that copyright notice and this permission notice 
 * appear in supporting documentation, and that the name of O'Reilly 
 * and Associates, Inc. not be used in advertising or publicity 
 * pertaining to distribution of the software without specific, 
 * written prior permission.  O'Reilly and Associates, Inc. makes no 
 * representations about the suitability of this software for any 
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * O'REILLY AND ASSOCIATES, INC. DISCLAIMS ALL WARRANTIES WITH REGARD 
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL O'REILLY AND ASSOCIATES, INC.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Adrian Nye of O'Reilly and Associates, Inc.
 */

/*
 * xbitmap.c -- an example application
 */
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#ifdef X11R3
#include <X11/Form.h>
#include <X11/Box.h>
#include <X11/Command.h>
#include <X11/Label.h>
#include <X11/Dialog.h>
#else
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Dialog.h>
#endif /* X11R3 */

#include "Tetris.h"

Dimension pixmap_width_in_cells, pixmap_height_in_cells;

Widget topLevel, tetris, form, score_label, pause, pshell, highscoretop;
Widget initialShell, initialDialog, gameoverShell, gameoverDialog;
int our_score;
int highscores[10];
Widget highscorewidget[10];
static Boolean paused = False;
static Boolean gamenotover = True;
static String initials[4];

void RemoveInitialPopup();

void
create_initial_popup(parent)
Widget parent;
{
	Widget initialCommand;

        initialShell = XtCreatePopupShell("initialShell", transientShellWidgetClass,
                parent, NULL, 0);

        initialDialog = XtCreateManagedWidget("initialDialog", dialogWidgetClass, 
		initialShell, NULL, 0);

        initialCommand = XtCreateManagedWidget("initialCommand", commandWidgetClass, 
		initialDialog, NULL, 0);

	XtAddCallback(initialCommand, XtNcallback, RemoveInitialPopup, NULL);
}

void
create_gameover_popup(parent)
Widget parent;
{
        gameoverShell = XtCreatePopupShell("gameoverShell", transientShellWidgetClass,
                parent, NULL, 0);

        gameoverDialog = XtCreateManagedWidget("gameoverDialog", dialogWidgetClass, 
		gameoverShell, NULL, 0);
}

/*
 * dialog button
 */
/* ARGSUSED */
void PopupDialog(w, shell, call_data)
Widget w;
Widget shell;        /* client_data */
caddr_t call_data;
{
        Position x, y;
        Dimension width, height;
        Arg arg[2];
        int i;

        /*
         * get the coordinates of the middle of topLevel widget.
         */
        i = 0;
        XtSetArg(arg[i], XtNwidth, &width);    i++;
        XtSetArg(arg[i], XtNheight, &height);  i++;
        XtGetValues(topLevel, arg, i);

        /*
         * translate coordinates in application top-level window
         * into coordinates from root window origin.
         */
        XtTranslateCoords(topLevel,                /* Widget */
                (Position) width/2,        /* x */
                (Position) height/2,       /* y */
                &x, &y);          /* coords on root window */

        /* move popup shell to this position (it's not visible yet) */
        i = 0;
        XtSetArg(arg[i], XtNx, x);  i++;
        XtSetArg(arg[i], XtNy, y);  i++;
        XtSetValues(shell, arg, i);
        /*
         * Indicate to user that no other application functions are
         * valid while dialog is popped up...
         */

	TetrisPauseGame(tetris);
        XtPopup(shell, XtGrabExclusive);
}

/*
 * dialog done button
 */
/* ARGSUSED */
void DialogDone(w, dialog, call_data)
Widget w;
Widget dialog;  /* client_data */
caddr_t call_data;
{
        XtPopdown(pshell);
	if (gamenotover)
		TetrisContinueGame(tetris);
}

/* ARGSUSED */
static void 
score(widget, score_label, current_score)
Widget widget;
Widget score_label;	/* client_data */
int current_score;	/* call_data */
{
	Arg arg;
	String buf[50];

	our_score = current_score;

	sprintf(buf, "Score: %d", current_score);

 	XtSetArg(arg, XtNlabel, buf);
	XtSetValues(score_label, &arg, 1);
}

/* ARGSUSED */
static void 
pausegame(widget, client_data, call_data)
Widget widget;
caddr_t client_data;
int call_data;
{
	Arg arg;

	if (paused) {
		TetrisContinueGame(tetris);
		paused = False;
		XtSetArg(arg, XtNlabel, "Pause");
		XtSetValues(pause, &arg, 1);
	}
	else {
		TetrisPauseGame(tetris);
		paused = True;
		XtSetArg(arg, XtNlabel, "Continue");
		XtSetValues(pause, &arg, 1);
	}
}

/* ARGSUSED */
void
RemoveInitialPopup(widget, client_data, call_data)
Widget widget;
caddr_t client_data;
int call_data;
{
	String string;

	XtPopdown(initialShell);

        /*
         * Changes to XawDialogGetValueString in R4
         */
#ifdef X11R3
        string = XtDialogGetValueString(initialDialog);
#else
        string = XawDialogGetValueString(initialDialog);
#endif /* X11R3 */

	strncpy(initials, string, 3);
}

GetInitials(widget, msg)
Widget widget;
String msg;
{
	Arg arg;

	XtSetArg(arg, XtNlabel, msg);
	XtSetValues(initialDialog, &arg, 1);
	PopupDialog(widget, initialShell, 0);
}

RemoveGameoverPopup(w)
Widget w; /* client_data */
{
	XtPopdown(w);
}

TellGameOver(widget, msg)
Widget widget;
String msg;
{
	Arg arg;

	(void) XtAddTimeOut(1000, RemoveGameoverPopup, gameoverShell);

	XtSetArg(arg, XtNlabel, msg);
	XtSetValues(initialDialog, &arg, 1);
	PopupDialog(widget, gameoverShell, 0);
}

/* ARGSUSED */
static void 
gameover(widget, client_data, current_score)
Widget widget;
caddr_t client_data;	/* client_data */
int current_score;	/* call_data */
{
	int i, j;
	Arg arg;
	int newhighscore;
	String buf[50];
	String msg[50];

	gamenotover = False;
	newhighscore = False;

	for (i = 9; i >= 0; i--) {
		if (current_score > highscores[i])
			newhighscore = True;
		else
			break;
	}
	i++;

	if (newhighscore && i == 0) {
		sprintf(msg, "Awesome score! Enter Initials:");
		GetInitials(widget, msg);
	}
	else if (newhighscore) {
		sprintf(msg, "On high score list: Enter Initials:");
		GetInitials(widget, msg);
	}
	else {
		sprintf(msg, "Game over; sorry, not a high score.");
		TellGameOver(widget, msg);
	}

	if (newhighscore) {
		for (j = 9; j > i; j--) {
			String p;
			highscores[j] = highscores[j-1];
			XtSetArg(arg, XtNlabel, &p);
			XtGetValues(highscorewidget[j - 1], &arg, 1);
			strcpy(buf, p);
			XtSetArg(arg, XtNlabel, buf);
			XtSetValues(highscorewidget[j], &arg, 1);
		}

		highscores[i] = current_score;
		sprintf(buf, "%d             %s", current_score, initials);
		XtSetArg(arg, XtNlabel, buf);
		XtSetValues(highscorewidget[i], &arg, 1);
	}
}

/* ARGSUSED */
static void 
newgame(widget, client_data, current_score)
Widget widget;
caddr_t client_data;	/* client_data */
int current_score;	/* call_data */
{
	Arg arg;

	gamenotover = True;

	/* This is a public function defined by Tetris widget */
	TetrisStartNewGame(tetris);

 	XtSetArg(arg, XtNlabel, "Score: 0   ");
	XtSetValues(score_label, &arg, 1);
}

main(argc, argv)
int argc;
char *argv[];
{
	Widget box, buttonbox, gethighscore, highscorelabel,
			 highscoredone, startnewgame, quit, level_label;
	int i;
	int level;
	String buf[50];

	Arg args[5];
	extern exit();

	static XrmOptionDescRec table[] = {
		{"-pw",            "*pixmapWidthInCells",        XrmoptionSepArg, NULL},
		{"-pixmapwidth",   "*pixmapWidthInCells",        XrmoptionSepArg, NULL},
		{"-ph",            "*pixmapHeightInCells",       XrmoptionSepArg, NULL},
		{"-pixmapheight",  "*pixmapHeightInCells",       XrmoptionSepArg, NULL},
		{"-c",             "*cellSizeInPixels",          XrmoptionSepArg, NULL},
		{"-cellsize",      "*cellSizeInPixels",          XrmoptionSepArg, NULL},
		{"-level",         "*level",                     XrmoptionSepArg, NULL},
	};
    
	topLevel = XtInitialize(argv[0], "XTetris", table, XtNumber(table), &argc, argv);

        pshell = XtCreatePopupShell("pshell", transientShellWidgetClass,
                topLevel, NULL, 0);

        highscoretop = XtCreateManagedWidget("highscoretop", boxWidgetClass, 
		pshell, NULL, 0);

        highscorelabel = XtCreateManagedWidget("highscorelabel",
                labelWidgetClass, highscoretop, NULL, 0);

	for (i = 0; i < 10; i++) {

		sprintf(buf, "highscorewidget%d", i);
        	highscorewidget[i] = XtCreateManagedWidget(buf,
                	labelWidgetClass, highscoretop, NULL, 0);
	}

        highscoredone = XtCreateManagedWidget("highscoredone",
                commandWidgetClass, highscoretop, NULL, 0);

	form = XtCreateManagedWidget("form", formWidgetClass, topLevel, NULL, 0);

	buttonbox = XtCreateManagedWidget("buttonbox", boxWidgetClass, form, NULL, 0);


 	XtSetArg(args[0], XtNlabel, "Score: 0   ");
	score_label = XtCreateManagedWidget("score_label", labelWidgetClass, buttonbox, args, 1);


	quit = XtCreateManagedWidget("quit", commandWidgetClass, buttonbox, NULL, 0);


	startnewgame = XtCreateManagedWidget("startnewgame", commandWidgetClass, buttonbox, NULL, 0);

	XtAddCallback(quit, XtNcallback, exit, NULL);
	XtAddCallback(startnewgame, XtNcallback, newgame, NULL);

	tetris = XtCreateManagedWidget("tetris", tetrisWidgetClass, form, NULL, 0);

	XtAddCallback(tetris, XtNcallback, score, score_label);


	XtAddCallback(tetris, XtNgameover, gameover, NULL);

        XtAddCallback(highscoredone, XtNcallback, DialogDone, highscoretop);

	/* need the following values for the printout routine. */
	i = 0;
	XtSetArg(args[i], XtNpixmapWidthInCells, &pixmap_width_in_cells);  i++;
	XtSetArg(args[i], XtNpixmapHeightInCells, &pixmap_height_in_cells);  i++;
	XtSetArg(args[i], XtNlevel, &level);  i++;
	XtGetValues(tetris, args, i);

	sprintf(buf, "Level: %d", level);
 	XtSetArg(args[0], XtNlabel, buf);
	level_label = XtCreateManagedWidget("level_label", labelWidgetClass, buttonbox, args, 1);

	gethighscore = XtCreateManagedWidget("gethighscore", commandWidgetClass, buttonbox, NULL, 0);

        XtAddCallback(gethighscore, XtNcallback, PopupDialog, pshell);

	pause = XtCreateManagedWidget("pause", commandWidgetClass, buttonbox, NULL, 0);
	XtAddCallback(pause, XtNcallback, pausegame, NULL);

	XtInstallAllAccelerators(tetris, topLevel);

	create_initial_popup(topLevel);
	create_gameover_popup(topLevel);

	XtRealizeWidget(topLevel);
	XtMainLoop();
}
