# Makefile for sony.

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 555

SRCS=	sony.c

OBJS=	sony.o

all:	sony .depend

sony: ${OBJS}
	cc -o $@ ${OBJS}

install:
	install -c -s ${IFLAGS} sony ${BINDIR}/sony
	cd ${BINDIR}; rm -f eject; ln -s sony eject

install.man:
	@echo "*** need man pages for sony"
#	install -c -o bin -g bin -m 444 sony.man ${MANDIR}/sony.${MANEXT}
#	cd ${MANDIR}; \
#	  rm -f eject.${MANEXT}; ln -s sony.${MANEXT} eject.${MANEXT}

clean:
	rm -f .depend ${OBJS} sony

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
