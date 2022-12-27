# Makefile for dm.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	dm.c

OBJS=	dm.o

all:	dm .depend

dm: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g kmem -m 6555 dm ${DESTDIR}/usr/games

install.man: FRC
	install -c -o bin -g bin -m 444 dm.man ${DESTDIR}/usr/man/man6/dm.6

clean:	FRC
	rm -f .depend ${OBJS} dm

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
