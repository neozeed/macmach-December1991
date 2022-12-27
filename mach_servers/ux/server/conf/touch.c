#if	CMU
/*
 ****************************************************************
 * Mach Operating System
 * Copyright (c) 1986 Carnegie-Mellon University
 *  
 * This software was developed by the Mach operating system
 * project at Carnegie-Mellon University's Department of Computer
 * Science. Software contributors as of May 1986 include Mike Accetta, 
 * Robert Baron, William Bolosky, Jonathan Chew, David Golub, 
 * Glenn Marcy, Richard Rashid, Avie Tevanian and Michael Young. 
 * 
 * Some software in these files are derived from sources other
 * than CMU.  Previous copyright and other source notices are
 * preserved below and permission to use such software is
 * dependent on licenses from those institutions.
 * 
 * Permission to use the CMU portion of this software for 
 * any non-commercial research and development purpose is
 * granted with the understanding that appropriate credit
 * will be given to CMU, the Mach project and its authors.
 * The Mach project would appreciate being notified of any
 * modifications and of redistribution of this software so that
 * bug fixes and enhancements may be distributed to users.
 *
 * All other rights are reserved to Carnegie-Mellon University.
 ****************************************************************
 */
 
#endif	CMU
#include <sys/types.h>
#include <sys/stat.h>
main(argc, argv)
	int argc;
	char **argv;
{

	argc--, argv++;
	while (argc > 0) {
		struct stat stb; int c, f;
		if (stat(*argv, &stb) < 0)
			goto bad;
		if (chmod(*argv, stb.st_mode | 0200) < 0)
			goto bad;
		f = open(*argv, 2);
		if (f < 0)
			goto bad;
		lseek(f, 0, 0);
		read(f, &c, 1);
		lseek(f, 0, 0);
		write(f, &c, 1);
		close(f);
		chmod(*argv, stb.st_mode);
		argc--, argv++;
		continue;
bad:
		perror(*argv);
		argc--, argv++;
		continue;
	}
}
