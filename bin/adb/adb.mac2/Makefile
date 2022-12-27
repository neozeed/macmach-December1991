# Makefile for mac2 adb, Mach 2.5 version.

PROG= adb

DESTDIR=

MANEXT=1

BINDIR=${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

CFLAGS= -O -MD -DM68881 -DVFORK -Dmach3

CSRCS=	access.c command.c expr.c format.c input.c main.c message.c \
	opset.c optab.c output.c pcs.c print.c runpcs.c setup.c sym.c trace.c

OBJS=	access.o command.o expr.o format.o input.o main.o message.o \
	opset.o optab.o output.o pcs.o print.o runpcs.o setup.o sym.o trace.o

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

message.o:
	cc -R ${CFLAGS} -c message.c

optab.o:
	cc -R ${CFLAGS} -c optab.c

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG}

.depend: ${CSRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend
