# Makefile for tf77 libplot.

CFLAGS=	-O -MD

SRCS=	arc.c box.c circle.c clospl.c cont.c dot.c erase.c label.c \
	line.c linemd.c move.c open.c point.c space.c

OBJS=	arc.o box.o circle.o clospl.o cont.o dot.o erase.o label.o \
	line.o linemd.o move.o open.o point.o space.o

all:	../libf77plot .depend

../libf77plot: ${OBJS}
	ar cu $@ `lorder ${OBJS} | tsort`

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
