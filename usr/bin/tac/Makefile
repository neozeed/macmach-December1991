# Makefile for tac.

PROG1=	tac
PROG2=	tmail

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG1}.c

OBJS1=	${PROG1}.o

all:	${PROG1} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c ${IFLAGS} ${PROG2}.sh ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
