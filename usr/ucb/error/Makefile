# Makefile for error.

PROG=	error

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -DDEBUG

IFLAGS=	-o bin -g bin -m 555

SRCS=	errormain.c errorinput.c errorpi.c errorsubr.c errorfilter.c \
	errortouch.c

OBJS=	errormain.o errorinput.o errorpi.o errorsubr.o errorfilter.o \
	errortouch.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
