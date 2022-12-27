.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 2
.mc |
.nr !_ 0
.mc
.H 1 "Using Motif Widgets in Programs"
.iX "Using Widgets"
.iX "Widgets" "using"
This chapter explains how to
write applications that use the Motif Widgets.
.mc |
Writing application programs involves nine steps, as shown in Table 3-1.
.sp 2
.mc
.iX "Including header files"
.iX "Header files"
.iX "Xt Intrinsics" "initializing"
.iX "Callback" "add"
.iX "Widget" "realize"
.iX "Realize widget"
.iX "Widget" "create"
.iX "Create" "widget"
.iX "Linking libraries"
.iX "Libraries"
.iX "Create" "defaults files"
.iX "Defaults files" "create"
.TB "Steps in Writing Widget Programs"
.mc |
.in0
.TS H
center, box, tab(;);
cB  |  lB  |  lB
c     |  l   |  lB  .
.mc
Step;Description;Related Functions
_
.mc |
.TH
.mc
.sp .3
1;T{
Include required header files.
T};T{
.nf
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/\fIwidget\fP.h>
.fi
T}
.sp .3
.mc |
2;Initialize Xt Intrinsics.;XtAppInitialize() 
.mc
.sp .3
.mc |
3;Add additional top-level windows.;XtAppCreateShell() 
.mc
_
.sp .3
.mc |
;Do steps 4 through 6 for each widget.
.mc
.sp .3
4;T{
Set up argument lists for widget.
.mc |
T};XtSetArg()
5;Create widget.;T{
XtCreateManagedWidget()
.br
\fRor\fP XmCreate\fI<WidgetName>\fP
.br
\fRfollowed by\fP
.br
XtManageChild(\fIwidget\fP)
.mc
T}
.mc |
6;Add callback routines.;XtAddCallback()
.mc
_
.sp .3
7;Realize widgets and loop.;T{
.nf
XtRealizeWidget(\fIparent\fP)
.mc |
XtAppMainLoop()
.mc
.fi
T}
.sp .3
8;Link relevant libraries.;T{
.nf
.mc |
cc \-o\fIapplication\fP \fIapplication\fP.c \e
   \-lXm \-lXt \-lX11 \-lPW
.mc
.fi
T}
.sp .3/
9;Create defaults files.;T{
.nf
/usr/lib/X11/app-defaults/\fIclass\fP
.mc |
$HOME/.Xdefaults
.mc
.fi
T}
.TE
.mc |
.in
.mc
.P
.mc |
Sections 3.2 through 3.9 of this chapter describe each of the steps 
shown in the table, except Step 3, which is covered in Section 3.12.  
.mc
The sample code segments of each section build a simple widget
.mc |
program (called \fBxmbutton\fP) that implements a PushButton widget.
Section 3.1 provides background that introduces the \fBxmbutton\fP program.
.P
\fBNOTE:\fP
This chapter assumes you have a working knowledge of the C programming
language.  In particular, you should be familiar with pointers and structures.
If you are not, be sure to study a book on programming with C.
Books on the topic are widely available in computer bookstores.
.H 2 "Introduction to the xmbutton Program"
This section shows the widget tree diagram, code listing, and
defaults file for the \fBxmbutton\fP program, as well as the
output produced by the program.
.H 3 "Widget Tree Diagram for the xmbutton Program"
.P
The following figure shows a widget tree for the \fBxmbutton\fP widget.
.mc
.iX "Widget" "tree"
.mc |
.sp
.FG "xmbutton Widget Tree"
.if n .ig +n
.mc
.DS CB
.PS 
boxwid = 1.25
boxht = 0.5
Box1: box "Top Level" "Shell"
move to Box1.s
move down .5
.mc |
Box2: box "XmPushButton"
.mc
line from Box1.s to Box2.n
.PE
.DE
.mc |
.+n
.mc
.br
.ne 3i
.mc |
.H 3 "Program Listing for xmbutton.c"
.P
The listing shown in this section, \fBxmbutton.c\fP, is
for the \fBxmbutton\fP widget program. 
.P
.mc
.iX "Sample program" "simple"
.iX "Widget" "simple program"
.mc |
.in0
.mc
.nf
.mc |
.S -2
.mc
.ft CW
.ta .5i 1i 1.5i
.mc |
/**------------------------------------------------------------------
.mc
***	
***	file:		xmbutton.c
***
***	project:	Motif Widgets example programs
***
***	description:	This program creates a PushButton widget.
***	
***	
***			(c) Copyright 1989 by Open Software Foundation, Inc.
***			    All Rights Reserved.
***
***			(c) Copyright 1989 by Hewlett-Packard Company.
***
.mc |
***----------------------------------------------------------------*/
.mc


/*  include files  */

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>


/*  functions defined in this program  */

void main();
void activateCB(); /* Callback for the PushButton */


/*  global variables  */

char *btn_text;	/* button label pointer for compound string */


/*-------------------------------------------------------------
**	main - main logic for demo1 program
*/
void main (argc,argv)
unsigned int argc;
char **argv;
{
	Widget	toplevel; /*  Shell widget	*/
	Widget	button;	/*  PushButton widget	*/
.mc |
	XtAppContext app_context; /* application context */
.mc
	Arg	args[10]; /*  arg list		*/
	register int n;	/*  arg count		*/

/*  initialize toolkit  */
.mc |
	toplevel =
          XtAppInitialize(&app_context, "XMdemos", NULL, 
             0, &argc, argv, NULL, NULL, 0);
.mc

/*  create compound string for the button text  */
.mc |
	btn_text =
           XmStringCreateLtoR("Push Here", XmSTRING_DEFAULT_CHARSET);
.mc

/*  set up arglist  */
	n = 0;
.mc *
.mc
	XtSetArg (args[n], XmNlabelString, btn_text);  n++;
	XtSetArg (args[n], XmNwidth, 250);  n++;
	XtSetArg (args[n], XmNheight, 150);  n++;
/*  create button  */
.mc |
	button =
          XtCreateManagedWidget ("button", xmPushButtonWidgetClass,
	                                              toplevel, args, n);
.mc
/*  add callback  */
	XtAddCallback (button, XmNactivateCallback, activateCB, NULL);
/*  realize widgets  */
	XtRealizeWidget (toplevel);
/*  process events  */
.mc |
	XtAppMainLoop (app_context);
.mc
}


/*-------------------------------------------------------------
**	activateCB - callback for button
*/
void activateCB (w, client_data, call_data) 
Widget	w;		/*  widget id		*/
caddr_t	client_data;	/*  data from application   */
caddr_t	call_data;	/*  data from widget class  */
{
.mc |
/*  print message, free compound string memory,
 *  and terminate program  */
.mc
	printf ("PushButton selected.\\n");
.mc |
	XmStringFree(btn_text);
.mc
	exit (0);
}
.mc |
.S
.mc
.ft R
.mc |
.fi
.in
.mc
.H 3 "Defaults File XMdemos Partial Listing"
.iX "Defaults file" "XMdemos"
.mc *
.mc
.P
.mc |
The \fBXMdemos\fP default file, part of which is shown in this section,  
contains default specifications for general
appearance and behavior, plus, in some cases, program-unique
specifications.  All the example programs in this guide use this defaults file.
.P
.mc
.ft CW
.mc |
.S -2
.mc
.nf
!
.mc |
!XMdemos app-defaults file for Motif demo programs
.mc
!
.mc |
!general appearance and behavior defaults
.mc
!
*foreground:			white
.mc *
.mc
*allowShellResize:		true
*borderWidth:			0
*highlightThickness:		2
.mc *
.mc
*keyboardFocusPolicy:		explicit
*menuAccelerator:		<Key>KP_F2
.fi
.ft R
.mc |
.S
.mc
.P
.mc |
.H 3 "Output Produced by the xmbutton Program"
The following figure shows the resulting screen display for the \fBxmbutton\fP program.
.P
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11.5P
.P
.sC graphics/xmbutton.tif tiff 300 14.5P 9.5P
\&
.P
.sp +1
.FG "Sample Program xmbutton Screen Display"
.sp +1.5
.\}
.el \{\
.br
.ne 11.5P
\&
.sp 9.5P
.ce
illustration xmbutton.eps goes here
.sp +1
.FG "Sample Program xmbutton Screen Display"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.25i
.mc
\0
.mc *
.mc
.FG "Sample Program xmbutton Screen Display"
.mc |
.sp 1.75i
.in +1.56i
.P! graphics/xmbutton.ps 
.in -1.56i
.mc
.P
.mc |
Section 3.10 describes the use of color in screen design, and
Section 3.11 introduces some advanced programming techniques.
Section 3.12 presents a more involved sample program.
.mc
.P
.mc *
.mc
The following sections describe the process for
.mc |
writing widget programs, which was summarized in Table 3-1.
.mc
Following these steps will help you start writing programs
that use the Motif Widgets.
.H 2 "Including Header Files"
.iX "Header files" "including"
.iX "Including header files"
Special variables and types of variables used by Motif programs are
defined in header files.
Include the appropriate files at the beginning of your program.
.mc |
The manpage for each widget specifies the header files that are needed.
.mc
.P
.br
.ne 2i
Usually this section in your program will look like this:
.P
.ft CW
.nf
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/\fIwidget\fP.h>
.fi
.ft R
.P
The order in which you place the header files is very important.  Generally
speaking, you should follow this format:
.AL
.LI
General header files, such as \fB<stdio.h>\fP.
.LI
Intrinsics header files, such as \fB<X11/Intrinsic.h>\fP.
.LI
Motif widget header files, beginning with \fB<Xm.h>\fP and
including a header file for each widget class you are using in
your program.  The order of the widget class headers is not
critical.
.LE
.P
For each widget you are using in your program,
replace \fIwidget\fP with the name of the widget.
.mc |
The manpage for each widget shows the exact spelling of all header files
you need.  The include files for all widgets are found in the 
\fB/usr/include/Xm\fP directory.
.mc
For the PushButton widget in the sample program \fBxmbutton\fP,
the header file name is \fBPushB.h\fP.
Put a \fB#include\fP statement in your program for each
type of widget you use.
You need to include a header file only once, even
if you use a given widget twice in your program.
.mc |
Do not forget to include any other header files (such as
.mc
\&\fB<stdio.h>\fP) that your program may need.
.P
\&\fBIntrinsic.h\fP defines the Xt structures and variables.
Variables common to all Motif Widgets are defined in \&\fBXm.h\fP.
.H 2 "Initializing the Xt Intrinsics"
.iX "Initializing Xt Intrinsics"
.iX "Xt Intrinsics" "initializing"
.mc |
You must initialize the XtIntrinsics before making any other
.mc
calls to XtIntrinsics functions.
.mc |
.iX "XtAppInitialize"
.mc
.iX "Shell"
.mc |
The function \&\fBXtAppInitialize\fP creates an application context,
establishes the connection to the
.mc
display server, parses the command line that invoked the application,
.mc |
loads the resource database, and creates a shell widget to serve
.mc
as the parent of your application widgets.
.P
.mc |
.iX "XtAppInitialize"
.mc
By passing the command line that invoked your application to
.mc |
\&\fBXtAppInitialize\fP, the function can parse the line to allow users
.mc
to specify certain resources (such as fonts and colors) for your
.mc |
application at run time.  \&\fBXtAppInitialize\fP scans the command line
.mc
and removes those options.  The rest of your application sees only the
remaining options.
.P
.mc |
The call to \&\fBXtAppInitialize\fP used by the sample program
\&\fBxmbutton\fP is as follows:
.mc
.P
.ft CW
.mc |
.ta .5i 1i 1.5i
.mc
.nf
.mc |
toplevel = XtAppInitialize(NULL, "XMdemos", NULL, 
	0, &argc, argv, NULL, NULL, 0);
.mc
.ft R
.fi
.mc |
.ta .5i 3i
.mc
.P
.mc |
This line names the application class
\fBXMdemos\fP, passes no additional options, and passes the command line that
.mc
invoked the application.
.mc |
The first parameters is used in setting up defaults files.
Defaults files are explained in Section 3.8, ``Creating Defaults Files.''
.mc
.P
.mc |
.iX "XtAppInitialize"
The syntax of the \&\fBXtAppInitialize\fP function follows.  Note that
it returns a value of type Widget; therefore, the variable \&\fItoplevel\fP
.mc
in \&\fBxmbutton\fP must be defined as type Widget.
.P
.\" includes syntax for call
.ds xL Programming With Xlib
.mc |
.iX "XtAppInitialize" "Defined"
\fBWidget XtAppInitialize\fP(\fIapp_context_return, application_class,
options, num_options, argc_in_out, argv_in_out, fallback_resources,
args, num_args\fP)
.mc
.br
.mc |
.ta 0.5i 2i
	\fBXtAppContext\fP	*\fIapp_context_return\fP;
.mc
.br
.mc |
	\fBString\fP	\fIapplication_class\fP;
.mc
.br
.mc |
	\fBXrmOptionDescList\fP	\fIoptions\fP;
.mc
.br
.mc |
	\fBCardinal\fP	\fInum_options\fP;
.mc
.br
.mc |
	\fBCardinal\fP	*\fIargc_in_out\fP;
.mc
.br
.mc |
	\fBString\fP	*\fIargv_in_out\fP;
.br
	\fBString\fP	*\fIfallback_resources\fP;
.br
	\fBArgList\fP	\fIargs\fP;
.br
	\fBCardinal\fP	\fInum_args\fP;
.br
.VL 1.5i
.LI \fIapp_context_return\fP
Specifies a pointer to the application context.
If this is not NULL, \fBXtAppInitialize\fP returns the application
context created by \fBXtCreateApplicationContext\fP.
.mc
.LI \fIapplication_class\fP
.mc |
.br
Specifies the class name of this application, which is usually the
.mc
generic name for all instances of this application.  By convention,
the class name is formed by reversing the case of the application's
first letter. The class name is used to locate the files used to
initialize the resource database. For example, the sample program
.mc |
\fBxmbutton\fP has a class name of \fBXMdemos\fP.
.mc
.LI \fIoptions\fP
Specifies how to parse the command line for any application-specific resources.
The options argument is passed as a parameter to
\fBXrmParseCommand\fP.
.PN XrmParseCommand .
.mc *
.mc
.LI \fInum_options\fP
.mc |
Specifies the number of entries in \fIoptions\fP.
.LI \fIargc_in_out\fP
.mc
Specifies a pointer to the number of command line parameters.
.ne 3
.mc |
.LI \fIargv_in_out\fP
.mc
Specifies the command line parameters.
.mc |
.LI \fIfallback_resources\fP
Specifies resources to be used if the application class resource file
cannot be read.
.LI \fIargs\fP
Specifies resource values to override any other resource specifications
for the shell widget created.
.LI \fInum_args\fP
Specifies the number of entries in \fIargs\fP.
.mc
.LE
.P
.iX "XtToolkitInitialize"
.mc |
You can use the alternate function, \fBXtToolkitInitialize\fP, to just initialize the 
Xt Intrinsics.  It is not as
convenient as \fBXtAppInitialize\fP, but it is more flexible because it
lets you decide the type of shell you want to use.  
.mc
.iX "XtOpenDisplay"
.iX "XtAppCreateShell"
It does not open the display or create an
application shell.  You must do this yourself using \fBXtOpenDisplay\fP
.mc |
and \fBXtAppCreateShell\fP.  The advanced sample program presented in Section
3.12 initializes the toolkit in this manner.
.mc
.H 2 "Creating Argument Lists for Widgets"
.iX "Argument lists for widgets"
.iX "Widgets" "argument lists"
.mc |
The steps in Sections 3.4 through 3.7 must be
.mc
performed for each widget you wish to create.
.P
Widgets accept argument lists (pairs of resource names and values)
that control their appearance and functionality.
The list of resources acceptable for a widget comprises not only resources
unique to the widget, but also those resources
inherited from other widgets.  The
.mc |
resources for a given widget are shown in the manpage for the
.mc
widget.
.P
.iX "XtSetArg"
The simplest way to set an element of an argument list is
by using the \&\fBXtSetArg\fP macro.
.mc |
Other methods are described in Section 3.10,
.mc
``Advanced Programming Techniques.''
.P
.mc |
The following program segment declares an array \&\fBargs\fP of up
.mc
to 10 arguments.  The size of the array is not important just so
long as the number of elements allocated is not less than the
number of elements used.  The first argument specifies the label for the
PushButton.  The label is actually a pointer to a compound string that
was created by a call to \fBXmStringCreateLtoR\fP earlier in the program.
.mc |
See Chapter 8 for more information about compound strings.  The
.mc
last two arguments specify that the widget will have a width of 250 pixels 
and a height of 150 pixels.
The third argument specifies the string to display in the PushButton.
.P
.ft CW
.nf
Arg args[10];
XtSetArg(args[0], XmNlabelString, btn_text);
XtSetArg(args[1], XmNwidth, 250);
XtSetArg(args[2], XmNheight, 150);
.fi
.ft R
.P
.iX "XtSetArg"
.mc |
An alternate method for \&\fBXtSetArg\fP uses a counter, \&\fIn\fP,
.mc
rather than a
hard-coded index.
.mc |
This method, shown as follows, makes it easier to add and delete
.mc
argument assignments. 
It is the method used in the sample program \&\fBxmbutton\fP.
.P
.nf
.ft CW
Arg args[10];
Cardinal n=0;
XtSetArg(args[0], XmNlabelString, btn_text); n++;
XtSetArg(args[n], XmNwidth, 250); n++;
XtSetArg(args[n], XmNheight, 150); n++;
.ft R
.fi
.P
.mc |
The variable \&\fIn\fP contains the number of resources set.
It can be passed to the widget create function (explained in Section 3.6) as the
.mc
argument list count.
.P
.mc |
\fBCAUTION\fP: Do not increment the counter from inside the call to \&\fBXtSetArg\fP.
.mc
As currently implemented, \&\fBXtSetArg\fP is a macro that
dereferences the first argument twice.  This means that if you 
increment the counter from inside the call, it would actually
be incremented twice for the one call.
.mc |
.br
.ne 9
.mc
.P
The syntax for using \&\fBXtSetArg\fP is as follows:
.P
.mc *
.mc
.nf
XtSetArg(\fIarg\fP, \fIname\fP, \fIvalue\fP)
.br
.mc |
  \fBArg\fP      \fIarg\fP;
.mc
.br
.mc |
  \fBString\fP   \fIname\fP;
.mc
.br
.mc |
  \fBXtArgVal\fP \fIvalue\fP;
.mc
.P
.VL 1i
.ne 3
.LI \fIarg\fP
Specifies the name-value pair to set.
.ne 3
.LI \fIname\fP
Specifies the name of the resource.
.ne 3
.LI \fIvalue\fP
Specifies the value of the resource if it will fit in an 
\&\fBXtArgVal\fP, 
otherwise the address.
.LE
.ne 2
.H 2 "Adding Callback Procedures"
.iX "Callback" "adding"
.iX "Adding callbacks"
Callbacks are one of the key features of the Motif
.mc |
Widget set.
.mc
They allow you to write procedures
that will be executed when certain events occur within a widget.
These events include mouse button presses, keyboard selections,
and cursor movements.
Callback procedures are the main mechanism your application uses to
actually get things done.
.P
You need to complete three steps to add callbacks:
.AL
.LI
Write the callback procedures.
.LI
.iX "Callback" "list"
.mc |
Add the appropriate callbacks.
.mc
.LI
.mc |
Set the widget's callback resources.
.mc
.LE
.P
Each of these steps is described in the following sections.
.H 3 "Writing a Callback Procedure"
.iX "Callback" "writing procedure"
Callback procedures return no values, but have three arguments:
.BL
.LI
The widget for which the callback is registered.
.LI
Data passed to the callback procedure by the application.
.LI
Data passed to the callback procedure by the widget.
.LE
.mc |
.br
.ne 6
.mc
.P
In the sample program \&\fBxmbutton\fP, the callback procedure prints
a message to the standard output device (usually the
terminal window from which the application was invoked), frees the
memory space used by \fBbtn_text\fP (the PushButton label), and
ends the program using the system \&\fBexit\fP call.
.P
.ft CW
.nf
void activateCB(w, client_data, call_data)
.mc |
 Widget    w;           /*widget id*/
 caddr_t   client_data; /*data from application*/
 caddr_t   call_data;   /*data from widget class*/
.mc
{
.mc |
/*print message and terminate program*/
.mc
 fprint("PushButton selected.\\n")
.mc |
 XmStringFree(btn_text);
.mc
 exit(0);
}
.fi
.ft R
.P
.mc |
The variable type \&\fIcaddr_t\fP is defined by the Xt
.mc
Intrinsics as a pointer to an area of memory.
The \&\fBcall_data\fP argument is used only by a few widgets.
.mc |
The manpage for each widget specifies whether it passes any data
.mc
to its callbacks.  
.P
The general syntax of a callback procedure is described below:
.iX "XtCallbackProc" 
.P
.nf
.mc |
\fBvoid\fP \fICallbackProc\fP(\fIw\fP, \fIclient_data\fP, \fIcall_data\fP)
.mc
.br
.mc |
      \fBWidget\fP  \fIw\fP;
.mc
.br
.mc |
      \fBcaddr_t\fP \fIclient_data\fP;
.mc
.br
.mc |
      \fBcaddr_t\fP \fIcall_data\fP;
.mc
.fi
.mc |
.VL 1.5i
.mc
.ne 3
.LI \fIw\fP
Specifies the widget for which this callback is invoked.
.mc *
.mc
.LI \fIclient_data\fP
Specifies the data that the widget should pass back to the client when
the widget invokes the client's callback.
This is a way for the
client registering the callback to also define client-specific data
to be passed to the client:
a pointer to additional information about the widget, a reason for invoking
the callback, and so on.
.mc |
It is perfectly normal to have \fIclient_data\fP be NULL 
.mc
if all necessary information is in the widget.
.ne 3
.LI \fIcall_data\fP
Specifies any callback-specific data the widget wants to pass to the client.
It is widget-specific and is usually set to NULL.  It will be defined in
.mc |
the widget's manpage if it is used.
.mc
.LE
.P
.H 3 "Adding Callbacks"
.iX "Callback" "adding"
.iX "Adding callbacks"
A callback contains information about the callback routine
associated with a particular user action.
.P
.iX "XtAddCallback"
The sample program \&\fBxmbutton\fP creates a callback by calling
the procedure \fBXtAddCallback\fP. 
.P
.ft CW
XtAddCallback (button, XmNactivate, activateCB, NULL);
.ft R
.P
.mc |
The general syntax of \fBXtAddCallback\fP is as follows:
.mc
.P
.iX "XtAddCallback" "Defined"
.mc |
\fBvoid XtAddCallback\fP(\fIw\fP, \fIcallback_name, \fP\fIcallback\fP,
.mc
\fIclient_data\fP)
.br
.mc |
      \fBWidget\fP         \fIw\fP;
.mc
.br
.mc |
      \fBString\fP         \fIcallback_name\fP;
.mc
.br
.mc |
      \fBXtCallbackProc\fP \fIcallback\fP;
.mc
.br
.mc |
      \fBcaddr_t\fP        \fIclient_data\fP;
.VL 1.5i
.mc
.ne 3
.LI \fIw\fP
Specifies the widget to add the callback to.
.ne 3
.LI \fIcallback_name\fP
.mc |
.br
.mc
Specifies the callback list within the widget to append to.
.ne 3
.LI \fIcallback\fP
Specifies the callback procedure to add.
.ne 3
.LI \fIclient_data\fP
Specifies the client data to be passed to the callback when it is invoked by
\fBXtCallCallbacks\fP.
.mc |
The \fIclient_data\fP parameter is often NULL.
.mc
.sp
.LE
To add more callbacks, just make another call to \fBXtAddCallback\fP.
In this way you can cause a user event to trigger many callback routines.
.P
.iX "XtAddCallbacks"
.iX "Callback" "list"
.mc |
You can add a list of callbacks by using the function
.mc
\fBXtAddCallbacks\fP.
.P
.mc |
The general syntax of \fBXtAddCallbacks\fP is as follows:
.mc
.P
.iX "XtAddCallbacks" "Defined"
.mc |
\fBvoid XtAddCallbacks\fP(\fIw\fP, \fIcallback_name, \fP\fIcallbacks\fP)
.mc
.br
.mc |
      \fBWidget\fP         \fIw\fP;
.mc
.br
.mc |
      \fBString\fP         \fIcallback_name\fP;
.mc
.br
.mc |
      \fBXtCallbackList\fP \fIcallbacks\fP;
.mc
.VL 1i
.ne 3
.LI \fIw\fP
Specifies the widget to add the callbacks to.
.mc |
.br
.ne 5
.mc
.LI \fIcallback_name\fP
.mc |
.br
.mc
Specifies the callback list within the widget to append to.
.ne 3
.LI \fIcallbacks\fP
Specifies the null-terminated list of callback procedures and corresponding
client data to add.
.sp
.LE
.H 3 "Setting Widgets' Callback Resources"
.iX "Callback" "resources"
.iX "Resources" "callback"
Many widgets define one or more callback resources.
Set the value of the resource to the name of the callback list.
.P
The callback resources for any particular widget are listed in the man
page for that widget.
.mc |
The Pushbutton widget used in
.mc
the sample program \&\fBxmbutton\fP supports three different kinds of callbacks.
Each callback could be set up by specifying the callback list as
the value of the
appropriate resource.
.BL
.LI
.mc |
Callback(s) invoked when the Pushbutton widget is activated (argument
.mc
\&\fBXmNactivateCallback\fP).
This is the callback you use in \&\fBxmbutton\fP.
.LI
.mc |
Callback(s) invoked when the Pushbutton widget is armed
.mc
(argument \&\fBXmNarmCallback\fP).
.LI
.mc |
Callback(s) invoked when the Pushbutton widget is disarmed (argument
.mc
\&\fBXmNdisarmCallback\fP).
.LE
.P
.mc |
The translation table for this widget has been set so that an
.mc
activate action occurs whenever
the pointer is within the widget and the user presses mouse
button 1.
An activate action then causes the widget to invoke each of the
callback routines on the callback list pointed to by its
\&\fBXmNactivateCallback\fP argument.
These routines are invoked in the order in which they appear in the
callback list. 
In the case of the sample program \&\fBxmbutton\fP, only the routine
\&\fBactivateCB\fP is executed.
.\"
.br
.ne 2i
.H 2 "Creating the Widget"
.iX "Widget" "create"
.iX "Create" "widget"
Now that you have established an argument list for the widget, you
can create the widget instance.
.iX "XtCreateManagedWidget"
.mc |
The following call to \&\fBXtCreateManagedWidget\fP comes from
.mc
the sample program \&\fBxmbutton\fP.
.nf
.mc |
.ta .5i 1i 1.5i
.mc
.P
.mc |
\f(CWbutton = XtCreateManagedWidget ("button", 
	xmpushButtonWidgetClass, toplevel, args, n);\fP
.ta .5i 3i
.mc
.fi
.P
This call names the newly created widget ``button'' and defines it
.mc |
to be a Pushbutton widget (from the class
.mc
\&\fBxmPushButtonWidgetClass\fP).
.mc |
The class name \fBXmPushButton\fP or
the name button can be used in defaults files (discussed in
Section 3.9) to refer to this widget.
The Pushbutton's parent is ``toplevel,'' the toplevel shell widget returned by
\&\fBXtAppInitialize\fP.
.mc
The argument list and number of arguments complete the call.
This call will create the widget and notify its parent so that the parent
can control its specific layout.
.P
There is another way to create widgets, one that does not automatically
manage them.  Instead, you manage them when you want them to be displayed.
.mc |
Each widget has a create function associated with it.  A create function
creates the widget it is associated with but does not manage it.
.mc
.iX "XtManageChild"
You manage the widget with \fBXtManageChild\fP.  The advanced program in
.mc |
Section 3.12 uses this method of creating widgets.
.mc
.P
.iX "Widget" "tree"
Widgets form a hierarchical structure called a widget tree.  The widget
.mc |
tree for the program \fBxmbutton\fP is shown in Figure 3-1.
.iX "XtAppInitialize"
The widget returned by \&\fBXtAppInitialize\fP is the invisible
.mc
parent for the toplevel application widget, in this case
\&\fBbutton\fP.
Usually there are several levels of widgets.
Widgets at the higher levels are layout widgets (also
called manager widgets) that control and coordinate the primitive
widgets located at the leaves of the widget tree.
The more advanced sample program later in this chapter illustrates multiple
levels of widgets.
.iX "XtCreateManagedWidget"
.P
.mc |
The syntax for \&\fBXtCreateManagedWidget\fP is as follows.
.mc
.P
.mc |
\fBWidget\fP XtCreateManagedWidget(\fIname\fP, \fIwidget_class\fP, \fIparent\fP,
.mc
\fIargs\fP, \fInum_args\fP)
.br
.mc |
      \fBString\fP     \fIname\fP;
.mc
.br
.mc |
      \fBWidgetClass\fP \fIwidget_class\fP;
.mc
.br
.mc |
      \fBWidget\fP      \fIparent\fP;
.mc
.br
.mc |
      \fBArgList\fP     \fIargs\fP;
.mc
.br
.mc |
      \fBCardinal\fP    \fInum_args\fP;
.mc
.VL 1i
.ne 3
.LI \fIname\fP
Specifies the resource name for the created widget.
This name is used for retrieving resources
and should not be the same as any other widget
that is a child of the same parent if unique values are necessary.
.ne 3
.LI \fIwidget_class\fP
Specifies the widget class pointer for the created widget.
.ne 3
.LI \fIparent\fP
Specifies the parent widget.
.ne 3
.LI \fIargs\fP
Specifies the argument list to override the resource defaults.
.ne 3
.LI \fInum_args\fP
Specifies the number of arguments in \fIargs\fP.
The number of arguments in an argument list can be automatically
computed by using the \fBXtNumber\fP
macro if the list is statically defined.
.iX  "XtNumber"
.LE
.H 2 "Making the Widget Visible"
.iX "Widget" "making visible"
All widgets are now created and linked together into
a widget tree.
.P
.iX "XtRealizeWidget"
\&\fBXtRealizeWidget\fP displays on the screen the widget that is passed to
it and the children of that widget.
.P
The final step in the program is to call the Xt Intrinsics routine that
causes the application to enter a loop, awaiting action by the user.
.P
.mc |
Sample code for this section is as follows:
.iX "XtAppMainLoop"
.mc
.P
.ft CW
.nf
XtRealizeWidget(toplevel);
.mc |
XtAppMainLoop(app_context);
.mc
.ft R
.fi
.P
.mc |
The preceding two statements from the sample program \&\fBxmbutton\fP
display the Pushbutton 
.mc
widget and cause the program to enter a loop, waiting for user input.
The main role of your application is the setting of widget
arguments and the writing of callback procedures.
Your application passes control to the Xt Intrinsics and the 
.mc |
Motif Widgets once the \&\fBXtAppMainLoop\fP function is called.
.br
.ne 7
.mc
.P
The syntax for \&\fBXtRealizeWidget\fP is shown below.
.P
.PN XtRealizeWidget .
.iX "XtRealizeWidget" "Defined"
.mc |
\fBvoid XtRealizeWidget\fP(\fIw\fP)
.mc
.br
.mc |
      \fBWidget\fP \fIw\fP;
.mc
.VL 1i
.mc *
.mc
.LI \fIw\fP
Specifies the widget.
.LE
.H 2 "Linking Libraries"
.iX "Linking libraries"
.mc |
When linking the program, be sure to include three libraries, in
the following order:
.mc
.BL
.LI
.mc |
\&\fBlibXm.a\fP, which contains the Motif Widgets
.mc
.LI
.mc |
\&\fBlibXt.a\fP, which contains the Xt Intrinsics
.mc
.LI
.mc |
\&\fBlibX11.a\fP, which contains the underlying Xlib library
.mc
.LE
.P
.mc |
\fBNOTE\fP:
The \fBXmFileSelectionBox\fP widget requires the \fBlibPW.a\fP library.  This
.mc
can be included after \fBlibX11.a\fP.
.P
.mc |
See Section 1.3 for information about compiling the
.mc
programs in this chapter.  The order in which you place the
.mc |
libraries is very important.  The preceding order shown is correct,
.mc
so be sure that you use the same order when linking in libraries.
.H 2 "Creating Defaults Files"
.iX "Defaults files"
Up to now, all widget resources have been set by the application
using widget argument lists.
.iX "Resources" "specifying"
An additional method for specifying resources is through a set of
ASCII files that you can set up for
your user.
You may also want your user to set up these files to customize
the application to individual requirements or preferences.
.P
When writing a program, consider the following factors
in deciding whether to specify an argument in a defaults file or
in the program itself.
.BL
.LI
Using a defaults file provides additional flexibility.
Any user can override settings to reflect 
personal preferences, and a systems administrator can modify the
application defaults file for system-wide customization.
.LI
Specifying settings in the program gives the programmer greater
control.
They cannot be overridden.
.LI
Using defaults files can speed application development.
To change a resource value in a defaults file, simply edit the file
(using any ASCII editor) and rerun the program.
No recompilation or relinking is necessary.
.LI
Using defaults files can simplify your program.
Resources in defaults files are specified as strings.
When resources are set in your program, they may have to be in
some internal format that takes several calls to compute.
.LI
.iX "Efficient operation"
Specifying options in your program may provide more efficient
operation for the
computer.
The process of reading defaults files
and interpreting their contents adds processing overhead.
.LE
.P
.mc |
Several files can be used for customization:
.mc
.BL
.LI
.iX "Defaults files" "app-defaults"
.mc |
An application-specific file containing system-wide resource defaults
for an entire class.
This file is usually located in the 
\&\fB/usr/lib/X11/app-defaults\fP directory, but the location can vary
depending on the user's current language environment.
.mc
.LI
.mc |
An application-specific file containing the user's resource defaults for
an entire class.
.LI
.iX "Defaults files" ".Xdefaults"
A file (called \fB\&.Xdefaults\fP) in the user's home directory
containing the user's default values for any application.
.LI
An environment-specific file containing the user's default values for
applications running on a particular host.
This is usually the file \fB\&.Xdefaults-\fP\fIhost\fP in the user's home
directory.
.mc
.LE
.P
All files are of the same format.
.mc |
Section ***II.4*** of this manual, ``Managing Windows With MWM,'' 
contains a detailed discussion of the format of defaults files.
.P
Defaults files are generally read by the Xt Intrinsics when an
application is initialized.
For a discussion of the order in which the files are loaded, the
location of the files, and the effects of environment variables and the
current language environment, see the ``Localization'' section in
Chapter 8.
.mc
.H 3 "Application Defaults Files"
.iX "Defaults files" "application"
.iX "Application defaults files"
.iX "Defaults files" "app-defaults"
These files are designed to be created by the applications
developer or systems administrator.
.mc |
They are usually located in the 
\&\fB/usr/lib/X11/app-defaults\fP directory on the machine where the
application resides, though there may be a different directory for each
of several language environments.
.mc
Application programs specify the file that contains the
.mc |
application defaults when they call \&\fBXtAppInitialize\fP.
The \fIapplication_class\fP argument to that function specifies
.mc
the name of the application defaults file.
Several applications can point to the same file.
.mc |
.br
.ne 6
.mc
.P
.mc |
The following call (taken from the sample program \&\fBxmbutton\fP)
causes the Xt Intrinsics to
look for an application-specific file (\fBXMdemos\fP in this case) for default
.mc
information.
.P
.nf
.mc |
.ta .5i 1i 1.5i
.mc
.ft CW
.mc |
toplevel = XtAppInitialize(NULL, "XMdemos", NULL, 
	0, &argc, argv, NULL, NULL, 0);
.mc
.ft R
.mc |
.ta .5i 3i
.mc
.fi
.P
The following sample defaults file sets the foreground color to white
and background color to black.
.P
.nf
.ft CW
*background: black
*foreground: white
.ft R
.fi
.H 3 "User Defaults Files"
.iX "Defaults files" "user"
.iX "Defaults files" "Xdefaults"
.mc |
Each user can create several files to specify resource defaults:
.BL
.LI
An application-specific file for an entire class of applications.
This file is often in the user's home directory, though there may be a
different directory for each of several language environments.
.LI
A \fB\&.Xdefaults\fP file in the user's home directory to specify resource
defaults for any application.
.LI
An environment-specific file to specify resource defaults for
applications running on a particular host.
.LI
.P
.mc
User defaults override application and system defaults and allow
different users running the same program to specify personal
display preferences, such as color and font selection.
.P
.mc |
The following sample file changes the background color to blue.
.mc
.P
.ft CW
.br
*background:  blue
.ft R
.H 3 "Defaults File Example"
.iX "Defaults file" "example"
.mc |
Here is an example to illustrate the interaction of the defaults
.mc
files with each other and with arguments specified in programs.
.mc *
.mc
Suppose a computer contains the program \&\fBxmbutton\fP as well as the
.mc |
application and user defaults files previously described.
.br
.ne 10
.mc
.P
To determine the color of the background,
the Xt Intrinsics will do the following:
.AL
.LI
Look for the system defaults and initialize the
.mc |
background color to white. 
.mc
(These defaults are compiled into the widgets.)
.LI
.mc |
Look for the application-specific class defaults file
and set the color to black.
.br
.ne 5
.mc
.LI
.mc |
Look for the user's application-class, general, and environment-specific
defaults files.
In this example, the Xt Intrinsics load the user's
\fB\&.Xdefaults\fP file and set the background color to blue.
.mc
.LI
.mc |
Look for any resource arguments on the command line that started the
application.
If the command line sets the background resource
(\&\fBXmNbackground\fP), this overrides any defaults that have been
set by resource files.
.mc
.LE
.H 2 "Using Color"
.iX "Color"
The Motif Widgets have been designed to support both color and monochrome
systems in a consistent and attractive manner.  This is
accomplished by incorporating into each widget a variety of visual attributes.  
.iX "Visual attributes"
.mc |
.iX "3-dimensional appearance"
.mc
Through proper use of these attributes, the
.mc |
widgets will present a dramatic 3-dimensional appearance, giving
you the impression that you are directly manipulating the components.
This section describes these color attributes and shows you
.mc
how to use them.
.mc |
.P 
.br
.ne 9
.mc
.H 3 "Visual Capabilities and Attributes"
.iX "Border drawing"
The Motif Widgets visual capabilities are based on specialized border
and background drawing.  The border drawing consists of a
band around the widget.  The band contains two regions:
.iX "Shadows"
.BL
.LI
.mc |
The top and left shadow
.mc
.LI
.mc |
The bottom and right shadow
.mc
.LE
.mc |
.br
.ne 15
.mc
.P
The background
drawing within the widget is referred to as background.  
.mc |
The following figure illustrates the drawing areas.
.mc
.iX "Widget" "drawing areas"
.mc |
.sp
.ne 15
.FG "Widget Drawing Areas"
.if n .ig +n
.mc
.DS CB
.mc |
.in 1i
.mc
.ps 8
.PS
arrowhead = 7
boxwid = 3.0
boxht = 2.0
linewid = .75
lineht = .1
Box1: box 
move to Box1.n
move down .25
boxwid = 2.5
boxht = 1.5
Box2: box 
line from Box2.ne to Box1.ne
line from Box2.sw to Box1.sw
move to Box1.ne
move down .125
move right .25
A1: arrow left 1.00
"Top and Left Shadow" at A1.start ljust
move to Box1.e
move right .25
A2: arrow left 1.00
"Background" at A2.start ljust
move to Box1.se
move up .125
move right .25
A3: arrow left 1.00
"Bottom and Right Shadow" at A3.start ljust
.mc |
.in
.mc
.PE
.ps
.DE
.mc |
.+n
.mc
.P
Each area can be drawn from either a color or a pixmap. The top and left
shadow is drawn using these Primitive widget resources:
.BL
.LI
\&\fBXmNtopShadowColor\fP
.LI
\&\fBXmNtopShadowPixmap\fP
.LE
.P
The background is drawn using these Primitive widget resources:
.BL
.LI
\&\fBXmNbackground\fP
.LI
\&\fBXmNbackgroundPixmap\fP
.LE
.P
The
.mc |
bottom and right shadow is drawn using these Primitive widget resources:
.mc
.BL
.LI
\&\fBXmNbottomShadowColor\fP
.LI
\&\fBXmNbottomShadowPixmap\fP
.LE
.P
All the widgets support the visual attributes for setting the background
.mc |
as described.  In general, only Primitive widgets support the border drawing.
.mc
To use the border drawing for manager widgets, a special manager widget,
\&\fBXmFrame\fP, is available.  This widget will maintain the geometry of a
single child and perform the border and background drawing.
.mc |
.br
.ne 10
.mc
.H 3 "Using the Capabilities"
.iX "Three-dimensional appearance"
.mc |
When planning the 3-dimensional appearance of your program's
.mc
windows, consider the following guidelines:
.BL
.LI
Any selectable area should appear to be raised.
.LI
.mc |
Nonselectable
.mc
areas should appear to be flat.  This can be accomplished by setting 
\fBXmNshadowThickness\fP to 0.
.LE
.P
To give the impression that the widget is raised above its
.mc |
parent, set these resources as follows:
.mc
.BL
.LI
Set \&\fBXmNtopShadowColor\fP to a light color.
.LI
Set \&\fBXmNbackground\fP to a medium color.
.LI
Set the \&\fBXmNbottomShadowColor\fP to a dark color.
.LE
.P
.iX "Color Defaults"
The foreground, background, and both top and bottom shadow resources are
dynamically defaulted.  This means that if you do not specify any color
for these resources, colors are automatically generated.  A black and 
white color scheme is generated on a monochrome system, while on a color
system a set of four colors is generated that displays the correct
.mc |
shading to achieve the 3-dimensional appearance.  If you specify only a
.mc
background color, the foreground and both shadow colors are generated 
.mc |
(based on the background color) to achieve the 3-dimensional
appearance.  This color generation works best with nonsaturated colors,
.mc
that is, using pure red, green, or blue will give poor results.  Also,
.mc |
colors are generated only at the time of the widget's creation.  Changing
the background color by using \fBXtSetValues\fP will not cause the
.mc
other colors to be regenerated.
.mc *
.mc
.P
Reversing the top shadow and bottom shadow colors will give the
appearance that the widget is set into its parent.  
.mc |
Several of the Primitive
.mc
widgets (buttons, toggles, and arrows, for example) automatically reverse
their shadowing
when selected to achieve the effect of being pressed.  They return to
their original shadowing when released.
.P
.mc |
.iX "Coordinated colors for 3-dimensional look"
.mc
Use coordinated
colors such as light blue for the top shadow color, sky blue for the
background color, and navy blue for the bottom shadow color to
.mc |
enhance the 3-dimensional appearance.  
.mc
Using dissimilar colors loses the effect.
.P
.mc |
The 3-dimensional appearance is more difficult to achieve on monochrome
.mc
systems.  The built-in defaults for all the widgets have been set up
for monochrome systems and provide the desired effect.  The top shadow
is drawn with a 50 percent pixmap, the background is solid white, and the
bottom shadow is solid black.  This appearance can be further enhanced
by setting the background of a manager containing a set of raised
children to a pixmap of 25 percent black and 75 percent white.
.H 2 "Advanced Programming Techniques"
.iX "Advanced programming techniques"
.iX "Programming, advanced"
The sample program \&\fBxmbutton\fP described in earlier sections of
this chapter illustrated the writing of a very simple widget program.
The Xt Intrinsics provide additional mechanisms for programmers.
.H 3 "Setting Argument Values"
.iX "Setting argument values"
.iX "Argument values, setting"
Section 3.3 described the use of \&\fBXtSetArg\fP for setting the values of
widget arguments.
This section describes three additional methods.
The code segments show how the earlier sample program could have been
rewritten to use the new methods.
.H 4 "Assigning Argument Values"
Each element of the type \&\fBArg\fP structure can be assigned
individually.
.P
.nf
.mc |
.ta .5i 1i 1.5i
.mc
\f(CW
XmString btn_text;
Arg args[10];

.mc |
btn_text = XmStringCreateLtoR ("Push Here", 
	XmSTRING_DEFAULT_CHARSET);
.mc
args[0].name = XmNwidth;
args[0].value = (XtArgVal) 250;
args[1].name = XmNheight;
args[1].value = (XtArgVal) 150;
args[2].name = XmNlabelString;
args[2].value = (XtArgVal) btn_text;
\fP
.mc |
.ta .5i 3i
.mc
.fi
.P
.iX "XtArgVal"
Be sure to keep name\(hyvalue pairs synchronized.
Note that all argument values have been cast to type
\&\fBXtArgVal\fP.
.H 4 "Static Initializing"
Initializing argument lists at compile time makes it easy to add and delete
argument settings in your program.
It avoids the need to hard-code the maximum number of arguments
when declaring your argument list.
These settings are frozen at compile time, however.
While the example below shows only a single argument list being
created, you can create any number of lists (be sure to declare each list
as type \&\fBArg\fP).  Note that you cannot use static initializing to
initialize a compound string.  You can combine static initializing with
run-time assignments to accomplish this, as shown in the next section.
.P
.nf
\f(CW
static Arg args[] = {
 {XmNwidth, (XtArgVal) 250},
 {XmNheight, (XtArgVal) 150},
};
\fP
.fi
.P
.mc |
Note that the values of each argument have been cast to 
.mc
type \&\fBXtArgVal\fP.
When the create widget function is invoked, passing it
\&\fBXtNumber(\&\fBargs\fP)\fR will compute the number of
elements in the argument list.
.P
.nf
.mc |
.ta .5i 1i 1.5i
\f(CWbutton = XtCreateManagedWidget("button", 
	xmPushButtonWidgetClass, toplevel, args, 
	XtNumber(args));\fP
.mc
.fi
.mc |
.ta .5i 3i
.mc
.P
.mc *
.mc
.iX "XtNumber"
.mc |
\fBNOTE:\fP Use the macro \&\fBXtNumber\fP only if you are declaring
the argument list of indefinite size as shown previously (\&\fBargs[]\fP).
.mc
\&\fBXtNumber\fP will return the number of elements that have
actually been allocated in program memory.
.H 4 "Combining Static Initialization with Run-Time Assignments"
The final method for creating argument lists initializes a list
.mc |
at compile time (described previously in ``Static Initializing'') and then
.mc
modifies the values of the settings using regular assignment statements.
The \&\fBXtNumber\fP macro can be used to count the number of
arguments, since the argument list is declared with no definite
number of arguments.
The values can be changed through assignments at run time, but the size
of the argument list (the number of arguments that can be specified) is frozen
at compile time and cannot be extended.
.P
.mc |
The following example initializes an argument list of three elements.
.mc
The last is initialized to \&\fBNULL\fP so it can be given a
value later.
The value for argument \&\fBXmNheight\fP is changed in the program
from its initialized value of 150 to a run-time value of 250.
.P
.nf
.mc |
.ta .5i 1i 1.5i
.mc
.ft CW
XmString btn_text;

static Arg args[] = {
 {XmNwidth, (XtArgVal) 500}, /* item 0 */
 {XmNheight, (XtArgVal) 150}, /* item 1 */
 {XmNlabelString, (XtArgVal) NULL}, /* item 2 */
};

.mc |
btn_text = XmStringCreateLtoR ("Push Here", 
	XmSTRING_DEFAULT_CHARSET);
.mc

args[1].value = (XtArgVal) 250;
args[2].value = (XtArgVal) btn_string;
.mc |
.ta .5i 3i
.mc
.fi
.ft R
.H 3 "Manipulating Created Widgets"
Widget programs to this point have set up argument lists and callbacks for
widgets prior to the widgets' creation.
You can also modify widgets after they have been created.
Such modification usually occurs in callback routines and is
illustrated in the sample program \&\fBxmfonts\fP discussed later
in this chapter.
.H 4 "Retrieving and Modifying Arguments"
.iX "Retrieving arguments"
.iX "Modifying arguments"
.iX "Arguments, retrieving and modifying"
.iX "XtGetValues"
.iX "XtSetValues"
\&\fBXtGetValues\fP will return the current value of specified arguments for
a created widget.
\&\fBXtSetValues\fP will change the value of specified arguments.
.H 4 "Adding Callbacks and Translations"
.iX "Callback" "add"
.iX "XtAddCallback"
\&\fBXtAddCallback\fP will
add a callback routine to a widget's callback list after the
widget has been created.
.P
Each widget has a translation table that ties user actions
(for example, button presses and keyboard presses) to widget actions.
Your application can modify the translation table for any widget.
This process is described in any manual on the Xt Intrinsics.
.H 4 "Separating Widget Creation and Management"
.iX "XtCreateManagedWidget"
By using \&\fBXtCreateManagedWidget\fP, the sample program
automatically adds the newly
created widget to its parent's set of managed children.
.iX "XtCreateWidget"
.iX "XtManageChildren"
To optimize programs that add a number of widgets to a single parent, you
may want to create the widgets using \&\fBXtCreateWidget\fP calls and then
add the entire list of children to its parent with a single
\&\fBXtManageChildren\fP call.
In this way, the parent widget performs its geometry processing of its
children only once.  This will increase the performance of applications
that have a large number of child widgets under a single parent.
.P
Usually, the function \&\fBXtRealizeWidget\fP will display a widget and all
of its children.
Using the function \&\fBXtSetMappedWhenManaged\fP allows you to turn off
automatic mapping (displaying) of particular widgets.
Your application can then use \&\fBXtMapWidget\fP to display the widget.
.P
The function \&\fBXtDestroyWidget\fP will destroy a created widget and its
children.
The destroyed widget is automatically removed from its parent's
list of children.
.H 2 "An Advanced Sample Program"
.iX "Advanced program, font selection"
.iX "Sample program" "font selection"
.iX "xmfonts"
The program presented in this section, \fBxmfonts\fP, displays each
.mc |
available font as a Push button.  
.mc
The source code and the application defaults file for this sample program
are listed later in this section.
They are located on your system
.mc |
in \fB./demos/xmsamplers/xmfonts.c\fP
and \fB\&./demos/xmsamplers/XMdemos.ad\fP.
.mc
.P
You can change the background and foreground colors and
other visual attributes by changing the 
.mc |
parameters in the \fBXMdemos\fP application defaults file.  
Remember that \fBXMdemos\fP is used as a defaults file for all the example programs
in this guide.  If you change any of the general defaults at the top
.mc
of the file, other programs will be affected.
.P
When you run the program, you will see the window 
.mc |
shown in Figure 3-4.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 22P
.P
.sC graphics/xmfontsm.tif tiff 300 28P 20P
\&
.P
.sp +1
.FG "Program xmfonts Main Window"
.\}
.el \{\
.br
.ne 22P
\&
.sp 20P
.ce
illustration xmfontsm.eps goes here
.sp +1
.FG "Program xmfonts Main Window"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.75i
.mc
\0
.mc *
.mc
.FG "Program xmfonts Main Window"
.mc |
.sp 3.5i
.in +.43i
.P! graphics/xmfontsm.ps 
.in -.43i
.mc
.P
.mc |
Move the pointer to the push button
.mc
representing the font you want to see displayed and press 
mouse button 1.  Text in the selected font is displayed in a separate
.mc |
popup window.  This window can be removed by pressing the \fBclose\fP 
push button or left on the screen to be compared with other text windows
.mc
that you might select.
You can continue this procedure for as long as you desire.
.iX "Popup window"
.iX "MenuBar"
Each time the mouse button is pressed, the selected font will be
.mc |
displayed in a separate pop-up window.  When you want to exit the program,
move the cursor to the \fBexit\fP button in the menu bar, then drag the
pointer down until the \fBquit\fP button appears.  Clicking mouse button 1
on the \fBquit\fP button will terminate the program.
.mc
.H 3 "Windows Used in xmfonts"
There are three independent windows
.mc |
displayed in this program (see Figures 3-4, 3-5, and 3-6):
.mc
.H 4 "Main Window"
.iX "MainWindow"
.iX "PushButtonGadget"
.iX "RowColumn"
.mc |
The main window is the window in which the push buttons
are displayed (see Figure 3-4). 
.mc
It is a combination of an application shell, a MainWindow widget,
.mc |
a RowColumn widget, and a number of push button gadgets. 
.mc
The MainWindow widget 
.mc |
was chosen because it has the capability of a menu bar and is a convenient
envelope for many applications.  Although a main window can have three areas (see
Chapter 2), only two of the areas are needed here, the
menu bar and work region.  In this case the menu bar 
is the parent of a pull-down menu for the exit function and a cascade button
for the Help function.  The work region consists of a RowColumn
widget and possibly a vertical scroll bar.  A number of push button gadgets,
.mc
one for each font,
are placed within the RowColumn widget.  These are
.mc |
used instead of push buttons to improve program performance.  To see the
difference for yourself, run the program as it exists.  Use the scroll bar
to view buttons not displayed.  Then change the code in the 
\fBxmfonts.c\fP file (be sure to move this file to your work directory first) so that
.mc
the line that now reads

\f(CWbutton = XmCreatePushButtonGadget(row_column, name, args, n);\fP

becomes

\f(CWbutton = XmCreatePushButton(row_column, name, args, n);\fP
.mc |
.br
.ne 4
.P
.mc
Then recompile the program and run it again.  You should see considerable
difference in the operation of the program, particularly when scrolling
through the buttons.
.P
.H 4 "Help Window"
.iX "MessageBox"
.mc |
The Help window is a pop-up window that is a message box
(see Figure 3-5).
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 12P
.P
.sC graphics/xmfontsh.tif tiff 300 27.5P 10P
\&
.P
.sp +1
.FG "Program xmfonts Help Display Window"
.\}
.el \{\
.br
.ne 12P
\&
.sp 10P
.ce
illustration xmfontsh.eps goes here
.sp +1
.\}
.P
.++
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "Program xmfonts Help Display Window"
.mc |
.sp 1.75i
.in +.56i
.P! graphics/xmfontsh.ps 
.in -.56i
.mc
.P
.H 4 "Font Display Window"
.P
.iX "MessageDialog"
The window that displays the selected font is also a popup window
.mc |
that is a MessageBoxDialog (see Figure 3-6).
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 9P
.P
.sC graphics/xmfontsf.tif tiff 300 24P 7P
\&
.P
.sp +1
.FG "Program xmfonts Font Display Window"
.\}
.el \{\
.br
.ne 9P
\&
.sp 7P
.ce
illustration xmfontsf.eps goes here
.sp +1
.FG "Program xmfonts Font Display Window"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 1.5i
.mc
\0
.mc *
.mc
.FG "Program xmfonts Font Display Window"
.mc |
.sp 1.25i
.in +.8i
.P! graphics/xmfontsf.ps 
.in -.8i
.br
.ne 6
.mc
.P
You can have as many text display windows as you want.  You can remove them
.mc |
all by simply exiting the program as explained previously, or you can remove
each window individually by moving the pointer to the \fBclose\fP button on
.mc
the window and pressing mouse button 1.
.H 3 "Widget Hierarchy"
.iX "Widget" "hierarchy"
.iX "Hierarchy"
.mc |
.iX "XtAppInitialize"
.mc
.iX "XtToolkitInitialize"
.iX "XtOpenDisplay"
.iX "XtAppCreateShell" 
This program produces three separate windows.  One contains
.mc |
all the push button gadgets and its shell
is created using \fBXtToolkitInitialize\fP, \fBXtCreateApplicationContext\fP,
.mc
\fBXtOpenDisplay\fP, and finally \fBXtAppCreateshell\fP.  Note the
difference between this program and \fBxmbutton\fP.  Since
.mc |
\fBXtAppInitialize\fP opens the display and creates a shell in addition to
.mc
initializing the toolkit, \fBxmbutton\fP did not need to use the functions
.mc |
\fBXtCreateApplicationContext\fP, \fBXtOpenDisplay\fP, and
\fBXtAppCreateShell\fP.  The other two windows
are the Help window and the window that displays text in the selected
.mc
font.  Both of these windows are MessageBoxDialogs created by the function
\fBXmCreateMessageDialog\fP.  This function creates a MessageBox widget
and a DialogShell widget.
.SK
.mc |
The widget tree for \fBxmfonts\fP is shown in the folllowing figure.
.mc
.iX "Widget" "tree"
.iX "Tree, widget"
.mc |
.sp
.FG "xmfonts Widget Tree"
.if n .ig +n
.mc
.P
.ps 8
.in +.25i
.PS
boxht = .4; boxwid = 1.00;
down
box "Application Shell"
line down .25
Box1: box "MainWindow"
line down .125
line left 1.5
line down .125
Box2: box "MenuBar"
line down .25
Box2a: box "Pulldown" "MenuPane"
move to Box2a.s
line down .25
Box2b: box "PushButton" "Quit"
move to Box2.s
line down .125
line left 1.25
line down .125
Box3a: box "Cascade Button" "Help"
move to Box2.s; move down .125; line right 1.25
line down .125
box "CascadeButton" "Exit"
move to Box1.s
move down .125;
line right 1.25
line down .125
Box5: box "RowColumn"
line down .25
box "PushButtonGadgets" "One Per Font"
move to Box2b.s
move down .5
move right .25
Box4a: box "MessageBox Dialog" "Font Display"
move to Box4a.e
move right .25
box "MessageBox Dialog" "Help Display"
.PE
.ps
.P
.in -.25i
.mc |
.+n
.mc
.SK
.H 3 "Source Code"
.iX "Source code, xmfonts.c"
.mc |
The source code for \fBxmfonts\fP and the 
\fBXMdemos\fP default file 
are shown in the following sections.
.mc
.H 4 "The Program"
.mc |
This section shows the program listing for
\fBxmfonts\fP.
.in0
.S -2
.mc
.nf
\f(CW
.ta .5i 1.00i 1.5i
/**---------------------------------------------------------------------
***	
***	file:		xmfonts.c
***
***	project:	Motif Widgets example programs
***
.mc |
***	description:	This program creates a button for every font.
***			When a button is selected,
.mc
***			a text sample is displayed using the font.
.mc |
***
.mc
***	
.mc |
***		(c) Copyright 1989, 1990 by Open Software Foundation, Inc.
***		    All Rights Reserved.
.mc
***
.mc |
***		(c) Copyright 1989 by Hewlett-Packard Company.
.mc
***
***
.mc |
***	defaults:	xmfonts.c depends on these defaults:
.mc
!
.mc *
.mc
*allowShellResize:		true
*borderWidth:			0
*highlightThickness:		2
*traversalOn:			true
*keyboardFocusPolicy:		explicit
*menuAccelerator:		<Key>KP_F2
.mc |
*fontList:			vr-20
.mc
!
xmfonts*XmScrolledWindow.height:	432
xmfonts*XmScrolledWindow.width:		690
xmfonts*menu_bar*background:		#58f
!
***-------------------------------------------------------------------*/

/*-------------------------------------------------------------
**	Include Files
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
.mc |
#ifdef SYS_DIR
.mc
#include <sys/dir.h>
#else
.mc |
#ifdef NDIR
#include <ndir.h>
#else
.mc
#include <dirent.h>
#endif
.mc |
#endif
.mc

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
.mc |
#include <Xm/RowColumn.h>
.mc
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>

/*-------------------------------------------------------------
**	Forward Declarations
*/

void main ();			/*  main logic for application	*/

Widget CreateApplication ();	/*  create main window		*/
Widget CreateFontSample ();	/*  create font display window	*/
.mc |
Widget CreateHelp ();		/*  create help window		*/
.mc

void SelectFontCB ();		/*  callback for font buttons	*/
void CloseCB ();		/*  callback for close button	*/
.mc |
void HelpCB ();			/*  callback for help button	*/
.mc
void QuitCB ();			/*  callback for quit button	*/

/*-------------------------------------------------------------
**	Global Variables
*/

#define MAX_ARGS 20
.mc *
.mc
#define TITLE_STRING "X Font Sampler"

.mc |
typedef struct {
  char *fontpath;
} ApplicationData, *ApplicationDataPtr;

ApplicationData AppData;

#define XtNfontPath "fontPath"
#define XtCFontPath "FontPath"
static XtResource resources[] = {
  { XtNfontPath, XtCFontPath, XmRString, sizeof(String),
    XtOffset(ApplicationDataPtr, fontpath), XmRString, "/usr/lib/X11/fonts" }
};

.mc
static XmStringCharSet	charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

/*-------------------------------------------------------------
**	main		- main logic for application
*/
void main (argc,argv) 
	unsigned int	argc;
	char 		**argv;
{
	Display		*display;	/*  Display		*/
	Widget		app_shell;	/*  ApplicationShell	*/
	Widget		main_window;	/*  MainWindow		*/
.mc |
	XtAppContext	app_context;
.mc

.mc *
.mc
	/*	Initialize toolkit and open the display.
	*/
	XtToolkitInitialize();
.mc |
	app_context = XtCreateApplicationContext();
	display = XtOpenDisplay (app_context, NULL, argv[0], "XMdemos", NULL, 0,
.mc
				&argc, argv);

	if (!display) {
		XtWarning ("xfonts: Can't open display, exiting...");
		exit (0);
	}

	/*	Create application shell.
	*/
	app_shell = XtAppCreateShell (argv[0], "XMdemos",
				applicationShellWidgetClass, display, NULL, 0);

.mc |
	XtGetApplicationResources(app_shell,
			      &AppData,
			      resources,
			      XtNumber(resources),
			      NULL,
			      0);

.mc
	/*	Create and realize main application window.
	*/
	main_window = CreateApplication (app_shell);
	XtRealizeWidget (app_shell);

	/*	Get and dispatch events.
	*/
.mc |
	XtAppMainLoop (app_context);
.mc
}

/*-------------------------------------------------------------
**	CreateApplication	- create main window
*/
Widget CreateApplication (parent) 
Widget		parent;		/*  parent widget	*/
{
	Widget		main_window;	/*  MainWindow		*/
	Widget		menu_bar;	/*  MenuBar		*/
	Widget		menu_pane;	/*  MenuPane		*/
	Widget		cascade;	/*  CascadeButton	*/
	Widget		frame;		/*  Frame		*/
	Widget		swindow;	/*  ScrolledWindow	*/
	Widget		row_column;	/*  RowColumn		*/
	Widget		button;		/*  PushButtonGadget	*/
	Widget		hsb, vsb;	/*  ScrollBars		*/

	Arg		args[MAX_ARGS];	/*  arg list		*/
	register int	n;		/*  arg count		*/

	DIR		*dirp;		/*  directory pointer	*/
.mc |
#if defined (SYS_DIR) || defined (NDIR)
.mc
	struct direct 	*item;		/*  entry in directory	*/
#else
	struct dirent 	*item;		/*  entry in directory	*/
#endif
	char		name[15];	/*  name string		*/
	int		len;		/*  string length	*/

	XmString	label_string;


	/*	Create MainWindow.
	*/
	n = 0;
	main_window = XmCreateMainWindow (parent, "main1", args, n);
	XtManageChild (main_window);

	/*	Create MenuBar in MainWindow.
	*/
	n = 0;
	menu_bar = XmCreateMenuBar (main_window, "menu_bar", args, n); 
	XtManageChild (menu_bar);

	/*	Create "Exit" PulldownMenu.
	*/
	n = 0;
	menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", args, n);

	n = 0;
	button = XmCreatePushButton (menu_pane, "Quit", args, n);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, QuitCB, NULL);

	n = 0;
	XtSetArg (args[n], XmNsubMenuId, menu_pane);  n++;
	cascade = XmCreateCascadeButton (menu_bar, "Exit", args, n);
	XtManageChild (cascade);

.mc |

.mc
	/*	Create "Help" button.
	*/
	n = 0;
	cascade = XmCreateCascadeButton (menu_bar, "Help", args, n);
	XtManageChild (cascade);
	XtAddCallback (cascade, XmNactivateCallback, HelpCB, NULL);

	n = 0;
	XtSetArg (args[n], XmNmenuHelpWidget, cascade);  n++;
	XtSetValues (menu_bar, args, n);

.mc |

.mc
	/*	Create Frame MainWindow and ScrolledWindow in Frame.
	*/
	n = 0;
	XtSetArg (args[n], XmNmarginWidth, 2);  n++;
	XtSetArg (args[n], XmNmarginHeight, 2);  n++;
	XtSetArg (args[n], XmNshadowThickness, 1);  n++;
	XtSetArg (args[n], XmNshadowType, XmSHADOW_OUT);  n++;
	frame = XmCreateFrame (main_window, "frame", args, n);
	XtManageChild (frame);

	n = 0;
	XtSetArg (args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED);  n++;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC);  n++;
	swindow = XmCreateScrolledWindow (frame, "swindow", args, n);
	XtManageChild (swindow);

.mc |

.mc
	/*	Create RowColumn in ScrolledWindow to manage buttons.
	*/
	n = 0;
	XtSetArg (args[n], XmNpacking, XmPACK_COLUMN);  n++;
	XtSetArg (args[n], XmNnumColumns, 5);  n++;
	row_column = XmCreateRowColumn (swindow, "row_column", args, n);
	XtManageChild (row_column);

.mc |

.mc
	/*	Set MainWindow areas and add tab groups
	*/
	XmMainWindowSetAreas (main_window, menu_bar, NULL, NULL, NULL,
			frame);
	n = 0;
	XtSetArg (args[n], XmNhorizontalScrollBar, &hsb);  n++;
	XtSetArg (args[n], XmNverticalScrollBar, &vsb);  n++;
	XtGetValues (main_window, args, n);
	XmAddTabGroup (row_column);
	if (hsb)
		XmAddTabGroup (hsb);
	if (vsb)
		XmAddTabGroup (vsb);

.mc |

.mc
	/*	Create a PushButton widget for each font.
	*/
	/*  open the font directory  */
.mc |
	dirp = opendir (AppData.fontpath);
.mc
	/*  read one entry each time through the loop  */
	for (item = readdir (dirp);  item != NULL;  item = readdir (dirp)) 
	{
		len =  (strlen (item -> d_name));
	/*  discard entries that don't end in ".xxx"  */
		if ((len < 5) || (item -> d_name[len-4] != '.')) continue;
	/*  copy the name (except extension) from the entry  */
		strncpy (name, item -> d_name, len-4);
		name[len-4] = '\0';
	/*  create PushButton in RowCol  */
		n = 0;
		label_string = XmStringCreateLtoR(name, charset);
		XtSetArg (args[n], XmNlabelString, label_string);  n++;
		button = XmCreatePushButtonGadget (row_column, name, args, n);
		XtManageChild (button);
		XtAddCallback (button, XmNarmCallback, SelectFontCB, NULL);
		XmStringFree (label_string);
	}

	/*	Return MainWindow.
	*/
	return (main_window);
}

/*-------------------------------------------------------------
**	CreateFontSample	- create font display window
*/
Widget CreateFontSample (parent) 
Widget		parent;		/*  parent widget	*/
{
	Widget		message_box;		/*  MessageBox Dialog	*/
	Widget		button;
	Arg		args[MAX_ARGS];		/*  arg list		*/
	register int	n;			/*  arg count		*/
	
	char		*name = NULL;		/*  font name		*/
	XFontStruct	*font = NULL;		/*  font pointer	*/
	XmFontList	fontlist = NULL;	/*  fontlist pointer	*/
	static char	message[BUFSIZ];	/*  text sample		*/
	XmString	name_string = NULL;
	XmString	message_string = NULL;
	XmString	button_string = NULL;

	
	/*	Get font name.
	*/
	n = 0;
	XtSetArg (args[n], XmNlabelString, &name_string); n++;
	XtGetValues (parent, args, n);
	XmStringGetLtoR (name_string, charset, &name);


	/*	Load font and generate message to display.
	*/
	if (name)
		font = XLoadQueryFont (XtDisplay (XtParent (parent)), name);
	if  (!font)
.mc |
		sprintf (message, "Unable to load font: %s\0", name);
.mc
	else
	{
		fontlist = XmFontListCreate (font, charset);
.mc |
		sprintf (message, "\e
This is font %s.\n\e
The quick brown fox jumps over the lazy dog.\0", name);
.mc
	}
	message_string = XmStringCreateLtoR (message, charset);
	button_string = XmStringCreateLtoR ("Close", charset);

.mc |

.mc
	/*	Create MessageBox dialog.
	*/
	n = 0;
	if (fontlist) 
	{
		XtSetArg (args[n], XmNlabelFontList, fontlist);  n++;
	}
	XtSetArg (args[n], XmNdialogTitle, name_string);  n++;
	XtSetArg (args[n], XmNokLabelString, button_string);  n++;
	XtSetArg (args[n], XmNmessageString, message_string);  n++;
.mc |
	message_box = XmCreateMessageDialog (XtParent (XtParent(parent)), "fontbox",
				args, n);
.mc

	button = XmMessageBoxGetChild (message_box, XmDIALOG_CANCEL_BUTTON);
	XtUnmanageChild (button);
	button = XmMessageBoxGetChild (message_box, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild (button);

	/*	Free strings and return MessageBox.
	*/
	if (fontlist) XtFree (fontlist);
	if (name_string) XtFree (name_string);
	if (message_string) XtFree (message_string);
	if (button_string) XtFree (button_string);
	return (message_box);
}

/*-------------------------------------------------------------
.mc |
**	CreateHelp		- create help window
.mc
*/
Widget CreateHelp (parent) 
	Widget		parent;		/*  parent widget	*/
{
	Widget		button;
	Widget		message_box;	/*  Message Dialog 	*/
	Arg		args[MAX_ARGS];	/*  arg list		*/
	register int	n;		/*  arg count		*/

.mc |
	static char	message[BUFSIZ];	/*  help text	*/
.mc
	XmString	title_string = NULL;
	XmString	message_string = NULL;
	XmString	button_string = NULL;

.mc |


.mc
	/*	Generate message to display.
	*/
.mc |
	sprintf (message, "\e
These are buttons for the fonts in the X11 font directory.  \n\e
The button label is the name of the font.  When you select \n\e
a button, a small window will display a sample of the font.  \n\n\e
Press the 'close' button to close a font window.  \n\e
Select 'quit' from the 'exit' menu to exit this application.\0");
.mc
	message_string = XmStringCreateLtoR (message, charset);
	button_string = XmStringCreateLtoR ("Close", charset);
	title_string = XmStringCreateLtoR ("xmfonts help", charset);


	/*	Create MessageBox dialog.
	*/
	n = 0;
	XtSetArg (args[n], XmNdialogTitle, title_string);  n++;
	XtSetArg (args[n], XmNokLabelString, button_string);  n++;
	XtSetArg (args[n], XmNmessageString, message_string);  n++;
	message_box = XmCreateMessageDialog (parent, "helpbox", args, n);

	button = XmMessageBoxGetChild (message_box, XmDIALOG_CANCEL_BUTTON);
	XtUnmanageChild (button);
	button = XmMessageBoxGetChild (message_box, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild (button);


	/*	Free strings and return MessageBox.
	*/
	if (title_string) XtFree (title_string);
	if (message_string) XtFree (message_string);
	if (button_string) XtFree (button_string);
	return (message_box);
}

.mc *
.mc
/*-------------------------------------------------------------
**	SelectFontCB		- callback for font buttons
*/
void SelectFontCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	Widget		message_box;


	/*	Create font sample window.
	*/
	message_box = CreateFontSample (w);


	/*	Display font sample window.
	*/
	XtManageChild (message_box);
}

.mc *
.mc
/*-------------------------------------------------------------
**	CloseCB			- callback for close button
*/
void CloseCB  (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  font pointer	*/
caddr_t		call_data;	/*  data from widget class  */
{
	XFontStruct	*font	= (XFontStruct *) client_data;
	Widget		message_box	= XtParent (w);
	Widget		shell	= XtParent (message_box);


	/*	Free font.
	*/
	if (font) XFreeFont (XtDisplay (w), font);


	/*	Unmanage and destroy widgets.
	*/
	XtUnmanageChild (message_box);
	XtDestroyWidget (shell);
}

.mc *
.mc
/*-------------------------------------------------------------
.mc |
**	HelpCB			- callback for help button
.mc
*/
void HelpCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	Widget		message_box;		/*  MessageBox		*/


.mc |
	/*	Create help window.
.mc
	*/
	message_box = CreateHelp (w);


.mc |
	/*	Display help window.
.mc
	*/
	XtManageChild (message_box);
}

.mc *
.mc
/*-------------------------------------------------------------
**	QuitCB			- callback for quit button
*/
void QuitCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
.mc |
caddr_t		client_data;	/*  data from applicaiton   */
.mc
caddr_t		call_data;	/*  data from widget class  */
{

.mc *
.mc
	/*	Terminate the application.
	*/
	exit (0);
}\fP
.fi
.mc |
.S
.in
.mc
.P
.H 4 "The Defaults File"
.P
.mc |
This file should be placed in the \fB/usr/lib/X11/app-defaults\fP directory
.mc
as a part of \fBXMdemos\fP.
.P
.nf
\f(CW
!
.mc |
!XMdemos app-defaults file for Motif demo programs
.mc
!
.mc |
!general appearance and behavior defaults
.mc
!
.mc |
*allowShellResize:              true
*borderWidth:                   0
*highlightThickness:            2
*traversalOn:                   true
*keyboardFocusPolicy:           explicit
*menuAccelerator:               <Key>KP_F2
*fontList:                      vr-20
.mc
!
xmfonts*XmScrolledWindow.height:	432
xmfonts*XmScrolledWindow.width:		690
xmfonts*menu_bar*background:		#58f
!
.fi
.ft R
.ta .5i 3i
