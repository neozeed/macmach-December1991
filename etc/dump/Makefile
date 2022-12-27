# Makefile for dump.

PROG1=	dump
PROG2=	rdump

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	dumpitime.c dumpmain.c dumpoptr.c dumprmt.c dumptape.c \
	dumptraverse.c unctime.c

OBJS1=	dumpitime.o dumpmain.o dumpoptr.o dumptape.o dumptraverse.o \
	unctime.o

OBJS2=	dumpitime.o dumprmain.o dumpoptr.o dumprmt.o dumprtape.o \
	dumptraverse.o unctime.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

dumprmain.o: dumpmain.c
	cc -c -DRDUMP ${CFLAGS} dumpmain.c -o dumprmain.o

dumprtape.o: dumptape.c
	cc -c -DRDUMP ${CFLAGS} dumptape.c -o dumprtape.o

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${USRETC}/${PROG1}
	rm -f ${BINDIR}/${PROG1}; ln -s ../usr/etc/${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${USRETC}/${PROG2}
	rm -f ${BINDIR}/${PROG2}; ln -s ../usr/etc/${PROG2} ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
