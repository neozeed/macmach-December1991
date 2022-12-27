# Makefile for man.

PROG1=	man
PROG2=	apropos
PROG3=	whatis

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	man.c apropos.c

OBJS1=	${PROG1}.o

OBJS2=	${PROG2}.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${BINDIR}/${PROG2}
	rm -f ${BINDIR}/${PROG3}; ln -s ${PROG2} ${BINDIR}/${PROG3}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG3}.man \
	  ${MANDIR}/${PROG3}.${MANEXT}

clean:	FRC
	rm -f .depend ${PROG1} ${PROG2}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
