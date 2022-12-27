# Makefile for vlp.

DESTDIR= 

all:
	cd vlp.${target_machine}; make

install:
	cd vlp.${target_machine}; make DESTDIR=${DESTDIR} install

install.man:
	cd vlp.${target_machine}; make DESTDIR=${DESTDIR} install.man

clean:
	cd vlp.${target_machine}; make clean
