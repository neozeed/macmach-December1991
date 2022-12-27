#ifdef ibm032

#include <stdio.h>
#ifndef __STDC__
#include <varargs.h>
#else
#include <stdarg.h>
#endif

int
vsprintf(s, fmt, args)
	char *s, *fmt;
	va_list args;
{
	struct _iobuf fakebuf;

	fakebuf._flag = _IOSTRG;	/* no _IOWRT: avoid stdio bug */
	fakebuf._ptr = s;
	fakebuf._cnt = 32767;
	_doprnt(fmt, args, &fakebuf);
	putc('\0', &fakebuf);
	return (strlen(s));
}

vprintf(fmt, args)
     char *fmt;
     va_list args;
{
  _doprnt(fmt, args, stdout);
}

#endif
