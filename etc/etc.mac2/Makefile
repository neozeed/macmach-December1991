# Makefile for mac2 things in etc.

DESTDIR=

THINGS=	mac2part mac2term makedev newsys sony

MANEXT=	8

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

MANS=	crash.man reboot.man

all:	FRC
	for i in ${THINGS}; do ( cd $$i; make all; ) done

ramdisk: FRC
	@sh ramdisk.sh

install: FRC
	for i in ${THINGS}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done
	install -c -o bin -g bin -m 444 Makefile.newconfig \
	  ${DESTDIR}/etc/newconfig/Makefile

install.man: FRC
	for i in ${THINGS}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done
	for i in ${MANS}; do \
	  install -c -o bin -g bin -m 444 $$i \
	    ${MANDIR}/`basename $$i .man`.${MANEXT}; \
	done

clean:	FRC
	for i in ${THINGS}; do ( cd $$i; make clean; ) done

FRC:
