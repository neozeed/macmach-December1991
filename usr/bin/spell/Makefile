# Makefile for spell.

SPECIAL= special.4bsd

CFLAGS=	-O -MD

SRCS=	spell.c spellin.c spellout.c

OBJS=	spell.o spellin.o spellout.o

all:	spell hlist hlista hlistb hstop spellin spellout .depend

spell:	spell.c
	cc ${CFLAGS} spell.c -o $@

spellin: spellin.c
	cc ${CFLAGS} spellin.c -o $@

spellout: spellout.c
	cc ${CFLAGS} spellout.c -o $@

hlist:	words spellin
	./spellin < words >hlist

hlista: american ${SPECIAL} ${LOCAL} hlist spellin
	(cat american ${SPECIAL} ${LOCAL}) | ./spellin hlist > hlista

hlistb: british ${SPECIAL} ${LOCAL} hlist spellin
	(cat british ${SPECIAL} ${LOCAL}) | ./spellin hlist > hlistb

hstop: stop spellin
	./spellin < stop >hstop

install: FRC
	-[ -d ${DESTDIR}/usr/dict/special ] || mkdir ${DESTDIR}/usr/dict/special
	install -c -s -o bin -g bin -m 755 spell ${DESTDIR}/usr/lib/spell
	install -c -s -o bin -g bin -m 755 spellin ${DESTDIR}/usr/bin/spellin
	install -c -s -o bin -g bin -m 755 spellout ${DESTDIR}/usr/bin/spellout
	install -c -o bin -g bin -m 755 spell.sh ${DESTDIR}/usr/bin/spell
	install -c -o bin -g bin -m 444 hlista ${DESTDIR}/usr/dict/hlista
	install -c -o bin -g bin -m 444 hlistb ${DESTDIR}/usr/dict/hlistb
	install -c -o bin -g bin -m 444 hstop ${DESTDIR}/usr/dict/hstop
	install -c -o bin -g bin -m 444 hlist ${DESTDIR}/usr/dict/hlist
	install -c -o bin -g bin -m 444 words ${DESTDIR}/usr/dict/words
	install -c -o bin -g bin -m 444 american ${DESTDIR}/usr/dict/american
	install -c -o bin -g bin -m 444 british ${DESTDIR}/usr/dict/british
	install -c -o bin -g bin -m 444 stop ${DESTDIR}/usr/dict/stop
	install -c -o bin -g bin -m 444 README ${DESTDIR}/usr/dict/README
	install -c -o bin -g bin -m 444 special.4bsd ${DESTDIR}/usr/dict/special/4bsd
	install -c -o bin -g bin -m 444 special.math ${DESTDIR}/usr/dict/special/math

install.man: FRC
	install -c -o bin -g bin -m 444 spell.man ${DESTDIR}/usr/man/man1/spell.1
	rm -f ${DESTDIR}/usr/man/man1/spellin.1
	ln -s spell.1 ${DESTDIR}/usr/man/man1/spellin.1
	rm -f ${DESTDIR}/usr/man/man1/spellout.1
	ln -s spell.1 ${DESTDIR}/usr/man/man1/spellout.1

clean:	FRC
	rm -f .depend ${OBJS} core spell spellin hlista hlistb hlist hstop spellout

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
