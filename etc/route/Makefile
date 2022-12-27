# Makefile for route.

PROG=	route

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c

OBJS=	${PROG}.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
