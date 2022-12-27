#define load_address
/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	mach_exec.c,v $
 * Revision 2.12  91/09/03  15:05:29  jsb
 * 	Undefined SYSV_COFF for mips (!)
 * 
 * Revision 2.11  91/09/03  11:11:57  jsb
 * 	From Intel SSD: Expand multimax COFF code to include support for i860.
 * 	Separate generic COFF stuff (ifdef'd as SYSV_COFF) from multimax and
 * 	i860 specific code.  If multimax or i860 is defined then SYSV_COFF is
 * 	defined explicitly in this file.
 * 	[91/09/03  10:54:20  jsb]
 * 
 * Revision 2.10  91/01/08  17:44:12  rpd
 * 	Allow non zero load address and "black" magic.
 * 	[90/12/14  15:26:29  rvb]
 * 
 * Revision 2.9  90/06/02  15:21:42  rpd
 * 	Removed CMUCS conditionals.  Removed u_text_start.
 * 	[90/05/13            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:35:58  rpd]
 * 
 * Revision 2.8  90/05/29  20:23:03  rwd
 * 	Move bsd_execve glue routine to bsd_server_side.c.
 * 	[90/04/06            dbg]
 * 
 * 	Exec no longer moves arguments - that is done by the emulator.
 * 	[90/03/22            dbg]
 * 
 * 	Use close_File instead of closef.
 * 	[90/04/03            rwd]
 * 
 * Revision 2.7  90/05/21  13:49:39  dbg
 * 	Move bsd_execve glue routine to bsd_server_side.c.
 * 	[90/04/06            dbg]
 * 
 * 	Exec no longer moves arguments - that is done by the emulator.
 * 	[90/03/22            dbg]
 * 
 * Revision 2.6  90/03/14  21:25:44  rwd
 * 	Change references for new MAP_UAREA code.
 * 	[90/01/31            rwd]
 * 
 * Revision 2.5  90/01/23  00:03:47  af
 * 	On mips, need to make ptrace even happier on exec of traced processes.
 * 	[90/01/20  17:42:57  af]
 * 
 * Revision 2.4  89/11/29  15:27:21  af
 * 	Removed debugging printouts.
 * 	[89/11/26  11:33:21  af]
 * 
 * 	Changes for mips.
 * 	[89/11/13            af]
 * 
 * Revision 2.3  89/11/15  13:26:43  dbg
 * 	Run '/mach_servers/mach_init' instead of '/etc/mach_init' as first
 * 	program.  Should be able to have a list of alternates, and to
 * 	optionally ask the user for the first program name.
 * 	[89/10/25            dbg]
 * 
 * Revision 2.2  89/10/17  11:25:13  rwd
 * 	Call mach_init instead of init.
 * 	[89/09/29            rwd]
 * 
 * Revision 2.1.1.1  89/09/21  20:36:01  dbg
 * 	Add interrupt return parameter to bsd_execve().
 * 	[89/09/21            dbg]
 * 
 * 	Give sun debugger exdata.ex_exec
 * 	[89/06/12            rwd]
 * 
 * 	Change execve to return arg_addr and entry point, for separate
 * 	emulator stack.
 * 	[89/05/03            dbg]
 * 
 * 	Remember to have execve deallocate the argument array!
 * 	[89/04/26            dbg]
 * 
 * 	Convert execve and friends to MiG interface.
 * 	[89/04/04            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	Move program_loadfile to machine/bsd_machdep.c.
 * 	Copy out arguments by creating argument list in memory
 * 	and using vm_write().
 * 	[89/01/02            dbg]
 * 
 * Revision 2.1  89/08/04  14:10:33  rwd
 * Created.
 * 
 * Revision 2.12  88/12/19  02:33:31  mwyoung
 * 	ca, sun3: Removed lint.
 * 	[88/12/17            mwyoung]
 * 	
 * 	Removed old MACH conditionals.
 * 	[88/12/13            mwyoung]
 * 	
 * 	Use copyout on *all* architectures to zero the partial page at
 * 	the end of the data region, to prevent a pagein error from being
 * 	improperly attributed to the kernel.
 * 	[88/11/15            mwyoung]
 * 	
 * 	Apply inode locking changes to all architectures.
 * 	[88/11/03  19:19:47  mwyoung]
 * 	
 * 	Unlock the inode after calling inode_pager_setup(), so that
 * 	the subsequent vm_map call and the copyout of zeroes can
 * 	take faults.
 * 	[88/11/03            mwyoung]
 * 
 * Revision 2.11  88/10/18  03:14:11  mwyoung
 * 	Watch out for zero return value from kmem_alloc_wait.
 * 	[88/09/13            mwyoung]
 * 
 * Revision 2.10  88/10/18  00:27:36  mwyoung
 * 	Oct-10-88 Mary Thompson (mrt) @ Carnegie Mellon
 * 	Changed program_loader for romp to use USRTEXT for
 * 	the start of the text area.
 * 	[88/10/17  16:51:54  mrt]
 * 
 * Revision 2.9  88/10/11  12:06:17  rpd
 * 	Removed the reserving of segment 0 on the RT
 * 	when ROMP+SHARED_SEG option is enabled. Lisp
 * 	counts on using that space. (From sanzi.)
 * 	[88/10/11  12:00:15  rpd]
 * 
 * Revision 2.8  88/08/25  18:09:48  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 	
 * 	Fix up arguments to vm_protect.
 * 	[88/08/17  19:54:11  mwyoung]
 * 	
 * 	Convert all architectures to use vm_map.  Correctly release
 * 	inode_pager when errors occur.  Use only user-visible VM calls.
 * 	Reserve all of segment 0 on the RT/PC regardless whether
 * 	the ROMP_SHARED_SEG option is enabled.
 * 	[88/08/15  02:24:35  mwyoung]
 * 	
 * 	Changed Vax getxfile to use vm_map.  Other architectures will
 * 	follow shortly.
 * 	Add inode_pager_release() calls to relinquish port
 * 	rights/reference acquired by inode_pager_setup().
 * 	[88/08/11  18:42:13  mwyoung]
 * 
 * Revision 2.7  88/07/17  17:47:33  mwyoung
 * Use new memory object types.
 * 
 * Revision 2.6  88/07/15  15:28:16  mja
 * Flushed obsolete cs_security.h include.
 * 
 * 16-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Record stack start, end, and direction of growth.
 *
 * 18-Apr-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Handle zero-size data regions on Sun, Multimax, RT/PC.  [Vax and
 *	Balance both map in text and data together.]  It's unclear to me
 *	why there are two vm_allocate_with_pager's in those getxfile's;
 *	it's probably just for historical reasons.
 *
 *  1-Mar-88  David Black (dlb) at Carnegie-Mellon University
 *	Changed for updated sysV header files.  Use cpp symbols for
 *	multimax and balance.
 *
 * 19-Jan-88  Robert Baron (rvb) at Carnegie-Mellon University
 *	sun3 only: Force page 0 to be allocated and VM_PROTECT_NONE so
 *	noone can use it.  Also change vm_map_find's to vm_allocate.
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 *  6-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed inode_pager_setup() calling sequence again.
 *	
 *	Removed ancient history.  Contributors so far: avie, dbg,
 *	mwyoung, bolosky, jjc, dlb, rvb, beck, jjk.
 *
 *  4-Nov-87  David Black (dlb) at Carnegie-Mellon University
 *	Changed multimax to copyout an array of zeroes instead of using
 *	user_zero.
 *
 * 28-Oct-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added check for security of kernel ports before allowing
 *	setuid/gid changes.
 *
 * 24-Sep-87  David Black (dlb) at Carnegie-Mellon University
 *	Added unix_master(), unix_release() to load_init_program() so it
 *	stays on master throughout exec.
 *
 *  6-Apr-87  David Golub (dbg) at Carnegie-Mellon University
 *	MACH: In vax_getxfile, round up text size separately from data+bss
 *	size.  Some text files (410 format) don't round up sizes
 *	correctly in the image file, but expect exec to do it for them.
 *
 * 05-Mar-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Modified execve() to save a.out header in pcb so it can be
 *	dumped into a core file and used by the debuggers.
 *
 * 12-Jan-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added "load_init_program()" routine to set up and exec /etc/init.
 *
 *  8-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Changed most "#ifdef ns32000"'s to multimax -- COFF stuff.
 *	Mods for balance a.out's -- add balance_getxfile() case and
 *	ZMAGIC magic number recognition.
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added hooks for ITC/Andrew remote file system.
 *
 *  21-Oct-86  Jonathan J. Chew (jjc) and David Golub (dbg)
 *		at Carnegie-Mellon University
 *	Merged in changes for Sun:
 *		1) Created version of getxfile() for the Sun 3.
 *		2) Don't need signal trampoline code in execve() for Sun.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	ns32000: Merged in Multimax changes; Multimax uses coff format.
 *
 * 11-Jun-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added stupid exect() call for adb on the RT.
 *
 * 28-Mar-86  David Golub (dbg) at Carnegie-Mellon University
 *	Remember that the loader's page-size is still
 *	(CLSIZE*NBPG), and that text, data and bss end on the old
 *	page boundaries, not the new ones (or we'd have to relink all
 *	programs whenever we changed the page size!).
 *
 * 23-Nov-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  enabled remote namei() processing for all
 *	routines in this module.
 *	[V1(1)]
 *
 * 21-May-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Upgraded from 4.1BSD.  Carried over changes below.
 *
 *	CMUCS:  Added setting of execute only bit and clearing of
 *	trace bit in process status if image is not readable (V3.00).
 *	[V1(1)]
 *
 **********************************************************************
 */
 
#include <cmucs_rfs.h>
#include <cmucs.h>
#include <vice.h>
#include <map_uarea.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_exec.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/map.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/inode.h>
#include <machine/vmparam.h>
#include <sys/file.h>
#include <sys/uio.h>
#include <sys/acct.h>
#include <sys/exec.h>
#include <sys/parallel.h>

#ifdef	multimax
#define SYSV_COFF
#endif	multimax

#ifdef	i860
#define SYSV_COFF
#endif	i860

#ifdef	mips
#undef	SYSV_COFF
#endif

#ifdef	SYSV_COFF
/* 
 * Handle COFF files.
 */
#include <sysV/scnhdr.h>
#include <sysV/aouthdr.h>
#include <sysV/filehdr.h>
/*
 * To avoid duplicating lots of code we will change the structure field names.
 */
#define	a_magic	magic
#define	a_text	tsize
#define a_data	dsize
#define	a_bss	bsize

#ifdef	i860
#define SECTALIGN 4096 
#endif	i860

#ifdef	multimax
#define SECTALIGN 1024
#endif	multimax

#define SECT_ROUNDUP(x) (((x) + (SECTALIGN - 1)) & ~(SECTALIGN - 1))
#endif	SYSV_COFF
 
/*
 *  Force all namei() calls to permit remote names since this module has
 *  been updated.
 */
#if	CMUCS_RFS
#undef	namei
#define	namei	rnamei
#endif	CMUCS_RFS

#ifdef	romp
#include <ca/debug.h>
#endif	romp

#include <sys/signal.h>

#include <uxkern/import_mach.h>

#include <sys/syscall.h>
#include <uxkern/syscalltrace.h>
#include <uxkern/syscall_subr.h>

/*
 *	A corrupted fileheader can cause getxfile to decide to bail
 *	out without setting up the address space correctly.  It is
 *	essential in this case that control never get back to the
 *	user.  The following error code is used by getxfile to tell
 *	execve that the process must be killed.
 */

#define	EGETXFILE	126

/*
 *	Flag to indicate the first exec (of /etc/init).
 *	Needed only for (HACK HACK) special parameter passing
 *	for /etc/init, in machine-dependent code.
 */
boolean_t	first_exec = TRUE;

void		copy_out_args_and_stack();	/* forward */

/*
 * exec system call, with and without environments.
 */
struct execa {
	char	*fname;
	char	**argp;
	char	**envp;
};

execv()
{
    panic("execv: syscall");
}

execve()
{
    panic("execve: syscall");
}

#ifdef	romp
exect()	/* New RXTUnix system call for execve with single step active */
{
	execve();
	if( u.u_error );
	else u.u_ar0[ICSCS] |= ICSCS_INSTSTEP;
}
#endif	romp

int
exec_file(fname, cfname, cfarg, entry, entry_count)
	char		*fname;
	char		*cfname;	/* OUT */
	char		*cfarg;		/* OUT */
	int		*entry;		/* pointer to OUT array */
	unsigned int	*entry_count;	/* out */
{
	int indir, uid, gid;
	struct inode *ip;
#if	VICE
	struct file *fp = NULL;
#endif	VICE
#define	SHSIZE	32
	char *	cmd_args[4];


#ifdef	SYSV_COFF
/* 
 * Handle COFF files.
 */
	struct aouthdr ahdr;
	int    aouthdr_offset;
	union {
		char	ex_shell[SHSIZE];  /* #! and name of interpreter */
		struct	filehdr	fhdr;
	} exdata;
#else	SYSV_COFF
	union {
		char	ex_shell[SHSIZE];  /* #! and name of interpreter */
		struct	exec ex_exec;
	} exdata;
#endif	SYSV_COFF

	register struct nameidata *ndp = &u.u_nd;
	int resid, error;

	/*
	 * Return command file name and argument as null strings
	 * by default.
	 */
	cfname[0] = '\0';
	cfarg[0] = '\0';

	ndp->ni_nameiop = LOOKUP | FOLLOW;
	ndp->ni_segflg = UIO_SYSSPACE;
	ndp->ni_dirp = fname;
#if	VICE
	u.u_rmt_requested = 1;
#endif	VICE
	if ((ip = namei(ndp)) == NULL)
#if	VICE
	    if (u.u_error == EVICEOP)
	    {
		int	fd = u.u_r.r_val1;

		u.u_error = 0;
		GETF(fp, fd);
		ip = (struct inode *)fp->f_data;
		ip->i_count++;	/* Because fp points to the inode also */
		u.u_ofile[fd] = NULL;	/* free user fd */
	    }
	    else
#endif	VICE
		return (u.u_error);

	indir = 0;
	uid = u.u_uid;
	gid = u.u_gid;
	/*
	 *	Can't let setuid things get abused by others through the
	 *	IPC interface
	 */

	if (ip->i_mode & (ISUID | ISGID)) {
	  if (task_secure(u.u_procp->p_task)) {

	    if (ip->i_mode & ISUID)
		uid = ip->i_uid;
	    if (ip->i_mode & ISGID)
		gid = ip->i_gid;

	  } else {
	    uprintf("%s: privileges disabled because of outstanding IPC access to task\n",
			u.u_comm);
	  }
	}

  again:
	if (access(ip, IEXEC)) {
	    error = u.u_error;
	    goto bad;
	}
	if ((u.u_procp->p_flag&STRC) && access(ip, IREAD)) {
	    error = u.u_error;
	    goto bad;
	}
	if ((ip->i_mode & IFMT) != IFREG ||
		(ip->i_mode & (IEXEC|(IEXEC>>3)|(IEXEC>>6))) == 0) {
	    error = EACCES;
	    goto bad;
	}

	/*
	 * Read in first few bytes of file for segment sizes, magic number:
	 *	407 = plain executable
	 *	410 = RO text
	 *	413 = demand paged RO text
	 * Also an ASCII line beginning with #! is
	 * the file name of a ``shell'' and arguments may be prepended
	 * to the argument list if given here.
	 *
	 * SHELL NAMES ARE LIMITED IN LENGTH.
	 *
	 * ONLY ONE ARGUMENT MAY BE PASSED TO THE SHELL FROM
	 * THE ASCII LINE.
	 */
 
	exdata.ex_shell[0] = '\0';	/* for zero length files */
	error = rdwri(UIO_READ, ip, (caddr_t)&exdata, sizeof (exdata),
	    (off_t)0, 1, &resid);
	if (error)
	    goto bad;
#ifndef lint
#ifdef 	SYSV_COFF
	if (resid > sizeof(exdata) - sizeof(exdata.fhdr) &&
#else 	SYSV_COFF
	if (resid > sizeof(exdata) - sizeof(exdata.ex_exec) &&
#endif 	SYSV_COFF
		exdata.ex_shell[0] != '#') {
	    error = ENOEXEC;
	    goto bad;
	}
#endif lint

#ifdef	SYSV_COFF
	/*
	 * If no COFF magic number then falling through
	 * to the default code handles shell scripts.
	 */
	switch(exdata.fhdr.f_magic) {

#ifdef	multimax
	    case N16WRMAGIC:
	    case N16ROMAGIC:
		aouthdr_offset = N16FILHSZ;
		break;
	    case NS32GMAGIC:
	    case NS32SMAGIC:
		aouthdr_offset = FILHSZ;
		break;
#endif	multimax

#ifdef	i860
	    case I860MAGIC:
	    case I386MAGIC: /* XXX 05-29-91 cfj: Tools use i386 magic number.*/
		            /* We should take this out later. */
		aouthdr_offset = sizeof(struct filehdr);
		break;
#endif	i860

#else	SYSV_COFF
#ifdef	load_address
	switch ((int)exdata.ex_exec.a_magic & 0xffff)
#else	load_address
	switch ((int)exdata.ex_exec.a_magic)
#endif	load_address
	{
#ifdef	romp
	/* Complain and I`ll fix it.  When I get time, that is. -BjB */
#else	romp
	case 0407:
#ifdef	mips
		exdata.ex_exec.a.data_start = exdata.ex_exec.a.text_start;
#endif	mips
		exdata.ex_exec.a_data += exdata.ex_exec.a_text;
		exdata.ex_exec.a_text = 0;
		break;
#endif	romp

#ifdef	balance
	case 0x10ea:				/* ZMAGIC: 0@0 */
						/* no XMAGIC yet */
		exdata.ex_exec.a_magic = 0413;	/* make other code easier */
		/* Fall through... */
#endif	balance
	case 0413:
	case 0410:
		if (exdata.ex_exec.a_text == 0) {
		    error = ENOEXEC;
		    goto bad;
		}
		break;

#endif	SYSV_COFF

	default:
	    {
		register char *cp;

		if (exdata.ex_shell[0] != '#' ||
		    exdata.ex_shell[1] != '!' ||
		    indir) {
			error = ENOEXEC;
			goto bad;
		}
		cp = &exdata.ex_shell[2];		/* skip "#!" */
		while (cp < &exdata.ex_shell[SHSIZE]) {
			if (*cp == '\t')
				*cp = ' ';
			else if (*cp == '\n') {
				*cp = '\0';
				break;
			}
			cp++;
		}
		if (*cp != '\0') {
			error = ENOEXEC;
			goto bad;
		}
		cp = &exdata.ex_shell[2];
		while (*cp == ' ')
			cp++;
		ndp->ni_dirp = cp;
		while (*cp && *cp != ' ')
			cp++;
		cfarg[0] = '\0';
		if (*cp) {
			*cp++ = '\0';
			while (*cp == ' ')
				cp++;
			if (*cp)
				bcopy((caddr_t)cp, (caddr_t)cfarg, SHSIZE);
		}
		indir = 1;
		iput(ip);
#if	VICE
		if (fp) {
			closef(fp);
			fp = NULL;
		}
#endif	VICE
		ndp->ni_nameiop = LOOKUP | FOLLOW;
		ndp->ni_segflg = UIO_SYSSPACE;
#if	VICE
		u.u_rmt_requested = 1;
#endif	VICE
		ip = namei(ndp);
		if (ip == NULL)
#if	VICE
		if (u.u_error == EVICEOP)
		{
		    int	fd = u.u_r.r_val1;

		    u.u_error = 0;
		    GETF(fp, fd);
		    ip = (struct inode *)fp->f_data;
		    ip->i_count++; /* Because fp points to the inode also */
		    u.u_ofile[fd] = NULL;	/* free user fd */
		}
		else
#endif	VICE
		    return (u.u_error);

		bcopy((caddr_t)ndp->ni_dent.d_name, (caddr_t)cfname,
		    MAXCOMLEN);
		cfname[MAXCOMLEN] = '\0';
		goto again;
	    }
	}

#ifdef	SYSV_COFF
/*
 * Now read the COFF a.out header.
 */
	error = rdwri(UIO_READ, ip, (caddr_t)&ahdr,
		sizeof(struct aouthdr), aouthdr_offset, 1, &resid);
	if (error)
		goto bad;

	/*
	 *	407 = plain executable
	 *	410 = RO text
	 *	413 = demand paged RO text
	 */
	switch (ahdr.magic) {
	    case 0407:
#ifdef	multimax
		/* 
		 * (The Opus tools currently create separate text and data 
	 	 * sections even for 407 files.)
	 	 */
#else	multimax
		ahdr.dsize += ahdr.tsize;
		ahdr.tsize = 0;
		break;
#endif	multimax

	    case 0410:
	    case 0413:
		if (ahdr.tsize == 0) {
			error = ENOEXEC;
			goto bad;
		}
		break;

	    default:
		error = ENOEXEC;
		goto bad;
	}

#endif	SYSV_COFF

#ifdef sun
	/* this is needed by machine_core in machine/bsd_machdep.c */
	u.u_procp->p_exec = exdata.ex_exec;
#endif sun

#ifdef	SYSV_COFF
#if	multimax
	error = mmax_getxfile(ip, &ahdr, nc + (na+4)*NBPW, uid, gid);
#else	multimax
	error = getxfile(ip, &ahdr, &exdata.fhdr, uid, gid);
#endif	multimax

#else	SYSV_COFF
	error = getxfile(ip, &exdata.ex_exec, uid, gid);
#endif	SYSV_COFF

	if (error) {
	    /*
	     *	NOTE: to prevent a race condition, getxfile had
	     *	to temporarily unlock the inode.  If new code needs to
	     *	be inserted here before the iput below, and it needs
	     *	to deal with the inode, keep this in mind.
	     */
	    goto bad;
	}
	iput(ip);
	ip = NULL;
#if	VICE
	if (u.u_textfile)
		closef(u.u_textfile);
	u.u_textfile = fp;
	fp = NULL;
#endif	VICE

	/*
	 * Set entry address
	 */
#ifdef	SYSV_COFF
	set_entry_address(&ahdr, entry, entry_count);
#else	SYSV_COFF
	set_entry_address(&exdata.ex_exec, entry, entry_count);
#endif	SYSV_COFF

	{
	    register struct utask *ut = &u.u_procp->p_utask;
	    register int	sigcatch, i;

	    /*
	     * Reset caught signals.  Held signals
	     * remain held through p_sigmask.
	     */
	    sigcatch = u.u_procp->p_sigcatch;
	    while (sigcatch) {
		i = ffs((long)sigcatch);
		sigcatch &= ~sigmask(i);
		ut->uu_signal[i] = SIG_DFL;
	    }
	    u.u_procp->p_sigcatch = 0;

	    /*
	     * Reset stack state to the user stack.
	     * Clear set of signals caught on the signal stack.
	     */
	    u.u_sigstack.ss_onstack = 0;
	    u.u_sigstack.ss_sp = 0;
	    u.u_sigonstack = 0;

	    for (i = ut->uu_lastfile; i >= 0; --i) {
		if (ut->uu_pofile[i] & UF_EXCLOSE) {
		    close_file(i);
		    ut->uu_ofile[i] = NULL;
		    ut->uu_pofile[i] = 0;
		}
		ut->uu_pofile[i] &= ~UF_MAPPED;
	    }
	    while (ut->uu_lastfile >= 0 &&
		   ut->uu_ofile[ut->uu_lastfile] == NULL)
		ut->uu_lastfile--;

	    /*
	     * Remember file name for accounting.
	     */
	    ut->uu_acflag &= ~AFORK;
	    if (indir)
		bcopy((caddr_t)cfname, (caddr_t)ut->uu_comm, MAXCOMLEN);
	    else {
		if (ndp->ni_dent.d_namlen > MAXCOMLEN)
			ndp->ni_dent.d_namlen = MAXCOMLEN;
		bcopy((caddr_t)ndp->ni_dent.d_name, (caddr_t)ut->uu_comm,
		    (unsigned)(ndp->ni_dent.d_namlen + 1));
	    }
	}

#if 0
	/*
	 * Change the default thread for the process to be
	 * the thread doing the exec... other threads are
	 * not likely to have survived.
	 */
	u.u_procp->p_thread = user_thread;
#endif 0
bad:
	if (ip)
		iput(ip);
#if	VICE
	if (fp)
		closef(fp);
#endif	VICE
	if (error == EGETXFILE) {
	    proc_exit(u.u_procp, SIGKILL, FALSE);
	    u.u_eosys = JUSTRETURN;
	}

	first_exec = FALSE;

	return (error);
}

#ifdef	multimax
#else	multimax
/*
 * Read in and set up memory for executed file.
 */
#ifdef	SYSV_COFF
int
getxfile(ip, ep, fhdr, uid, gid)
	struct inode *ip;
	struct aouthdr	*ep;
	struct filehdr	*fhdr;
#else	SYSV_COFF
int
getxfile(ip, ep, uid, gid)
	struct inode *ip;
	struct exec	*ep;
#endif	SYSV_COFF
	int uid, gid;
{
	size_t ts, ds, ss;
	int pagi;
	vm_size_t	text_size, data_size;
	int		error;

#ifdef	load_address
	if ((ep->a_magic&0xffff) == 0413)
#else	load_address
	if (ep->a_magic == 0413)
#endif	load_address
		pagi = SPAGI;
	else
		pagi = 0;

	/*
	 *	The vm system handles text that is modified
	 *	for tracing - we don't have to worry about it.
	 */

#ifdef	load_address
	if ((ep->a_magic&0xffff) != 0407 && (ip->i_flag&ITEXT) == 0 &&
	    ip->i_count != 1)
#else	load_address
	if (ep->a_magic != 0407 && (ip->i_flag&ITEXT) == 0 &&
	    ip->i_count != 1)
#endif	load_address
	{
		register struct file *fp;

		for (fp = file; fp < fileNFILE; fp++) {
#if	VICE
			if ((fp->f_type == DTYPE_INODE ||
			     fp->f_type == DTYPE_VICEINO) &&
#else	VICE
			if (fp->f_type == DTYPE_INODE &&
#endif	VICE
			    fp->f_count > 0 &&
			    (struct inode *)fp->f_data == ip &&
			    (fp->f_flag&FWRITE)) {
				return (ETXTBSY);
			}
		}
	}

	/*
	 * Load the new file, checking for sizes.
	 */
	error = machine_getxfile(ip,
#ifdef	SYSV_COFF
			 fhdr,
#endif	SYSV_COFF
			 ep,
			 u.u_rlimit[RLIMIT_STACK].rlim_cur,
			 &text_size,
			 &data_size,
			 TRUE);
	if (error)
	    return (error);

	ts = btoc(text_size);				/* machine pages */
	ds = btoc(data_size);				/* machine pages */
	ss = btoc(round_page(ctob(SSIZE)));		/* machine pages */
					/* dump a minimum-size stack */

	u.u_procp->p_flag &= ~(SPAGI|SOUSIG|SXONLY);
	if (access(ip, IREAD))
	{
		u.u_procp->p_flag |= SXONLY;
		u.u_procp->p_flag &= ~STRC;
		u.u_error = 0;
	}
	u.u_procp->p_flag |= pagi;

	/*
	 * set SUID/SGID protections, if no tracing
	 */
	if ((u.u_procp->p_flag&STRC)==0) {
		u.u_uid = uid;
		u.u_procp->p_uid = uid;
		u.u_gid = gid;
	} else {
#ifdef	mips
		u.u_procp->p_stopcode = -1;/*CAUSEEXEC*/
#endif	mips
		psignal(u.u_procp, SIGTRAP);
	}
	u.u_tsize = ts;
	u.u_dsize = ds;
	u.u_ssize = ss;
	u.u_prof.pr_scale = 0;
	return (0);
}
#endif	multimax

boolean_t	getxfile_use_map = FALSE;

#ifdef	multimax

mmax_getxfile(ip, ap, nargc, uid, gid)
	struct inode *ip;
	struct aouthdr *ap;
	int nargc, uid, gid;
{
	size_t ts, ds, ss;
	int pagi;
	vm_offset_t	addr;
	vm_size_t	size;
	task_t		my_map;
	vm_offset_t	data_end;
	vm_offset_t	vm_text_start, vm_text_end;
	vm_offset_t	vm_data_start, vm_data_end;

	if (ap->magic == 0413)
		pagi = SPAGI;
	else
		pagi = 0;

	/*
	 *	The vm system handles text that is modified
	 *	for tracing - we don't have to worry about it.
	 */

	if (ap->magic != 0407 && (ip->i_flag&ITEXT) == 0 &&
	    ip->i_count != 1) {
		register struct file *fp;

		for (fp = file; fp < fileNFILE; fp++) {
			if (fp->f_type == DTYPE_INODE &&
			    fp->f_count > 0 &&
			    (struct inode *)fp->f_data == ip &&
			    (fp->f_flag&FWRITE)) {
				u.u_error = ETXTBSY;
				goto bad;
			}
		}
	}

	/*
	 * Compute text, data and stack sizes.
	 */

	ts = btoc(loader_round_page(ap->tsize));
	ds = btoc(loader_round_page(ap->bsize + ap->dsize));
	ss = SSIZE + btoc(loader_round_page(nargc));

	u.u_procp->p_flag &= ~(SPAGI|SOUSIG|SXONLY);
	if (access(ip, IREAD))
	{
		u.u_procp->p_flag |= SXONLY;
		u.u_procp->p_flag &= ~STRC;
		u.u_error = 0;
	}
	u.u_procp->p_flag |= pagi;

#define	unix_stack_size	(u.u_rlimit[RLIMIT_STACK].rlim_cur)

	my_map = u.u_procp->p_task;
	/*
	 *	Even if we are exec'ing the same image (the RFS server
	 *	does this, for example), we don't have to unlock the
	 *	inode; deallocating it doesn't require it to be locked.
	 */
	(void) vm_deallocate(my_map, vm_map_min(my_map),
		vm_map_max(my_map)-vm_map_min(my_map));
	
	/*
	 *	Allocate low-memory stuff: text, data, bss.
	 *	Read text and data into lowest part, then make text read-only.
	 */

	addr = VM_MIN_ADDRESS;
	size = round_page(ap->data_start + ap->dsize + ap->bsize);

	/*
	 *	Remember where data starts.
	 */
	u.u_data_start = (caddr_t) ap->data_start;

	/*
	 *	Note vm boundaries for data and text segments.  If data
	 *	and text overlap a page, that is considered data.
	 */
	vm_text_start = trunc_page(ap->text_start);
	vm_text_end = round_page(ap->text_start + ap->tsize);
	vm_data_start = trunc_page(ap->data_start);
	vm_data_end = round_page(ap->data_start + ap->dsize);
	if (vm_text_end > vm_data_start)
		vm_text_end = vm_data_start;

	u.u_error = 0;

	if (pagi == 0) {	/* not demand paged */
		if (vm_allocate(my_map, &addr, size, FALSE) != KERN_SUCCESS) {
			uprintf("Cannot find space for exec image.\n");
			goto suicide;
		}

		/*
		 *	Read in the data segment (0407 & 0410).  It goes on
		 *	the next loader_page boundary after the text.
		 */
		u.u_error = rdwri(UIO_READ, ip,
				(caddr_t) ap->data_start,
				(int) ap->dsize,
				(off_t) SECT_ROUNDUP(sizeof(struct filehdr) +
						ap->tsize),
				0, (int *)0);
		/*
		 *	Read in text segment if necessary (0410), 
		 *	and read-protect it.
		 */
		if ((u.u_error == 0) && (ap->tsize > 0)) {
			u.u_error = rdwri(UIO_READ, ip,
				(caddr_t) ap->text_start,
				(int) ap->tsize,
				(off_t) SECT_ROUNDUP(sizeof(struct filehdr)),
				2, (int *) 0);
			if (u.u_error == 0) {
				(void) vm_protect(my_map,
					vm_text_start,
					vm_text_end - vm_text_start,
					FALSE,
					VM_PROT_READ|VM_PROT_EXECUTE);
			}
		}
	}
	else {
		memory_object_t	pager;

		/*
		 *	Allocate a region backed by the exec'ed inode.
		 */

		pager = (memory_object_t)inode_pager_setup(ip, TRUE, TRUE);
		iunlock(ip);

		if (vm_map(my_map, &vm_text_start, vm_text_end - vm_text_start,
			0, FALSE,
			pager, (vm_offset_t)SECTALIGN, TRUE,
			VM_PROT_READ|VM_PROT_EXECUTE, VM_PROT_ALL,
			VM_INHERIT_COPY)
		     != KERN_SUCCESS) {
			uprintf("Cannot map text into user space.\n");
			inode_pager_release(pager);
			ilock(ip);
			goto suicide;
		}

		if (vm_data_end > vm_data_start) {
			if (vm_map(my_map, &vm_data_start,
			    vm_data_end - vm_data_start, 0, FALSE, pager,
			    (vm_offset_t) SECTALIGN + SECT_ROUNDUP(ap->tsize),
			    TRUE, VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_COPY)
			      != KERN_SUCCESS) {
				uprintf("Cannot map data into user space.\n");
				inode_pager_release(pager);
				ilock(ip);
				goto suicide;
			}
		}

		inode_pager_release(pager);
		ilock(ip);

		/* BSS goes on the end of data */

		addr = data_end;
		size =
			(ap->dsize + ap->bsize) - 	/* Size of data +bss */
			(vm_data_end - vm_data_start);	/* What data has now */

		if (vm_allocate(my_map, &addr, size, FALSE) != KERN_SUCCESS) {
			uprintf("Cannot find space for bss.\n");
			goto suicide;
		}

		/*
		 *	If the data segment does not end on a VM page
		 *	boundary, we have to clear the remainder of the VM
		 *	page it ends on so that the bss segment will
		 *	(correctly) be zero.
		 *	The loader has already guaranteed that the (text+data)
		 *	segment ends on a loader_page boundary.
		 */

		data_end = ap->data_start + loader_round_page(ap->dsize);
		if (vm_data_end > data_end) {
			if (vm_data_end - data_end > PAGE_SIZE) {
			    	uprintf("Can't clear front of bss.\n");
				goto suicide;
			}
			iunlock(ip);
			if (copyout(vm_kern_zero_page, (caddr_t)data_end,
						copy_end - data_end)) {
				ilock(ip);
				uprintf("Cannot zero partial data page\n");
				goto suicide;
			}
			ilock(ip);
		}
	}

	/*
	 *	Create the stack.  (Deallocate the old one and create a 
	 *	new one).
	 */

	size = round_page(unix_stack_size);
	u.u_stack_start = (caddr_t) (addr = trunc_page(VM_MAX_ADDRESS - size));
	u.u_stack_end = u.u_stack_start + size;
	u.u_stack_grows_up = FALSE;
	(void) vm_deallocate(my_map, addr, size);
	if (vm_allocate(my_map, &addr, size, FALSE) != KERN_SUCCESS) {
		uprintf("Cannot find space for stack.\n");
		goto suicide;
	}

	/*
	 * set SUID/SGID protections, if no tracing
	 */
	if ((u.u_procp->p_flag&STRC)==0) {
		u.u_uid = uid;
		u.u_procp->p_uid = uid;
		u.u_gid = gid;
	} else
		psignal(u.u_procp, SIGTRAP);
	u.u_tsize = ts;
	u.u_dsize = ds;
	u.u_ssize = ss;
	u.u_prof.pr_scale = 0;
bad:
	return;
suicide:
	u.u_error = EGETXFILE;
	return;
}
#endif	multimax

void
copy_out_args_and_stack(cp, nc, na, ne, indir, new_arg_addr)
	register char *	cp;	/* argument and environment strings */
	register int	nc;	/* total size of strings */
	register int	na;	/* number of arguments + number of env. */
	int		ne;	/* number of environment entries */
	char **		indir;	/* if not NULL, pointer to script args */
	vm_offset_t	*new_arg_addr;
				/* argument address in user code (OUT) */
{
	register char **k_ap;	/* kernel arglist address */
	register char *	u_cp;	/* user argument string address */
	register char *	k_cp;	/* kernel argument string address */
	vm_offset_t	u_arg_start;
				/* user start of argument list block */
	vm_offset_t	k_arg_start;
				/* kernel start of argument list block */
	vm_size_t	arg_size;
				/* size of argument list block */
	vm_offset_t	u_arg_page_start;
				/* user start of args, page-aligned */
	vm_size_t	arg_page_size;
				/* page_aligned size of args */
	vm_offset_t	k_arg_page_start;
				/* kernel start of args, page-aligned */

	/*
	 * Ask machine-dependent code for argument list address
	 */
	set_arg_addr(na*NBPW + 3*NBPW + nc + NBPW,
		     &u_arg_start,
		     &arg_size);

	/*
	 * Round to page boundaries, and allocate kernel copy.
	 */
	u_arg_page_start = trunc_page(u_arg_start);
	arg_page_size = (vm_size_t)(round_page(u_arg_start + arg_size)
					- u_arg_page_start);

	(void) vm_allocate(mach_task_self(),
			   &k_arg_page_start,
			   (vm_size_t)arg_page_size,
			   TRUE);

	k_arg_start = k_arg_page_start + (u_arg_start - u_arg_page_start);

	k_ap = (char **)k_arg_start;
	u_cp = (char *)u_arg_start + na*NBPW + 3*NBPW;
	k_cp = (char *)k_arg_start + na*NBPW + 3*NBPW;

	*k_ap++ = (char *)(na - ne);	/* set number of arguments */

	for (;;) {
	    int	len;

	    if (na == ne)
		*k_ap++ = 0;

	    if (--na < 0)
		break;

	    *k_ap++ = u_cp;
	    if (indir && *indir) {
		(void) copystr(*indir++, k_cp, (unsigned)nc, &len);
	    }
	    else {
		(void) copystr(cp, k_cp, (unsigned)nc, &len);
		cp += len;
	    }
	    u_cp += len;
	    k_cp += len;
	    nc -= len;
	}
	*k_ap = 0;

	/*
	 * Write the argument list out to user space.
	 */
	(void) vm_write(u.u_procp->p_task,
			u_arg_page_start,
			(pointer_t)k_arg_page_start,
			arg_page_size);
	(void) vm_deallocate(mach_task_self(),
			     k_arg_page_start,
			     arg_page_size);

	/*
	 * Return the arg-list address.
	 */
	*new_arg_addr = u_arg_start;
}

/*
 * Load the system call emulator, and tell it to exec() the first
 * program.
 */
#include <sys/reboot.h>

char	emulator_name[] = "/mach_servers/emulator";	/* XXX */

char	arg_buf[256];
char	first_program_name[] = "/mach_servers/mach_init";	/* XXX */
char	first_program_args[] = "-xx";                           /* XXX */

load_emulator(p)
	struct proc *p;
{
	struct inode *ip;
#ifdef	SYSV_COFF
	int    aouthdr_offset;
	struct	filehdr	fhdr;
	struct	aouthdr ex_exec;

#else	SYSV_COFF
	struct exec ex_exec;
#endif	SYSV_COFF
	register struct nameidata *ndp = &u.u_nd;
	int resid, error;
	vm_offset_t text_size, data_size;
	vm_offset_t new_arg_addr;

	struct proc *save_proc = u.u_procp;

	/*
	 * Find the file, and make sure it is executable.
	 */
	ndp->ni_nameiop = LOOKUP | FOLLOW;
	ndp->ni_segflg = UIO_SYSSPACE;
	ndp->ni_dirp = emulator_name;

	if ((ip = namei(ndp)) == NULL)
	    return;

	if ((ip->i_mode & IFMT) != IFREG ||
	    (ip->i_mode & (IEXEC|(IEXEC>>3)|(IEXEC>>6))) == 0) {
		u.u_error = EACCES;
		goto bad;
	}

#ifdef	SYSV_COFF
/*
 * Read COFF header.
 */
	u.u_error = rdwri(UIO_READ, ip, (caddr_t)&fhdr, sizeof(fhdr),
			(off_t)0, UIO_SYSSPACE, &resid);

	if (u.u_error)
	    goto bad;

	/*
	 * Check for COFF magic number
	 */
	switch(fhdr.f_magic) {

#ifdef	multimax
	    case N16WRMAGIC:
	    case N16ROMAGIC:
		aouthdr_offset = N16FILHSZ;
		break;
	    case NS32GMAGIC:
	    case NS32SMAGIC:
		aouthdr_offset = FILHSZ;
		break;
#endif	multimax

#ifdef	i860
	    case I860MAGIC:
	    case I386MAGIC: /* XXX 05-29-91 cfj: Tools use i386 magic number.*/
		            /* We should take this out later. */
		aouthdr_offset = sizeof(struct filehdr);
		break;
#endif	i860

	    default:
		u.u_error = ENOEXEC;
		goto bad;
	}

/*
 * Now get the a.out header.
 * XXX - Warning: we are really using the COFF a.out header but have
 * #defined the structure field names.
 */
	u.u_error = rdwri(UIO_READ, ip, (caddr_t)&ex_exec, sizeof(ex_exec),
			aouthdr_offset, UIO_SYSSPACE, &resid);

#else	SYSV_COFF
	u.u_error = rdwri(UIO_READ, ip, (caddr_t)&ex_exec, sizeof(ex_exec),
			(off_t)0, UIO_SYSSPACE, &resid);
#endif	SYSV_COFF

	if (u.u_error)
	    goto bad;

 	/*
	 * Check the exec header format.
	 */
	switch ((int)ex_exec.a_magic) {
	    case 0407:
#ifdef	multimax
		/* 
		 * (The Opus tools currently create separate text and data 
	 	 * sections even for 407 files.)
	 	 */
#else	multimax
#ifdef	mips
		ex_exec.a.data_start = ex_exec.a.text_start;
#endif	mips
		ex_exec.a_data += ex_exec.a_text;
		ex_exec.a_text = 0;
		break;
#endif	multimax

	    case 0413:
	    case 0410:
		if (ex_exec.a_text == 0) {
		    u.u_error = ENOEXEC;
		    goto bad;
		}
		break;

	    default:
		u.u_error = ENOEXEC;
		goto bad;
	}

	/*
	 * Temporarily become the process and the calling thread.
	 */
	u.u_procp = p;

	/*
	 * Prepare for loading the emulator.
	 */
	emul_init_process(p);

#if	MAP_UAREA
	/*
	 * Remap shared area
	 */
	mapin_user(p);
#endif	MAP_UAREA

	/*
	 * Load the emulator.
	 */
	u.u_error = 
	    machine_getxfile(ip,
#ifdef	SYSV_COFF
			&fhdr,
#endif	SYSV_COFF
			&ex_exec,
			u.u_rlimit[RLIMIT_STACK].rlim_cur,
			&text_size, &data_size, FALSE);
	if (u.u_error)
	    goto bad;

	iput(ip);
	ip = NULL;

	/*
	 * Copy out arglist
	 */
	{
	    register char *cp;
	    register int  len;

	    first_program_args[1] = (boothowto & RB_SINGLE) ? 's' : 'x';
	    first_program_args[2] = (boothowto & RB_ASKNAME) ? 'a' : 'x';

	    cp = arg_buf;
	    len = strlen(first_program_name) + 1;
	    bcopy(first_program_name, cp, len);
	    cp += len;

	    len = strlen(first_program_args) + 1;
	    bcopy(first_program_args, cp, len);
	    cp += len;
	 
	    copy_out_args_and_stack(arg_buf,
				    cp - arg_buf,
				    2,
				    0,
				    (char **)0,
				    &new_arg_addr);
	}

	/*
	 * Set initial user registers.  Must do all the stuff that
	 * exec in the emulator normally does.
	 */
	set_emulator_state(&ex_exec, new_arg_addr);

    bad:
	if (ip)
	    iput(ip);

	/*
	 * Restore our process ID.
	 */
	u.u_procp = save_proc;
}


#if	0
#include <sys/reboot.h>

char		init_program_name[128] = "/etc/mach_init\0";

char		init_args[128] = "-xx\0";
struct execa	init_exec_args;
int		init_attempts = 0;

void load_init_program()
{
	vm_offset_t	init_addr;
	vm_size_t	init_size;
	int		*old_ap;
	char		*argv[3];

	unix_master();

	u.u_error = 0;

	init_args[1] = (boothowto & RB_SINGLE) ? 's' : 'x';
	init_args[2] = (boothowto & RB_ASKNAME) ? 'a' : 'x';
	
	do {
#if	defined(balance) || defined(multimax)
		if (init_attempts == 2)
			panic("Can't load init");
#else	balance || multimax
		if (boothowto & RB_INITNAME) {
			printf("init program? ");
			gets(init_program_name);
		}

#endif	balance || multimax
		if (u.u_error && ((boothowto & RB_INITNAME) == 0) &&
					(init_attempts == 1)) {
			static char other_init[] = "/etc/init";
			u.u_error = 0;
			bcopy(other_init, init_program_name,
							sizeof(other_init));
		}

		init_attempts++;

		if (u.u_error) {
			printf("Load of %s failed, errno %d\n",
					init_program_name, u.u_error);
			u.u_error = 0;
			boothowto |= RB_INITNAME;
		}

		/*
		 *	Copy out program name.
		 */

		init_size = round_page(sizeof(init_program_name) + 1);
		init_addr = VM_MIN_ADDRESS;
		(void) vm_allocate(mach_task_self(), &init_addr, init_size, TRUE);
		if (init_addr == 0)
			init_addr++;
		(void) copyout((caddr_t) init_program_name,
				(caddr_t) (init_addr),
				(unsigned) sizeof(init_program_name));

		argv[0] = (char *) init_addr;

		/*
		 *	Put out first (and only) argument, similarly.
		 */

		init_size = round_page(sizeof(init_args) + 1);
		init_addr = VM_MIN_ADDRESS;
		(void) vm_allocate(mach_task_self(), &init_addr, init_size, TRUE);
		if (init_addr == 0)
			init_addr++;
		(void) copyout((caddr_t) init_args,
				(caddr_t) (init_addr),
				(unsigned) sizeof(init_args));

		argv[1] = (char *) init_addr;

		/*
		 *	Null-end the argument list
		 */

		argv[2] = (char *) 0;
		
		/*
		 *	Copy out the argument list.
		 */
		
		init_size = round_page(sizeof(argv));
		init_addr = VM_MIN_ADDRESS;
		(void) vm_allocate(mach_task_self(), &init_addr, init_size, TRUE);
		(void) copyout((caddr_t) argv,
				(caddr_t) (init_addr),
				(unsigned) sizeof(argv));

		/*
		 *	Set up argument block for fake call to execve.
		 */

		init_exec_args.fname = argv[0];
		init_exec_args.argp = (char **) init_addr;
		init_exec_args.envp = 0;

		old_ap = u.u_ap;
		u.u_ap = (int *) &init_exec_args;
		execve();
		u.u_ap = old_ap;
	} while (u.u_error);

	unix_release();
}


#endif	0
