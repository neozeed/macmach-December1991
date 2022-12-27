# Makefile for gprof.

PROG=	gprof

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CSU=	../../../lib/libc/${target_machine}/csu

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	gprof.c arcs.c dfn.c lookup.c ${target_machine}.c hertz.c \
	printgprof.c printlist.c

HGENS=	gcrt0.h

OBJS=	gprof.o arcs.o dfn.o lookup.o ${target_machine}.o hertz.o \
	printgprof.o printlist.o

all:	${PROG} .depend

${PROG}: ${HGENS} ${OBJS}
	cc -o $@ ${OBJS}

gcrt0.h:
	ln -s ${CSU}/gmon.h gcrt0.h

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 gprof.flat gprof.callg ${LIBDIR}/

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${HGENS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
