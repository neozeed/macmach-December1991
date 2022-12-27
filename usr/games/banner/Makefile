# Makefile for banner.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	banner.c

OBJS=	banner.o

all:	banner .depend

banner: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 banner ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f banner; ln -s dm banner

install.man: FRC
	install -c -o bin -g bin -m 444 banner.man ${DESTDIR}/usr/man/man6/banner.6

clean:	FRC
	rm -f .depend ${OBJS} banner

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
