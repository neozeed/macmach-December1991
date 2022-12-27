# Makefile for mac2part.

PROG= mac2part

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 544

CFLAGS= -O

CSRCS=	${PROG}.c

# object files
OBJS= ${PROG}.o

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	@echo "*** need man page for mac2part"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	${CSRCS}
	rm -f .depend ${OBJS} ${PROG}

.depend: ${CSRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${CSRCS}

-include .depend
