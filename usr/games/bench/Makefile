# Makefile for bench.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	bench.c

OBJS=	bench.o

LIBS=	-lmach

all:	bench .depend

bench: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 bench ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f bench; ln -s dm bench

install.man: FRC
	@echo "*** need man pages for bench"
#	install -c -o bin -g bin -m 444 bench.man ${DESTDIR}/usr/man/man6/bench.6

clean:	FRC
	rm -f .depend ${OBJS} bench

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
