# Makefile for macintosh emulator

all:	FRC

install: FRC
	install -c -o bin -g bin -m 555 macintosh.sh ${DESTDIR}/usr/bin/macintosh
	install -c -o bin -g bin -m 555 macserver ${DESTDIR}/usr/lib/macserver
	install -c -o bin -g bin -m 444 macpatches ${DESTDIR}/usr/lib/macpatches

install.man: FRC

clean:	FRC

FRC:
