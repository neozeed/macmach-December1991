# Makefile for battlestar.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	battlestar.c com1.c com2.c com3.c com4.c com5.c com6.c com7.c \
	init.c cypher.c getcom.c parse.c room.c save.c fly.c misc.c \
	globals.c dayfile.c nightfile.c dayobjs.c nightobjs.c words.c

OBJS=	battlestar.o com1.o com2.o com3.o com4.o com5.o com6.o com7.o \
	init.o cypher.o getcom.o parse.o room.o save.o fly.o misc.o \
	globals.o dayfile.o nightfile.o dayobjs.o nightobjs.o words.o

LIBS=	-lcurses -ltermlib

all:	battlestar .depend

battlestar: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 battlestar ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f battlestar; ln -s dm battlestar
	install -c -o games -g bin -m 444 /dev/null \
	  ${DESTDIR}/etc/newconfig/battlestar.log

install.man: FRC
	install -c -o bin -g bin -m 444 battlestar.man ${DESTDIR}/usr/man/man6/battlestar.6

clean:	FRC
	rm -f .depend ${OBJS} battlestar

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
