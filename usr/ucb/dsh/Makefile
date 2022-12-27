# Makefile for dsh.

PROG1=	dsh
PROG2=	dbid

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g kmem -m 2555

SRCS=	dbid.c dsh.c error.c getbids.c getloadave.c getpath.c getrc.c \
	getwd.c ipc.c

OBJS1=	dsh.o error.o getrc.o ipc.o getwd.o getbids.o

OBJS2=	getloadave.o getpath.o dbid.o error.o getrc.o ipc.o

all:	${PROG1} ${PROG2}

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	@echo "*** need man pages for dbid"

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}
	rm -f x.c xs.c xhead.c strings

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
