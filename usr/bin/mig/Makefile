# Makefile for mig, Mach 3.0 version.

PROG=	mig

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -traditional

IFLAGS=	-o bin -g bin -m 555

SRCS= 	parser.c lexxer.c mig.c error.c string.c type.c routine.c \
	statement.c global.c header.c user.c server.c utils.c

CGENS=	parser.c lexxer.c
HGENS=	parser.h

OBJS= 	parser.o lexxer.o mig.o error.o string.o type.o routine.o \
	statement.o global.o header.o user.o server.o utils.o

LIBS=	-ll

all: migcom .depend

migcom:	${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

parser.c parser.h: parser.y
	yacc -d parser.y && mv y.tab.c parser.c && mv y.tab.h parser.h

lexxer.c: lexxer.l
	lex lexxer.l &&	mv lex.yy.c lexxer.c

install: FRC
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}
	install -c -s ${IFLAGS} migcom ${LIBDIR}/migcom

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} migcom ${CGENS} ${HGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
