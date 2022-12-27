# Makefile for sendbug.

PROG1=	sendbug
PROG2=	bugfiler

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS1= -o bin -g bin -m 555
IFLAGS2= -o root -g bin -m 4555

SRCS=	bugfiler.c error.c gethead.c process.c redist.c reply.c

OBJS2=	bugfiler.o error.o gethead.o process.o redist.o reply.o

all:	${PROG2} .depend

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c ${IFLAGS1} ${PROG1}.sh ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS2} ${PROG2} ${LIBDIR}/${PROG2}
	install -c -o bin -g bin -m 444 bugformat ${LIBDIR}/bugformat

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS2} ${PROG2}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
