# Makefile for atc.

DESTDIR=

LIBDIR=${DESTDIR}/usr/games/lib/atc

CFLAGS= -O -MD -DBSD -DDEST=\"${LIBDIR}/\"

YFLAGS=	-d

SRCS=	extern.c graphics.c input.c list.c log.c main.c tunable.c update.c

CGENS=	grammar.c lex.c

HGENS=	y.tab.h

OBJS=	extern.o grammar.o input.o lex.o list.o log.o main.o tunable.o \
	graphics.o update.o

LIBS=	-ll -lm -lcurses -ltermcap

all:	atc .depend

atc:	${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 atc ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f atc; ln -s dm atc
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	chmod 755 ${LIBDIR}; chown games.bin ${LIBDIR}
	cd games; install -c -o games -g bin -m 400 * ${LIBDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 atc.man ${DESTDIR}/usr/man/man6/atc.6

clean:	FRC
	rm -f .depend ${OBJS} atc ${CGENS} ${HGENS}

.depend: ${CGENS} ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
