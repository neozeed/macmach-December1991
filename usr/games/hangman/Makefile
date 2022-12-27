# Makefile for hangman.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	endgame.c extern.c getguess.c getword.c main.c playgame.c \
	prdata.c prman.c prword.c setup.c

OBJS=	endgame.o extern.o getguess.o getword.o main.o playgame.o \
	prdata.o prman.o prword.o setup.o

LIBS=	-lcurses -ltermlib

all:	hangman .depend

hangman: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 hangman ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f hangman; ln -s dm hangman

install.man: FRC
	install -c -o bin -g bin -m 444 hangman.man ${DESTDIR}/usr/man/man6/hangman.6

clean:	FRC
	rm -f .depend ${OBJS} hangman

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
