# Makefile for awk.

PROG=	awk

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

CFLAGS= -O -MD

YFLAGS= -d

SRCS=	b.c lib.c main.c parse.c freeze.c run.c tran.c

CGENS=	awk.lx.c proctab.c token.c
HGENS=	awk.h y.tab.h

OBJS=	awk.g.o awk.lx.o b.o main.o token.o tran.o lib.o run.o parse.o \
	proctab.o freeze.o

LIBS=	-lm

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

y.tab.h: awk.g.o

awk.h:	y.tab.h
	-cmp -s y.tab.h awk.h || cp y.tab.h awk.h

token.c: awk.h tokenscript
	ex - < tokenscript

proctab.c: proc
	./proc > proctab.c

proc:	awk.h proc.o token.o
	cc -o $@ proc.o token.o

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${HGENS} ${CGENS} proc proc.o temp

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
