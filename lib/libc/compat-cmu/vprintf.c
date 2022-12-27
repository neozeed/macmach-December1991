/*
 * varargs versions of printf routines
 *
 **********************************************************************
 * HISTORY
 * $Log:	vprintf.c,v $
 * Revision 2.4  89/08/03  14:40:10  mja
 * 	Add vsnprintf() routine.
 * 	[89/07/12            mja]
 * 
 * 	Terminate vsprintf() string with null byte.
 * 	[89/04/21            mja]
 * 
 * 	Change to use new hidden name for _doprnt on MIPS.
 * 	[89/04/18            mja]
 * 
 * Revision 2.3  89/06/10  14:13:43  gm0w
 * 	Added putc of NULL byte to vsprintf.
 * 	[89/06/10            gm0w]
 * 
 * Revision 2.2  88/12/13  13:53:17  gm0w
 * 	From Brad White.
 * 	[88/12/13            gm0w]
 * 
 **********************************************************************
 */
#include <stdio.h>
#include <varargs.h>

#if	!defined(vax) && !defined(sun3) && !defined(ibmrt)
/* 
 *  No new architectures make _doprnt() visible.
 */
#define	_doprnt	_doprnt_va
#endif


int
vprintf(fmt, args)
	char *fmt;
	va_list args;
{
	_doprnt(fmt, args, stdout);
	return (ferror(stdout) ? EOF : 0);
}

int
vfprintf(f, fmt, args)
	FILE *f;
	char *fmt;
	va_list args;
{
	_doprnt(fmt, args, f);
	return (ferror(f) ? EOF : 0);
}

int
vsprintf(s, fmt, args)
	char *s, *fmt;
	va_list args;
{
	FILE fakebuf;

	fakebuf._flag = _IOSTRG;	/* no _IOWRT: avoid stdio bug */
	fakebuf._ptr = s;
	fakebuf._cnt = 32767;
	_doprnt(fmt, args, &fakebuf);
	putc('\0', &fakebuf);
	return (strlen(s));
}

int
vsnprintf(s, n, fmt, args)
	char *s, *fmt;
	va_list args;
{
	FILE fakebuf;

	fakebuf._flag = _IOSTRG;	/* no _IOWRT: avoid stdio bug */
	fakebuf._ptr = s;
	fakebuf._cnt = n-1;
	_doprnt(fmt, args, &fakebuf);
	fakebuf._cnt++;
	putc('\0', &fakebuf);
	if (fakebuf._cnt<0)
	    fakebuf._cnt = 0;
	return (n-fakebuf._cnt-1);
}
