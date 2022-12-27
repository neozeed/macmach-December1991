# Makefile for rain.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	rain.c

OBJS=	rain.o

LIBS=	-ltermcap

all:	rain .depend

rain: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 500 rain ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f rain; ln -s dm rain

install.man: FRC
	install -c -o bin -g bin -m 444 rain.man ${DESTDIR}/usr/man/man6/rain.6

clean:	FRC
	rm -f .depend ${OBJS} rain

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
