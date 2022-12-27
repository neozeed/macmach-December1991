/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989 HEWLETT-PACKARD COMPANY.
*  ALL RIGHTS RESERVED
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/**---------------------------------------------------------------------
***	
***	file:		xmeditor.c
***
***	project:	Motif Widgets example programs
***
***	description:	This program demonstrates the Motif text, main window,
***			and dialog widgets, as well as the cut and paste
***			functions.
***	
***	defaults:	xmeditor.c depends on these defaults:
***
#
*allowShellResize:		true
*borderWidth:			0
*highlightThickness:		2
*traversalOn:			true
*keyboardFocusPolicy:		explicit
#
xmeditor*menu_bar*background:	#58f
#
***-------------------------------------------------------------------*/



/*-------------------------------------------------------------
**	Include Files
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/BulletinB.h>
#include <Xm/FileSB.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/Text.h>
#include <Xm/CutPaste.h>



/*-------------------------------------------------------------
**	Global Variables
*/

#define MENU_HELP		200
#define MENU_EXIT		201
#define MENU_OPEN		202
#define MENU_NEW		203
#define MENU_CLOSE		204
#define MENU_SAVE		205
#define MENU_SAVE_AS		206
#define MENU_PRINT		207
#define MENU_CUT		208
#define MENU_COPY		209
#define MENU_PASTE		210
#define MENU_CLEAR		211

#define DIALOG_FSELECT		300
#define DIALOG_CWARNING		301
#define DIALOG_XWARNING		302
#define DIALOG_NEW		303
#define DIALOG_SAVE		304
#define DIALOG_HELP		305
#define DIALOG_PRINT		306

/* defines a temporary file for file transfers */

Widget text;			/* multi-line text widget		    */
Widget cut_button;		/* clipboard cut button 		    */
Widget copy_button;		/* clipboard copy button 		    */
Widget paste_button;		/* clipboard paste button 		    */
Widget clear_button;		/* clipboard clear button 		    */
Widget open_dialog;		/* file selection dialog 		    */
Widget new_dialog;		/* file name prompt dialog 		    */
Widget close_warning;		/* special internal selection dialog	    */
Widget exit_warning;		/* special internal selection dialog	    */
Widget general_warning;		/* warning dialog	 		    */
Widget save_dialog;		/* save as prompt dialog	 	    */
Widget print_warning;		/* warning dialog		 	    */
Boolean file_saved = True;	/* indicates that the present file is saved */
char *filename = NULL;		/* string containing file name 		    */
int start_pos, end_pos;		/* start and end position of last action    */

XmStringCharSet charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
				/* used to set up XmStrings */

char Error[128];

/* bits for exclamation point in dialog */
char warningBits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00,
   0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};


/************************************************************************
 *
 *  CreateDefaultImage - create a default images for warning symbol.
 *
 **********************************<->***********************************/
static XImage * CreateDefaultImage (bits, width, height)
char *bits;
int   width, height;
{
    XImage *image;

    image = (XImage *) XtMalloc (sizeof (XImage));
    image->width = width;
    image->height = height;
    image->data = bits;
    image->depth = 1;
    image->xoffset = 0;
    image->format = XYBitmap;
    image->byte_order = LSBFirst;
    image->bitmap_unit = 8;
    image->bitmap_bit_order = LSBFirst;
    image->bitmap_pad = 8;
    image->bytes_per_line = (width+7)/8;
    return (image);
}



/*-------------------------------------------------------------
**	OpenFile
**		Open the present file.  Returns true if file 
**  exists and open is sucessful.
*/
Boolean OpenFile()
{
   struct stat statbuf;		/* Information on a file. */
   int file_length;		/* Length of file. 	  */
   unsigned char * file_string;	/* Contents of file. 	  */
   FILE *fp = NULL;		/* Pointer to open file   */
   
   if ((fp = fopen(filename, "r+")) == NULL)
	if ((fp = fopen(filename, "r")) != NULL) {
	    fprintf(stderr, "Warning: file opened read only.\n");
	} else {
	    return(False);
	}

   if (stat(filename, &statbuf) == 0)
	 file_length = statbuf.st_size;
   else
	 file_length = 1000000; /* arbitrary file length */

   /* read the file string */
   file_string = (unsigned char *) XtMalloc((unsigned)file_length);
   fread(file_string, sizeof(char), file_length, fp);

   /* close up the file */
   if (fclose(fp) != NULL) fprintf(stderr, "Warning: unable to close file.\n");

   /* added the file string to the text widget */
   XmTextSetString(text, file_string);

   file_saved = True; /* intialize to True */
	 
   /* make appropriate item sensitive */
   XtSetSensitive(text, True);
   XtSetSensitive(cut_button, True);
   XtSetSensitive(copy_button, True);
   XtSetSensitive(clear_button, True);

   return(True);
}



/*-------------------------------------------------------------
**	SaveFile
**		Save the present file.
*/
Boolean SaveFile()
{
    unsigned char * file_string = NULL;	   /* Contents of file.		      */
    FILE *tfp;				   /* Pointer to open temporary file. */
	char	namebuf[BUFSIZ]; /* for "system" call below */
	int		status;
    char *tempname = (char *)XtMalloc(25); /* Temporary file name. 	      */

    strcpy(tempname, mktemp("/tmp/xmeditXXXXXX"));
    
    if ((tfp = fopen(tempname, "w")) == NULL) {
       fprintf(stderr, "Warning: unable to open temp file, text not saved.\n");
       return(False);;
    }

    /* get the text string */
    file_string = (unsigned char *)XmTextGetString(text);

    /* write to a temp file */
    fwrite(file_string, sizeof(char), strlen(file_string) + 1, tfp);

    /* flush and close the temp file */
    if (fflush(tfp) != NULL) fprintf(stderr,"Warning: unable to flush file.\n");
    if (fclose(tfp) != NULL) fprintf(stderr,"Warning: unable to close file.\n");

    if (file_string != NULL) {
        XtFree(file_string); /* free the text string */
    }

    /* move the tempname to the saved file, but do it independent
	   of filesystem boundaries */
	sprintf (namebuf, "cp %s %s\0", tempname, filename);
	status = system(namebuf);
	unlink (tempname);
	if (status == 0) {
        file_saved = True;
    } else {
        fprintf(stderr, "Warning: unable to save file.\n");
        XtFree(tempname);
        return(False);
    }
           
    XtFree(tempname);
    return(True);
}



/*-------------------------------------------------------------
**      CloseFile
**              Close the present file.
*/
void CloseFile()
{
    /* zero out the text string in the text widget.
       caution: is causes a value changed callack. */
    XmTextSetString(text, "");

    file_saved = True; /* reinitialize file_saved flag */

    /* free the file name */
    if (filename != NULL) {
	XtFree(filename);
        filename = NULL;
    }

    /* set text to insensitive */
    XtSetSensitive(text, False);
}



/*-------------------------------------------------------------
**	CopyFileToClipboard
**		Copy the present file to the clipboard.
*/
void CopyFileToClipboard(time)
Time time;
{
   char *selected_string = XmTextGetSelection (text); /* text selection    */
   unsigned long item_id = 0;			      /* clipboard item id */
   int data_id = 0;				      /* clipboard data id */
   int status = 0;				      /* clipboard status  */
   XmString clip_label;

   /* using the clipboard facilities, copy the selected text to the clipboard */
   if (selected_string != NULL) {
	clip_label = XmStringCreateLtoR ("XM_EDITOR", charset);
	/* start copy to clipboard, and continue till
	   a sucessful start copy is made */
	status = 0;
	while (status != ClipboardSuccess)
	    status = XmClipboardStartCopy (XtDisplay(text), XtWindow(text),
				       	clip_label, time,
				       	text, NULL, &item_id);

   	/* move the data to the clipboard, and
	   continue till a sucessful copy is made */
	status = 0;
	while (status != ClipboardSuccess)
	    status = XmClipboardCopy (XtDisplay(text), XtWindow(text),
				      item_id, "STRING", selected_string,
                       	     	      (long)strlen(selected_string)+1, 0,
				      &data_id);

	/* end the copy to the clipboard and continue till
	   a sucessful end copy is made */
	status = 0;
	while (status != ClipboardSuccess)
	    status = XmClipboardEndCopy (XtDisplay(text), XtWindow(text),
					 item_id);

	/* allow pasting when an item is sucessfully copied to the clipboard */
	XtSetSensitive(paste_button, True);

   }

}



/*-------------------------------------------------------------
**	DeletePrimarySelection
**		Deletes the primary selection.
*/
void DeletePrimarySelection()
{
   XClientMessageEvent cm;

   /* send a client message to the text widget
      to delete the current selection */
   cm.type = ClientMessage;
   cm.display = XtDisplay(text);
   cm.message_type = XmInternAtom(XtDisplay(text), "KILL_SELECTION", FALSE);
   cm.window = XtWindow(text);
   cm.format = 32;
   cm.data.l[0] = XA_PRIMARY;
   XSendEvent(XtDisplay(text), cm.window, TRUE, NoEventMask, &cm);
}


/*-------------------------------------------------------------
**	PasteItemFromClipboard
**		paste item from the clipboard to the current cursor location
*/
void PasteItemFromClipboard()
{
   /* retrieve the current data from the clipboard
      and paste it at the current cursor position */
   char * selected_string = XmTextGetSelection (text);
					      /* containts of selection  */
   int status = 0;			      /* clipboard status	 */
   char *buffer;			      /* temporary text buffer 	 */
   int length;				      /* length of buffer     	 */
   int outlength = 0;			      /* length of bytes copied	 */
   int private_id = 0;			      /* id of item on clipboard */
   XmTextPosition cursorPos;		      /* text cursor position 	 */
   register int ac;			      /* arg count  	      	 */
   Arg al[10];				      /* arg list	      	 */

   /* find the length of the paste item, continue till the length is found */
   while (status != ClipboardSuccess) {
     status = XmClipboardInquireLength(XtDisplay(text), XtWindow(text),
				      "STRING", &length);
     if (status == ClipboardNoData) {
	length = 0;
	break;
     }
   }
			
   if (length == 0) {
	fprintf(stderr, "Warning: paste failed, no items to paste.\n");	
	return;
   }

   /* malloc to necessary space */
   buffer = XtMalloc(length);

   status = XmClipboardRetrieve (XtDisplay(text), XtWindow(text), "STRING",
				 buffer, length, &outlength, &private_id);
			
   /* Dialogs need to be added to indicate errors in pasting */
   if (status != ClipboardSuccess) {
	fprintf(stderr, "Warning: paste failed, status = %d\n", status);	
	return;
   }

   /* get cursor position for pasting */
   XtSetArg(al[0], XmNcursorPosition, &cursorPos);
   XtGetValues(text, al, 1);
			
   /* add new text */
   XmTextReplace(text, cursorPos, cursorPos, buffer);
}


/*-------------------------------------------------------------
**	FileChangedCB
**		Process callback from Text.
*/
void FileChangedCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
    /* set the file_saved flag to indicate that the
       file has been modified and the user should be
       notified before exiting. */

    file_saved = False;
}


/*-------------------------------------------------------------
**	MenuCB
**		Process callback from PushButtons in PulldownMenus.
*/
void MenuCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	register int ac;		/* arg count		    */
	Arg al[10];			/* arg list		    */
	char *command;			/* command used in printing */

	switch ((int)client_data)
	{
		case MENU_OPEN:
			/* display the file selection dialog */
			XtManageChild (open_dialog);
			break;

		case MENU_NEW:
			/* display the prompt dialog */
			XtManageChild (new_dialog);
			break;

		case MENU_CLOSE:
	 		/* the present file has not been saved since
			   the last modification */
			if (!file_saved) /* display the 'save' message dialog */
			   XtManageChild (close_warning);
			else
			   CloseFile();
			break;

		case MENU_SAVE:
			/* open a temp file for writing */
			SaveFile();
			break;

		case MENU_SAVE_AS:
			/* Display the 'save as' dialog with the
			   present filename displayed in it. */
			ac = 0;
			XtSetArg(al[ac], XmNtextString,
			     XmStringCreateLtoR (filename, charset));  ac++;
			XtSetValues(save_dialog, al, ac);
			XtManageChild (save_dialog);
			break;

		case MENU_PRINT:
			if (!file_saved)
			   XtManageChild(print_warning);
			else if (filename != NULL) {
				/* malloc space for the command name. 
			   	   Note: command = size of the filename +
			   	   "lp " + null terminator */
				command = XtMalloc(strlen(filename) + 4);
				sprintf(command, "lp %s\0", filename);
				if (system(command) != NULL)
					fprintf(stderr, "print failed");
				XtFree(command);
			}
			break;

		case MENU_EXIT:
			/* exit if there is no files open */
			if (!file_saved) /* display the 'save' message dialog */
			   XtManageChild (exit_warning);
			else {
			   /* close up file pointers and descriptors */
			   CloseFile();

			   /* exit this program */
			   exit (0);
			}
			break;

		case MENU_CUT:
			{
			   /* needed to get the event time */
                           XmAnyCallbackStruct * cb =
					    (XmAnyCallbackStruct *) call_data;

			   /* call routine to copy selection to clipboard */
			   CopyFileToClipboard(cb->event->xbutton.time);

			   /* call routine to delete primary selection */
			   DeletePrimarySelection();
    			}
			break;

		case MENU_COPY:
    			{
			   /* needed to get the event time */
                           XmAnyCallbackStruct * cb =
					    (XmAnyCallbackStruct *) call_data;

			   /* call routine to copy selection to clipboard */
			   CopyFileToClipboard(cb->event->xbutton.time);

    			}
			break;

		case MENU_PASTE:
			/* call the routine that pastes the
			   text at the cursor position */
			PasteItemFromClipboard();
			break;

		case MENU_CLEAR:
			/* call routine to delete primary selection */
			DeletePrimarySelection();
			break;

		case MENU_HELP:
			/* no help at this time */
			break;

		default:
			/* unknown client_data was recieved and
			   there is no setup to handle this */
			fprintf(stderr, "Warning: in menu callback\n");
			break;
	}

}



/*-------------------------------------------------------------
**	DialogApplyCB
**		Process callback from Dialog apply actions.
*/
static void DialogApplyCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	char *command;			/* command used in printing */

	switch ((int)client_data)
	{
		case DIALOG_PRINT:
			if (filename != NULL) {
			       /* malloc space for the command name. 
			          Note: command = size of the filename +
			          "lp " + null terminator */
				command = XtMalloc(strlen(filename) + 4);
				sprintf(command, "lp %s\0", filename);
				if (system(command) != NULL)
					fprintf(stderr, "print failed");
				XtFree(command);
			}
		case DIALOG_CWARNING:
			CloseFile();
			file_saved = True; /* reset the default */
			break;

		case DIALOG_XWARNING:
			CloseFile();
			exit();
			break;		

		default:
			/* unknown client_data was recieved and
			   there is no setup to handle this */
			fprintf (stderr, "Warning: in apply callback\n");
			break;

	}
}


/*-------------------------------------------------------------
**	DialogCancelCB
**		Process callback from Dialog cancel actions.
*/
static void DialogCancelCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	switch ((int)client_data)
	{
		case DIALOG_FSELECT:
			/* popdown the file selection box */
		  	XtUnmanageChild (open_dialog);
			break;

		case DIALOG_CWARNING:
		case DIALOG_XWARNING:
		case DIALOG_NEW:
		case DIALOG_PRINT:
		case DIALOG_SAVE:
		case DIALOG_HELP:
			/* no action is necessary at this time */
			break;

		default:
			/* a unknown client_data was recieved and
			   there is no setup to handle this */
			fprintf (stderr, "Warning: in cancel callback\n");
			break;
	}
}



/*-------------------------------------------------------------
**	DialogAcceptCB
**		Process callback from Dialog actions.
*/
static void DialogAcceptCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	char *command;		/* command used in printing */

	switch ((int)client_data)
	{
		case DIALOG_FSELECT:
		        /* open the file and read it into the text widget */
			if (filename != NULL) {
			   XtFree(filename);
			   filename = NULL;
                        }
			{
			   XmFileSelectionBoxCallbackStruct *fcb =
				 (XmFileSelectionBoxCallbackStruct *) call_data;

			   /* get the filename from the file selection box */
			   XmStringGetLtoR(fcb->value, charset, &filename);

			   /* Open file, print error if it does not exist. */
			   if (!OpenFile())
	   		      fprintf(stderr, "Warning: unable to open file\n");

			   /* popdown the file selection box */
		  	   XtUnmanageChild (open_dialog);
			}
			break;

		case DIALOG_NEW:
		        /* open the file and read it into the text widget */
			if (filename != NULL) {
			   XtFree(filename);
			   filename = NULL;
                        }
			{
			   XmSelectionBoxCallbackStruct *scb =
				 (XmSelectionBoxCallbackStruct *) call_data;

			   /* get the filename string from the file
			      name prompt box */
			   XmStringGetLtoR(scb->value, charset, &filename);

			   /* open file if it exists,
			      if not set items sensitive */
			   if (!OpenFile(True)) {
   			   	/* make appropriate item sensitive */
   			   	XtSetSensitive(text, True);
   			   	XtSetSensitive(cut_button, True);
   			   	XtSetSensitive(copy_button, True);
   			   	XtSetSensitive(clear_button, True);
			   }

			   /* popdown the file selection box */
		  	   XtUnmanageChild (new_dialog);
			}
			break;

		case DIALOG_CWARNING:
			/* save the file */
			if (SaveFile()) {
			   CloseFile(); /* close the file */
			} else
			   fprintf(stderr, 
			       "Warning: unable to save file, file not closed");
			break;

		case DIALOG_XWARNING:
			/* save the file */
			if (SaveFile()) {
			   CloseFile(); /* close the file */
			   exit(0);
                        } else
			   fprintf(stderr,
				 "Warning: unable to save file, exit aborted");
			break;

		case DIALOG_SAVE:
			{
			   XmSelectionBoxCallbackStruct *scb =
				 (XmSelectionBoxCallbackStruct *) call_data;

			   /* get the filename string from the file
			      selection box */
			   XmStringGetLtoR(scb->value, charset, &filename);
			
			   SaveFile();

			   XtUnmanageChild (save_dialog);
			}
			break;

		case DIALOG_PRINT:
			/* save the file */
			if (SaveFile()) {
			   if (filename != NULL) {
			       /* malloc space for the command name. 
			          Note: command = size of the filename +
			          "lp " + null terminator */
				command = XtMalloc(strlen(filename) + 4);
				sprintf(command, "lp %s\0", filename);
				if (system(command) != NULL)
					fprintf(stderr, "print failed");
				XtFree(command);
				}
			} else
			   fprintf(stderr, 
			      "Warning: unable to save file, file not printed");
			break;

		case DIALOG_HELP:
			/* no help at this time */
			break;

		default:
			/* unknown callback type */
			fprintf (stderr, "Warning: in accept callback\n");
			break;
	}
}



/*-------------------------------------------------------------
**	CreateSpecialWarningDialog
**		Create special 4 button message box out of a
**	Selection box.
*/
static Widget CreateSpecialWarningDialog (parent, name, image_string, message,
					  arglist, argcount)
Widget		parent;
String		name;
String		image_string;
String		message;
ArgList		arglist;
int		argcount;
{
	Widget 		warning_dialog;	/*  special warning selection box */
	Widget 		work_area;	/*  rowcolumn for pixmap and text */
	Widget 		pixmap_label;	/*  pixmap label 		  */
	Widget 		text_label;	/*  text label 			  */
	Widget 		apply_button;	/*  apply button		  */
	Widget 		ok_button;	/*  ok button			  */
        Widget          kid[5];         /*  buttons		          */
	Pixel		foreground;	/*  dialog foreground		  */
	Pixel		background;	/*  dialog background		  */
	Pixmap		pixmap;		/*  dialog pixmap		  */
        register int    i;              /*  kid index			  */
        Arg             al[10];         /*  arg list		          */
        register int    ac;             /*  arg count		          */


	warning_dialog = XmCreatePromptDialog(parent, name, arglist, argcount);

	ac = 0;
	XtSetArg(al[ac], XmNorientation, XmHORIZONTAL); ac++;
	work_area = XmCreateRowColumn(warning_dialog, "workarea", al, ac);
	XtManageChild(work_area);

	ac = 0;
	XtSetArg(al[ac], XmNforeground, &foreground); ac++;
	XtSetArg(al[ac], XmNbackground, &background); ac++;
	XtGetValues(warning_dialog, al, ac);

	ac = 0;
	XtSetArg(al[ac], XmNlabelType, XmPIXMAP); ac++;
	pixmap = XmGetPixmap(XtScreen(warning_dialog), image_string,
			     foreground, background);
	XtSetArg(al[ac], XmNlabelPixmap, pixmap); ac++;
	pixmap_label = XmCreateLabel(work_area, "pixmap_label", al, ac);
	XtManageChild(pixmap_label);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR(message, charset)); ac++;
	text_label = XmCreateLabel(work_area, "text_label", al, ac);
	XtManageChild(text_label);

        apply_button = XmSelectionBoxGetChild (warning_dialog,
							 XmDIALOG_APPLY_BUTTON);
	
	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Discard", charset)); ac++;
	XtSetValues(apply_button, al, ac);
	XtManageChild(apply_button);

        ok_button = XmSelectionBoxGetChild (warning_dialog,
							 XmDIALOG_OK_BUTTON);
	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Save", charset)); ac++;
	XtSetValues(ok_button, al, ac);

	
        /*      Unmanage unneeded children.
        */
        i = 0;
        kid[i++] = XmSelectionBoxGetChild (warning_dialog, XmDIALOG_TEXT);
        kid[i++] = XmSelectionBoxGetChild (warning_dialog,
						     XmDIALOG_SELECTION_LABEL);
        XtUnmanageChildren (kid, i);

	return(warning_dialog);
}


/*-------------------------------------------------------------
**	CreateMenuBar
**		Create MenuBar in MainWindow
*/
static Widget CreateMenuBar (parent)
Widget		parent;
{
	Widget		menu_bar;	/*  RowColumn	 		*/
	Widget		cascade;	/*  CascadeButton		*/
	Widget		menu_pane;	/*  RowColumn	 		*/
	Widget		button;		/*  PushButton			*/

	XImage		*image;		/*  image for warning pixmap	*/

	Arg		al[10];		/*  arg list			*/
	register int	ac;		/*  arg count			*/

	/*	Create MenuArea.
	*/
	ac = 0;
	menu_bar = XmCreateMenuBar (parent, "menu_bar", al, ac);

	/*	Create "Options" PulldownMenu.
	*/
	ac = 0;
	menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", al, ac);

        image = CreateDefaultImage (warningBits, 32, 32);
        XmInstallImage (image, "warning_image");
					
	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Open", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'O'); ac++;
	button = XmCreatePushButton (menu_pane, "Open", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_OPEN);
	XtManageChild (button);

	open_dialog = XmCreateFileSelectionDialog(menu_pane,
			   "file selection dialog", NULL, 0);

	XtAddCallback (open_dialog, XmNokCallback,
			DialogAcceptCB, DIALOG_FSELECT);
	XtAddCallback (open_dialog, XmNcancelCallback,
			DialogCancelCB, DIALOG_FSELECT);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("New", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'N'); ac++;
	button = XmCreatePushButton (menu_pane, "New", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_NEW);
	XtManageChild (button);

	ac = 0;
	XtSetArg(al[ac], XmNselectionLabelString, XmStringCreateLtoR
	   ("Enter name of new file.", charset));  ac++;
	new_dialog = XmCreatePromptDialog(menu_pane,
			   "new file dialog", al, ac);
	XtAddCallback (new_dialog, XmNokCallback,
			DialogAcceptCB, DIALOG_NEW);
	XtAddCallback (new_dialog, XmNcancelCallback,
			DialogCancelCB, DIALOG_NEW);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Close", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'C'); ac++;
	button = XmCreatePushButton (menu_pane, "Close", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_CLOSE);
	XtManageChild (button);

	close_warning = CreateSpecialWarningDialog(menu_pane, "save_warning",
				      "warning_image", "Save Changes?", al, ac);

	XtAddCallback (close_warning, XmNapplyCallback,
		       DialogApplyCB, DIALOG_CWARNING);
	XtAddCallback (close_warning, XmNokCallback,
		       DialogAcceptCB, DIALOG_CWARNING);


	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Save", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'S'); ac++;
	button = XmCreatePushButton (menu_pane, "Save", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_SAVE);
	XtManageChild (button);


	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Save As...", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'A'); ac++;
	button = XmCreatePushButton (menu_pane, "Save As...", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_SAVE_AS);
	XtManageChild (button);

	ac = 0;
	XtSetArg(al[ac], XmNselectionLabelString, XmStringCreateLtoR
	   ("Save As...", charset));  ac++;
	save_dialog = XmCreatePromptDialog(menu_pane, "save dialog", al, ac);
	XtAddCallback (save_dialog, XmNokCallback,
		       DialogAcceptCB, DIALOG_SAVE);


	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Print", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'P'); ac++;
	button = XmCreatePushButton (menu_pane, "Print", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_PRINT);
	XtManageChild (button);

	ac = 0;
	XtSetArg(al[ac], XmNselectionLabelString, XmStringCreateLtoR
	   ("Save file before printing?", charset));  ac++;
	print_warning = CreateSpecialWarningDialog(menu_pane, "print_warning",
			 "warning_image", "Save file before printing?", al, ac);
	XtAddCallback (print_warning, XmNokCallback,
		       DialogAcceptCB, DIALOG_PRINT);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Exit", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'E'); ac++;
	XtSetArg(al[ac], XmNacceleratorText,
		 XmStringCreateLtoR("F3", charset)); ac++;
	XtSetArg(al[ac], XmNaccelerator, "<Key>F3:"); ac++;
	button = XmCreatePushButton (menu_pane, "Exit", al, ac);
	XtAddCallback (button, XmNactivateCallback, MenuCB, MENU_EXIT);
	XtManageChild (button);

	exit_warning = CreateSpecialWarningDialog(menu_pane, "exit warning",
				      "warning_image", "Save Changes?", al, ac);
	XtAddCallback (exit_warning, XmNapplyCallback,
		       DialogApplyCB, DIALOG_XWARNING);
	XtAddCallback (exit_warning, XmNokCallback,
			DialogAcceptCB, DIALOG_XWARNING);

	ac = 0;
	XtSetArg (al[ac], XmNsubMenuId, menu_pane);  ac++;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("File", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'F'); ac++;
	cascade = XmCreateCascadeButton (menu_bar, "File", al, ac);
	XtManageChild (cascade);

	/*	Create "Options" PulldownMenu.
	*/
	ac = 0;
	menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", al, ac);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Cut", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 't'); ac++;
	XtSetArg(al[ac], XmNacceleratorText,
		 XmStringCreateLtoR("Shift+Del", charset)); ac++;
#ifdef hpux
	XtSetArg(al[ac], XmNaccelerator, "Shift<Key>DeleteChar:"); ac++;
#else /* hpux */
	XtSetArg(al[ac], XmNaccelerator, "Shift<Key>Delete:"); ac++;
#endif /* hpux */
	cut_button = XmCreatePushButton (menu_pane, "Cut", al, ac);
	XtAddCallback (cut_button, XmNactivateCallback, MenuCB, MENU_CUT);
	XtManageChild (cut_button);
	XtSetSensitive(cut_button, False);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Copy", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'C'); ac++;
	XtSetArg(al[ac], XmNacceleratorText,
		 XmStringCreateLtoR("Ctrl+Ins", charset)); ac++;
#ifdef hpux
	XtSetArg(al[ac], XmNaccelerator, "Ctrl<Key>InsertChar:"); ac++;
#else /* hpux */
	XtSetArg(al[ac], XmNaccelerator, "Ctrl<Key>Insert:"); ac++;
#endif /* hpux */
	copy_button = XmCreatePushButton (menu_pane, "Copy", al, ac);
	XtAddCallback (copy_button, XmNactivateCallback, MenuCB, MENU_COPY);
	XtManageChild (copy_button);
	XtSetSensitive(copy_button, False);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Paste", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'P'); ac++;
	XtSetArg(al[ac], XmNacceleratorText,
		 XmStringCreateLtoR("Shift+Ins", charset)); ac++;
#ifdef hpux
	XtSetArg(al[ac], XmNaccelerator, "Shift<Key>InsertChar:"); ac++;
#else /* hpux */
	XtSetArg(al[ac], XmNaccelerator, "Shift<Key>Insert:"); ac++;
#endif /* hpux */
	paste_button = XmCreatePushButton (menu_pane, "Paste", al, ac); ac++;
	XtAddCallback (paste_button, XmNactivateCallback, MenuCB, MENU_PASTE);
	XtManageChild (paste_button);
	XtSetSensitive(paste_button, False);

	ac = 0;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Clear", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'e'); ac++;
	/* pseudo accelerator - Text already handles
	   this action appropriately */
	XtSetArg(al[ac], XmNacceleratorText,
		 XmStringCreateLtoR("Del", charset)); ac++;
	clear_button = XmCreatePushButton (menu_pane, "Clear", al, ac);
	XtAddCallback (clear_button, XmNactivateCallback, MenuCB, MENU_CLEAR);
	XtManageChild (clear_button);
	XtSetSensitive(clear_button, False);

	ac = 0;
	XtSetArg (al[ac], XmNsubMenuId, menu_pane);  ac++;
	XtSetArg(al[ac], XmNlabelString,
		 XmStringCreateLtoR("Edit", charset)); ac++;
	XtSetArg(al[ac], XmNmnemonic, 'E'); ac++;
	cascade= XmCreateCascadeButton (menu_bar, "Edit", al, ac);
	XtManageChild (cascade);


	/*	Create "Help" button.
	*/
	ac = 0;
	cascade = XmCreateCascadeButton (menu_bar, "Help", al, ac);
	XtAddCallback (cascade, XmNactivateCallback, MenuCB, MENU_HELP);
	XtManageChild (cascade);

	ac = 0;
	XtSetArg (al[ac], XmNmenuHelpWidget, cascade);  ac++;
	XtSetValues (menu_bar, al, ac);

	return (menu_bar);
}



/*-------------------------------------------------------------
**	CreateText
**		Create Text.
*/
static void CreateText (parent)
Widget		parent;
{
	Arg		al[10];		/*  arg list		*/
	register int	ac;		/*  arg count		*/
	register int	i;		/*  counter		*/
   	XFontStruct * newfont;

	/* create text widget */
	ac = 0;
	XtSetArg (al[ac], XmNrows, 24);  ac++;
	XtSetArg (al[ac], XmNcolumns, 80);  ac++;
	XtSetArg (al[ac], XmNresizeWidth, False);  ac++;
	XtSetArg (al[ac], XmNresizeHeight, False);  ac++;
	XtSetArg (al[ac], XmNscrollVertical, True);  ac++;
	XtSetArg (al[ac], XmNscrollHorizontal, True);  ac++;
	XtSetArg (al[ac], XmNeditMode, XmMULTI_LINE_EDIT);  ac++;

	text = XmCreateScrolledText (parent, "text", al, ac);

	/* add value changed callback */
	XtAddCallback (text, XmNmodifyVerifyCallback, FileChangedCB, NULL);

	return;
}


/*-------------------------------------------------------------
**	main
**		Initialize toolkit.
**		Create MainWindow and subwidgets.
**		Realize toplevel widgets.
**		Process events.
*/
void main (argc,argv)
unsigned int	argc;
char 		**argv;
{
	Display		*display;	/*  Display		*/
	Widget		app_shell;	/*  ApplicationShell 	*/
	Widget		main;		/*  MainWindow	 	*/
	Widget		menu_bar;	/*  RowColumn	 	*/
	Widget		form;		/*  Form		*/
	XtAppContext	app_context;	/*  Application Context */

	Arg		al[10];		/*  arg list		*/
	register int	ac;		/*  arg count		*/
	register int	i;		/*  counter		*/
	char	*progname; /* program name without the full pathname */


	if (progname=strrchr(argv[0], '/'))
		{
		progname++;
		}
	else
		{
		progname = argv[0];
		}

	/*	Initialize toolkit and open display.
	*/
	XtToolkitInitialize ();
	app_context = XtCreateApplicationContext();
	display = XtOpenDisplay (app_context, NULL, progname, "XMdemos",
			NULL, 0, &argc, argv);
	if (!display)
	{
		XtWarning ("xmeditor: can't open display, exiting...");
		exit (0);
	}


	/*	Create ApplicationShell.
	*/
	app_shell = XtAppCreateShell (progname, "XMdemos",
			applicationShellWidgetClass, display, NULL, 0);


	/*	Create MainWindow.
	*/
	ac = 0;
	XtSetArg (al[ac], XmNshadowThickness, 0);  ac++;
	main = XmCreateMainWindow (app_shell, "main", al, ac);
	XtManageChild (main);


	/*	Create MenuBar in MainWindow.
	*/
	menu_bar = CreateMenuBar (main);
	XtManageChild (menu_bar);


	/*	Create Text.
	*/
	CreateText (main);
	XtManageChild (text);

	XmAddTabGroup(text);

	XtSetSensitive(text, False);

	/*	Realize toplevel widgets.
	*/
	XtRealizeWidget (app_shell);


	/*	Process events.
	*/
	XtAppMainLoop (app_context);
}

