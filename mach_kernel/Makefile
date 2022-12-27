# Makefile for Mach 3.0 kernel.

DESTDIR=

LPATH=	/lib:/usr/lib

# Uncomment this to make a 24-bit compatible kernel
CONFIG=	MACMACH+mode24

# Uncomment this to make a 32-bit clean kernel
#CONFIG=	MACMACH

KERNEL=	kernel/${CONFIG}/mach_kernel

all:	FRC
	LPATH="$${LPATH-${LPATH}}"; export LPATH; \
	echo "LPATH=$$LPATH"; echo "CPATH=$$CPATH"; \
	cd kernel; make CONFIG=${CONFIG}

install: FRC
	install -c -o root -g bin -m 444 ${KERNEL} ${DESTDIR}/mach_kernel

install.man: FRC
	for i in kernel/man/*.man; do \
	  install -c -o bin -g bin -m 444 $$i \
	    ${DESTDIR}/usr/man/man2/`basename $$i .man`.2; \
	done

clean:	FRC
	rm -rf kernel/${CONFIG}
	cd kernel/src/config; make clean
	cd kernel/src/mig; \
	  rm -f mig *.o Makedep parser.c parser.h lexxer.c migcom.x
	cd kernel/bin; rm -f config mig
	cd kernel/lib; rm -f migcom

FRC:
