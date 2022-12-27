# Makefile for vgrind.

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

RSRCS=	retest.c regexp.c
ROBJS=	retest.o regexp.o
VSRCS=	regexp.c vfontedpr.c vgrindefs.c
VOBJS=	regexp.o vfontedpr.o vgrindefs.o

all:	retest vfontedpr .depend

vfontedpr: ${VOBJS}
	cc -o $@ ${VOBJS}

retest:	${ROBJS}
	cc -o $@ ${ROBJS}

install: FRC
	install -c -s ${IFLAGS} vfontedpr ${LIBDIR}/vfontedpr
	install -c ${IFLAGS} vgrind.sh ${BINDIR}/vgrind
	install -c -o bin -g bin -m 444 vgrindefs.src ${LIBDIR}/vgrindefs
	install -c -o bin -g bin -m 444 tmac.vgrind ${LIBDIR}/tmac/tmac.vgrind

install.man: FRC
	install -c -o bin -g bin -m 444 vgrind.man ${MANDIR}/vgrind.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${ROBJS} ${VOBJS} retest vfontedpr

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
