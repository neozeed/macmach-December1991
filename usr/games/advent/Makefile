# Makefile for advent.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	done.c init.c io.c main.c save.c subr.c vocab.c wizard.c

OBJS=	done.o init.o io.o main.o save.o subr.o vocab.o wizard.o

all:	advent .depend

advent: ${OBJS}
	cc -n ${OBJS}
	rm -f $@
	echo y | ./a.out

install: FRC
	install -c -s -o games -g bin -m 500 advent ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f advent; ln -s dm advent

install.man: FRC
	install -c -o bin -g bin -m 444 advent.man ${DESTDIR}/usr/man/man6/advent.6

clean:	FRC
	rm -f .depend ${OBJS} advent

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
