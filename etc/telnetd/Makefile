# Makefile for telnetd.

PROG=	telnetd

DESTDIR=

GETTY=	../getty

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c

GENS=	get_date.c gettytab.c

OBJS=	${PROG}.o get_date.o gettytab.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

get_date.c: ${GETTY}/get_date.c
	ln -s ${GETTY}/get_date.c get_date.c

gettytab.c: ${GETTY}/gettytab.c
	ln -s ${GETTY}/gettytab.c gettytab.c

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${GENS}

.depend: ${SRCS} ${GENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
