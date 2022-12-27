# Makefile for canfield.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	canfield.c cfscores.c

OBJS1=	canfield.o

OBJS2=	cfscores.o

LIBS=	-lcurses -ltermcap

all:	canfield cfscores .depend

canfield: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

cfscores: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s -o games -g bin -m 500 canfield ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f canfield; ln -s dm canfield
	install -c -s -o games -g bin -m 500 cfscores ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f cfscores; ln -s dm cfscores
	install -c -o games -g bin -m 444 /dev/null ${DESTDIR}/etc/newconfig/cfscores

install.man: FRC
	install -c -o bin -g bin -m 444 canfield.man ${DESTDIR}/usr/man/man6/canfield.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} canfield cfscores

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
