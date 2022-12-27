# Makefile for things in /bin, Mach 3.0 version.

DESTDIR=

THINGS=	adb ar as awk cat chmod cmp cp cpio csh date dd df diff du echo ed \
	expr false hostid hostname kill ld ln login ls mach3 machine make md \
	mkdir mkill mnice ms mt mv nice nm od pagesize passwd pr ps pwd rcp \
	rm rmdir sed sh size strip stty su sync tar tee test time tp true \
	wall who write

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

clean:	FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Cleaning $$i."; \
	  cd $$i; make clean; \
	) done

FRC:
