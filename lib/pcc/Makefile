# Makefile for pcc.

all:
	cd pcc.${target_machine}; make ${MFLAGS}

install:
	cd pcc.${target_machine}; make DESTDIR=${DESTDIR} install

install.man:
	cd pcc.${target_machine}; make DESTDIR=${DESTDIR} install.man

clean:
	cd pcc.${target_machine}; make ${MFLAGS} clean
