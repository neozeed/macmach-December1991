# Makefile for factor.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	factor.c

OBJS=	factor.o

all:	factor .depend

factor: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 factor ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f factor; ln -s dm factor

install.man: FRC
	@echo "*** need man page for factor"
#	install -c -o bin -g bin -m 444 factor.man ${DESTDIR}/usr/man/man6/factor.6

clean:	FRC
	rm -f .depend ${OBJS} factor

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
