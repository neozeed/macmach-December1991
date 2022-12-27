# Makefile for netstat.

PROG=	netstat

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g kmem -m 2555

SRCS=	host.c inet.c if.c main.c mbuf.c route.c unix.c ns.c

OBJS=	host.o inet.o if.o main.o mbuf.o route.o unix.o ns.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
