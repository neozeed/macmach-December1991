# Makefile for ppt.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	ppt.c

OBJS=	ppt.o

all:	ppt .depend

ppt: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 ppt ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f ppt; ln -s dm ppt

install.man: FRC
	@echo "*** need man pages for ppt"
#	install -c -o bin -g bin -m 444 ppt.man ${DESTDIR}/usr/man/man6/ppt.6

clean:	FRC
	rm -f .depend ${OBJS} ppt

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
