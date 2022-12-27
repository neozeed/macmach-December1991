# Makefile for worm.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	worm.c

OBJS=	worm.o

LIBS=	-lcurses -ltermcap

all:	worm .depend

worm: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 worm ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f worm; ln -s dm worm

install.man: FRC
	install -c -o bin -g bin -m 444 worm.man ${DESTDIR}/usr/man/man6/worm.6

clean:	FRC
	rm -f .depend ${OBJS} worm

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
