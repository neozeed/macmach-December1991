# Makefile for robots.

DESTDIR=

CFLAGS= -O -MD -DMAX_PER_UID=5

SRCS=	extern.c init_field.c main.c make_level.c move.c move_robs.c \
	play_level.c query.c rnd_pos.c score.c flush_in.c

OBJS=	extern.o init_field.o main.o make_level.o move.o move_robs.o \
	play_level.o query.o rnd_pos.o score.o flush_in.o

LIBS=	-lcurses -ltermlib

all:	robots .depend

robots: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 robots ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f robots; ln -s dm robots
	install -c -o games -g bin -m 444 /dev/null ${DESTDIR}/etc/newconfig/robots_roll

install.man: FRC
	install -c -o bin -g bin -m 444 robots.man ${DESTDIR}/usr/man/man6/robots.6

clean:	FRC
	rm -f .depend ${OBJS} robots

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
