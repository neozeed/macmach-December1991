# Makefile for lpr filters.

DESTDIR=

CFLAGS=	-O -MD

LIBDIR=	/usr/lib

SRCS=	lpf.c

OBJS=	lpf.o

all:	lpf .depend

lpf:	lpf.c
	cc -o $@ ${CFLAGS} lpf.c

install: FRC
	install -c -s -o bin -g bin -m 755 lpf ${DESTDIR}/${LIBDIR}/lpf

clean:	FRC
	rm -f .depend ${OBJS} core lpf

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
