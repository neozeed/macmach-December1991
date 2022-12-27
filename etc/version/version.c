/*
 **********************************************************************
 * HISTORY
 * $Log:	version.c,v $
 * Revision 1.5  90/03/01  15:30:27  bww
 * 	Included <machine/mmu.h> for SGOFSET on Sun 4.
 * 	From "[89/02/06            jjc]" at CMU.
 * 	[90/03/01            bww]
 * 
 * Revision 1.4  90/01/14  19:48:37  bww
 * 	Added ZMAGIC case for sun. The pure kernel is of type ZMAGIC.
 * 	From "[89/12/13            rwd]" at CMU.
 * 	Support for mips, COFF too.
 * 	From "[89/08/01            af]" at CMU.
 * 	[90/01/13            bww]
 * 
 * Revision 1.3  89/07/27  18:54:51  bww
 * 	Handle both pageable and non-pageable Multimax COFF formats.
 * 	From "[89/07/27  00:39:26  mbj]" at CMU.  Handle COFF for the
 * 	Multimax.  From "[89/07/25  16:09:18  mbj]" at CMU.
 * 	[89/07/27            bww]
 * 
 * 21-Oct-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Modified to retrieve the version string from the name-list file
 *	by dafault; added -k switch to examine the image file instead
 *	with an alternate file name available as an optional argument;
 *	updated error messages to use diagnostic output and
 *	distinguished exit codes.
 *
 *  7-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Changed to allow specification of "vmunix" binary.  Support for
 *	-i option which prints out the "INCLUDE_VERSION" of the kernel.
 *	Use only one read for the version string instead of reading one
 *	character at a time.
 *
 * 24-Sep-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */

#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <ctype.h>

/*
 *  The <a.out.h> and <nlist.h> files apparently use incompatible
 *  definitions for "struct nlist".  We want the <nlist.h> version.
 */
#if	!defined(multimax)
#define	nlist	nlist_x
#endif
#include <a.out.h>
#if	!defined(multimax)
#undef	nlist
#include <nlist.h>
#else
#include <sgs.h>	/* a.out magic numbers */
#endif
#if	defined(mips)
#include <sys/exec.h>
#endif

struct nlist nl[] = {
#define	X_VERSION	0
	{ "_version" },
#define X_INCLUDE_VERSION 1
	{ "_INCLUDE_VERSION" },
	{ "" },
};

#define	MAXSIZE	1024

extern	uid_t getuid();
extern	gid_t getgid();
extern	off_t lseek();

char	buf[MAXSIZE];
int	include_version = 0;
int	read_kmem = 0;
char	*vmunix = "/vmunix";
char	*kmem = "/dev/kmem";
char    *data = 0;
char	*pgmname;
#ifdef mac2
char	*version = 0;
#endif

main(argc, argv)
	int argc;
	char **argv;
{
	register char	c, *cp;
	register int	len;
	int		mf;
	off_t		addr;
	
	pgmname = argv[0];
	while (argc > 1) {
		argv++;
		cp = *argv;
		if (*cp == '-') {
			cp++;
			if (*cp == 'i')
				include_version = 1;
			else if (*cp == 'k')
				read_kmem = 1;
#ifdef mac2
			else if (*cp == 'v') {
				argc -= 2;
				argv++;
				if (!argc) version = pgmname;
				else {
					version = *argv;
					argc--;
					argv++;
				}
				if (include_version || read_kmem || argc) {
					usage();
					exit(-1);
				}
				break;
			}
#endif
			else
			{
				usage();
				exit(-1);
			}
		}
		else {
			static int alt = 0;

			if (alt++ == 0)
				vmunix = cp;
			else
				kmem = cp;
			/*
			 *  Don't let normal users access a protected symbol
			 *  table or kernel image.
			 */
			(void) setgid(getgid());
			(void) setuid(getuid());
		}
		argc--;
	}
#ifdef mac2
	if (version) {
	  char buffer[1000];
	  sprintf(buffer, "PROGRAM=`/usr/bin/wh %s` && echo \"$PROGRAM: `/usr/ucb/strings $PROGRAM | /usr/ucb/grep version | (read VERSION; echo $VERSION)`",
	    version, version);
	  exit(system(buffer));
	}
#endif
	if (vmnlist(vmunix, nl) && nlist(vmunix, nl) < 0) {
		fprintf(stderr, "%s: %s: missing or incomplete namelist: %s\n",
			 pgmname, vmunix, errmsg(-1));
		exit(-2);
	}
	if (include_version) {
		printf("%d\n", nl[X_INCLUDE_VERSION].n_value);
		exit(0);
	}
	if (nl[X_VERSION].n_value == 0) {
		fprintf(stderr, "%s: %s: no version ??\n",
			 pgmname, vmunix);
		exit(-2);
	}
	if (read_kmem)
		data = kmem;
	else
		data = vmunix;
	mf = open(data, 0);
	if(mf < 0) {
		fprintf(stderr, "%s: %s: cannot open: %s\n",
			 pgmname, data, errmsg(-1));
		exit(-3);
	}
	/*
	 *  If we are reading the namelist file, compute the proper offset
	 *  based on the exec header. For NMAGIC files, the file offset must be
	 *  adjusted to correct for the 1024-byte alignment of the data segment
	 *  address.
	 */
	if (read_kmem == 0) {
#if	defined(multimax) || defined(mips)
		struct	filehdr x;
		struct	aouthdr ahdr;
		int	aouthdr_offset;
		vm_offset_t	sectalign;
#else
		struct exec x;
#endif

		len = read(mf, (char *)&x, sizeof(x));
		if (len != sizeof(x)) {
			fprintf(stderr, "%s: %s: cannot read  header\n",
			       pgmname, data);
			exit(-4);
		}
#if	defined(multimax) || defined(mips)
		switch(x.f_magic) {
		case N16WRMAGIC:
		case N16ROMAGIC:
			aouthdr_offset = N16FILHSZ;
			break;
		case NS32GMAGIC:
		case NS32SMAGIC:
		case MIPSEBMAGIC:
		case MIPSELMAGIC:
			aouthdr_offset = FILHSZ;
			break;
		default:
			fprintf(stderr, "%s: %s: Illegal magic number 0x%x\n",
				pgmname, data, x.f_magic);
			exit(-4);
		}

		len = lseek(mf, aouthdr_offset, L_SET);
		if (len >= 0) len = read(mf, (char *)&ahdr, sizeof(ahdr));
		if (len != sizeof(ahdr)) {
			fprintf(stderr, "%s: %s: cannot read aouthdr header\n",
			       pgmname, data);
			exit(-4);
		}
#if	defined(multimax)
		if (aouthdr_offset == N16FILHSZ)
			ahdr.flags |= U_AL_1024;

		/*
		 *	Page size used by loader is encoded in flags field.
		 *	Only 1024 and 4096 are supported currently.
		 */
		if (ahdr.flags & U_AL_1024)
			sectalign = 1024;
		else if (ahdr.flags & U_AL_4096)
			sectalign = 4096;
		else {
			fprintf(stderr, "%s: %s: Illegal alignment flags\n",
			       pgmname, data);
			exit(-4);
		}

#define SECT_ROUNDUP(x) ((((vm_offset_t)(x)) + (sectalign - 1)) & \
					~(sectalign - 1))

		if (ahdr.magic != PAGEMAGIC) {
			addr = sectalign + ahdr.tsize /* Data start in file */
				+ (nl[X_VERSION].n_value - ahdr.data_start);
		} else {
			addr = sectalign + SECT_ROUNDUP(ahdr.tsize)
				+ (nl[X_VERSION].n_value - ahdr.data_start);
		}
#endif	/* multimax */
#if	defined(mips)
		addr =  N_TXTOFF(x,ahdr) + ahdr.tsize /* Data start in file */
			+ (nl[X_VERSION].n_value - ahdr.data_start);
#endif
#else	/* multimax || mips */
		addr = N_TXTOFF(x)+(nl[X_VERSION].n_value
#if	defined(vax)
							 &~0x80000000
#endif
#if	defined(ibmrt)
							 &~0xf0000000
#endif
#if	defined(sun) || defined(mac2)
							 -x.a_entry
#endif
								     );
		switch ((int)x.a_magic)
		{
		    case OMAGIC:
			break;
		    case NMAGIC:
			addr -= ((x.a_text+1024-1)&~(1024-1))-x.a_text;
			break;
		    case ZMAGIC:
#if	defined(sun) || defined(mac2)
#include <machine/machparam.h>
#if	defined(sun4)
#include <machine/mmu.h>	/* for SGOFSET */
#endif
			addr = nl[X_VERSION].n_value + x.a_text -
			    ((x.a_text + NBPG + SGOFSET) & ~SGOFSET);
#endif
			break;
		    default:
			fprintf(stderr, "%s: %s: unknown magic number: %o\n",
			       pgmname, data, x.a_magic);
			exit(-5);
		}
#endif	/* multimax || mips */
	} else {
		addr = nl[X_VERSION].n_value;
	}
	if (lseek(mf, addr, 0) == -1) {
		fprintf(stderr, "%s: %s: cannot seek to _version: %s\n",
		       pgmname, data, errmsg(-1));
		exit(-6);
	}
	len = read(mf, buf, MAXSIZE);
	cp = buf;
	while (len > 0) {
		c = *cp++;
		if (c == 0)
			break;
		if (isascii(c) && (isprint(c) || isspace(c)))
			(void) putchar(c);
		len--;
	}
	exit(0);
}

usage()
{
	fprintf(stderr, "Usage: %s [ -i ] [ namelist-file ]\n", pgmname);
	fprintf(stderr, "       %s -k [ namelist-file [ image-file ] ]\n", pgmname);
	fprintf(stderr, "       %s -v\n", pgmname);
}
