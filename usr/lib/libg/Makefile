# Makefile for libg.

all:
	cd libg.${target_machine}; make

install:
	cd libg.${target_machine}; make DESTDIR=${DESTDIR} install

install.man:
	cd libg.${target_machine}; make DESTDIR=${DESTDIR} install.man

clean:
	cd libg.${target_machine}; make clean
