# Makefile for as.

DESTDIR=

SUBDIR= as.${target_machine}

all:
	for i in ${SUBDIR}; do ( cd $$i; make ); done

install:
	for i in ${SUBDIR}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install \
	); done

install.man:
	for i in ${SUBDIR}; do ( \
	  cd $$i; make DESTDIR=${DESTDIR} install.man \
	); done

clean:
	for i in ${SUBDIR}; do ( cd $$i; make clean ); done
