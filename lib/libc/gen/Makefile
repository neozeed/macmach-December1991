# Makefile for libc gen.

VPATH=	regexp

DEFS=

CFLAGS=	-O -MD ${DEFS}

STDSRC=	abort.c alarm.c atoi.c atol.c calloc.c closedir.c crypt.c ctime.c \
	ctype_.c disklabel.c errlst.c execvp.c fakcu.c fstab.c \
	getenv.c getgrent.c getgrgid.c getgrnam.c getlogin.c getpass.c \
	getpwent.c getpwnamuid.c getttyent.c getttynam.c getusershell.c \
	getwd.c initgroups.c isatty.c malloc.c mktemp.c ndbm.c nlist.c \
	opendir.c perror.c popen.c psignal.c putenv.c qsort.c random.c \
	readdir.c regerror.c regex.c regexp.c regsub.c rewinddir.c scandir.c \
	seekdir.c setegid.c setenv.c seteuid.c setgid.c setjmperr.c \
	setrgid.c setruid.c setuid.c siginterrupt.c siglist.c signal.c \
	sleep.c strcasecmp.c strdup.c swab.c syslog.c system.c telldir.c \
	time.c timezone.c ttyname.c ttyslot.c ualarm.c usleep.c valloc.c

STDOBJ=	abort.o alarm.o atoi.o atol.o calloc.o closedir.o crypt.o ctime.o \
	ctype_.o disklabel.o errlst.o execvp.o fakcu.o fstab.o \
	getenv.o getgrent.o getgrgid.o getgrnam.o getlogin.o getpass.o \
	getpwent.o getpwnamuid.o getttyent.o getttynam.o getusershell.o \
	getwd.o initgroups.o isatty.o malloc.o mktemp.o ndbm.o nlist.o \
	opendir.o perror.o popen.o psignal.o putenv.o qsort.o random.o \
	readdir.o regerror.o regex.o regexp.o regsub.o rewinddir.o scandir.o \
	seekdir.o setegid.o setenv.o seteuid.o setgid.o setjmperr.o \
	setrgid.o setruid.o setuid.o siginterrupt.o siglist.o signal.o \
	sleep.o strcasecmp.o strdup.o swab.o syslog.o system.o telldir.o \
	time.o timezone.o ttyname.o ttyslot.o ualarm.o usleep.o valloc.o

MAC2SRC=

MAC2OBJ=

VAXSRC=

VAXOBJ=

TAHOESRC=index.c rindex.c

TAHOEOBJ=index.o rindex.o

PORTSRC=bcmp.c bcopy.c bzero.c ffs.c index.c insque.c \
	remque.c rindex.c strcat.c strcmp.c strcpy.c strlen.c \
	strncat.c strncmp.c strncpy.c

PORTOBJ=bcmp.o bcopy.o bzero.o ffs.o index.o insque.o \
	remque.o rindex.o strcat.o strcmp.o strcpy.o strlen.o \
	strncat.o strncmp.o strncpy.o

TAGSFILE=tags

all:	lib.${MACHINE} .depend

link:	all link.${MACHINE}

.c.o:
	@cc -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

syslog.o:
	@cc -traditional -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc -traditional ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

link.mac2: FRC
	(cd ../library; rm -f  ${STDOBJ} ${MAC2OBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${MAC2OBJ})
	cp  ${STDOBJ} ${MAC2OBJ} ../library
	(cd profiled; cp  ${STDOBJ} ${MAC2OBJ} ../../profiled)

link.vax: FRC
	(cd ../library; rm -f  ${STDOBJ} ${VAXOBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${VAXOBJ})
	cp  ${STDOBJ} ${VAXOBJ} ../library
	(cd profiled; cp  ${STDOBJ} ${VAXOBJ} ../../profiled)

link.tahoe: FRC
	(cd ../library; rm -f  ${STDOBJ} ${TAHOEOBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${TAHOEOBJ})
	cp  ${STDOBJ} ${TAHOEOBJ} ../library
	(cd profiled; cp  ${STDOBJ} ${TAHOEOBJ} ../../profiled)

lib.mac2: ${STDOBJ} ${MAC2OBJ}

lib.vax: ${STDOBJ} ${VAXOBJ}

lib.tahoe: ${STDOBJ} ${TAHOEOBJ}

errlst.o: errlst.c
	cc -S ${DEFS} errlst.c
	ed - <:errfix errlst.s
	as -o errlst.o errlst.s
	cp errlst.o profiled/errlst.o
	rm -f errlst.s

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags: tags.${MACHINE}

tags.mac2: ${STDSRC} ${MAC2SRC}
	cwd=`pwd`; \
	for i in ${STDSRC} ${MAC2SRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.vax: ${STDSRC} ${VAXSRC}
	cwd=`pwd`; \
	for i in ${STDSRC} ${VAXSRC}; do \
		ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.tahoe: ${STDSRC} ${TAHOESRC}
	cwd=`pwd`; \
	for i in ${STDSRC} ${TAHOESRC}; do \
		ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

.depend: depend.${MACHINE}

depend.mac2: ${STDSRC} ${MAC2SRC}
	md -d -f -u .depend *.d 2>/dev/null

depend.vax: ${STDSRC} ${VAXSRC}
	md -d -f -u .depend *.d 2>/dev/null

depend.tahoe: ${STDSRC} ${TAHOESRC}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
