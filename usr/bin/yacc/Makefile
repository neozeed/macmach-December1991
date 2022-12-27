# Makefile for yacc.

PROG=	yacc

DESTDIR=

MANEXT=1

BINDIR=${DESTDIR}/usr/bin

LIBDIR=${DESTDIR}/usr/lib

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -DWORD32

IFLAGS=	-o bin -g bin -m 555

SRCS=	y1.c y2.c y3.c y4.c

OBJS=	y1.o y2.o y3.o y4.o

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 yaccpar ${LIBDIR}/yaccpar

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
