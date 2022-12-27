# Makefile for diff3.

PROG=	diff3

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

LIBDIR=	${DESTDIR}/usr/lib

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG}.c

OBJS=	${PROG}.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${LIBDIR}/${PROG}
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
