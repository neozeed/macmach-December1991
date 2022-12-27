# Makefile for troff_font.

CFLAGS=	-O -MD

SRCS=	mkfont.c

OBJS=	mkfont.o

all:	mkfont .depend

mkfont: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} mkfont

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
