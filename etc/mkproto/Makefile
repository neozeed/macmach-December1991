# Makefile for mkproto.

PROG=	mkproto

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c

OBJS=	${PROG}.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
