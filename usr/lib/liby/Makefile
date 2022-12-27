# Makefile for liby.

CFLAGS=	-O -MD

SRCS=	libmai.c libzer.c

OBJS=	libmai.o libzer.o

all:	liby.a .depend

liby.a: ${OBJS}
	ar cr $@ ${OBJS}

install: FRC
	install -c -o bin -g bin -m 644 liby.a ${DESTDIR}/usr/lib/liby.a
	ranlib ${DESTDIR}/usr/lib/liby.a

install.man: FRC

clean:	FRC
	rm -f .depend ${OBJS} core liby.a

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
