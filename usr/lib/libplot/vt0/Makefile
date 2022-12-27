# Makefile for vt0 libplot.

CFLAGS=	-O -MD

SRCS=	arc.c box.c circle.c close.c dot.c erase.c label.c \
	line.c linmod.c move.c open.c point.c space.c subr.c

OBJS=	arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linmod.o move.o open.o point.o space.o subr.o

all:	../libvt0 .depend

../libvt0: ${OBJS}
	ar cu $@ ${OBJS}

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
