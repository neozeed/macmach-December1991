#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)uilsymdump.c	1.6 90/08/01";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

#include <uil/UilDef.h>
#include <stdio.h>
#include <ctype.h>

/*
**
** Table of Contents
**
*/
Uil_status_type		MessageCB		();
Uil_status_type	        StatusCB		();


/*
** globals
*/
FILE	*source_file;


/*
**  Entry point
*/
main()
{
    Uil_command_type	    command_desc;
    Uil_compile_desc_type   compile_desc;
    Uil_status_type	    return_status;
    int			    x = 1, y = 1;
    char		    *include_list[3];
    char		    file_name[255], listing_name[255], uid_name[255];

    /*
    ** Ask for the file name.
    */
    printf("Enter file to be compiled (leave off the .uil): ");
    gets(file_name);

    strcpy (uid_name, file_name);	strcat (uid_name, ".uid");
    strcpy (listing_name, file_name);	strcat (listing_name, ".lis");
    strcat (file_name, ".uil");

    /*
    **  Fill in the command decs
    */
    command_desc.resource_file = uid_name;
    command_desc.listing_file = listing_name;
    command_desc.include_dir_count = 2;
    command_desc.include_dir = (char **)include_list;
    command_desc.source_file = file_name;
    include_list[0] = "com:";
    include_list[1] = "../include";


    command_desc.listing_file_flag = 1;
    command_desc.resource_file_flag = 1;
    command_desc.machine_code_flag = 1;
    command_desc.report_info_msg_flag = 1;
    command_desc.report_warn_msg_flag = 1;
    command_desc.parse_tree_flag = 1;
    command_desc.status_update_delay = 400;

    return_status = Uil(&command_desc, &compile_desc, MessageCB, &x,
			StatusCB, &y);

    UilDumpSymbolTable( compile_desc.parse_tree_root );
    return return_status;
}

Uil_status_type	MessageCB(  message_user_data, 
			    msg_number, 
			    msg_severity,
			    msg_text,
			    src_text, 
			    ptr_text, 
			    loc_text,
			    message_summary)

    int    *message_user_data;	    /* Opaque, user-supplied argument */
    int	    msg_number;	    /* constant to identify the error */
    int	    msg_severity;   /* severity level */
    char    *msg_text;	    /* text of message */
    char    *src_text;	    /* text of source line on which error occured */
    char    *ptr_text;	    /* source column information */
    char    *loc_text;	    /* location line */
    unsigned int (*message_summary)[Uil_k_max_status+1];
 
{
    printf ("Message(%d) -- %d-%d-%s\n    %s\n    %s\n    %s\n",
		*message_user_data, msg_number, msg_severity, msg_text,
		src_text, ptr_text, loc_text);

    *message_user_data = *message_user_data + 1;
    return Uil_k_continue;
}



Uil_status_type	StatusCB(	status_user_data, 
				percent_complete, 
				lines_processed,
				current_file,
				message_summary)

    int			*status_user_data;
    unsigned int	percent_complete;
    unsigned int	lines_processed;
    char		*current_file;
    unsigned int (*message_summary)[Uil_k_max_status+1];

{
    printf("Status(%d) -- Percent: %d,  Lines: %d, File: %s\n",
	    *status_user_data, percent_complete, lines_processed, current_file);

    *status_user_data = *status_user_data + 1;
    return Uil_k_continue;
}

