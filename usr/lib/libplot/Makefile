# Makefile for libplot.

DESTDIR=

CFLAGS=	-O -MD

STD=	libf77plot libplot lib300 lib300s lib4013 lib4014 lib450 libvt0 \
	libplotaed libplotbg libplotdumb libplotgigi libplot2648 libplot7221 \
	libplotimagen libplotgrn

SUBDIRS=tf77 plot t4013 t4014 t300 t300s t450 vt0 aed bitgraph dumb gigi \
	hp2648 hp7221 imagen grn

all: ${STD}

libf77plot:
	cd tf77; make ${MFLAGS}

libplot:
	cd plot; make ${MFLAGS}

lib4013:
	cd t4013; make ${MFLAGS}

lib4014:
	cd t4014; make ${MFLAGS}

lib300:
	cd t300; make ${MFLAGS}

lib300s:
	cd t300s; make ${MFLAGS}

lib450:
	cd t450; make ${MFLAGS}

libvt0:
	cd vt0; make ${MFLAGS}

libplotaed:
	cd aed; make ${MFLAGS}

libplotbg:
	cd bitgraph; make ${MFLAGS}

libplotdumb:
	cd dumb; make ${MFLAGS}

libplotgigi:
	cd gigi; make ${MFLAGS}

libplot2648:
	cd hp2648; make ${MFLAGS}

libplot7221:
	cd hp7221; make ${MFLAGS}

libplotimagen:
	cd imagen; make ${MFLAGS}

libplotgrn:
	cd grn; make ${MFLAGS}

install: FRC
	for i in ${STD}; do \
		install -c -o bin -g bin -m 644 $$i ${DESTDIR}/usr/lib/$$i.a; \
		ranlib ${DESTDIR}/usr/lib/$$i.a; \
	done

install.man: FRC

clean:	FRC
	rm -f .depend ${STD} core
	for i in ${SUBDIRS}; do (cd $$i; make ${MFLAGS} clean); done

tags:	FRC
	for i in ${SUBDIRS}; do \
		(cd $$i; make ${MFLAGS} tags); \
	done

FRC:
