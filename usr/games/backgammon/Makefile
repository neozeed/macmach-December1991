# Makefile for backgammon.

DESTDIR=

CFLAGS=	-O -MD -DV7

SRCS=	allow.c board.c check.c data.c extra.c fancy.c init.c main.c move.c \
	odds.c one.c save.c subs.c table.c teach.c text.c ttext1.c ttext2.c \
	tutor.c version.c

OBJS1=	allow.o board.o check.o extra.o fancy.o init.o main.o move.o \
	odds.o one.o save.o subs.o table.o text.o version.o

OBJS2=	allow.o board.o check.o data.o fancy.o init.o odds.o one.o save.o \
	subs.o table.o teach.o ttext1.o ttext2.o tutor.o

LIBS=	-ltermlib

all:	backgammon teachgammon .depend

backgammon: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

teachgammon: ${OBJS2}
	cc -o $@ ${OBJS2} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 backgammon ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f backgammon; ln -s dm backgammon
	install -c -s -o games -g bin -m 500 teachgammon ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f teachgammon; ln -s dm teachgammon

install.man: FRC
	install -c -o bin -g bin -m 444 backgammon.man ${DESTDIR}/usr/man/man6/backgammon.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} backgammon teachgammon

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
