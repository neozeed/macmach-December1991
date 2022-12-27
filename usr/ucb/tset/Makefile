# Makefile for tset.

PROG1=	tset
PROG2=	reset

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG1}.c

OBJS1=	${PROG1}.o

LIB=	-ltermcap

all:	${PROG1} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIB}

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	rm -f ${BINDIR}/${PROG2}; ln -s ${PROG1} ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${PROG1}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
