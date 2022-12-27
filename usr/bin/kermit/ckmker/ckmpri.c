/*
 * file ckmpri.c
 *
 * Module of mackermit containing code for handling printing.  This code was
 * originally put into ckmusr by John A. Oberschelp of Emory U.
 *
 */

/*
 Copyright (C) 1989, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained.
*/

#include "ckcdeb.h"
#include "ckcker.h"

#include "ckmdef.h"		/* General Mac defs */
#include "ckmres.h"		/* Mac resource equates */
#include "ckmasm.h"		/* new A8 and A9 traps */
#include "ckmcon.h"		/* for fonts */


int		to_printer = FALSE;
int		to_screen = TRUE;
int		printer_is_on_line_num;
Handle	hPrintBuffer = 0L;
long	lPrintBufferSize;
long	lPrintBufferChars;
long	lPrintBufferAt;

extern	MenuHandle menus[];	/* handle on our menus */

DialogPtr	printingDialog;
DialogPtr	bufferingDialog;
DialogPtr	overflowingDialog;
DialogPtr	overflowedDialog;
DialogPtr	pauseDialog;

#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) ((a)>(b))?(a):(b)

void
now_print()
{
    short	itemhit;
    long	lPrintBufferIndex;
    char	PrintBufferChar;
    CursHandle	watchcurs;
    int		typeOfDriver;
    int		chrExtra;
    int		leftMargin;
    GrafPtr	oldPort;
    int		temp;
    TPPrPort	myPrPort;
    THPrint	PrintStuff;
    TPrStatus	myStRec;

	if (lPrintBufferChars >= lPrintBufferSize) {
		overflowedDialog = GetNewDialog(OVERFLOWEDBOXID, NILPTR, (WindowPtr) - 1);
		circleOK(overflowedDialog);

		ModalDialog ((ModalFilterProcPtr) NILPROC, &itemhit);
		DisposDialog(overflowedDialog);
		if (itemhit == 2) return;	/* Cancel */
		if (itemhit == 3) {
			DisableItem(menus[PRNT_MENU], 0);
			DrawMenuBar();
			DisposHandle(hPrintBuffer);
			hPrintBuffer = 0L;
			return;
		}

	}

	PrintStuff = (THPrint)NewHandle(sizeof(TPrint));
	GetPort(&oldPort);
	PrOpen();
	if (PrError() == noErr) {
		temp = PrValidate(PrintStuff);
		temp = PrJobDialog(PrintStuff);
		if (!temp) {
			PrClose();
			SetPort(oldPort);
			return;
		}
		printingDialog = GetNewDialog(PRINTINGBOXID, NILPTR, (WindowPtr) - 1);
		DrawDialog (printingDialog);
		watchcurs = GetCursor(watchCursor);
		SetCursor(*watchcurs);

		myPrPort = PrOpenDoc(PrintStuff, nil, nil);
		typeOfDriver = ((*PrintStuff)->prStl.wDev) >> 8;
		if (typeOfDriver == 3) { /*laser*/
			TextFont(courier);
			TextSize(10);
			chrExtra = 0;
			leftMargin = 36;
		} else {
			TextFont(VT100FONT);
			TextSize(9);
			chrExtra = 1;
			leftMargin = 36;
		}
		printer_is_on_line_num = 1;
		lPrintBufferIndex = (lPrintBufferChars > lPrintBufferSize)
					  		? lPrintBufferAt : 0L;
		PrOpenPage(myPrPort, nil);
		MoveTo(leftMargin, 1 * 12);
		do {
			if (PrError() != noErr) break;
			PrintBufferChar = (*hPrintBuffer)[lPrintBufferIndex];
			switch (PrintBufferChar) {
			  case 13:
				if (++printer_is_on_line_num > 60) {
					PrClosePage(myPrPort);
					PrOpenPage(myPrPort, nil);
					printer_is_on_line_num = 1;
				}
				MoveTo(leftMargin, printer_is_on_line_num * 12);
			  break;

			  case 14:
				if (printer_is_on_line_num != 1) {
					PrClosePage(myPrPort);
					PrOpenPage(myPrPort, nil);
					printer_is_on_line_num = 1;
				}
				MoveTo(leftMargin, printer_is_on_line_num * 12);
			  break;

			  default:
				DrawChar(PrintBufferChar);
				Move(chrExtra, 0);
			  break;
			}

			if (++lPrintBufferIndex == lPrintBufferSize) {
				lPrintBufferIndex = 0L;
			}

		} while (lPrintBufferIndex != lPrintBufferAt);
		PrClosePage(myPrPort);

		PrCloseDoc(myPrPort);
		if ((PrError() == noErr) &&
			((**PrintStuff).prJob.bJDocLoop == bSpoolLoop))
			PrPicFile(PrintStuff, nil, nil, nil, &myStRec);
		if ((PrError() != noErr) && (PrError() != 128)) {
			printerr ("Printer error: ", PrError());
		}

		DisposDialog(printingDialog);
	}
	PrClose();
	SetPort(oldPort);
	InitCursor();
	DisableItem(menus[PRNT_MENU], 0);
	DrawMenuBar();
	DisposHandle(hPrintBuffer);
	hPrintBuffer = 0L;
}

void
pr_stat()
{
    DialogPtr	printDialog;
    Str255	arg1, arg2, arg3;
    short	itemhit;


    printDialog = GetNewDialog(PRINTBOXID, NILPTR, (WindowPtr) - 1);
    circleOK(printDialog);

    NumToString(MAX(lPrintBufferChars - lPrintBufferSize, 0L), arg1);
    NumToString(MIN(lPrintBufferSize, lPrintBufferChars), arg2);
    NumToString(lPrintBufferSize, arg3);
    ParamText (arg1, arg2, arg3, "");

    do {
	ModalDialog ((ModalFilterProcPtr) NILPROC, &itemhit);

	switch (itemhit) {
	  case 1:
	  case 2:
	  case 3:
	  break;

	}
    } while (itemhit > 3);

    DisposDialog(printDialog);
}
