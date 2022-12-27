# Makefile for quotaon.

PROG1=	quotaon
PROG2=	quotaoff

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG1}.c

OBJS=	${PROG1}.o

all:	${PROG1} .depend

${PROG1}: ${OBJS}
	cc -o $@ ${OBJS}


install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${USRETC}/${PROG1}
	rm -f ${BINDIR}/${PROG1}; ln -s ../usr/etc/${PROG1} ${BINDIR}/${PROG1}
	rm -f ${BINDIR}/${PROG2}; ln -s ${PROG1} ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f  ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG1}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
