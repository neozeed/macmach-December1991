# Makefile for btlgammon.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	btlgammon.c

OBJS=	btlgammon.o

all:	btlgammon .depend

btlgammon: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 btlgammon ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f btlgammon; ln -s dm btlgammon

install.man: FRC
	@echo "*** need man pages for btlgammon"
#	install -c -o bin -g bin -m 444 btlgammon.man ${DESTDIR}/usr/man/man6/btlgammon.6

clean:	FRC
	rm -f .depend ${OBJS} btlgammon

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
