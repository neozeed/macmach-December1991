# Makefile for snames, Mach 3.0 version.

PROG=	snames

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c nprocs.c

OBJS=	${PROG}.o nprocs.o

LIBS=	-lmach

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

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
