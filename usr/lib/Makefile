# Makefile for things in /usr/lib, Mach 3.0 version.

DESTDIR=

THINGS=	getNAME learn lib2648 libcurses libdbm libg libln libm libmach \
	libmach_sa libmp libplot libtermcap libthreads liby lpr \
	makekey man me sendmail 

all:	FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Building $$i."; \
	  cd $$i; make; \
	) done

install: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Installing $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done

install.man: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Installing man pages for $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done

clean: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Cleaning $$i."; \
	  cd $$i; make clean; \
	) done

FRC:
