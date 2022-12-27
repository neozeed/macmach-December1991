# Makefile for chown.

PROG1=	chown
PROG2=	chgrp

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 555

SRCS=	${PROG1}.c

OBJS=	${PROG1}.o

all:	${PROG1} .depend

${PROG1}: ${OBJS}
	cc -o $@ ${OBJS}

install:
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	rm -f ${BINDIR}/${PROG2}; ln -s ${PROG1} ${BINDIR}/${PROG2}

install.man:
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG1}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
