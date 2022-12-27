/*
 * $XConsortium: init.c,v 2.44 89/12/16 03:33:44 converse Exp $
 *
 *
 *		        COPYRIGHT 1987, 1989
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
 * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
 * ADDITION TO THAT SET FORTH ABOVE.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Digital Equipment Corporation not be
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 *
 */

/* Init.c - Handle start-up initialization. */

#include "xmh.h"
#include "actions.h"
#include <sys/errno.h>

#define MIN_APP_DEFAULTS_VERSION 1

static Boolean static_variable;	 /* whose address is not a widget ID */

/* This is for the check mark in the Options menu */
#define check_width 9
#define check_height 8
static char check_bits[] = {
   0x00, 0x01, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00,
   0x31, 0x00, 0x1b, 0x00, 0x0e, 0x00, 0x04, 0x00
};

#define offset(field) XtOffset(struct _resources *, field)

/* Xmh application resources. */

static XtResource resources[] = {
    {"debug", "Debug", XtRBoolean, sizeof(Boolean),
	 offset(debug), XtRImmediate, (XtPointer)False},
    {"tempDir", "TempDir", XtRString, sizeof(char *),
	 offset(temp_dir), XtRString, "/tmp"},
    {"mhPath", "MhPath", XtRString, sizeof(char *),
	 offset(mh_path), XtRString, "/usr/local/mh6"},
    {"initialFolder", "InitialFolder", XtRString, sizeof(char *),
	 offset(initial_folder_name), XtRString, "inbox"},
    {"initialIncFile", "InitialIncFile", XtRString, sizeof(char *),
         offset(initial_inc_file), XtRString, NULL},
    {"replyInsertFilter", "ReplyInsertFilter", XtRString, sizeof(char *),
	 offset(insert_filter), XtRString, NULL},
    {"draftsFolder", "DraftsFolder", XtRString, sizeof(char *),
	 offset(drafts_folder_name), XtRString, "drafts"},
    {"sendWidth", "SendWidth", XtRInt, sizeof(int),
	 offset(send_line_width), XtRImmediate, (XtPointer)72},
    {"sendBreakWidth", "SendBreakWidth", XtRInt, sizeof(int),
	 offset(break_send_line_width), XtRImmediate, (XtPointer)85},
    {"printCommand", "PrintCommand", XtRString, sizeof(char *),
	 offset(print_command), XtRString,
	 "enscript > /dev/null 2>/dev/null"},
    {"tocWidth", "TocWidth", XtRInt, sizeof(int),
	 offset(toc_width), XtRImmediate, (XtPointer)100},
    {"skipDeleted", "SkipDeleted", XtRBoolean, sizeof(Boolean),
	 offset(skip_deleted), XtRImmediate, (XtPointer)True},
    {"skipMoved", "SkipMoved", XtRBoolean, sizeof(Boolean),
	 offset(skip_moved), XtRImmediate, (XtPointer)True},
    {"skipCopied", "SkipCopied", XtRBoolean, sizeof(Boolean),
	 offset(skip_copied), XtRImmediate, (XtPointer)False},
    {"hideBoringHeaders", "HideBoringHeaders", XtRBoolean, sizeof(Boolean),
	 offset(hide_boring_headers), XtRImmediate, (XtPointer)True},
    {"geometry", "Geometry", XtRString, sizeof(char *),
	 offset(geometry), XtRString, NULL},
    {"tocGeometry", "TocGeometry", XtRString, sizeof(char *),
	 offset(toc_geometry), XtRString, NULL},
    {"viewGeometry", "ViewGeometry", XtRString, sizeof(char *),
	 offset(view_geometry), XtRString, NULL},
    {"compGeometry", "CompGeometry", XtRString, sizeof(char *),
	 offset(comp_geometry), XtRString, NULL},
    {"pickGeometry", "PickGeometry", XtRString, sizeof(char *),
	 offset(pick_geometry), XtRString, NULL},
    {"tocPercentage", "TocPercentage", XtRInt, sizeof(int),
	 offset(toc_percentage), XtRImmediate, (XtPointer)33},
    {"checkNewMail", "CheckNewMail", XtRBoolean, sizeof(Boolean),
	 offset(new_mail_check), XtRImmediate, (XtPointer)True},
    {"makeCheckpoints", "MakeCheckpoints", XtRBoolean, sizeof(Boolean),
	 offset(make_checkpoints), XtRImmediate, (XtPointer)False},
    {"checkFrequency", "CheckFrequency", XtRInt, sizeof(int),
	 offset(check_frequency), XtRImmediate, (XtPointer)1},
    {"mailPath", "MailPath", XtRString, sizeof(char *),
	 offset(mail_path), XtRString, NULL},
    {"mailWaitingFlag", "MailWaitingFlag", XtRBoolean, sizeof(Boolean),
	 offset(mail_waiting_flag), XtRImmediate, (XtPointer)False},
    {"cursor", "Cursor", XtRCursor, sizeof(Cursor),
	 offset(cursor), XtRString, "left_ptr"},
    {"pointerColor", "PointerColor", XtRPixel, sizeof(Pixel),
	 offset(pointer_color), XtRString, XtDefaultForeground},
    {"stickyMenu", "StickyMenu", XtRBoolean, sizeof(Boolean), 	
	 offset(sticky_menu), XtRImmediate, (XtPointer)False},
    {"prefixWmAndIconName", "PrefixWmAndIconName", XtRBoolean, sizeof(Boolean),
	 offset(prefix_wm_and_icon_name), XtRImmediate, (XtPointer)True},
    {"reverseReadOrder", "ReverseReadOrder", XtRBoolean, sizeof(Boolean),
	 offset(reverse_read_order), XtRImmediate, (XtPointer)False},
    {"blockEventsOnBusy", "BlockEventsOnBusy", XtRBoolean, sizeof(Boolean),
	 offset(block_events_on_busy), XtRImmediate, (XtPointer)True},
    {"busyCursor", "BusyCursor", XtRCursor, sizeof(Cursor),
	 offset(busy_cursor), XtRString, "watch"},
    {"busyPointerColor", "BusyPointerColor", XtRPixel, sizeof(Pixel),
	 offset(busy_pointer_color), XtRString, XtDefaultForeground},
    {"commandButtonCount", "CommandButtonCount", XtRInt, sizeof(int),
	 offset(command_button_count), XtRImmediate, (XtPointer)0},
    {"appDefaultsVersion", "AppDefaultsVersion", XtRInt, sizeof(int),
	 offset(app_defaults_version), XtRImmediate, (XtPointer)0},
    {"banner", "Banner", XtRString, sizeof(char *),
	 offset(banner), XtRString, "xmh    MIT X Consortium    R4"},
};

#undef offset

static XrmOptionDescRec table[] = {
    {"-debug",	"debug",		XrmoptionNoArg,	"on"},
    {"-flag",	"mailWaitingFlag",	XrmoptionNoArg, "on"},
    {"-initial","initialFolder",	XrmoptionSepArg, NULL},
    {"-path",	"mailPath",		XrmoptionSepArg, NULL},
};

/* Tell the user how to use this program. */
Syntax(call)
    char *call;
{
    extern void exit();
    (void) fprintf(stderr, "usage: %s [-path <path>] [-initial <folder>]\n",
		   call);
    exit(2);
}


static char *FixUpGeometry(geo, defwidth, defheight)
char *geo;
Dimension defwidth, defheight;
{
    int gbits;
    int x, y;
    unsigned int width, height;
    if (geo == NULL) geo = app_resources.geometry;
    x = y = 0;
    gbits = XParseGeometry(geo, &x, &y, &width, &height);
    if (!(gbits & WidthValue)) {
	width = defwidth;
	gbits |= WidthValue;
    }
    if (!(gbits & HeightValue)) {
	height = defheight;
	gbits |= HeightValue;
    }
    return CreateGeometry(gbits, x, y, (int) width, (int) height);
}


static _IOErrorHandler(dpy)
    Display *dpy;
{
    extern char* SysErrMsg();
    (void) fprintf (stderr,
	     "%s:\tfatal IO error after %lu requests (%lu known processed)\n",
		    progName,
		    NextRequest(dpy) - 1, LastKnownRequestProcessed(dpy));
    (void) fprintf (stderr, "\t%d unprocessed events remaining.\r\n",
		    QLength(dpy));

    if (errno == EPIPE) {
	(void) fprintf (stderr,
     "\tThe connection was probably broken by a server shutdown or KillClient.\r\n");
    }

    Punt("Cannot continue from server error.");
}

/*ARGSUSED*/
static void PopupAppDefaultsWarning(w, closure, event, cont)
    Widget w;
    XtPointer closure;
    XEvent *event;
    Boolean *cont;
{
    if (event->type == MapNotify) {
	PopupError(
"The minimum application default resources\n\
were not properly installed; many features\n\
will not work properly, if at all.  See the\n\
xmh man page for further information."
		   );
	XtRemoveEventHandler(w, XtAllEvents, True,
			     PopupAppDefaultsWarning, closure);
    }
}


/* All the start-up initialization goes here. */

InitializeWorld(argc, argv)
unsigned int argc;
char **argv;
{
    int l;
    FILEPTR fid;
    char str[500], str2[500], *ptr;
    Scrn scrn;
    static XtActionsRec actions[] = {

	/* general Xmh action procedures */

	{"XmhClose",			XmhClose},
	{"XmhComposeMessage",		XmhComposeMessage},

	/* actions upon folders */

	{"XmhOpenFolder",		XmhOpenFolder},
	{"XmhOpenFolderInNewWindow",	XmhOpenFolderInNewWindow},
	{"XmhCreateFolder",		XmhCreateFolder},
	{"XmhDeleteFolder",		XmhDeleteFolder},

	/* actions to support easier folder manipulation */

	{"XmhPushFolder",		XmhPushFolder},
	{"XmhPopFolder",		XmhPopFolder},
        {"XmhPopupFolderMenu",		XmhPopupFolderMenu},
        {"XmhSetCurrentFolder",		XmhSetCurrentFolder},
        {"XmhLeaveFolderButton",	XmhLeaveFolderButton},

	/* actions upon the Table of Contents */

	{"XmhIncorporateNewMail",	XmhIncorporateNewMail},
	{"XmhCommitChanges",		XmhCommitChanges},
	{"XmhPackFolder",		XmhPackFolder},
	{"XmhSortFolder",		XmhSortFolder},
	{"XmhForceRescan",		XmhForceRescan},

	/* actions upon the currently selected message(s) */

	{"XmhViewNextMessage",		XmhViewNextMessage},
	{"XmhViewPreviousMessage",	XmhViewPreviousMessage},
	{"XmhMarkDelete",		XmhMarkDelete},
	{"XmhMarkMove",			XmhMarkMove},
	{"XmhMarkCopy",			XmhMarkCopy},
	{"XmhUnmark",			XmhUnmark},
	{"XmhViewInNewWindow",		XmhViewInNewWindow},
	{"XmhReply",			XmhReply},
	{"XmhForward",			XmhForward},
	{"XmhUseAsComposition",		XmhUseAsComposition},
	{"XmhPrint",			XmhPrint},

	/* actions upon sequences */

	{"XmhPickMessages",		XmhPickMessages},
	{"XmhOpenSequence",		XmhOpenSequence},
	{"XmhAddToSequence",		XmhAddToSequence},
	{"XmhRemoveFromSequence",	XmhRemoveFromSequence},
	{"XmhDeleteSequence",		XmhDeleteSequence},

	/* actions to support easier sequence manipulation */

	{"XmhPushSequence",		XmhPushSequence},
	{"XmhPopSequence",		XmhPopSequence},

	/* actions upon the currently viewed message */

	{"XmhCloseView",		XmhCloseView},
	{"XmhViewReply",		XmhViewReply},
	{"XmhViewForward",		XmhViewForward},
	{"XmhViewUseAsComposition",	XmhViewUseAsComposition},
	{"XmhEditView",			XmhEditView},
	{"XmhSaveView",			XmhSaveView},
	{"XmhPrintView",		XmhPrintView},

       	/* actions upon a composition, reply, or forward */

	/* Close button			XmhCloseView	  (see above) */
	{"XmhResetCompose",		XmhResetCompose},
	/* Compose button 		XmhComposeMessage (see above) */
	{"XmhSave",			XmhSave},
	{"XmhSend",			XmhSend},
	{"XmhInsert",			XmhInsert},

	/* popup dialog box button action procedures */

	{"XmhPromptOkayAction",		XmhPromptOkayAction},

	/* additional actions to implement support for WM_PROTOCOLS */

	{"XmhCancelPick",		XmhCancelPick}
    };

    static Arg shell_args[] = {
	{XtNinput, (XtArgVal)True},
    };

    ptr = rindex(argv[0], '/');
    if (ptr) progName = ptr + 1;
    else progName = argv[0];

    toplevel = XtInitialize("main", "Xmh", table, XtNumber(table),
			    &argc, argv);
    if (argc > 1) Syntax(progName);

    XSetIOErrorHandler(_IOErrorHandler);

    XtSetValues(toplevel, shell_args, XtNumber(shell_args));

    theDisplay = XtDisplay(toplevel);

    homeDir = XtNewString(getenv("HOME"));

    XtGetApplicationResources( toplevel, (XtPointer)&app_resources,
			       resources, XtNumber(resources),
			       NULL, (Cardinal)0 );

    if (app_resources.app_defaults_version < MIN_APP_DEFAULTS_VERSION)
	XtAddEventHandler(toplevel, StructureNotifyMask, False,
			  PopupAppDefaultsWarning, NULL);

    if (app_resources.mail_waiting_flag) app_resources.new_mail_check = True;

    (void) sprintf(str, "%s/.mh_profile", homeDir);
    fid = myfopen(str, "r");
    if (fid) {
	while (ptr = ReadLine(fid)) {
	    (void) strncpy(str2, ptr, 5);
	    str2[5] = '\0';
	    LowerCase(str2, str2);
	    if (strcmp(str2, "path:") == 0) {
		ptr += 5;
		while (*ptr == ' ' || *ptr == '\t')
		    ptr++;
		(void) strcpy(str, ptr);
	    }
	}
	(void) myfclose(fid);
    } else {
	(void) strcpy(str, "Mail");
    }
    for (l=strlen(str) - 1; l>=0 && (str[l] == ' ' || str[l] == '\t'); l--)
	str[l] = 0;
    if (str[0] == '/')
	(void) strcpy(str2, str);
    else
	(void) sprintf(str2, "%s/%s", homeDir, str);

    (void) sprintf(str, "%s/draft", str2);
    draftFile = XtNewString(str);
    (void) sprintf(str, "%s/xmhdraft", str2);
    xmhDraftFile = XtNewString(str);

    if (app_resources.mail_path == NULL)
	app_resources.mail_path = XtNewString(str2);

    NullSource = (Widget) NULL;

    l = strlen(app_resources.mh_path) - 1;
    if (l > 0 && app_resources.mh_path[l] == '/')
	app_resources.mh_path[l] = 0;

    rootwidth = WidthOfScreen(XtScreen(toplevel));
    rootheight = HeightOfScreen(XtScreen(toplevel));

    app_resources.toc_geometry =
	FixUpGeometry(app_resources.toc_geometry,
		      rootwidth / 2, 3 * rootheight / 4);
    app_resources.view_geometry =
	FixUpGeometry(app_resources.view_geometry,
		      rootwidth / 2, rootheight / 2);
    app_resources.comp_geometry =
	FixUpGeometry(app_resources.comp_geometry,
		      rootwidth / 2, rootheight / 2);
    app_resources.pick_geometry =
	FixUpGeometry(app_resources.pick_geometry,
		      rootwidth / 2, rootheight / 2);

    numScrns = 0;
    scrnList = (Scrn *) NULL;
    NoMenuForButton = (Widget) &static_variable;
    LastMenuButtonPressed = (Widget) NULL;

    TocInit();
    InitPick();
    IconInit();
    BBoxInit();

    XtAppAddActions( XtWidgetToApplicationContext(toplevel),
		    actions, XtNumber(actions));

    wm_delete_window = XInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW",
				   False);

    MenuItemBitmap =
	XCreateBitmapFromData( XtDisplay(toplevel),
			      RootWindowOfScreen( XtScreen(toplevel)),
			      check_bits, check_width, check_height);

    DEBUG("Making screen ... ")

    scrn = CreateNewScrn(STtocAndView);

    SetCursorColor(scrn->parent, app_resources.cursor,
		   app_resources.pointer_color);
    if (app_resources.block_events_on_busy)
	SetCursorColor(scrn->parent, app_resources.busy_cursor, 
		       app_resources.busy_pointer_color);

    DEBUG(" setting toc ... ")

    TocSetScrn(InitialFolder, scrn);

    DEBUG("done.\n");

    MapScrn(scrn);
}
