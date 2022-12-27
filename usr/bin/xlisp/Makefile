# Makefile for xlisp

CFLAGS=	-g -O

DOC=	XlispOOP.doc XlispRef.doc xlisp.doc
MANDOC=	${DESTDIR}/usr/man/doc

SRCS=	unixstuff.c xlbfun.c xlcont.c xldbug.c xldmem.c xleval.c xlfio.c \
	xlftab.c xlglob.c xlimage.c xlinit.c xlio.c xlisp.c xljump.c \
	xllist.c xlmath.c xlobj.c xlpp.c xlprin.c xlread.c xlstr.c \
	xlstruct.c xlsubr.c xlsym.c xlsys.c osfun.c

OBJS=	unixstuff.o xlbfun.o xlcont.o xldbug.o xldmem.o xleval.o xlfio.o \
	xlftab.o xlglob.o xlimage.o xlinit.o xlio.o xlisp.o xljump.o \
	xllist.o xlmath.o xlobj.o xlpp.o xlprin.o xlread.o xlstr.o \
	xlstruct.o xlsubr.o xlsym.o xlsys.o osfun.o

all:	xlisp

xlisp:	${OBJS}
	cc ${OBJS} -o xlisp -lcurses -ltermcap

install: FRC
	install -c -s -o bin -g bin -m 755 xlisp ${DESTDIR}/usr/bin

install.man: FRC
	-[ -d ${MANDOC}/xlisp ] || mkdir ${MANDOC}/xlisp
	install -c -o bin -g bin -m 444 ${DOC} ${MANDOC}/xlisp

clean:	FRC
	rm -f .depend ${OBJS} core log xlisp

xlftab.o: osdefs.h osptrs.h

.depend: Makefile ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
