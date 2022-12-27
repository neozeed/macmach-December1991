/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach3_configuration.c,v $
 * Revision 2.2  90/07/26  12:37:10  dpj
 * 	First version
 * 	[90/07/24  14:28:12  dpj]
 * 
 *
 */

/*
 * Global variables defining the configuration for a particular instance
 * of the library.
 */

int	configuration = 1;
#undef configuration
char	*mach3_configuration = "CONFIGURATION";
