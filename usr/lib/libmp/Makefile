# Makefile for libmp.

CFLAGS=	-O -MD

SRCS=	pow.c gcd.c msqrt.c mdiv.c mout.c mult.c madd.c util.c

OBJS=	pow.o gcd.o msqrt.o mdiv.o mout.o mult.o madd.o util.o

all:	libmp.a .depend

libmp.a: ${OBJS}
	ar cr $@ ${OBJS}

install: FRC
	install -c -o bin -g bin -m 644 libmp.a ${DESTDIR}/usr/lib/libmp.a
	ranlib ${DESTDIR}/usr/lib/libmp.a

install.man: FRC

clean:	FRC
	rm -f .depend ${OBJS} core libmp.a

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
