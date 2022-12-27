# Makefile for boggle.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	boggle.c comp.c

OBJS1=	boggle.o

OBJS2=	comp.o

all:	boggle bogdict .depend

boggle: ${OBJS1}
	cc -o $@ ${OBJS1}

comp:	${OBJS2}
	cc -o $@ ${OBJS2}

bogdict: comp sfile
	-if [ -f /usr/local/lib/w2a ]; \
	  then dict=/usr/local/lib/w2a; \
	  else dict=/usr/dict/words; fi; \
	sed -f sfile $$dict | uniq | ./comp >bogdict

install: FRC
	install -c -s -o games -g bin -m 500 boggle ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f boggle; ln -s dm boggle
	install -c -o games -g bin -m 400 bogdict ${DESTDIR}/usr/games/lib

install.man: FRC
	install -c -o bin -g bin -m 444 boggle.man ${DESTDIR}/usr/man/man6/boggle.6

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} boggle comp

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
