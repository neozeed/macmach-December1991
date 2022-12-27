# Makefile for snake.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	snake.c move.c snscore.c

OBJS1=	snake.o move.o

OBJS2=	snscore.o

LIBS=	-lm -ltermcap

all:	snake snscore .depend

snake: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

snscore: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s -o games -g bin -m 500 snake ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f snake; ln -s dm snake
	install -c -s -o games -g bin -m 500 snscore ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f snscore; ln -s dm snscore
	install -c -o games -g bin -m 444 /dev/null \
	  ${DESTDIR}/etc/newconfig/snakerawscores

install.man: FRC
	install -c -o bin -g bin -m 444 snake.man ${DESTDIR}/usr/man/man6/snake.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} snake snscore

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
