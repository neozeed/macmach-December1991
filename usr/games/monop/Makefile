# /usr/src/games/monop/Makefile for MacMach

# Nifty foo monopoly maker
#	Program:  Ken Arnold
#	Makefile:  K.S.
#
CFLAGS=	-O -MD

SRCS=	monop.c cards.c execute.c getinp.c houses.c jail.c misc.c morg.c \
	print.c prop.c rent.c roll.c spec.c trade.c initdeck.c

OBJS1=	monop.o cards.o execute.o getinp.o houses.o jail.o misc.o morg.o \
	print.o prop.o rent.o roll.o spec.o trade.o

OBJS2=	initdeck.o

all:	monop cards.pck .depend

monop:	${OBJS1}
	cc -o $@ ${OBJS1}

initdeck: ${OBJS2}
	cc -o $@ ${OBJS2}

cards.pck: initdeck
	./initdeck

install: FRC
	install -c -o games -g bin -m 400 cards.pck ${DESTDIR}/usr/games/lib
	install -c -s -o games -g bin -m 500 monop ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f monop; ln -s dm monop

install.man: FRC
	@echo "*** need man pages for monop"

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} monop initdeck cards.pck

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
