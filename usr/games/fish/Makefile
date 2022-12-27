# Makefile for fish.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	fish.c

OBJS=	fish.o

all:	fish .depend

fish: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 fish ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f fish; ln -s dm fish

install.man: FRC
	install -c -o bin -g bin -m 444 fish.man ${DESTDIR}/usr/man/man6/fish.6

clean:	FRC
	rm -f .depend ${OBJS} fish

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
