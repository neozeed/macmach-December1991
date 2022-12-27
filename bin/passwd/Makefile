# Makefile for passwd (chfn, chsh).

PROG=	passwd

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o root -g bin -m 4555

CFLAGS= -O -MD

SRCS=	${PROG}.c

OBJS=	${PROG}.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	rm -f ${BINDIR}/chfn
	ln -s ${PROG} ${BINDIR}/chfn
	rm -f ${BINDIR}/chsh
	ln -s ${PROG} ${BINDIR}/chsh

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}
	rm -f ${MANDIR}/chfn.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/chfn.${MANEXT}
	rm -f ${MANDIR}/chsh.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/chsh.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
