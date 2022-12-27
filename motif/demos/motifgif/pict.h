/*
 *  xgif.h  -  header file for xgif, but you probably already knew as much
 */


#define REVDATE   "Rev: 2/13/89"
#define MAXEXPAND 16

/* include files */
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
/*  X headers  */
#include <X11/IntrinsicP.h>
/*#include <X11/Shell.h>
#include <X11/AtomMgr.h>
#include <X11/Protocols.h>
*/
/*  Xm headers  */
#include <Xm/Xm.h>
#include <Xm/ArrowB.h>
#include <Xm/ArrowBG.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Command.h>
#include <Xm/CutPaste.h>
#include <Xm/DialogS.h>
#include <Xm/DrawingA.h>
#include <Xm/DrawnB.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/MainW.h>
#include <Xm/MenuShell.h>
#include <Xm/MessageB.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>
#include <Xm/SelectioB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#define MAX_ARGS     100


#ifndef MAIN
#define WHERE extern
#else
#define WHERE
#endif

typedef unsigned char byte;

#define CENTERX(f,x,str) ((x)-XTextWidth(f,str,strlen(str))/2)
#define CENTERY(f,y) ((y)-((f->ascent+f->descent)/2)+f->ascent)


/* X stuff */
WHERE Display       *theDisp;
WHERE int           theScreen, dispcells;
WHERE Colormap      theCmap;
WHERE Window        rootW, mainW;
WHERE GC            theGC;
WHERE unsigned long fcol,bcol;
WHERE Font          mfont;
WHERE XFontStruct   *mfinfo;
WHERE Visual        *theVisual;
WHERE XImage        *theImage, *expImage;

/* global vars */
WHERE int            iWIDE,iHIGH,eWIDE,eHIGH,expand,numcols,strip,nostrip;
WHERE unsigned long  cols[256];
WHERE XColor         defs[256];
WHERE char          *cmd;

Widget Shell;
Widget MainWindow;
Widget WorkRegion;
Widget MenuBar;
Widget PullDown1;
Widget MenuBtn1;
Widget Label1A;
Widget Label1B;
Widget DrawingArea;
Widget MainBoard;
Widget Label;
Widget FileDialog;
Widget FileSelection;
