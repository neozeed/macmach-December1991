#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "@(#)Tables.c	3.3 91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
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
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

#include <stdio.h>

#include "Tables.h"

main(argc, argv)
     unsigned int argc;
     char *argv[];
{ 	
    char	mrm_err[50];

/*
 * Initialize Mrm
 */
    MrmInitialize();

/*
 *  Initialize the toolkit.  This call returns the id of the "toplevel" widget.
 */

    toplevel = XtInitialize("Simple",		/* application name	   */
			    "genericclass",	/* application class	   */
			    NULL, 0,		/* options		   */
			    &argc, argv);	/* command line parameters */

/*
 *  Open the Mrm hierarchy
 */

    if(MrmOpenHierarchy(filename_num,		/* number of files	*/
			filename_vec,		/* files		*/
			NULL,			/* os_ext_list (null)	*/
			&s_MrmHierarchy)	/* ptr to returned id	*/
       != MrmSUCCESS) 
    {
	error_proc("Can't open hierarchy\n");
    }

/*
 * Register the names of the callback routines with Mrm
 */
    
    if(MrmRegisterNames(reglist,	/* list of names	*/
			reglist_num)	/* number of names	*/
       != MrmSUCCESS)
    {
	error_proc("Can't register names\n");
    }

/*
 * Get display
 */

    dsp = XtDisplay(toplevel);

/*
 * Fetch widget which sends a table to its callback routine
 */

    for(ndx=0 ; ndx<NUM_TESTS ; ndx++)
    {

/*
 * Print header for test results
 */

	fprintf(stdout,"\n\n%s Test Results:\n\n",
		test_result_headers[ndx]);

	if(MrmFetchWidget(s_MrmHierarchy,
			  UIL_widget_names[ndx],
			  toplevel,
			  &UIL_widgets[ndx],
			  &class) != MrmSUCCESS)
	{
	    sprintf(mrm_err, "Can't fetch UIL widget %s",
		    UIL_widget_names[ndx]);
	    error_proc(mrm_err);
	}
	else
	{
	    XtCallCallbacks(UIL_widgets[ndx], XmNactivateCallback, NULL);
	}

/*
 * Print error summary
 */

	if(errors[ndx])
	{
	    fprintf(stdout,"\n%s test failed with %d errors\n\n",
		    test_result_headers[ndx], errors[ndx]);
	}
	else
	{
	    fprintf(stdout,"\n%s test passed.\n\n",
		    test_result_headers[ndx]);
	}
    }

/*
 * Close Mrm Hierarchy
 */

    MrmCloseHierarchy(s_MrmHierarchy);
}

/*
 * Callback routine to receive integer table
 */

void integer_table_proc(w, UIL_integer_table, reason)
     Widget		w;
     int		*UIL_integer_table;
     unsigned long	*reason;
{
    int			i;

/*
 * Print test result header
 */

    fprintf(stdout,"Integer Table\t Actual \tExpected\n");
    fprintf(stdout,"   Element   \t Value  \t Value  \tError\n");
    fprintf(stdout,"-------------\t--------\t--------\t-----\n");

    
    for(i=0 ; i<=4 ; i++)
    {
	fprintf(stdout,"      %1d      \t%8d\t%8d",i+1, UIL_integer_table[i],
		X_integer_table[i]);

	if(UIL_integer_table[i] != X_integer_table[i])
	{
	    errors[ndx]++;
	    
	    fprintf(stdout,"\t  *\n");
	}
	else
	{
	    fprintf(stdout,"\n");
	}
    }
}

/*
 * Callback routine to receive compound string table
 */

void compound_string_table_proc(w, UIL_compound_string_table, reason)
     Widget		w;
     XmString		*UIL_compound_string_table;
     unsigned long	*reason;
{
    XmStringContext	context;
    XmStringCharSet	charset;
    XmStringDirection	direction;
    Boolean		separator;
    char		*next_string;
    char                *the_whole_string=NULL;
    int			i;
    Boolean             valid_segment;

/*
 * Print test result header
 */

    fprintf(stdout,"Compound String\t         Actual \t        Expected\n");
    fprintf(stdout," Table Element \t         Value  \t         Value  \tError\n");
    fprintf(stdout,"---------------\t        --------\t        --------\t-----\n");

    for(i=0 ; i<=2 ; i++)
    {
	XmStringInitContext(&context, UIL_compound_string_table[i]);

	if (the_whole_string != NULL)
	    {
	    XtFree (the_whole_string);
	    the_whole_string = NULL;
	    valid_segment = True;
	    }

	valid_segment = XmStringGetNextSegment(context,
			   &next_string,
			   &charset,
			   &direction,
			   &separator);
	while (valid_segment)
	    {

		if (the_whole_string == NULL)
		    {
		    the_whole_string = XtCalloc (1, strlen(next_string) + 1);
		    strcpy (the_whole_string, next_string);
		    }
		else
		    {
		    the_whole_string = XtRealloc (the_whole_string, 
			          strlen (the_whole_string) +
				  strlen (next_string) + 1);
		    strcat (the_whole_string, next_string);
		    }
		XtFree (next_string);
	    valid_segment = XmStringGetNextSegment(context,
			       &next_string,
			       &charset,
			       &direction,
			       &separator);
	    }

	fprintf(stdout,"       %1d       \t%16s\t%16s",i+1, the_whole_string,
		X_compound_string_table[i]);

	if(strcmp(the_whole_string, X_compound_string_table[i]) != 0)
	{
	    errors[ndx]++;
	    
	    fprintf(stdout,"\t  *\n");
	}
	else
	{
	    fprintf(stdout,"\n");
	}

	XmStringFreeContext(context);
    }
}

/*
 * Callback routine to receive asciz string table
 */

void asciz_string_table_proc(w, UIL_asciz_string_table, reason)
     Widget		w;
     String		*UIL_asciz_string_table;
     unsigned long	*reason;
{
    int			i;
    
/*
 * Print test result header
 */

    fprintf(stdout,"Asciz String   \t         Actual \t        Expected\n");
    fprintf(stdout,"Table Element  \t         Value  \t         Value  \tError\n");
    fprintf(stdout,"-------------  \t        --------\t        --------\t-----\n");

    for(i=0 ; i<=3 ; i++)
    {
	fprintf(stdout,"       %1d       \t%16s\t%16s",i+1,
		UIL_asciz_string_table[i], X_asciz_string_table[i]);

	if(strcmp(UIL_asciz_string_table[i], X_asciz_string_table[i]) != 0)
	{
	    errors[ndx]++;
	    
	    fprintf(stdout,"\t  *\n");
	}
	else
	{
	    fprintf(stdout,"\n");
	}
    }
}

/*
 * Callback routine to receive font table
 */

void font_table_proc(w, UIL_font_table, reason)
     Widget		w;
     XmFontList		UIL_font_table;
     unsigned long	*reason;
{
    XFontStruct		*X_font;
    
    int			i,
    			font_errors = 0;
    
/*
 * Print test result header
 */

    fprintf(stdout,"Font Table\n");
    fprintf(stdout," Element  \t       Font Name\tError\n");
    fprintf(stdout,"----------\t       ---------\t-----\n");

    for(i=0 ; i<=2 ; i++)
    {
	fprintf(stdout,"      %1d     \t%16s",i+1, X_font_names[i]);

	if((X_font = XLoadQueryFont(dsp,			 /* display  */
				    X_font_names[i])) == NULL)	/* font list */
	{
	    fprintf(stdout, "Can't fetch X font %s", X_font_names[i]);
	}
	else
	{
	    compare_fonts(UIL_font_table->font, X_font, &font_errors, NULL);
	    UIL_font_table++;
	}
	

	if(font_errors)
	{
	    errors[ndx]++;
	    
	    fprintf(stdout,"\tFonts differ\n");
	}
	else
	{
	    fprintf(stdout,"\n");
	}
    }
}

/*
 * Callback routine to receive identifier data type
 */

void identifier_proc(w, ident_tag, reason)
     Widget		w;
     int		ident_tag;
     unsigned long	*reason;
{
    int		i,
    		nargs = 0;

    short	ident_resource = 0;

    Arg	args[1];
    
    fprintf(stdout, "                                   \tActual\tExpected\n");
    fprintf(stdout, "Identifier Use                     \tValue \t Value  \t");
    fprintf(stdout, "Error\n");
    fprintf(stdout, "--------------                     \t------\t--------\t");
    fprintf(stdout, "-----\n");

    i = 0;

/*
 * Identifier as widget resource value
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, &ident_resource); nargs++;
    XtGetValues(w, args, nargs);
    
    fprintf(stdout, "%35s\t%6d\t%8d", ident_type[i], ident_resource,
	    expected_ident[i]);

    if(ident_resource != expected_ident[i])
    {
	errors[ndx]++;
	
	fprintf(stdout, "\t  *\n");
    }
    else
    {
	fprintf(stdout, "\n");
    }

    i++;
    
/*
 * Identifier as callback tag value
 */
    
    fprintf(stdout, "%35s\t%6d\t%8d", ident_type[i], ident_tag,
	    expected_ident[i]);

    if(ident_tag != expected_ident[i])
    {
	errors[ndx]++;
	
	fprintf(stdout, "\t  *\n");
    }
    else
    {
	fprintf(stdout, "\n");
    }

}

/*
 * Error handling
 */

void error_proc(error_string)
     String	error_string;
{
    fprintf(stdout,"Mrm error: %s\n", error_string);
    fflush(stdout);
}
