# Makefile for htable.

PROG=	htable

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

YFLAGS=	-d

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	htable.c parse.c scan.c

CGENS=	parse.c scan.c

HGENS=	y.tab.h

OBJS=	htable.o parse.o scan.o

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS}

y.tab.h: parse.y
	yacc -d parse.y
	rm -f y.tab.c

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${CGENS} ${HGENS} ${PROG}

.depend: ${SRCS} ${CGENS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS} ${CGENS}

-include .depend

FRC:
