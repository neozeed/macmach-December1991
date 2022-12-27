# Makefile for ipc_test.

PROG=	ipc_test

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	main.c mtest.c ntest.c ptest.c utest.c nstest.c stest.c utils.c

OBJS=	main.o mtest.o ntest.o ptest.o utest.o nstest.o stest.o utils.o

LIBS=	-lmach

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	rm -f ${BINDIR}/stest; ln -s ${PROG} ${BINDIR}/stest

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
