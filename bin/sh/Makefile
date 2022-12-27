# Makefile for sh.

PROG=	sh

CFGS= sh.profile

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

CFGDIR=${DESTDIR}/etc/newconfig

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

CFLAGS= -O -MD -w

SRCS=	setbrk.c builtin.c blok.c stak.c cmd.c fault.c main.c word.c \
	string.c name.c args.c xec.c service.c error.c io.c print.c \
	macro.c expand.c ctype.c msg.c

OBJS=	setbrk.o builtin.o blok.o stak.o cmd.o fault.o main.o word.o \
	string.o name.o args.o xec.o service.o error.o io.o print.o \
	macro.o expand.o ctype.o msg.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}
	rm -f ${MANDIR}/case.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/case.${MANEXT}
	rm -f ${MANDIR}/for.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/for.${MANEXT}
	rm -f ${MANDIR}/if.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/if.${MANEXT}
	rm -f ${MANDIR}/while.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/while.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
