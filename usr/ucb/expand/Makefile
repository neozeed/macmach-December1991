# Makefile for expand.

PROG1=	expand
PROG2=	unexpand

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG1}.c ${PROG2}.c

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

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
