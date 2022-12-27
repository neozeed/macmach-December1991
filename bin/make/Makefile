# Makefile for make.

PROG=	make

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

CFLAGS= -O -MD -DASCARCH

SRCS=	main.c doname.c misc.c files.c dirs.c libs.c rules.c \
	dosys.c gram.c dyndep.c

CGENS=	gram.c

OBJS=	main.o doname.o misc.o files.o dirs.o libs.o rules.o \
	dosys.o gram.o dyndep.o

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS}

gram.c:	gram.y
	yacc gram.y
	mv y.tab.c gram.c

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
