# Makefile for rogue.

CFLAGS=	-O -MD -DUNIX -DUNIX_BSD4_2

SRCS=	curses.c hit.c init.c inventory.c level.c machdep.c main.c \
	message.c monster.c move.c object.c pack.c play.c random.c ring.c \
	room.c save.c score.c spec_hit.c throw.c trap.c use.c zap.c

OBJS=	curses.o hit.o init.o inventory.o level.o machdep.o main.o \
	message.o monster.o move.o object.o pack.o play.o random.o ring.o \
	room.o save.o score.o spec_hit.o throw.o trap.o use.o zap.o

all:	rogue .depend

rogue:	${OBJS}
	cc -o $@ ${OBJS} -lcurses -ltermlib

clean:	FRC
	rm -f .depend ${OBJS} rogue

install: FRC
	install -c -s -o games -g bin -m 500 rogue ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f rogue; ln -s dm rogue

install.man: FRC
	@echo "*** need man pages for rogue"

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
