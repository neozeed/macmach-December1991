# Makefile for bitgraph libplot.

CFLAGS=	-O -MD

SRCS=	box.c circle.c arc.c cont.c dot.c erase.c line.c linemod.c point.c \
	label.c move.c open.c close.c space.c

OBJS=	box.o circle.o arc.o cont.o dot.o erase.o line.o linemod.o point.o \
	label.o move.o open.o close.o space.o

all:	../libplotbg .depend

../libplotbg: ${OBJS}
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
