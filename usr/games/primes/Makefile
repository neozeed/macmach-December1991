# Makefile for primes.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	primes.c

OBJS=	primes.o

all:	primes .depend

primes: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 primes ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f primes; ln -s dm primes

install.man: FRC
	@echo "*** need man pages for primes"
#	install -c -o bin -g bin -m 444 primes.man ${DESTDIR}/usr/man/man6/primes.6

clean:	FRC
	rm -f .depend ${OBJS} primes

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
