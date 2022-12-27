# Makefile for newfs.

PROG=	newfs

CFGS=	disktab

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFGDIR= ${DESTDIR}/etc/newconfig

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c mkfs.c

OBJS=	${PROG}.o mkfs.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
