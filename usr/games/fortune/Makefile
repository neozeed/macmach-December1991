# Makefile for fortune.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	fortune.c rnd.c strfile.c

OBJS1=	fortune.o rnd.o

OBJS2=	strfile.o rnd.o

all:	fortune fortunes.dat .depend

fortune: ${OBJS1}
	cc -o $@ ${OBJS1}


sort: sort.scene sort.obscene

strfile: ${OBJS2}
	cc -o $@ ${OBJS2}

unstr:	unstr.c
	cc -o $@ unstr.c

sort.scene: strfile unstr
	strfile -oi scene
	mv scene Oscene
	unstr -o scene

sort.obscene: strfile unstr
	strfile -oi obscene
	mv obscene Oobscene
	unstr -o obscene

fortunes: scene obscene
	(cat scene; echo "%-"; cat obscene) > fortunes

fortunes.dat: fortunes strfile
	./strfile -r fortunes

install: FRC
	install -c -s -o games -g bin -m 500 fortune ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f fortune; ln -s dm fortune
	install -c -o bin -g bin -m 400 fortunes.dat \
	  ${DESTDIR}/usr/games/lib/fortunes.dat

install.man: FRC
	install -c -o bin -g bin -m 444 fortune.man ${DESTDIR}/usr/man/man6/fortune.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} fortune fortunes.dat strfile

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
