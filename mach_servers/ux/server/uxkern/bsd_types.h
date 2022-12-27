/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_types.h,v $
 * Revision 2.4  90/06/02  15:27:07  rpd
 * 	Fixed type of rlimit_t.
 * 	[90/03/26  20:09:53  rpd]
 * 
 * Revision 2.3  90/05/29  20:24:59  rwd
 * 	Added cfname_t to return command processor name and args from
 * 	exec.
 * 	[90/03/22            dbg]
 * 
 * Revision 2.2  90/05/21  14:01:30  dbg
 * 	Fix type for rlimit_t so it is copied as a by-reference
 * 	parameter.
 * 	[90/04/12            dbg]
 * 
 * 	Added cfname_t to return command processor name and args from
 * 	exec.
 * 	[90/03/22            dbg]
 * 
 */

#ifndef	_UXKERN_BSD_TYPES_H_
#define	_UXKERN_BSD_TYPES_H_

/*
 * Types for BSD kernel interface.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>

#include <uxkern/bsd_types_gen.h>

typedef	char		*char_array;
typedef char		small_char_array[SMALL_ARRAY_LIMIT];
typedef	char		path_name_t[PATH_LENGTH];
typedef struct timeval	timeval_t;
typedef	struct timeval	timeval_2_t[2];
typedef	struct timeval	timeval_3_t[3];
typedef
struct statb_t {
	int	s_dev;
	int	s_ino;
	int	s_mode;
	int	s_nlink;
	int	s_uid;
	int	s_gid;
	int	s_rdev;
	int	s_size;
	int	s_atime;
	int	s_mtime;
	int	s_ctime;
	int	s_blksize;
	int	s_blocks;
} statb_t;

typedef	struct rusage	rusage_t;
typedef	char		sockarg_t[128];
typedef	int		entry_array[16];
typedef	int		gidset_t[NGROUPS];
typedef	struct rlimit	rlimit_t[1];
typedef	struct sigvec	sigvec_t;
typedef	struct sigstack	sigstack_t;
typedef struct timezone	timezone_t;
typedef	struct itimerval itimerval_t;
typedef	char		hostname_t[MAXHOSTNAMELEN];
typedef	char		cfname_t[64];

#endif	_UXKERN_BSD_TYPES_H_
