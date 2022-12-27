# Makefile for c2.

DESTDIR= 

all:
	cd c2.${target_machine}; make

install:
	cd c2.${target_machine}; make DESTDIR=${DESTDIR} install

install.man:
	cd c2.${target_machine}; make DESTDIR=${DESTDIR} install.man

clean:
	cd c2.${target_machine}; make clean
