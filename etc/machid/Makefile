# Makefile for machid.mach3.

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

USRETC=${DESTDIR}/usr/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS= -o bin -g bin -m 555

CFLAGS= -O

SRCS= machid.c machid_procs.c machid_debug_procs.c machid_notify_procs.c

OBJS= machid.o machid_procs.o machid_debug_procs.o machid_notify_procs.o

LIBS=	-lmach

all: machid .depend

machid: ${GENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s ${IFLAGS} machid ${USRETC}/machid
	rm -f ${BINDIR}/machid; ln -s ../usr/etc/machid ${BINDIR}/machid

install.man: FRC
	install -c -o bin -g bin -m 444 machid.man ${MANDIR}/machid.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} machid

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
