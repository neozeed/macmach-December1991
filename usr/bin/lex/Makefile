# Makefile for lex.

PROG=	lex

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

LIBDIR=	${DESTDIR}/usr/lib/lex

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	lmain.c sub1.c sub2.c header.c

CGENS=	parser.c

OBJS=	lmain.o parser.o sub1.o sub2.o header.o

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	-chmod 555 ${LIBDIR}
	-chown bin.bin ${LIBDIR}
	install -c -o bin -g bin -m 444 ncform ${LIBDIR}/ncform
	install -c -o bin -g bin -m 444 nrform ${LIBDIR}/nrform

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
