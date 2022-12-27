# Makefile for test.

DESTDIR=

CFLAGS= -O -MD

SRCS=	test.c

OBJS=	test.o

all:	test .depend

test: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 555 test ${DESTDIR}/bin
	cd ${DESTDIR}/bin; rm -f "["; ln -s test "["

install.man: FRC
	install -c -o bin -g bin -m 444 test.man ${DESTDIR}/usr/man/man1/test.1

clean:	FRC
	rm -f .depend ${OBJS} test

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
