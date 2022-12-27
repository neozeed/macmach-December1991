# Makefile for find.

PROG1=	find
PROG2=	bigram
PROG3=	code

DESTDIR=

MANEXT=	1

BINDIR=${DESTDIR}/usr/bin

LIBDIR=${DESTDIR}/usr/lib/find

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG1}.c ${PROG2}.c ${PROG3}.c

OBJS1=	${PROG1}.o

OBJS2=	${PROG2}.o

OBJS3=	${PROG3}.o

all:	${PROG1} ${PROG2} ${PROG3} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

${PROG3}: ${OBJS3}
	cc -o $@ ${OBJS3}

install: FRC
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	chown bin.bin ${LIBDIR}
	chmod 555 ${LIBDIR}
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${LIBDIR}/${PROG2}
	install -c -s ${IFLAGS} ${PROG3} ${LIBDIR}/${PROG3}
	install -c ${IFLAGS} updatedb.csh ${LIBDIR}updatedb

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${OBJS3} ${PROG1} ${PROG2} ${PROG3}

.depend: ${SRC}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
