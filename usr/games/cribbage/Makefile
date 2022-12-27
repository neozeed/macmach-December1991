# Makefile for cribbage.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	extern.c crib.c support.c cards.c score.c io.c

OBJS1=	extern.o crib.o support.o cards.o score.o io.o

OBJS2=	test.o cards.o score.o io.o extern.o

LIBS=	-lcurses -ltermlib

all:	cribbage crib.instr .depend

cribbage: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

crib.instr: cribbage.n macro
	nroff cribbage.n > crib.instr

crib.test: ${OBJS2}
	cc -o $@ ${OBJS2} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 cribbage ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f cribbage; ln -s dm cribbage
	install -c -o bin -g bin -m 400 crib.instr ${DESTDIR}/usr/games/lib

install.man: FRC
	install -c -o bin -g bin -m 444 cribbage.man ${DESTDIR}/usr/man/man6/cribbage.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} cribbage crib.test

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
