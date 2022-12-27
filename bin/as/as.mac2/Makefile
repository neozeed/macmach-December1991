# Makefile for mac2 as.

PROG= as

DESTDIR=

MANEXT=1

BINDIR=${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

all:	../gas/a68

../gas/a68:
	cd ../gas; make G0="-I. -O" O3="-Dm68851" a68

install:
	install -c -s ${IFLAGS} ../gas/a68 ${BINDIR}/gas
	rm -f ${BINDIR}/${PROG}
	ln -s gas ${BINDIR}/${PROG}

install.man:
	@echo "*** need man page for mac2 as"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	../gas
	cd ../gas; make clean
