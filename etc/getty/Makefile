# Makefile for getty.

PROG=	getty

CFGS=	gettytab

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

USRETC=${DESTDIR}/usr/etc

CFGDIR=	${DESTDIR}/etc/newconfig

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o root -g bin -m 544

CFLAGS= -O

SRCS=	main.c init.c subr.c get_date.c gettytab.c

OBJS=	main.o init.o subr.o get_date.o gettytab.o

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
