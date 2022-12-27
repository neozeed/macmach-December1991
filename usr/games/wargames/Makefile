# Makefile for wargames

DESTDIR=

all:	FRC

install: FRC
	install -c -o games -g bin -m 500 wargames.sh \
	  ${DESTDIR}/usr/games/hide/wargames
	cd ${DESTDIR}/usr/games; rm -f wargames; ln -s dm wargames

install.man: FRC
	@echo "*** need man pages for wargames"
#	install -c -o bin -g bin -m 444 wargames.man ${DESTDIR}/usr/man/man6/wargames.6

clean:	FRC

FRC:
