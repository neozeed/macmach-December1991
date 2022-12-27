# Makefile for hash_info.

PROG=	hash_info

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	hash_info.c expected.c

OBJS=	hash_info.o expected.o

LIBS=	-lmach -lm

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	@echo "*** need man pages for hash_info"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
