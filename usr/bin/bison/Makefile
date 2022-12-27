# Makefile for bison.

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin
LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}
INFODIR= $(DESTDIR)/usr/man/info

INFO=	bison.info bison.info-1 bison.info-2 bison.info-3 bison.info-4

PFILES=	-DXPFILE=\\\"${LIBDIR}/bison.simple\\\" \
	-DXPFILE1=\\\"${LIBDIR}/bison.hairy\\\"

all:
	make -f Makefile.orig PFILES="${PFILES}" bison

install:
	install -c -s -o bin -g bin -m 555 bison ${BINDIR}/bison
	awk -f mergedir.awk dir=${LIBDIR} bison.simple >bison.simple.rm
	install -o bin -g bin -m 444 bison.simple.rm ${LIBDIR}/bison.simple
	install -c -o bin -g bin -m 444 bison.hairy ${LIBDIR}/bison.hairy

install.man:
	install -c -o bin -g bin -m 444 bison.1 ${MANDIR}/bison.${MANEXT}
	install -c -o bin -g bin -m 444 ${INFO} ${INFODIR}

clean:
	make -f Makefile.orig clean
