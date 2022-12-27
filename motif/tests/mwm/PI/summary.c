#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)summary.c	3.2 91/01/11";
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <PITests.h>

/*
 * Local declarations
 */
#define PASSED		1
#define FAILED		0

String	result_choice[2] =
{
    "FAILED",
	"Passed"
};

/*
 * Return value strings
 */

String	retstr[2] = 
{
    "False        ",
    "True         ",
};

/*
 * summary():
 * summary() will return a String formed by formatting the test results
 * together with some other verbage to make a nice pretty results display.
 *
 * The String that summary() returns to the calling routine is allocated 
 * within this routine.  It is the calling routine's responsibility to
 * ***FREE THE STRING THAT IS RETURNED***
 * after it is used in the calling routine.
 */
String
summary(num_tests, description_length, test_info)
	int		num_tests,
			description_length;
	struct TestInfo	*test_info;
{
/*
 * All of the headers except header_top and trailer will be used as
 * strings to satisfy the %s's in testline (below) so that the format
 * of the headers will always match the format of the description lines.
 */
    String	testline			= "%s    %s  %s  %s\n",
			header_top			= "Test completed.\n\nResults:\n\n",
			header_test			= "Test",
			header_exp			= "Expected Rslt",
			header_act			= "Actual Result",
				/* exp and act sizes match the name field of retstr[] above */
    		header_rslt			= "Result",
			header_test_dash	= "----",
			header_exp_act_dash	= "-------------",
    		header_rslt_dash	= "------\n",
    		trailer				= "\n\nPress the exit button to end the test.",
				/* no specific size for padding, just a lot of spaces */
			padding				= "                                                                                                                          ";
	String	retval,
			result_this_test,
			temp_padded_str;

    int		i,
			padding_len,	/* amount of padding to get "Test" out to 
								description_length size */
			testline_len;	/* length of a full test description line */

/*
 * Allocate space for string we're creating
 */
	testline_len = strlen(testline) +
					description_length +
					strlen(header_exp) +
					strlen(header_act) +
					strlen(header_rslt);
	/* 
	 * Yes, I know I haven't accounted for the extra space occupied by
	 * the %s's and the \ in the \n in testline, but that's okay.  So,
	 * we allocate a few extra bytes - it will allow for things like
	 * NULLs, etc.
	 */
    result_this_test = XtCalloc(testline_len, 1);

	padding_len = description_length - strlen(header_test);
    retval = XtCalloc(((num_tests + 2) * testline_len) +
							strlen(header_top) +
							strlen(trailer),
							1);
	/*
	 * num_tests + 2 because there are two header lines on top of the
	 * description lines
	 */

	temp_padded_str = XtCalloc(description_length + 5, 1);
	/* add 5 more bytes just for good luck */

/*
 * Build and add header lines, use testline as the format to insure uniformity
 */
	strcpy(retval, header_top);
	/* have to pad the test header out to the size of description_length */
	strcpy(temp_padded_str, header_test);
	strncat(temp_padded_str, padding, padding_len);
	sprintf(result_this_test, testline,
			temp_padded_str,
			header_exp,
			header_act,
			header_rslt);
	strcat(retval, result_this_test);

	/* have to pad the test_dash header out to the size of description_length */
	strcpy(temp_padded_str, header_test_dash);
	strncat(temp_padded_str, padding, padding_len);
	sprintf(result_this_test, testline,
			temp_padded_str,
			header_exp_act_dash,
			header_exp_act_dash,
			header_rslt_dash);
	strcat(retval, result_this_test);

/*
 * Add description of, and results from, each individual test
 */
    for(i=0 ; i<num_tests ; i++)
    {
		sprintf(result_this_test, testline,
			(*test_info).description,
			retstr[(*test_info).expected_return],
			retstr[(*test_info).actual_return],
			((*test_info).actual_return == (*test_info).expected_return) ?
			result_choice[PASSED] : result_choice[FAILED]);
	
		strcat(retval, result_this_test);
		*test_info++;
    }

/*
 * Add closing message
 */
    strcat(retval, trailer);
    
    return(retval);
}


