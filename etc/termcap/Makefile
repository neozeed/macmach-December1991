# Makefile for termcap.

DESTDIR=

all:	termcap

termcap: reorder termcap.src
	ex - termcap.src < reorder

install:
	install -c -o bin -g bin -m 444 map3270 ${DESTDIR}/etc/map3270
	install -c -o bin -g staff -m 444 termcap ${DESTDIR}/etc/termcap
	rm -rf ${DESTDIR}/usr/lib/tabset
	mkdir ${DESTDIR}/usr/lib/tabset
	chown bin.bin ${DESTDIR}/usr/lib/tabset
	chmod 555 ${DESTDIR}/usr/lib/tabset
	install -c -o bin -g bin -m 444 tabset/* ${DESTDIR}/usr/lib/tabset

install.man:
	install -c -o bin -g bin -m 444 termcap.man ${DESTDIR}/usr/man/man5/termcap.5
	install -c -o bin -g bin -m 444 map3270.man ${DESTDIR}/usr/man/man5/map3270.5

clean:
	rm -f termcap
