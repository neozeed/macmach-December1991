# Makefile for ftp.

PROG=	ftp

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	cmds.c cmdtab.c ftp.c getpass.c glob.c main.c pclose.c ruserpass.c\
	domacro.c

OBJS=	cmds.o cmdtab.o ftp.o getpass.o glob.o main.o pclose.o ruserpass.o\
	domacro.o

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
