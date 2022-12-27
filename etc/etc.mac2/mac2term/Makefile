# Makefile for mac2term.

PROG= mac2term

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

CFLAGS= -O

CSRCS=	${PROG}.c

# object files
OBJS= ${PROG}.o

LIBS=	-ltermcap

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install:
	install -c -s -o bin -g bin -m 555 ${PROG} ${BINDIR}/${PROG}

install.man:
	@echo "*** need man page for mac2term"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	${CSRCS}
	rm -f .depend ${OBJS} ${PROG}

.depend: ${CSRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${CSRCS}

-include .depend
