# Makefile for trek.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	abandon.c attack.c autover.c capture.c check_out.c checkcond.c \
	compkl.c computer.c damage.c damaged.c dcrept.c destruct.c \
	dock.c dumpgame.c dumpme.c dumpssradio.c events.c externs.c \
	getcodi.c getpar.c help.c impulse.c initquad.c kill.c klmove.c \
	lose.c lrscan.c main.c move.c nova.c out.c phaser.c play.c ram.c \
	ranf.c rest.c schedule.c score.c setup.c setwarp.c shell.c \
	shield.c snova.c srscan.c systemname.c torped.c utility.c \
	visual.c warp.c win.c cgetc.c

OBJS=	abandon.o attack.o autover.o capture.o check_out.o checkcond.o \
	compkl.o computer.o damage.o damaged.o dcrept.o destruct.o \
	dock.o dumpgame.o dumpme.o dumpssradio.o events.o externs.o \
	getcodi.o getpar.o help.o impulse.o initquad.o kill.o klmove.o \
	lose.o lrscan.o main.o move.o nova.o out.o phaser.o play.o ram.o \
	ranf.o rest.o schedule.o score.o setup.o setwarp.o shell.o \
	shield.o snova.o srscan.o systemname.o torped.o utility.o \
	visual.o warp.o win.o cgetc.o

LIBS=	-lm

all:	trek .depend

trek: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 trek ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f trek; ln -s dm trek

install.man: FRC
	install -c -o bin -g bin -m 444 trek.man ${DESTDIR}/usr/man/man6/trek.6

clean:	FRC
	rm -f .depend ${OBJS} trek

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
