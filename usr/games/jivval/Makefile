# Makefile for jive and valspeak.

DESTDIR=

CFLAGS= -O

all:	jive valspeak

valspeak: valspeak.l
	lex valspeak.l
	cc ${CFLAGS} -o valspeak lex.yy.c -ll
	rm -f lex.yy.c

jive:	jive.l
	lex jive.l
	cc ${CFLAGS} -o jive lex.yy.c -ll
	rm -f lex.yy.c

install: FRC
	install -c -s -o games -g bin -m 500 jive ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f jive; ln -s dm jive
	install -c -s -o games -g bin -m 500 valspeak ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f valspeak; ln -s dm valspeak

install.man: FRC
	install -c -o bin -g bin -m 444 valspeak.man ${DESTDIR}/usr/man/man6/valspeak.6
	install -c -o bin -g bin -m 444 jive.man ${DESTDIR}/usr/man/man6/jive.6

clean:	jive.l valspeak.l
	rm -f jive valspeak

FRC:
