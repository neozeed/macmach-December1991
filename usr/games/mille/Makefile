# Makefile for mille.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	comp.c end.c extern.c init.c mille.c misc.c move.c print.c \
	roll.c save.c table.c types.c varpush.c

OBJS1=	comp.o end.o extern.o init.o mille.o misc.o move.o print.o \
	roll.o save.o types.o varpush.o

OBJS2=	table.o extern.o

LIBS=	-lcurses -ltermlib

all:	mille .depend

mille: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

table:	${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s -o games -g bin -m 500 mille ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f mille; ln -s dm mille

install.man: FRC
	install -c -o bin -g bin -m 444 mille.man ${DESTDIR}/usr/man/man6/mille.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} mille table

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
