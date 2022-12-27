# Makefile for number.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	number.c

OBJS=	number.o

all:	number .depend

number: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 number ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f number; ln -s dm number

install.man: FRC
	install -c -o bin -g bin -m 444 number.man ${DESTDIR}/usr/man/man6/number.6

clean:	FRC
	rm -f .depend ${OBJS} number

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
