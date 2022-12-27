# Makefile for mkhosts.

PROG=	mkhosts

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG}.c

OBJS=	${PROG}.o

all:	${PROG} .depend

${PROG}: ${OBJS}
	-@cc -o $@ ${OBJS} || { \
	  echo "#! /bin/sh" >${PROG}; \
	  echo "echo \"mkhosts: system uses name server\"" >>${PROG}; \
	  echo "exit 1" >>${PROG}; \
	  echo "*** system uses name server"; \
	}

install:
	install -c ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
