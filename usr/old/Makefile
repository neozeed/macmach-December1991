# Makefile for things in /usr/old, Mach 3.0 version.

DESTDIR=

THINGS=	mig

all:
	for i in ${THINGS}; do ( cd $$i; make all; ) done

install:
	for i in ${THINGS}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done

install.man:
	for i in ${THINGS}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done

clean:
	for i in ${THINGS}; do ( cd $$i; make clean; ) done
