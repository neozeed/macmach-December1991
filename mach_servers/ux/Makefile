# Makefile for Mach 3.0 ux.

DESTDIR=

BINDIR=	${DESTDIR}/mach_servers

LPATH=	/lib:/usr/lib
CPATH=	/lib/gcc-include:/usr/include:.
MKINC=	/usr/include
MKLIB=	/usr/lib

CONFIG=	MACMACH

UX=	server/${CONFIG}/vmunix

EMULATOR= emulator/emulator

all:	${UX} ${EMULATOR}

${UX}:	FRC
	@LPATH="$${LPATH-${LPATH}}"; export LPATH; \
	CPATH="$${CPATH-${CPATH}}"; export CPATH; \
	MKINC="$${MKINC-${MKINC}}"; export MKINC; \
	MKLIB="$${MKLIB-${MKLIB}}"; export MKLIB; \
	cd server; make CONFIG=${CONFIG}

${EMULATOR}: FRC
	@LPATH="$${LPATH-${LPATH}}"; export LPATH; \
	CPATH="$${CPATH-${CPATH}}"; export CPATH; \
	MKINC="$${MKINC-${MKINC}}"; export MKINC; \
	MKLIB="$${MKLIB-${MKLIB}}"; export MKLIB; \
	cd emulator; make MKLIB="$$MKLIB" MKINC="$$MKINC"

install: FRC
	install -c -o root -g bin -m 544 ${UX} ${BINDIR}/startup
	install -c -o root -g bin -m 544 ${EMULATOR} ${BINDIR}/emulator
	rm -f ${DESTDIR}/vmunix; ln -s mach_servers/startup ${DESTDIR}/vmunix

install.man: FRC
	@echo "*** need man page for unix server"

clean:	FRC
	rm -rf server/${CONFIG}
	cd server/src/config; make clean
	rm -f server/bin/config
	cd emulator; \
	  rm -rf *.o emulator uxkern Makedep emulator_base a.out \
	  mach_interface.h mach_user.c bsd_types_gen

FRC:
