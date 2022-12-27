# Makefile for arithmetic.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	arithmetic.c

OBJS=	arithmetic.o

all:	arithmetic .depend

arithmetic: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 arithmetic ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f arithmetic; ln -s dm arithmetic

install.man: FRC
	install -c -o bin -g bin -m 444 arithmetic.man ${DESTDIR}/usr/man/man6/arithmetic.6

clean:	FRC
	rm -f .depend ${OBJS} arithmetic

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
