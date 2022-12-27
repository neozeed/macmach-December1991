# Makefile for telnet.

# the telnet sources are shared with tn3270

PROG=	telnet

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

VPATH=	Source

CFLAGS=	-O -MD -ISource

IFLAGS=	-o bin -g bin -m 555

SRCS=	commands.c main.c network.c ring.c sys_bsd.c sys_dos.c \
	telnet.c terminal.c tn3270.c utilities.c

OBJS=	commands.o main.o network.o ring.o sys_bsd.o sys_dos.o \
	telnet.o terminal.o tn3270.o utilities.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

clist:	FRC
	@for i in ${SRCS}; do (echo ${DIRPATH}$$i); done

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} errs l.errs

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
