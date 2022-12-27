/*
 * $XConsortium: folder.c,v 2.31 89/12/16 03:33:16 converse Exp $
 *
 *
 *		       COPYRIGHT 1987, 1989
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
 */

/* folder.c -- implement buttons relating to folders and other globals. */


#include <X11/Xos.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <ctype.h>
#include "xmh.h"
#include "bboxint.h"
#include "tocintrnl.h"
#include <X11/Xaw/Cardinals.h>
extern void exit();
extern void free();

typedef struct {	/* client data structure for callbacks */
    Scrn	scrn;		/* the xmh scrn of action */
    Toc		toc;		/* the toc of the selected folder */
    Toc		original_toc;	/* the toc of the current folder */
} DeleteDataRec, *DeleteData;


static void CreateFolderMenu();
static void AddFolderMenuEntry();
static void DeleteFolderMenuEntry();

/* Close this toc&view scrn.  If this is the last toc&view, quit xmh. */

/*ARGSUSED*/
void DoClose(widget, client_data, call_data)
    Widget	widget;
    XtPointer	client_data;
    XtPointer	call_data;
{
    Scrn	scrn = (Scrn) client_data;
    register int i, count;
    Toc		toc;
    XtCallbackRec	confirm_callbacks[2];

    count = 0;
    for (i=0 ; i<numScrns ; i++)
	if (scrnList[i]->kind == STtocAndView && scrnList[i]->mapped)
	    count++;

    confirm_callbacks[0].callback = (XtCallbackProc) DoClose;
    confirm_callbacks[0].closure = (XtPointer) scrn;
    confirm_callbacks[1].callback = (XtCallbackProc) NULL;
    confirm_callbacks[1].closure = (XtPointer) NULL;

    if (count <= 1) {

	for (i = numScrns - 1; i >= 0; i--)
	    if (scrnList[i] != scrn) {
		if (MsgSetScrn((Msg) NULL, scrnList[i], confirm_callbacks,
			       (XtCallbackList) NULL) == NEEDS_CONFIRMATION)
		    return;
	    }
	for (i = 0; i < numFolders; i++) {
	    toc = folderList[i];

	    if (TocConfirmCataclysm(toc, confirm_callbacks,
				    (XtCallbackList) NULL))
		return;
	}
/* 	if (MsgSetScrn((Msg) NULL, scrn))
 *	    return;
 * %%%
 *	for (i = 0; i < numFolders; i++) {
 *	    toc = folderList[i];
 *	    if (toc->scanfile && toc->curmsg)
 *		CmdSetSequence(toc, "cur", MakeSingleMsgList(toc->curmsg));
 *	}
 */
	XtUnmapWidget(scrn->parent);
	XtDestroyApplicationContext
	    (XtWidgetToApplicationContext(scrn->parent));
	exit(0);
    }
    else {
	if (MsgSetScrn((Msg) NULL, scrn, confirm_callbacks, 
		       (XtCallbackList) NULL) == NEEDS_CONFIRMATION)
	    return;
	DestroyScrn(scrn);	/* doesn't destroy first toc&view scrn */
    }
}

/*ARGSUSED*/
void XmhClose(w, event, params, num_params)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    Scrn	scrn;
    if (event->type == ClientMessage &&
	event->xclient.data.l[0] != wm_delete_window)
	return;
    scrn = ScrnFromWidget(w);
    DoClose(w, (XtPointer) scrn, (XtPointer) NULL);
}

/* Open the selected folder in this screen. */

/* ARGSUSED*/
void DoOpenFolder(widget, client_data, call_data)
    Widget	widget;
    XtPointer	client_data;
    XtPointer	call_data;
{
    /* Invoked by the Folder menu entry "Open Folder"'s notify action. */

    Scrn	scrn = (Scrn) client_data;
    Toc		toc  = SelectedToc(scrn);
    TocSetScrn(toc, scrn);
}


/*ARGSUSED*/
void XmhOpenFolder(w, event, params, num_params)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    Scrn	scrn = ScrnFromWidget(w);

    /* This action may be invoked from folder menu buttons or from folder
     * menus, as an action procedure on an event specified in translations.
     * In this case, the action will open a folder only if that folder
     * was actually selected from a folder button or menu.  If the folder
     * was selected from a folder menu, the menu entry callback procedure,
     * which changes the selected folder, and is invoked by the "notify" 
     * action, must have already executed; and the menu entry "unhightlight"
     * action must execute after this action.
     *
     * This action does not execute if invoked as an accelerator whose
     * source widget is a menu button or a folder menu.  However, it 
     * may be invoked as a keyboard accelerator of any widget other than
     * the folder menu buttons or the folder menus.  In that case, it will
     * open the currently selected folder.
     *
     * If given a parameter, it will take it as the name of a folder to
     * select and open.
     */

    if (! UserWantsAction(w, scrn)) return;
    if (*num_params) SetCurrentFolderName(scrn, params[0]);
    DoOpenFolder(w, (XtPointer) scrn, (XtPointer) NULL);
}


/* Compose a new message. */

/*ARGSUSED*/
void DoComposeMessage(widget, client_data, call_data)
    Widget	widget;
    XtPointer	client_data;
    XtPointer	call_data;
{
    Scrn        scrn = NewCompScrn();
    Msg		msg = TocMakeNewMsg(DraftsFolder);
    MsgLoadComposition(msg);
    MsgSetTemporary(msg);
    MsgSetReapable(msg);
    MsgSetScrnForComp(msg, scrn);
    MapScrn(scrn);
}

   
/*ARGSUSED*/
void XmhComposeMessage(w, event, params, num_params)
    Widget	w;		/* unused */
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    DoComposeMessage(w, (XtPointer) NULL, (XtPointer) NULL);
}


/* Make a new scrn displaying the given folder. */

/*ARGSUSED*/
void DoOpenFolderInNewWindow(widget, client_data, call_data)
    Widget	widget;
    XtPointer	client_data;
    XtPointer	call_data;
{
    Scrn	scrn = (Scrn) client_data;
    Toc 	toc = SelectedToc(scrn);
    scrn = CreateNewScrn(STtocAndView);
    TocSetScrn(toc, scrn);
    MapScrn(scrn);
}


/*ARGSUSED*/
void XmhOpenFolderInNewWindow(w, event, params, num_params)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    Scrn scrn = ScrnFromWidget(w);
    DoOpenFolderInNewWindow(w, (XtPointer) scrn, (XtPointer) NULL);
}


/* Create a new folder with the given name. */

static char *previous_label = NULL;
/*ARGSUSED*/
static void CreateFolder(widget, client_data, call_data)
    Widget	widget;		/* the okay button of the dialog widget */
    XtPointer	client_data;	/* the dialog widget */
    XtPointer	call_data;
{
    Toc		toc;
    register int i;
    char	*name;
    Widget	dialog = (Widget) client_data;
    Arg		args[3];
    char 	str[300], *label;

    name = XawDialogGetValueString(dialog);
    for (i=0 ; name[i] > ' ' ; i++) ;
    name[i] = '\0';
    toc = TocGetNamed(name);
    if ((toc) || (i==0) || (name[0]=='/') || ((toc = TocCreateFolder(name))
					      == NULL)) {
	if (toc) 
	    (void) sprintf(str, "Folder \"%s\" already exists.  Try again.",
			   name);
	else if (name[0]=='/')
	    (void) sprintf(str, "Please specify folders relative to \"%s\".",
			   app_resources.mail_path);
	else 
	    (void) sprintf(str, "Cannot create folder \"%s\".  Try again.",
			   name);
	label = XtNewString(str);
	XtSetArg(args[0], XtNlabel, label);
	XtSetArg(args[1], XtNvalue, "");
	XtSetValues(dialog, args, TWO);
	if (previous_label)
	    XtFree(previous_label);
	previous_label = label;
	return;
    }
    for (i = 0; i < numScrns; i++)
	if (scrnList[i]->folderbuttons) {
	    char	*c;
	    Button	button;
	    if (c = index(name, '/')) { /* if is subfolder */
		c[0] = '\0';
		button = BBoxFindButtonNamed(scrnList[i]->folderbuttons,
					     name);
		c[0] = '/';
		if (button) AddFolderMenuEntry(button, name);
	    }
	    else
		BBoxAddButton(scrnList[i]->folderbuttons, name,
			      menuButtonWidgetClass, True);
	}
    DestroyPopup(widget, (XtPointer) XtParent(dialog), (XtPointer) NULL);
}


/* Create a new folder.  Requires the user to name the new folder. */

/*ARGSUSED*/
void DoCreateFolder(widget, client_data, call_data)
    Widget	widget;		/* unused */
    XtPointer	client_data;	/* unused */
    XtPointer	call_data;	/* unused */
{
    PopupPrompt("Create folder named:", CreateFolder);
}


/*ARGSUSED*/
void XmhCreateFolder(w, event, params, num_params)
    Widget	w;		/* unused */
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    DoCreateFolder(w, (XtPointer) NULL, (XtPointer) NULL);
}


/*ARGSUSED*/
void CancelDeleteFolder(widget, client_data, call_data)
    Widget	widget;		/* unused */
    XtPointer	client_data;
    XtPointer	call_data;	/* unused */
{
    DeleteData	deleteData = (DeleteData) client_data;

    TocClearDeletePending(deleteData->toc);

    /* When the delete request is made, the toc currently being viewed is
     * changed if necessary to be the toc under consideration for deletion.
     * Once deletion has been confirmed or cancelled, we revert to display
     * the toc originally under view, unless the toc originally under
     * view has been deleted.
     */

    if (deleteData->original_toc != NULL)
	TocSetScrn(deleteData->original_toc, deleteData->scrn);
    XtFree((char *) deleteData);
}


/*ARGSUSED*/
void CheckAndConfirmDeleteFolder(widget, client_data, call_data)
    Widget	widget;		/* unreliable; sometimes NULL */
    XtPointer	client_data;	/* data structure */
    XtPointer	call_data;	/* unused */
{
    DeleteData  deleteData = (DeleteData) client_data;
    Scrn	scrn = deleteData->scrn;
    Toc		toc  = deleteData->toc;
    char	str[300];
    XtCallbackRec confirms[2];
    XtCallbackRec cancels[2];
    void CheckAndDeleteFolder();

    static XtCallbackRec yes_callbacks[] = {
	{CheckAndDeleteFolder,	(XtPointer) NULL},
	{(XtCallbackProc) NULL,	(XtPointer) NULL}
    };

    static XtCallbackRec no_callbacks[] = {
	{CancelDeleteFolder,	(XtPointer) NULL},
	{(XtCallbackProc) NULL,	(XtPointer) NULL}
    };

    /* Display the toc of the folder to be deleted. */

    TocSetScrn(toc, scrn);

    /* Check for pending delete, copy, move, or edits on messages in the
     * folder to be deleted, and ask for confirmation if they are found.
     */

    confirms[0].callback = (XtCallbackProc) CheckAndConfirmDeleteFolder;
    confirms[0].closure = client_data;
    confirms[1].callback = (XtCallbackProc) NULL;
    confirms[1].closure = (XtPointer) NULL;
    
    cancels[0].callback = (XtCallbackProc) CancelDeleteFolder;
    cancels[0].closure = client_data;
    cancels[1].callback = (XtCallbackProc) NULL;
    cancels[1].closure = (XtPointer) NULL;

    if (TocConfirmCataclysm(toc, confirms, cancels) ==	NEEDS_CONFIRMATION)
	return;

    /* Ask the user for confirmation on destroying the folder. */

    yes_callbacks[0].closure = client_data;
    no_callbacks[0].closure =  client_data;
    (void) sprintf(str, "Are you sure you want to destroy %s?", TocName(toc));
    PopupConfirm(scrn->tocwidget, str, yes_callbacks, no_callbacks);
}


/*ARGSUSED*/
void CheckAndDeleteFolder(widget, client_data, call_data)
    Widget	widget;		/* unused */
    XtPointer	client_data;	/* data structure */
    XtPointer	call_data;	/* unused */
{
    DeleteData  deleteData = (DeleteData) client_data;
    Scrn	scrn = deleteData->scrn;
    Toc		toc =  deleteData->toc;
    XtCallbackRec confirms[2];
    XtCallbackRec cancels[2];
    int 	i;
    char	*foldername;
    
    /* Check for changes occurring after the popup was first presented. */

    confirms[0].callback = (XtCallbackProc) CheckAndConfirmDeleteFolder;
    confirms[0].closure = client_data;
    confirms[1].callback = (XtCallbackProc) NULL;
    confirms[1].closure = (XtPointer) NULL;
    
    cancels[0].callback = (XtCallbackProc) CancelDeleteFolder;
    cancels[0].closure = client_data;
    cancels[1].callback = (XtCallbackProc) NULL;
    cancels[1].closure = (XtPointer) NULL;
    
    if (TocConfirmCataclysm(toc, confirms, cancels) == NEEDS_CONFIRMATION)
	return;

    /* Delete.  Restore the previously viewed toc, if it wasn't deleted. */

    foldername = TocName(toc);
    TocSetScrn(toc, (Scrn) NULL);
    TocDeleteFolder(toc);
    for (i=0 ; i<numScrns ; i++)
	if (scrnList[i]->folderbuttons) {

	    if (IsSubfolder(foldername)) {
		char parent_folder[300];
		char *c = index( strcpy(parent_folder, foldername), '/');
		*c = '\0';

/* Since menus are built upon demand, and are a per-screen resource, 
 * resources, not all toc & view screens will have the same menus built.
 * So the menu entry deletion routines must be able to handle a button
 * whose menu field is null.  It would be better to share folder menus
 * between screens, but accelerators call action procedures which depend
 * upon being able to get the screen from the widget argument.
 */

		DeleteFolderMenuEntry
		    ( BBoxFindButtonNamed( scrnList[i]->folderbuttons,
					  parent_folder), 
		     foldername);
	    }
	    else {
		BBoxDeleteButton
		    (BBoxFindButtonNamed( scrnList[i]->folderbuttons,
					 foldername));
	    }

	    /* If we've deleted the current folder, show the Initial Folder */

	    if ((! strcmp(scrnList[i]->curfolder, foldername)) 
		&& (BBoxNumButtons(scrnList[i]->folderbuttons))
		&& (strcmp(foldername, app_resources.initial_folder_name)))
		TocSetScrn(InitialFolder, scrnList[i]);
	}
    XtFree(foldername);
    if (deleteData->original_toc != NULL) 
	TocSetScrn(deleteData->original_toc, scrn);
    XtFree((char *) deleteData);
}


/* Delete the selected folder.  Requires confirmation! */

/*ARGSUSED*/
void DoDeleteFolder(w, client_data, call_data)
    Widget	w;
    XtPointer	client_data;
    XtPointer	call_data;
{
    Scrn	scrn = (Scrn) client_data;
    Toc		toc  = SelectedToc(scrn);
    DeleteData	deleteData;

    /* Prevent more than one confirmation popup on the same folder. 
     * TestAndSet returns true if there is a delete pending on this folder.
     */
    if (TocTestAndSetDeletePending(toc))	{
	PopupError("There is a delete pending on this folder.");
	return;
    }

    deleteData = XtNew(DeleteDataRec);
    deleteData->scrn = scrn;
    deleteData->toc = toc;
    deleteData->original_toc = CurrentToc(scrn);
    if (deleteData->original_toc == toc)
	deleteData->original_toc = (Toc) NULL;

    CheckAndConfirmDeleteFolder(w, (XtPointer) deleteData, (XtPointer) NULL);
}


/*ARGSUSED*/
void XmhDeleteFolder(w, event, params, num_params)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*params;	/* unused */
    Cardinal	*num_params;	/* unused */
{
    Scrn	scrn = ScrnFromWidget(w);
    DoDeleteFolder(w, (XtPointer) scrn, (XtPointer) NULL);
}


/*-----	Notes on MenuButtons as folder buttons ---------------------------
 *
 * I assume that the name of the button is identical to the name of the folder.
 * Only top-level folders have buttons.
 * Only top-level folders may have subfolders.
 * Top-level folders and their subfolders may have messages.
 *
 */

static char filename[500];	/* for IsFolder() and for callback */
static int  flen = 0;		/* length of a substring of filename */


/* Function name:	IsFolder
 * Description:		determines if a file is an mh subfolder.
 */
static int IsFolder(ent)
    struct direct *ent;
{
    register int i, len;
    char *name = ent->d_name;
    struct stat buf;

    /* mh does not like subfolder names to be strings of digits */

    if (isdigit(name[0]) || name[0] == '#') {
	len = strlen(name);
	for(i=1; i < len && isdigit(name[i]); i++)
	    ;
	if (i == len) return FALSE;
    }
    else if (name[0] == '.')
	return FALSE;

    (void) sprintf(filename + flen, "/%s", name);
    if (stat(filename, &buf) /* failed */) return False;
    return (buf.st_mode & S_IFMT) == S_IFDIR;
}


/* menu entry selection callback for folder menus. */

/*ARGSUSED*/
static void DoSelectFolder(w, closure, data)
    Widget 	w;		/* the menu entry object */
    XtPointer	closure;	/* foldername */
    XtPointer	data;	
{
    Scrn	scrn = ScrnFromWidget(w);
    SetCurrentFolderName(scrn, (char *) closure);
}

/*ARGSUSED*/
void FreeMenuData(w, client_data, call_data)
    Widget	w;
    XtPointer	client_data, call_data;
{
    XtFree((char*) client_data);
}

/* Function name:	AddFolderMenuEntry
 * Description:	
 *	Add an entry to a menu.  If the menu is not already created,
 *	create it, including the (already existing) new subfolder directory.
 * 	If the menu is already created,	add the new entry.
 */

static void AddFolderMenuEntry(button, entryname)
    Button	button;		/* the corresponding menu button */
    char	*entryname;	/* the new entry, relative to MailDir */
{
    Arg		args[4];
    char *	name;
    char *	c;
    char        tmpname[300];
    char *	label;
    static XtCallbackRec callbacks[] = {
	{ DoSelectFolder,		(XtPointer) NULL },
	{ (XtCallbackProc) NULL,	(XtPointer) NULL}
    };
    static XtCallbackRec destroyCallbacks[] = {
	{ FreeMenuData,			(XtPointer) NULL },
	{ (XtCallbackProc) NULL,	(XtPointer) NULL}
    };

    /* The menu must be created before we can add an entry to it. */

    if (button->menu == NULL || button->menu == NoMenuForButton) {
	CreateFolderMenu(button);
	return;
    }
    name = XtNewString(entryname);
    callbacks[0].closure = (XtPointer) name;
    destroyCallbacks[0].closure = (XtPointer) name;
    XtSetArg(args[0], XtNcallback, callbacks);			/* ONE */
    XtSetArg(args[1], XtNdestroyCallback, destroyCallbacks);	/* TWO */

    /* When a subfolder and its parent folder have identical names,
     * the widget name of the subfolder's menu entry must be unique.
     */
    label = entryname;
    c = index( strcpy(tmpname, entryname), '/');
    if (c) {
	*c = '\0';
	label = ++c;
	if (strcmp(tmpname, c) == 0) {
	    c--;
	    *c = '_';
	}
	name = c;
    }
    XtSetArg(args[2], XtNlabel, label);				/* THREE */
    XtCreateManagedWidget(name, smeBSBObjectClass, button->menu, 
			  args, THREE);
}



/* Function name:	CreateFolderMenu
 * Description:	
 *	Menus are created for folder buttons if the folder has at least one
 *	subfolder.  For the directory given by the concatentation of 
 *	app_resources.mail_path, '/', and the name of the button, 
 *	CreateFolderMenu creates the menu whose entries are
 *	the subdirectories which do not begin with '.' and do not have
 *	names which are all digits, and do not have names which are a '#'
 *	followed by all digits.  The first entry is always the name of the
 *	parent folder.  Remaining entries are alphabetized.
 */

static void CreateFolderMenu(button)
    Button	button;
{
    struct direct **namelist;
    register int i, n, length;
    extern	alphasort();
    char	directory[500];

    n = strlen(app_resources.mail_path);
    (void) strncpy(directory, app_resources.mail_path, n);
    directory[n++] = '/';
    (void) strcpy(directory + n, button->name);
    flen = strlen(directory);		/* for IsFolder */
    (void) strcpy(filename, directory);	/* for IsFolder */
    n = scandir(directory, &namelist, IsFolder, alphasort);
    if (n <= 0) {
	/* no subfolders, therefore no menu */
	button->menu = NoMenuForButton;
	return;
    }

    button->menu = XtCreatePopupShell("menu", simpleMenuWidgetClass,
				      button->widget, (ArgList) NULL, ZERO);
	
    /* The first entry is always the parent folder */

    AddFolderMenuEntry(button, button->name);

    /* Build the menu by adding all the current entries to the new menu. */

    length = strlen(button->name);
    (void) strncpy(directory, button->name, length);
    directory[length++] = '/';
    for (i=0; i < n; i++) {
	(void) strcpy(directory + length, namelist[i]->d_name);
	free((char *) namelist[i]);
	AddFolderMenuEntry(button, directory);
    }
    free((char *) namelist);
}


/* Function:	DeleteFolderMenuEntry
 * Description:	Remove a subfolder from a menu.
 */

static void DeleteFolderMenuEntry(button, foldername)
    Button	button;
    char	*foldername;
{
    char *	c;
    Arg		args[2];
    char *	subfolder;
    int		n;
    char	tmpname[300];
    Widget	entry;
    
    if (button == NULL || button->menu == NULL) return;
    XtSetArg(args[0], XtNnumChildren, &n);
    XtSetArg(args[1], XtNlabel, &c);
    XtGetValues(button->menu, args, TWO);
    if ((n <= 3 && c) || n <= 2) {
	XtDestroyWidget(button->menu);	
	button->menu = NoMenuForButton;
	return;
    }

    c = index( strcpy(tmpname, foldername), '/');
    if (c) {
	*c = '\0';
	subfolder = ++c;
	if (strcmp(button->name, subfolder) == 0) {
	    c--;
	    *c = '_';
	    subfolder = c;
	}
	if ((entry = XtNameToWidget(button->menu, subfolder)) != NULL)
	    XtDestroyWidget(entry);
    }
}

/* Function Name:	PopupFolderMenu
 * Description:		This action should alwas be taken when the user
 *	selects a folder button.  A folder button represents a folder 
 *	and zero or more subfolders.  The menu of subfolders is built upon
 *	the first reference to it, by this routine.  If there are no 
 *	subfolders, this routine will mark the folder as having no 
 *	subfolders, and no menu will be built.  In that case, the menu
 *	button emulates a command button.  When subfolders exist,
 *	the menu will popup, using the menu button action PopupMenu.
 */

/*ARGSUSED*/
void XmhPopupFolderMenu(w, event, vector, count)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*vector;	/* unused */
    Cardinal	*count;		/* unused */
{
    Button	button;
    Scrn	scrn;

    scrn = ScrnFromWidget(w);
    if ((button = BBoxFindButton(scrn->folderbuttons, w)) == NULL)
	return;
    if (button->menu == NULL)
	CreateFolderMenu(button);

    if (button->menu == NoMenuForButton)
	LastMenuButtonPressed = w;
    else {
	XtCallActionProc(button->widget, "PopupMenu", (XEvent *) NULL,
			 (String *) NULL, (Cardinal) 0);
	XtCallActionProc(button->widget, "reset", (XEvent *) NULL,
			 (String *) NULL, (Cardinal) 0);
    }
}


/* Function Name:	XmhSetCurrentFolder
 * Description:		This action procedure allows menu buttons to 
 *	emulate toggle widgets in their function of folder selection.
 *	Therefore, mh folders with no subfolders can be represented
 * 	by a button instead of a menu with one entry.  Sets the currently
 *	selected folder.
 */

/*ARGSUSED*/
void XmhSetCurrentFolder(w, event, vector, count)
    Widget	w;
    XEvent	*event;		/* unused */
    String	*vector;	/* unused */
    Cardinal	*count;		/* unused */
{
    Button	button;
    Scrn	scrn;

    /* The MenuButton widget has a button grab currently active; the
     * currently selected folder will be updated if the user has released
     * the mouse button while the mouse pointer was on the same menu button
     * widget that orginally activated the button grab.  This mechanism is
     * insured by the XmhPopupFolderMenu action setting LastMenuButtonPressed.
     * The action XmhLeaveFolderButton, and it's translation in the application
     * defaults file, bound to LeaveWindow events, insures that the menu
     * button behaves properly when the user moves the pointer out of the 
     * menu button window.
     *
     * This action is for menu button widgets only.
     */

    if (w != LastMenuButtonPressed)
	return;
    scrn = ScrnFromWidget(w);
    if ((button = BBoxFindButton(scrn->folderbuttons, w)) == NULL)
	return;
    SetCurrentFolderName(scrn, button->name);
}


/*ARGSUSED*/
void XmhLeaveFolderButton(w, event, vector, count)
    Widget	w;
    XEvent	*event;
    String	*vector;
    Cardinal	*count;
{
    LastMenuButtonPressed = NULL;
}


void Push(stack_ptr, data)
    Stack	*stack_ptr;
    char 	*data;
{
    Stack	new = XtNew(StackRec);
    new->data = data;
    new->next = *stack_ptr;
    *stack_ptr = new;
}

char * Pop(stack_ptr)
    Stack	*stack_ptr;
{
    Stack	top;
    char 	*data = NULL;

    if ((top = *stack_ptr) != NULL) {
	data = top->data;
	*stack_ptr = top->next;
	XtFree((char *) top);
    }
    return data;
}

/* Parameters are taken as names of folders to be pushed on the stack.
 * With no parameters, the currently selected folder is pushed.
 */

/*ARGSUSED*/
void XmhPushFolder(w, event, params, count)
    Widget	w;
    XEvent 	*event;
    String 	*params;
    Cardinal 	*count;
{
    Scrn	scrn = ScrnFromWidget(w);
    int		i;

    for (i=0; i < *count; i++) 
	Push(&scrn->folder_stack, params[i]);

    if (*count == 0 && scrn->curfolder)
	Push(&scrn->folder_stack, scrn->curfolder);
}

/* Pop the stack & take that folder to be the currently selected folder. */

/*ARGSUSED*/
void XmhPopFolder(w, event, params, count)
    Widget	w;
    XEvent 	*event;
    String 	*params;
    Cardinal 	*count;
{
    Scrn	scrn = ScrnFromWidget(w);
    char	*folder;

    if ((folder = Pop(&scrn->folder_stack)) != NULL)
	SetCurrentFolderName(scrn, folder);
}
