/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	main.c,v $
 * Revision 2.8  91/09/12  16:54:02  bohman
 * 	Added mac2.
 * 	[91/09/11  17:22:03  bohman]
 * 
 * Revision 2.7  91/07/08  16:59:26  danner
 * 	Luna88k support.
 * 	[91/06/26            danner]
 * 
 * Revision 2.6  91/06/19  11:58:37  rvb
 * 	cputypes.h->platforms.h
 * 	[91/06/12  13:46:00  rvb]
 * 
 * Revision 2.5  91/02/05  17:53:05  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:51:44  mrt]
 * 
 * Revision 2.4  90/11/25  17:48:43  jsb
 * 	Added i860 support.
 * 	[90/11/25  16:53:42  jsb]
 * 
 * Revision 2.3  90/08/27  22:09:35  dbg
 * 	Merged CMU changes into Tahoe release.
 * 	[90/08/16            dbg]
 * 
 * Revision 2.2  90/05/03  15:50:15  dbg
 * 		Cast all sprintf's (void).
 * 		[90/01/22            rvb]
 * 
 * Revision 2.1  89/08/03  16:54:33  rwd
 * Created.
 * 
 * Revision 2.6  89/02/25  19:21:48  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.5  89/02/07  22:56:02  mwyoung
 * Code cleanup cataclysm.
 * 
 * Revision 2.4  89/01/23  22:22:36  af
 * 	Changes for MIPS and I386.
 * 	Add get_rest() for "|" processing.
 * 	[89/01/09            rvb]
 * 
 * 17-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Allow source directory to be specified.
 *
 * 03-Mar-88  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Made changes for Sun 4.
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	main() now handles CONFTYPE_SQT specific configuration stuff.
 *
 * 16-Nov-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Don't create a symbolic link for "sys" in the build directory.
 *
 * 27-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in David Black's changes for the Multimax.
 *
 * 22-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in changes for Sun 2 and 3.
 *
 *  4-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Changed references of CMUCS to CMU.
 *
 *  18-Apr-86  Robert V. Baron (rvb) at Carnegie-Mellon University
 *	No more make depend warning.
 *
 */
/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)main.c	5.9 (Berkeley) 6/18/88";
#endif /* not lint */

#include <stdio.h>
#include <ctype.h>
#include "y.tab.h"
#include "config.h"

/*
 * Config builds a set of files for building a UNIX
 * system given a description of the desired system.
 */
main(argc, argv)
	int argc;
	char **argv;
{

	source_directory = "..";	/* default */
	object_directory = "..";
	config_directory = (char *) 0;
	while ((argc > 1) && (argv[1][0] == '-')) {
		char		*c;

		argv++; argc--;
		for (c = &argv[0][1]; *c ; c++) {
			switch (*c) {
				case 'd':
					source_directory = argv[1];
					goto check_arg;

				case 'o':
					object_directory = argv[1];
					goto check_arg;

				case 'c':
					config_directory = argv[1];

				 check_arg:
				 	if (argv[1] == (char *) 0)
						goto usage_error;
					argv++; argc--;
					break;

				case 'p':
					profiling++;
					break;
				default:
					goto usage_error;
			}
		}
	}
	if (config_directory == (char *) 0) {
		config_directory =
			malloc((unsigned) strlen(source_directory) + 6);
		(void) sprintf(config_directory, "%s/conf", source_directory);
	}
	if (argc != 2) {
		usage_error: ;
		fprintf(stderr, "usage: config [ -dco dir ] [ -p ] sysname\n");
	}
	PREFIX = argv[1];
	if (freopen(argv[1], "r", stdin) == NULL) {
		perror(argv[1]);
		exit(2);
	}
	dtab = NULL;
	confp = &conf_list;
	opt = 0;
	if (yyparse())
		exit(3);
	switch (conftype) {

	case CONFTYPE_VAX:
		vax_ioconf();		/* Print ioconf.c */
		ubglue();		/* Create ubglue.s */
		break;

	case CONFTYPE_SUN:
		sun_ioconf();
		break;

	case CONFTYPE_SUN2:
	case CONFTYPE_SUN3:
	case CONFTYPE_SUN4:
		sun_ioconf();           /* Print ioconf.c */
		mbglue();               /* Create mbglue.s */
		break;

	case CONFTYPE_ROMP:
		romp_ioconf();
		break;

	case CONFTYPE_MMAX:
		mmax_ioconf();
		break;

	case CONFTYPE_SQT:
		sqt_ioconf();
		break;

	case CONFTYPE_I386:
	case CONFTYPE_IX:
		/* i386_ioconf(); */
		break;

	case CONFTYPE_MIPSY:
	case CONFTYPE_MIPS:
		mips_ioconf();
		break;

	case CONFTYPE_I860:
		/* i860_ioconf(); */
		break;

	case CONFTYPE_LUNA88K:
		luna88k_ioconf();
		break;

        case CONFTYPE_MAC2:
		break;

	default:
		printf("Specify conftype type, e.g. ``conftype vax''\n");
		exit(1);
	}

	makefile();			/* build Makefile */
	headers();			/* make a lot of .h files */
	swapconf();			/* swap config files */
	exit(0);
}

/*
 * get_word
 *	returns EOF on end of file
 *	NULL on end of line
 *	pointer to the word otherwise
 */
char *
get_word(fp)
	register FILE *fp;
{
	static char line[80];
	register int ch;
	register char *cp;

	while ((ch = getc(fp)) != EOF)
		if (ch != ' ' && ch != '\t')
			break;
	if (ch == EOF)
		return ((char *)EOF);
	if (ch == '\n')
		return (NULL);
	if (ch == '|')
		return( "|");
	cp = line;
	*cp++ = ch;
	while ((ch = getc(fp)) != EOF) {
		if (isspace(ch))
			break;
		*cp++ = ch;
	}
	*cp = 0;
	if (ch == EOF)
		return ((char *)EOF);
	(void) ungetc(ch, fp);
	return (line);
}

/*
 * get_rest
 *	returns EOF on end of file
 *	NULL on end of line
 *	pointer to the word otherwise
 */
char *
get_rest(fp)
	register FILE *fp;
{
	static char line[80];
	register int ch;
	register char *cp;

	cp = line;
	while ((ch = getc(fp)) != EOF) {
		if (ch == '\n')
			break;
		*cp++ = ch;
	}
	*cp = 0;
	if (ch == EOF)
		return ((char *)EOF);
	return (line);
}

/*
 * prepend the path to a filename
 */
char *
path(file)
	char *file;
{
	register char *cp;

	cp = malloc((unsigned)(strlen(PREFIX)+
			       strlen(file)+
			       strlen(object_directory)+
			       3));
	(void) sprintf(cp, "%s/%s/%s", object_directory, PREFIX, file);
	return (cp);
}
