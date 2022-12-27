# Makefile for libc stdio.

# Many of these routines have been rewritten in assembly.  The assembly
# source can be found in the directory stdio.${MACHINE}.  The macro
# ${STDSRC} lists the files needed by all current implementations.  The
# macros ${VAXSRC} and ${TAHOESRC} are the supplemental files needed by
# the VAX and the TAHOE.  ${PORTSRC} is a list of all of the supplemental
# files.

DEFS=

CFLAGS=	-O -MD ${DEFS}

STDSRC=	clrerr.c doprnt.c doscan.c exit.c fdopen.c fgetc.c filbuf.c \
	findiop.c flsbuf.c fopen.c fprintf.c fputc.c fread.c freopen.c \
	fseek.c ftell.c fwrite.c getchar.c getw.c printf.c putchar.c \
	putw.c rew.c scanf.c setbuf.c setbuffer.c sprintf.c ungetc.c \
	vfprintf.c vprintf.c vsprintf.c

STDOBJ=	clrerr.o doprnt.o doscan.o exit.o fdopen.o fgetc.o filbuf.o \
	findiop.o flsbuf.o fopen.o fprintf.o fputc.o fread.o freopen.o \
	fseek.o ftell.o fwrite.o getchar.o getw.o printf.o putchar.o \
	putw.o rew.o scanf.o setbuf.o setbuffer.o sprintf.o ungetc.o \
	vfprintf.o vprintf.o vsprintf.o

MAC2SRC=${PORTSRC}

MAC2OBJ=${PORTOBJ}

VAXSRC=

VAXOBJ=

TAHOESRC=fgets.c fputs.c gets.c puts.c

TAHOEOBJ=fgets.o fputs.o gets.o puts.o

PORTSRC=fgets.c fputs.c gets.c puts.c

PORTOBJ=fgets.o fputs.o gets.o puts.o

TAGSFILE=tags

all:	lib.${MACHINE} .depend

link:	all link.${MACHINE}

.c.o:
	@cc -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

link.mac2: FRC
	(cd ../library; rm -f  ${STDOBJ} ${MAC2OBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${MAC2OBJ})
	cp ${STDOBJ} ${MAC2OBJ} ../library
	(cd profiled; cp ${STDOBJ} ${MAC2OBJ} ../../profiled)

link.vax: FRC
	(cd ../library; rm -f  ${STDOBJ} ${VAXOBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${VAXOBJ})
	cp ${STDOBJ} ${VAXOBJ} ../library
	(cd profiled; cp ${STDOBJ} ${VAXOBJ} ../../profiled)

link.tahoe: FRC
	(cd ../library; rm -f  ${STDOBJ} ${TAHOEOBJ})
	(cd ../profiled; rm -f  ${STDOBJ} ${TAHOEOBJ})
	cp ${STDOBJ} ${TAHOEOBJ} ../library
	(cd profiled; cp ${STDOBJ} ${TAHOEOBJ} ../../profiled)

lib.mac2: ${STDOBJ} ${MAC2OBJ}

lib.vax: ${STDOBJ} ${VAXOBJ}

lib.tahoe: ${STDOBJ} ${TAHOEOBJ}

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags: tags.${MACHINE}

tags.mac2: FRC
	cwd=`pwd`; \
	for i in ${STDSRC} ${MAC2SRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.vax: FRC
	cwd=`pwd`; \
	for i in ${STDSRC} ${VAXSRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.tahoe: FRC
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
